#include <atomic>
#include <chrono>
#include <mutex>
#include <iostream>

class ThreadMetricsTracker {
private:
    std::atomic<uint64_t> messageCount{0};
    std::atomic<uint64_t> lastSecondMessageCount{0};
    std::chrono::steady_clock::time_point lastMetricReset;
    std::mutex printMutex;

public:
    void recordMessage() {
        messageCount++;
        lastSecondMessageCount++;
    }

    void printMetricsIfNeeded() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastMetricReset);
        
        if (elapsed.count() >= 1) {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "Messages/sec: " << lastSecondMessageCount 
                      << ", Total messages: " << messageCount << std::endl;
            
            lastSecondMessageCount = 0;
            lastMetricReset = now;
        }
    }
};