#pragma once

#include <typeinfo>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/lockfree/spsc_queue.hpp>

class AbstractProcessor {
public:
    virtual ~AbstractProcessor() = default; // Virtual destructor
    virtual void process() = 0; // Pure virtual function
    virtual void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> sourceQueue, std::shared_ptr<boost::lockfree::spsc_queue<json>> sinkQueue) = 0; // Pure virtual function
};

template<typename InputType, typename OutputType>
class RecordProcessor : public AbstractProcessor {
public:
    virtual OutputType process(InputType in) = 0; // Pure virtual function
    void process() override {
        // Needed otherwise the derived classes would think they're abstract
    };
    void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> sourceQueue, std::shared_ptr<boost::lockfree::spsc_queue<json>> sinkQueue) override {
        // Needed otherwise the derived classes would think they're abstract
    };
};