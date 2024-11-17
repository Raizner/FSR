#pragma once

#include <mutex>
#include <optional>
#include <queue>
#include <string>

/// @brief Thread-safe message queue for storing and retrieving messages.
class MessagesQueue {
private:
  std::queue<std::pair<std::string, float>> queue;
  mutable std::mutex mutex;

public:
  MessagesQueue();
  MessagesQueue(MessagesQueue &&) = delete;
  MessagesQueue(const MessagesQueue &) = delete;
  MessagesQueue &operator=(MessagesQueue &&) = delete;
  MessagesQueue &operator=(const MessagesQueue &) = delete;

  void push(const std::pair<std::string, float> &item);
  std::optional<std::pair<std::string, float>> pop(); // queue might be empty

  /// @brief Pops all messages currently in the queue.
  std::optional<std::vector<std::pair<std::string, float>>> popAll();
};