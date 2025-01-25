#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <thread>

#include "bloom_filter.hpp"

#include "bloom.h"

struct FolderData
{
    int recordCount = 0;
    int bundleId = 0;
    std::string buffer;
    bloom bloomFilter;

    FolderData()
    {
        // Pre-allocate since we'll be appending to this a lot
        buffer.reserve(1024);

        // Initialize the bloom filter
        // TODO: Drive the bloom config off the ini file
        // The bloom filter must at minimum be 1000 in size
        if(bloom_init2(&bloomFilter, 1000, 0.01)) {
            std::cerr << "Failed to initialize bloom filter" << std::endl;
        }
    }
};

class FolderDataSink : public DataSink
{
public:
    FolderDataSink(std::function<std::string(std::string)> getPathFunc);
    bool writeNext(json in);
    void start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue);
    ~FolderDataSink();

private:
    std::function<std::string(std::string)> _getPathFunc;
    std::map<std::string, FolderData> _pathToHook;
    std::thread _thread;
    const int BUNDLE_SIZE = 1000;
};
