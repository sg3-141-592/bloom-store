#pragma once

#include <atomic>
#include <memory>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>

#include "../CommonData.h"

class DataSource {
public:
    virtual ~DataSource() = default;
    virtual void start(std::shared_ptr<boost::lockfree::spsc_queue<Record>> queue) = 0;
    virtual bool isCompleted() { return completed.load(); };
    virtual void stop() = 0;
protected:
    std::atomic<bool> completed = false;
    std::atomic<bool> _stopFlag{false}; // Used for handling cancelling processing
};
