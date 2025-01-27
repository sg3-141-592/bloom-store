#include <atomic>
#include <memory>
#include <vector>

#include "Config.h"
#include "DataSink/DataSink.h"
#include "DataSource/DataSource.h"
#include "RecordProcessor/RecordProcessor.h"

class DataPipeline {
public:
  DataPipeline(std::shared_ptr<DataSource> source,
               std::shared_ptr<DataSink> sink, std::shared_ptr<Config> config);

  void process();
  void stop();

private:
  std::vector<std::shared_ptr<AbstractProcessor>> _processors;
  std::shared_ptr<DataSource> _source;
  std::shared_ptr<DataSink> _sink;
  std::shared_ptr<Config> _config;
};
