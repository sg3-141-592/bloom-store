#include <iostream>

#include "DataSource/FileDataSource.h"
#include "DataSink/FolderDataSink.h"
#include "RecordProcessor/JsonDeserializer.h"
#include "DataPipeline.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "config.h"

int main()
{
    std::shared_ptr<FileDataSource> fileSource = std::make_shared<FileDataSource>("./artists.json");
    
    auto sourceToProcessorQueue = std::make_shared<boost::lockfree::spsc_queue<std::string>>(10024);
    auto processorToSinkQueue = std::make_shared<boost::lockfree::spsc_queue<json>>(10024);

    fileSource->start(sourceToProcessorQueue);

    std::shared_ptr<JsonDeserializer> deserializer = std::make_shared<JsonDeserializer>();
    deserializer->start(sourceToProcessorQueue, processorToSinkQueue);
    

    std::shared_ptr<FolderDataSink> folderSink = std::make_shared<FolderDataSink>(get_path_func);
    folderSink->start(processorToSinkQueue);

    // DataPipeline pipeline(fileSource, folderSink);

    // pipeline.addProcessor<std::string, json>(deserializer);

    // pipeline.process();

    return 0;
}