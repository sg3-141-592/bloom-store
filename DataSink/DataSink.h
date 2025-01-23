#pragma once

#include <string>

class DataSink {
public:
    virtual bool writeNext(std::string in) = 0;
    virtual ~DataSink() = default;
};
