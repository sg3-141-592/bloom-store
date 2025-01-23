#pragma once

#include <optional>
#include <string>

class DataSource {
public:
    virtual std::optional<std::string> readNext() = 0;
    virtual ~DataSource() = default;
};
