#include <iostream>

#include "DataSource/FileDataSource.h"
#include "DataSink/FolderDataSink.h"
#include "RecordProcessor/JsonDeserializer.h"
#include "RecordProcessor/ExtractFields.h"
#include "DataPipeline.h"

#include "config.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
    std::shared_ptr<FileDataSource> fileSource = std::make_shared<FileDataSource>("./artists.json");
    std::shared_ptr<FolderDataSink> folderSink = std::make_shared<FolderDataSink>(get_path_func);

    DataPipeline pipeline(fileSource, folderSink);

    std::shared_ptr<JsonDeserializer> deserializer = std::make_shared<JsonDeserializer>();
    std::shared_ptr<ExtractFields> extractFields = std::make_shared<ExtractFields>();

    pipeline.addProcessor<std::string, json>(deserializer);
    pipeline.addProcessor<json, std::string>(extractFields);

    pipeline.process();

    return 0;
}