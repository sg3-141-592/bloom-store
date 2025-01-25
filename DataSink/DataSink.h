#pragma once

#include <atomic>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/lockfree/spsc_queue.hpp>

class DataSink {
public:
    virtual bool writeNext(json in) = 0;
    virtual ~DataSink() = default;
    virtual void start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue) = 0;
    virtual bool isCompleted() { return completed; };
protected:
    std::atomic<bool> completed = false;
};
