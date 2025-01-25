#pragma once

#include <atomic>
#include <memory>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>

#include "../CommonData.h"
#include "../Utilities/TSQueue.h"

class DataSource
{
public:
    virtual void start(std::shared_ptr<TSQueue<Record>> queue) = 0;
    virtual bool isCompleted() { return _completed.load(); };
    virtual void stop() = 0;
    virtual ~DataSource() = default;

protected:
    std::atomic<bool> _completed{false};
    std::atomic<bool> _stopFlag{false};
};
