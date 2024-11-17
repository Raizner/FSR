#pragma once

#include "messages.h"
#include "six_dof.h"
#include <chrono>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/// @brief Alias for mapping task names to their associated Task's message
/// queues.
using TasksMessageQueues =
    std::unordered_map<std::string, std::shared_ptr<MessagesQueue>>;

/// @brief Base class for tasks that handle message exchange and updates to a
/// shared SixDof storage.
class Task {
protected:
  static constexpr float PI = 3.14f;
  std::string taskName;
  int intervals; // interval in milliseconds

  std::shared_ptr<MessagesQueue> messageQueue;
  TasksMessageQueues
      &tasksMessageQueues; // Reference to the all tasks messages queues

  /// @brief For sending messages to multi other Tasks.
  virtual void sendMessages() = 0;

  /// @brief Sends a single message to a target task.
  void sendOneMessage(const std::string &target_task, const std::string &key,
                      float value);

  /// @brief Receives ALL messages from this task's message queue.
  std::optional<std::vector<std::pair<std::string, float>>> receiveMessages();

  /// @brief Updates the shared SixDof storage.
  void updateStorage(std::vector<std::pair<std::string, float>> &vecOfValues,
                     SixDof &storage);

  float oscillate(float min, float max, float timePeriod);

  /// @brief Checks if a given interval has elapsed since a specific time point.
  bool hasElapsed(const std::chrono::steady_clock::time_point &last,
                  int intervalMs);

public:
  Task(const std::string &name, int interval,
       TasksMessageQueues &tasksMessagesQueue);
  virtual ~Task() = default;

  /// @brief Runs the task, processing messages and sending updates.
  void run(SixDof &storage);
};

////////////////////////////////////////////////////////////////////////
//////////// Derived Tasks (Task_10, Task_20, Task_40) /////////////////
////////////////////////////////////////////////////////////////////////

class Task_10 : public Task {
private:
  std::chrono::steady_clock::time_point lastSentTo20;
  std::chrono::steady_clock::time_point lastSentTo40;

protected:
  void sendMessages() override;

public:
  Task_10(TasksMessageQueues &tasksMessagesQueue);
};

class Task_20 : public Task {
private:
  std::chrono::steady_clock::time_point lastSentTo20;
  std::chrono::steady_clock::time_point lastSentTo40;

protected:
  void sendMessages() override;

public:
  Task_20(TasksMessageQueues &tasksMessagesQueue);
};

class Task_40 : public Task {
private:
  std::chrono::steady_clock::time_point lastSentTo40;

protected:
  void sendMessages() override;

public:
  Task_40(TasksMessageQueues &tasksMessagesQueue);
};
