#include "DataPipeline.h"

#include "RecordProcessor/JsonDeserializer.h"
#include "Utilities/CommonTypes.h"
#include "Utilities/TSQueue.h"

#include <iostream>
#include <optional>
#include <typeinfo>

#include <boost/lockfree/spsc_queue.hpp>

DataPipeline::DataPipeline(std::shared_ptr<DataSource<std::string, std::streampos>> source, std::shared_ptr<DataSink> sink, std::shared_ptr<RecordProcessor<std::string, json>> processor, Config config)
{
    _source = source;
    _sink = sink;
    _processor = processor;
    _config = config;
};

void DataPipeline::process()
{
    // Setup Source
    auto sourceToProcessorQueue = std::make_shared<TSQueue<StringRecord>>(_config.generalConfig.QueueSize);
    _source->start(sourceToProcessorQueue);

    // Setup Sink
    auto processorToSinkQueue = std::make_shared<TSQueue<JsonRecord>>(_config.generalConfig.QueueSize);
    _sink->start(processorToSinkQueue);

    // TODO: Start the specified number of thread processors
    _processor->start(sourceToProcessorQueue, processorToSinkQueue);

    // Wait for source, sink and processors to finish
    bool completed = false;
    while (!completed)
    {
        completed = _source->isCompleted() && _sink->isCompleted() && _processor->isCompleted();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
};

void DataPipeline::stop()
{
    _source->stop();
    _sink->stop();
    _processor->stop();
};