#include <csignal>
#include <iostream>

#include "DataPipeline.h"
#include "DataSink/FolderDataSink.h"
#include "DataSource/FileDataSource.h"
#include "RecordProcessor/JsonDeserializer.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Config.h"

void signalHandler(int signum);

DataPipeline *pipeline;

int main() {
  // Make Archive program interruptible
  if (signal(SIGINT, signalHandler) == SIG_ERR) {
    std::cerr << "Error: Unable to register signal handler." << std::endl;
    return EXIT_FAILURE;
  }

  // Load default config.ini on start-up
  Config config;

  auto fileSource =
      std::make_shared<FileDataSource>(config.sourceConfig.Path);
  auto folderSink =
      std::make_shared<FolderDataSink>(get_path_func);
  auto jsonProcessor = std::make_shared<JsonDeserializer>();

  pipeline = new DataPipeline(fileSource, folderSink, jsonProcessor, config);

  pipeline->process();

  return 0;
}

void signalHandler(int signum) {
  
  // Send a signal to the DataProcessor to complete gracefully
  pipeline->stop();

  exit(signum);
}