#pragma once

#include "DataSink.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>

class FolderDataSink : public DataSink {
public:
    FolderDataSink(std::function<std::string(json)> getPathFunc);
    bool writeNext(json in);
    ~FolderDataSink();
private:
    std::function<std::string(json)> _getPathFunc;
    std::map<std::string, std::ofstream> _pathToHook;
};
