#include <atomic>
#include <memory>
#include <vector>

#include "RecordProcessor/RecordProcessor.h"
#include "DataSource/DataSource.h"
#include "DataSink/DataSink.h"
#include "Config.h"

class DataPipeline {
public:
    DataPipeline(std::shared_ptr<DataSource<std::string, std::streampos>> source, std::shared_ptr<DataSink> sink, std::shared_ptr<RecordProcessor<std::string, json>> processor, Config config);
    
    void process();
    void stop();
private:
    std::shared_ptr<RecordProcessor<std::string, json>> _processor;
    std::shared_ptr<DataSource<std::string, std::streampos>> _source;
    std::shared_ptr<DataSink> _sink;
    Config _config;
};

