#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <thread>

class FolderDataSink : public DataSink {
public:
    FolderDataSink(std::function<std::string(json)> getPathFunc);
    bool writeNext(json in);
    void start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue);
    ~FolderDataSink();
private:
    std::function<std::string(json)> _getPathFunc;
    std::map<std::string, std::ofstream> _pathToHook;
    std::thread _thread;
};
