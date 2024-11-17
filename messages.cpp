#include "messages.h"

MessagesQueue::MessagesQueue() = default;

void MessagesQueue::push(const std::pair<std::string, float> &item) {
  {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(item);
  }
}

std::optional<std::pair<std::string, float>> MessagesQueue::pop() {
  std::lock_guard<std::mutex> lock(mutex);
  if (queue.empty()) {
    return std::nullopt;
  }
  std::pair<std::string, float> item = queue.front();
  queue.pop();
  return item;
}

std::optional<std::vector<std::pair<std::string, float>>>
MessagesQueue::popAll() {
  std::vector<std::pair<std::string, float>> items;
  std::lock_guard<std::mutex> lock(mutex); // pop also changes the queue
  if (queue.empty()) {
    return std::nullopt;
  }
  while (!queue.empty()) {
    items.push_back(queue.front());
    queue.pop();
  }
  return items;
}