#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <thread>

#include "DataSource.h"
#include "../MetricsTracker.h"

class FileDataSource : public DataSource {
private:
    std::ifstream file;
    std::string _filename;
    std::shared_ptr<TSQueue<Record>> _queue;
    std::thread _thread;
    MetricsTracker* _metricsTracker;
public:
    FileDataSource(std::string filename);
    void start(std::shared_ptr<TSQueue<Record>> queue);
    ~FileDataSource();
    void stop();
};
