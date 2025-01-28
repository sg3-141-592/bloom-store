#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>

class MetricsTracker {
private:
  std::string name;
  std::atomic<uint64_t> messageCount{0};
  std::atomic<uint64_t> lastSecondMessageCount{0};
  std::chrono::steady_clock::time_point lastMetricReset;
  std::mutex printMutex;
  std::fstream logFile;

public:
  MetricsTracker(std::string name) : name(name) {
    // Randomly name files to avoid collisions between duplicate threads
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    int randomId = dis(gen);
    logFile.open("logging/" + name + "." + std::to_string(randomId), std::ios::out);
  }
  
  void recordMessage() {
    messageCount++;
    lastSecondMessageCount++;
  }

  void printMetricsIfNeeded() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastMetricReset);

    if (elapsed.count() >= 200) {
      std::lock_guard<std::mutex> lock(printMutex);

      auto now_time = std::chrono::system_clock::now();
      auto now_c = std::chrono::system_clock::to_time_t(now_time);
      auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now_time.time_since_epoch())
                        .count() %
                    1000; // Get just the milliseconds part

      // Format time with milliseconds
      std::ostringstream timestamp;
      timestamp << std::put_time(std::localtime(&now_c), "%F %T") << "."
                << std::setfill('0') << std::setw(3) << now_ms;

      auto threadId = std::this_thread::get_id();
      logFile << timestamp.str() << "," << name << "." << threadId << ","
        << lastSecondMessageCount << "," << messageCount << std::endl;

      lastSecondMessageCount = 0;
      lastMetricReset = now;
    }
  }

  ~MetricsTracker() { logFile.close(); }
};