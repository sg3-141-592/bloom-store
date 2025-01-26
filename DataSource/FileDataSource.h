#pragma once

#include <fstream>
#include <optional>
#include <thread>

#include "DataSource.h"
#include "../MetricsTracker.h"

class FileDataSource : public DataSource<std::string, std::streampos>
{
public:
    FileDataSource(std::string filename);
    void start(std::shared_ptr<TSQueue<Record<std::string, std::streampos>>> queue);
    void stop();
    ~FileDataSource();
private:
    std::ifstream file;
    std::string _filename;
    std::thread _thread;
    std::unique_ptr<MetricsTracker> _metricsTracker;
};
