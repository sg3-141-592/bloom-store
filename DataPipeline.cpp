#include "DataPipeline.h"

#include "RecordProcessor/JsonDeserializer.h"
#include "RecordProcessor/ExtractFields.h"

#include <iostream>
#include <optional>
#include <typeinfo>

DataPipeline::DataPipeline(std::shared_ptr<DataSource> source, std::shared_ptr<DataSink> sink)
{
    _source = source;
    _sink = sink;
};

void DataPipeline::process()
{
    std::optional<std::string> line;
    while ((line = _source->readNext()))
    {
        for (const auto& processor : _processors)
        {
            if (processor->getInstanceType() == typeid(JsonDeserializer)) {
                auto castedProcessor = dynamic_cast<JsonDeserializer*>(processor.get());
                std::cout << castedProcessor->process(*line) << std::endl;
                _sink->writeNext(*line);
            }
        }
    }
    
};