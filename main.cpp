#include "task.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>

int main() {
  TasksMessageQueues messagesQueus;

  SixDof storage;

  Task_10 task10(messagesQueus);
  Task_20 task20(messagesQueus);
  Task_40 task40(messagesQueus);

  // Launch tasks in separate threads
  std::thread t1(&Task::run, &task10, std::ref(storage));
  std::thread t2(&Task::run, &task20, std::ref(storage));
  std::thread t3(&Task::run, &task40, std::ref(storage));

  std::ofstream csv_file("plot.csv");
  csv_file << "time_ms,roll_value\n";
  auto start_time = std::chrono::steady_clock::now();
  for (int elapsed = 0; elapsed <= 10000; elapsed += 5) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    float x_value = storage.get("roll");
    csv_file << elapsed << "," << x_value << "\n";
  }

  csv_file.close();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  t1.detach();
  t2.detach();
  t3.detach();

  std::cout << "CSV plot created: plot.csv" << std::endl;

  return 0;
}