#pragma once

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class DataSink {
public:
    virtual bool writeNext(json in) = 0;
    virtual ~DataSink() = default;
};
