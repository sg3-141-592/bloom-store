#pragma once

#include <atomic>
#include <iostream>
#include <typeinfo>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/lockfree/spsc_queue.hpp>

#include "../DataSource/DataSource.h"
#include "../Utilities/TSQueue.h"

class AbstractProcessor {
public:
    virtual ~AbstractProcessor() = default; // Virtual destructor
    virtual void process() = 0; // Pure virtual function
    virtual void start(std::shared_ptr<TSQueue<Record>> sourceQueue, std::shared_ptr<TSQueue<json>> sinkQueue) = 0;
    virtual bool isCompleted() { return completed; };
    virtual void stop() = 0;
protected:
    std::atomic<bool> completed = false;
    std::atomic<bool> _stopFlag{false}; // Used for handling cancelling processing
};

template<typename InputType, typename OutputType>
class RecordProcessor : public AbstractProcessor {
public:
    virtual OutputType process(InputType in) = 0; // Pure virtual function
    void process() override {
        // Needed otherwise the derived classes would think they're abstract
    };
    void start(std::shared_ptr<TSQueue<Record>> sourceQueue, std::shared_ptr<TSQueue<json>> sinkQueue) override {
        // Needed otherwise the derived classes would think they're abstract
    };
};