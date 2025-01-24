#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <thread>

struct FolderData {
    std::ofstream file;
    int recordCount = 0;
    int bundleId = 0;
    std::string buffer;

    FolderData() {
        // Pre-allocate since we'll be appending to this a lot
        buffer.reserve(1024);
    }
};

class FolderDataSink : public DataSink {
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
