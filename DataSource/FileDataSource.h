#pragma once

#include <fstream>
#include <optional>
#include <thread>

#include "DataSource.h"

#include "../Config.h"
#include "../MetricsTracker.h"

class FileDataSource : public DataSource<std::string, std::streampos>
{
public:
    FileDataSource(std::shared_ptr<Config> config);
    void start(std::shared_ptr<TSQueue<StringRecord>> queue);
    void stop();
    ~FileDataSource();
private:
    std::ifstream file;
    std::string _filename;
    std::thread _thread;
    std::unique_ptr<MetricsTracker> _metricsTracker;
    std::shared_ptr<Config> _config;
};
