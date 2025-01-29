#pragma once

#include <fstream>
#include <optional>
#include <thread>

#include "DataSource.h"

#include "../Config.h"
#include "../MetricsTracker.h"

class FileDataSource : public DataSource {
public:
  FileDataSource(std::shared_ptr<Config> config) : _config(std::move(config)){};
  void start(std::shared_ptr<TSQueue<GenericRecord>> queue) override;
  void stop() override;
  ~FileDataSource() override;

private:
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker =
      std::make_unique<MetricsTracker>("FileDataSource");
  std::shared_ptr<Config> _config;
};
