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
  unsigned int recordCount = 0;
  unsigned int bundleId = 0;
  std::string buffer;
  bloom bloomFilter;

  FolderData(unsigned int bloomSize = 1000, double bloomProbability = 0.01) {
    buffer.reserve(
        1024); // Pre-allocate since we'll be appending to the buffer a lot

    // Initialize the bloom filter
    // The bloom filter must at minimum be 1000 in size
    if (bloom_init2(&bloomFilter, bloomSize, bloomProbability) > 0) {
      std::cerr << "Failed to initialize bloom filter" << std::endl;
    }
  }

  ~FolderData() { bloom_free(&bloomFilter); }
};

class FolderDataSink : public DataSink {
public:
  FolderDataSink(getPathFuncType getPathFunc, std::shared_ptr<Config> config) : _getPathFunc(std::move(getPathFunc)), _config(std::move(config)) {};
  bool writeNext(json itemIn);
  void start(std::shared_ptr<TSQueue<GenericRecord>> queue) override;
  void stop() override;
  ~FolderDataSink() override;

private:
  getPathFuncType _getPathFunc;
  std::map<std::string, std::unique_ptr<FolderData>> _pathToHook;
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker = std::make_unique<MetricsTracker>("FolderDataSink");
  std::shared_ptr<Config> _config;
};
