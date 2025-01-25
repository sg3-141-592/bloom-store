#include "DataPipeline.h"

#include "RecordProcessor/JsonDeserializer.h"

#include <iostream>
#include <optional>
#include <typeinfo>

#include <boost/lockfree/spsc_queue.hpp>

DataPipeline::DataPipeline(std::shared_ptr<DataSource> source, std::shared_ptr<DataSink> sink, Config config)
{
    _source = source;
    _sink = sink;
    _config = config;
};

void DataPipeline::process()
{
    // Setup Source
    auto sourceToProcessorQueue = std::make_shared<boost::lockfree::spsc_queue<std::string>>(_config.generalConfig.QueueSize);
    _source->start(sourceToProcessorQueue);

    // Setup Sink
    auto processorToSinkQueue = std::make_shared<boost::lockfree::spsc_queue<json>>(_config.generalConfig.QueueSize);
    _sink->start(processorToSinkQueue);

    // Start the processors
    // auto nextQueue = std::make_shared<boost::lockfree::spsc_queue<json>>(1024);
    for (const auto &processor : _processors)
    {
        
        // TODO: This will have to change in future to build the queues it needs to on the fly
        processor->start(sourceToProcessorQueue, processorToSinkQueue);
    }
};