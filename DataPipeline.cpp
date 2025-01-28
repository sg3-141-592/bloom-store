#include "DataPipeline.h"

#include "RecordProcessor/JsonDeserializer.h"
#include "Utilities/CommonTypes.h"
#include "Utilities/ProgramLock.h"
#include "Utilities/TSQueue.h"

#include <iostream>
#include <optional>
#include <typeinfo>

DataPipeline::DataPipeline(
    std::shared_ptr<DataSource> source,
    std::shared_ptr<DataSink> sink,
    std::shared_ptr<Config> config)
{
  _source = source;
  _sink = sink;
  _config = config;
};

template <typename ProcessorType>
void DataPipeline::process() {
  // Setup Source
  auto sourceToProcessorQueue =
      std::make_shared<TSQueue<GenericRecord>>(_config->generalConfig.QueueSize);
  _source->start(sourceToProcessorQueue);

  // Setup Sink
  auto processorToSinkQueue =
      std::make_shared<TSQueue<GenericRecord>>(_config->generalConfig.QueueSize);
  _sink->start(processorToSinkQueue);

  // Create the number of processors specified in the config
  for (size_t i = 0; i < _config->generalConfig.NumberProcessingThreads; i++) {
    auto processor = std::make_shared<ProcessorType>();
    processor->start(sourceToProcessorQueue, processorToSinkQueue);
    _processors.push_back(processor);
  }

  // Wait for source, sink and processors to finish
  bool completed = false;
  while (!completed) {
    
    // Short pause to stop thrashing the `isCompleted` locks()
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    completed = _source->isCompleted() && _sink->isCompleted();
    for (auto processor : _processors) {
      completed = completed && processor->isCompleted();
    }
  }
};


// Have to explicitly instantiate this template, otherwise we'd need to make
// process<> visible in the header file
template void DataPipeline::process<JsonDeserializer>();

void DataPipeline::stop() {
  _source->stop();
  for (const auto processor : _processors) {
    processor->stop();
  }
  _sink->stop();
};