#pragma once

#include <typeinfo>

class AbstractProcessor {
public:
    virtual ~AbstractProcessor() = default; // Virtual destructor
    virtual void process() = 0; // Pure virtual function
    virtual std::type_info const& getInputType() const = 0;
    virtual std::type_info const& getOutputType() const = 0;
    virtual std::type_info const& getInstanceType() const = 0;
};

template<typename InputType, typename OutputType>
class RecordProcessor : public AbstractProcessor {
public:
    virtual OutputType process(InputType in) = 0; // Pure virtual function
    void process() override {
        // Needed otherwise the derived classes would think they're abstract
    };

    std::type_info const& getInputType() const override { return typeid(InputType); }
    std::type_info const& getOutputType() const override { return typeid(OutputType); }
    std::type_info const& getInstanceType() const override { return typeid(*this); }
};