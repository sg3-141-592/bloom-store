#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "bloom.h"

#include "../MetricsTracker.h"

struct FolderData {
  int recordCount = 0;
  int bundleId = 0;
  std::string buffer;
  bloom bloomFilter;

  FolderData(int bloomSize = 1000, float bloomProbability = 0.01f) {
    buffer.reserve(
        1024); // Pre-allocate since we'll be appending to the buffer a lot

    // Initialize the bloom filter
    // TODO: Drive the bloom config off the ini file
    // The bloom filter must at minimum be 1000 in size
    if (bloom_init2(&bloomFilter, bloomSize, bloomProbability)) {
      std::cerr << "Failed to initialize bloom filter" << std::endl;
    }
  }

  ~FolderData() { bloom_free(&bloomFilter); }
};

class FolderDataSink : public DataSink {
public:
  FolderDataSink(getPathFuncType getPathFunc, std::shared_ptr<Config> config);
  bool writeNext(json in);
  void start(std::shared_ptr<TSQueue<GenericRecord>> queue);
  ~FolderDataSink();
  void stop();

private:
  getPathFuncType _getPathFunc;
  std::map<std::string, std::unique_ptr<FolderData>> _pathToHook;
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker;
  std::shared_ptr<Config> _config;
};
