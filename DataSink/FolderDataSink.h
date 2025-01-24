#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <thread>

#include "bloom_filter.hpp"

struct FolderData
{
    int recordCount = 0;
    int bundleId = 0;
    std::string buffer;
    bloom_filter filter;

    FolderData() : filter([]()
                          {
       bloom_parameters parameters;
       parameters.projected_element_count = 256; // TODO: This should be driven off a constant BUNDLE_SIZE
       parameters.false_positive_probability = 0.01;
       parameters.compute_optimal_parameters();
       return parameters; }())
    {
        // Pre-allocate since we'll be appending to this a lot
        buffer.reserve(1024);
    }
};

class FolderDataSink : public DataSink
{
public:
    FolderDataSink(std::function<std::string(json)> getPathFunc);
    bool writeNext(json in);
    void start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue);
    ~FolderDataSink();

private:
    std::function<std::string(json)> _getPathFunc;
    std::map<std::string, FolderData> _pathToHook;
    std::thread _thread;
    const int BUNDLE_SIZE = 256;
};
