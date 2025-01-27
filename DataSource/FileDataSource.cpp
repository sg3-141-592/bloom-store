#include "FileDataSource.h"

#include "../Utilities/CommonTypes.h"
#include "../Utilities/TSQueue.h"

#include <iostream>

#define SLEEP_TIME 100 // how long to sleep when queues are full

FileDataSource::FileDataSource(std::shared_ptr<Config> config) {
  _metricsTracker = std::make_unique<MetricsTracker>("FileDataSource");
  _config = std::move(config);
};

void FileDataSource::start(std::shared_ptr<TSQueue<GenericRecord>> queue) {
  _thread = std::thread([this, queue]() {
    std::cout << "Starting loading messages\n";

    std::ifstream infile(_config->sourceConfig.Path);
    if (!infile.is_open()) {
      std::cerr << "Failed to open file: " << _config->sourceConfig.Path
                << std::endl;
      return;
    }

    // Get initial checkpoint for the file
    std::streampos checkpoint = infile.tellg();

    std::string nextline;
    while (getline(infile, nextline) && !_stopFlag) {
      
      // Push value to the queue
      if (!queue->try_push(StringRecord{nextline, checkpoint})) {
        // If can't push to the queue, then pause and try again
        while (!queue->try_push(StringRecord{nextline, checkpoint})) {
          std::this_thread::yield();
          std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        }
      }

      // Once we've pushed the file we can update our checkpoint
      checkpoint = infile.tellg();

      _metricsTracker->recordMessage();
      _metricsTracker->printMetricsIfNeeded();
    }

    // Push EOF to terminate the queue
    // We have to push one of these for each worker we have
    for (size_t i = 0; i < _config->generalConfig.NumberProcessingThreads;
         i++) {
      queue->push(StringRecord{"EOF", -1});
    }

    std::cout << "Finished loading messages\n";

    infile.close();

    _completed.store(true);
  });
}

void FileDataSource::stop() {
  _stopFlag.store(true);
  if (_thread.joinable()) {
    _thread.join();
  }
}

FileDataSource::~FileDataSource() { stop(); };