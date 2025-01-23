#include "RecordProcessor/RecordProcessor.h"
#include "DataSource/DataSource.h"
#include "DataSink/DataSink.h"

#include <memory>
#include <vector>

class DataPipeline {
public:
    DataPipeline(std::shared_ptr<DataSource> source, std::shared_ptr<DataSink> sink);
    
    template<typename InputType, typename OutputType>
    void addProcessor(std::shared_ptr<RecordProcessor<InputType, OutputType>> processor);
    void process();
private:
    std::vector<std::shared_ptr<AbstractProcessor>> _processors;
    std::shared_ptr<DataSource> _source;
    std::shared_ptr<DataSink> _sink;
};

// Template functions have to be visible in the header file
template<typename InputType, typename OutputType>
void DataPipeline::addProcessor(std::shared_ptr<RecordProcessor<InputType, OutputType>> processor)  
{
    _processors.push_back(processor);
};
