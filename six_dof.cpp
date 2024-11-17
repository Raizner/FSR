#include "six_dof.h"

void SixDof::validateValue(const std::string &key, float value) {
  if (key == "x" || key == "y" || key == "z") {
    if (value < -1.0f || value > 1.0f) {
      throw std::out_of_range(key + " should be betweem -1 and 1.0");
    }
  } else if (key == "roll" || key == "pitch" || key == "yaw") {
    if (value < -PI || value > PI) {
      throw std::out_of_range(key + " should be betweem -PI and PI");
    }
  } else {
    throw std::invalid_argument("The key: " + key + " is not part of 6DOF");
  }
}

SixDof::SixDof()
    : values{{"x", 0.0f},    {"y", 0.0f},     {"z", 0.0f},
             {"roll", 0.0f}, {"pitch", 0.0f}, {"yaw", 0.0f}} {}

float SixDof::get(const std::string &key) const {
  std::shared_lock lock(mutex); // reader
  auto i = values.find(key);
  if (i == values.end()) {
    throw std::invalid_argument("Invalid key: " + key +
                                ". Keys: x,y,z,roll,pitch,yaw");
  }
  return i->second;
}

void SixDof::set(const std::string &key, float value) {
  validateValue(key, value);
  std::unique_lock lock(mutex); // writer
  auto i = values.find(key);
  if (i == values.end()) {
    throw std::invalid_argument("Invalid key: " + key +
                                ". Keys: x,y,z,roll,pitch,yaw");
  }
  i->second = value;
}