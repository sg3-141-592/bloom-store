#include <iostream>

#include "DataSource/FileDataSource.h"
#include "DataSink/FolderDataSink.h"
#include "RecordProcessor/JsonDeserializer.h"
#include "DataPipeline.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Config.h"

int main()
{
    // Load config on start-up
    Config config;

    // /home/sean/bloom-filter-store/build/dataset/mbdump/artist
    std::shared_ptr<FileDataSource> fileSource = std::make_shared<FileDataSource>("./artists.json");
    std::shared_ptr<JsonDeserializer> deserializer = std::make_shared<JsonDeserializer>();
    std::shared_ptr<FolderDataSink> folderSink = std::make_shared<FolderDataSink>(get_path_func);

    DataPipeline pipeline(fileSource, folderSink, config);

    pipeline.addProcessor<std::string, json>(deserializer);

    pipeline.process();

    return 0;
}