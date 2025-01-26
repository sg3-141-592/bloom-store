#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "bloom.h"

#include "../Config.h"
#include "../MetricsTracker.h"

struct FolderData
{
    int recordCount = 0;
    int bundleId = 0;
    std::string buffer;
    bloom bloomFilter;

    FolderData(int bloomSize = 1000, float bloomProbability = 0.01)
    {
        buffer.reserve(1024); // Pre-allocate since we'll be appending to the buffer a lot

        // Initialize the bloom filter
        // TODO: Drive the bloom config off the ini file
        // The bloom filter must at minimum be 1000 in size
        if (bloom_init2(&bloomFilter, bloomSize, bloomProbability))
        {
            std::cerr << "Failed to initialize bloom filter" << std::endl;
        }
    }

    ~FolderData()
    {
        bloom_free(&bloomFilter);
    }
};

class FolderDataSink : public DataSink
{
public:
    FolderDataSink(std::function<std::string(std::string)> getPathFunc);
    bool writeNext(json in);
    void start(std::shared_ptr<TSQueue<json>> queue);
    ~FolderDataSink();
    void stop();

private:
    std::function<std::string(std::string)> _getPathFunc;
    std::map<std::string, FolderData> _pathToHook;
    std::thread _thread;
    const int BUNDLE_SIZE = 1000;
    MetricsTracker *_metricsTracker;
};
