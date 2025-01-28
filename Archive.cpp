#include <csignal>
#include <iostream>
#include <memory>

#include "DataPipeline.h"
#include "DataSink/FolderDataSink.h"
#include "DataSource/FileDataSource.h"
#include "DataSource/SqliteDataSource.h"
#include "RecordProcessor/JsonDeserializer.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Config.h"

void signalHandler(int signum);

std::unique_ptr<DataPipeline> pipeline;

int main() {
  // Make Archive program interruptible
  if (signal(SIGINT, signalHandler) == SIG_ERR) {
    std::cerr << "Error: Unable to register signal handler." << std::endl;
    return EXIT_FAILURE;
  }

  // Load default config.ini on start-up
  auto config = std::make_shared<Config>();

  auto fileSource = std::make_shared<FileDataSource>(config);
  // auto sqliteSource = std::make_shared<SqliteDataSource>(config);
  auto folderSink = std::make_shared<FolderDataSink>(get_path_func, config);

  pipeline = std::make_unique<DataPipeline>(fileSource, folderSink, config);

  pipeline->process();

  return 0;
}

void signalHandler(int signum) {

  // Send a signal to the DataProcessor to complete gracefully
  pipeline->stop();

  exit(signum);
}