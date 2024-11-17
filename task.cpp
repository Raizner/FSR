#include "task.h"
#include <cmath>
#include <thread>

Task::Task(const std::string &name, int interval,
           TasksMessageQueues &tasksMessagesQueue)
    : taskName(name), intervals(interval),
      tasksMessageQueues(tasksMessagesQueue) {
  messageQueue = std::make_shared<MessagesQueue>();
  tasksMessageQueues[taskName] = messageQueue;
}

void Task::sendOneMessage(const std::string &target_task,
                          const std::string &key, float value) {
  auto it = tasksMessageQueues.find(target_task);
  if (it == tasksMessageQueues.end()) {
    throw std::invalid_argument("Wrong target task: " + target_task);
  }
  it->second->push({key, value}); // Thread-safe push MessagesQueue
}

std::optional<std::vector<std::pair<std::string, float>>>
Task::receiveMessages() {
  return messageQueue->popAll(); // Thread-safe pop MessagesQueue
}

void Task::updateStorage(
    std::vector<std::pair<std::string, float>> &vecOfValues, SixDof &storage) {
  for (const auto &[key, value] : vecOfValues) {
    storage.set(key, value);
  }
}

float Task::oscillate(float min, float max, float timePeriod) {
  using namespace std::chrono;
  float elapsed_seconds =
      duration<float>(steady_clock::now().time_since_epoch()).count();
  float t = std::fmod(elapsed_seconds, timePeriod) / timePeriod;
  return min +
         (max - min) * (t < 0.5f ? 2 * t : 2 * (1 - t)); // oscillation formula
}

bool Task::hasElapsed(const std::chrono::steady_clock::time_point &last,
                      int intervalMs) {
  return std::chrono::steady_clock::now() - last >=
         std::chrono::milliseconds(intervalMs);
}

void Task::run(SixDof &storage) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(intervals));

    // Read messages on current task
    auto messages = receiveMessages();
    if (messages) {
      updateStorage(*messages, storage);
    }

    // Send messages to other tasks
    sendMessages();
  }
}

////////////////////////////////////////////////////////////////////////
//////////// Derived Tasks (Task_10, Task_20, Task_40) /////////////////
////////////////////////////////////////////////////////////////////////

Task_10::Task_10(TasksMessageQueues &tasksMessagesQueue)
    : Task("Task_10", 10, tasksMessagesQueue),
      lastSentTo20(std::chrono::steady_clock::now()),
      lastSentTo40(std::chrono::steady_clock::now()) {}

void Task_10::sendMessages() {
  auto now = std::chrono::steady_clock::now();

  // Send to Task_20 every 20ms
  if (hasElapsed(lastSentTo20, 20)) {
    sendOneMessage("Task_20", "z", oscillate(-1.0f, 1.0f, 1.0f));
    lastSentTo20 = now;
  }

  // Send to Task_40 every 40ms
  if (hasElapsed(lastSentTo40, 40)) {
    sendOneMessage("Task_40", "roll", oscillate(-PI, PI, 1.0f));
    lastSentTo40 = now;
  }
}

Task_20::Task_20(TasksMessageQueues &tasksMessagesQueue)
    : Task("Task_20", 20, tasksMessagesQueue),
      lastSentTo20(std::chrono::steady_clock::now()),
      lastSentTo40(std::chrono::steady_clock::now()) {}

void Task_20::sendMessages() {
  auto now = std::chrono::steady_clock::now();

  // Send to Task_10 every 20ms
  if (hasElapsed(lastSentTo20, 20)) {
    sendOneMessage("Task_10", "x", oscillate(-1.0f, 1.0f, 1.0f));
    lastSentTo20 = now;
  }

  // Send to Task_40 every 40ms
  if (hasElapsed(lastSentTo40, 40)) {
    sendOneMessage("Task_40", "pitch", oscillate(-PI, PI, 1.0f));
    lastSentTo40 = now;
  }
}

// --- Task_40 Implementation ---
Task_40::Task_40(TasksMessageQueues &tasksMessagesQueue)
    : Task("Task_40", 40, tasksMessagesQueue),
      lastSentTo40(std::chrono::steady_clock::now()) {}

void Task_40::sendMessages() {
  auto now = std::chrono::steady_clock::now();

  // Send to Task_10 and Task_20 every 40ms
  if (hasElapsed(lastSentTo40, 40)) {
    sendOneMessage("Task_10", "y", oscillate(-1.0f, 1.0f, 1.0f));
    sendOneMessage("Task_20", "yaw", oscillate(-PI, PI, 1.0f));
    lastSentTo40 = now;
  }
}
