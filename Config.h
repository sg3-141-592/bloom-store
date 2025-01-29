#pragma once

#include <functional>
#include <string>

typedef std::function<std::string(std::string, std::string)> getPathFuncType;

std::string GetPathFunc(std::string name, std::string type = "");

struct GeneralConfig {
  int QueueSize = 1024;
  int NumberProcessingThreads = 1;
};

struct SourceConfig {
  std::string Path;
};

struct SinkConfig {
  unsigned int NumberItemsPerBundle = 1000;
  double BloomFalsePositiveProbability = 0.01;
  int CheckpointFrequency = 1000;
};

class Config {
public:
  Config() : Config("config.ini") {}
  Config(const std::string &path);

  GeneralConfig generalConfig;
  SourceConfig sourceConfig;
  SinkConfig sinkConfig;
};