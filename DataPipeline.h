#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "Config.h"
#include "DataSink/DataSink.h"
#include "DataSource/DataSource.h"
#include "RecordProcessor/RecordProcessor.h"

template <typename SourceType, typename SinkType>
class DataPipeline {
public:
  DataPipeline(
      std::shared_ptr<Config> config,
      getPathFuncType pathFunc = nullptr)
      : _config(config) {
    _source = std::make_shared<SourceType>(_config);
    _sink = std::make_shared<SinkType>(pathFunc, _config);
  }

  template <typename ProcessorType> void process();

  void stop();

private:
  std::vector<std::shared_ptr<AbstractProcessor>> _processors;
  std::shared_ptr<DataSource> _source;
  std::shared_ptr<DataSink> _sink;
  std::shared_ptr<Config> _config;
};


template <typename SourceType, typename SinkType>
template <typename ProcessorType>
void DataPipeline<SourceType, SinkType>::process() {
  // Setup Source
  auto sourceToProcessorQueue =
      std::make_shared<TSQueue<GenericRecord>>(_config->generalConfig.QueueSize);
  _source->start(sourceToProcessorQueue);

  // Setup Sink
  auto processorToSinkQueue =
      std::make_shared<TSQueue<GenericRecord>>(_config->generalConfig.QueueSize);
  _sink->start(processorToSinkQueue);

  // Create the number of processors specified in the config
  for (int i = 0; i < _config->generalConfig.NumberProcessingThreads; i++) {
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

template <typename SourceType, typename SinkType>
void DataPipeline<SourceType, SinkType>::stop() {
  _source->stop();
  for (const auto &processor : _processors) {
    processor->stop();
  }
  _sink->stop();
};
