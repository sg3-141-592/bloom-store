#pragma once

#include <memory>
#include <optional>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>

class DataSource {
public:
    virtual std::optional<std::string> readNext() = 0;
    virtual ~DataSource() = default;
    virtual void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue) = 0;
};
