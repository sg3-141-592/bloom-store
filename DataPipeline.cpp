#include "DataPipeline.h"

#include "RecordProcessor/JsonDeserializer.h"

#include <iostream>
#include <optional>
#include <typeinfo>

#include <boost/lockfree/spsc_queue.hpp>

DataPipeline::DataPipeline(std::shared_ptr<DataSource> source, std::shared_ptr<DataSink> sink)
{
    _source = source;
    _sink = sink;
};

void DataPipeline::process()
{
    // Setup a queue for the source to processor
    auto sourceToProcessorQueue = std::make_shared<boost::lockfree::spsc_queue<std::string>>(1024);

    // Create the Source
    _source->start(sourceToProcessorQueue);
    
    //
    std::optional<std::string> line;
    while ((line = _source->readNext()))
    {
        std::string result;
        sourceToProcessorQueue->consume_one([&result](const std::string& data) {
            result = data;
        });
        for (const auto& processor : _processors)
        {
            if (processor->getInstanceType() == typeid(JsonDeserializer)) {
                auto castedProcessor = dynamic_cast<JsonDeserializer*>(processor.get());
                _sink->writeNext(castedProcessor->process(*line));
            }
        }
    }
    
};