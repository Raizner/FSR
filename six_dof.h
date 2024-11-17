#pragma once
#include <cmath>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

/// @brief Represents a 6-degree-of-freedom (6DoF) system, with thread-safe
/// access to position (x, y, z) and orientation (roll, pitch, yaw).
class SixDof {
private:
  static constexpr float PI = 3.14f;
  std::unordered_map<std::string, float> values;
  mutable std::shared_mutex mutex;

  /// @brief Validates the value for a given key to ensure it lies within the
  /// allowed range.
  static void validateValue(const std::string &key, float value);

public:
  SixDof();
  SixDof(SixDof &&) = delete;
  SixDof(const SixDof &) = delete;
  SixDof &operator=(SixDof &&) = delete;
  SixDof &operator=(const SixDof &) = delete;

  float get(const std::string &key) const;
  void set(const std::string &key, float value);
};