#pragma once

#include "DataSink.h"

#include <functional>
#include <string>

class FolderDataSink : public DataSink {
public:
    FolderDataSink(std::function<std::string(std::string)> getPathFunc);
    bool writeNext(std::string in);
    ~FolderDataSink();
private:
    std::function<std::string(std::string)> _getPathFunc;
};
