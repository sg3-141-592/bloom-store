#pragma once

#include <thread>

#include <sqlite3.h>

#include "../Config.h"
#include "../MetricsTracker.h"
#include "DataSource.h"

class SqliteDataSource : public DataSource {
public:
  SqliteDataSource(std::shared_ptr<Config> config);
  void start(std::shared_ptr<TSQueue<GenericRecord>> queue);
  void stop();
  ~SqliteDataSource();

private:
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker;
  std::shared_ptr<Config> _config;
  sqlite3 *_db;
};
