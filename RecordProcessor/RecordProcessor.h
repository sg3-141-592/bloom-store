#pragma once

#include <typeinfo>

#include <boost/lockfree/spsc_queue.hpp>

class AbstractProcessor {
public:
    virtual ~AbstractProcessor() = default; // Virtual destructor
    virtual void process() = 0; // Pure virtual function
    virtual void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue) = 0; // Pure virtual function

    const std::type_info& getInstanceType() const {
        return typeid(*this);
    }
};

template<typename InputType, typename OutputType>
class RecordProcessor : public AbstractProcessor {
public:
    virtual OutputType process(InputType in) = 0; // Pure virtual function
    void process() override {
        // Needed otherwise the derived classes would think they're abstract
    };
    void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> sourceQueue) override {
        // Needed otherwise the derived classes would think they're abstract
    };
};