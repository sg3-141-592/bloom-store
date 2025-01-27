#pragma once

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>

class MetricsTracker {
private:
  std::string name;
  std::atomic<uint64_t> messageCount{0};
  std::atomic<uint64_t> lastSecondMessageCount{0};
  std::chrono::steady_clock::time_point lastMetricReset;
  std::mutex printMutex;

public:
  MetricsTracker(std::string name) : name(name) {}
  void recordMessage() {
    messageCount++;
    lastSecondMessageCount++;
  }

  void printMetricsIfNeeded() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - lastMetricReset);

    if (elapsed.count() >= 1) {
      std::lock_guard<std::mutex> lock(printMutex);
      auto now_c = std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now());
      auto threadId = std::this_thread::get_id();
      std::cout << std::put_time(std::localtime(&now_c), "%F %T") << ","
                << name << "," << lastSecondMessageCount << "," <<
                messageCount
                << "," << threadId << std::endl;

      lastSecondMessageCount = 0;
      lastMetricReset = now;
    }
  }
};