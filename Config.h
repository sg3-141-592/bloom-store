#pragma once

#include <string>

std::string get_path_func(std::string name);

struct GeneralConfig
{
    int QueueSize = 1024;
};

struct SourceConfig
{
    std::string Path;
};

struct SinkConfig
{
    int NumberItemsPerBundle = 1000;
    float BloomFalsePositiveProbability = 0.01f;
    int CheckpointFrequency = 1000;
};

class Config
{
public:
    Config() : Config("config.ini") {}
    Config(std::string path);

    GeneralConfig generalConfig;
    SourceConfig sourceConfig;
    SinkConfig sinkConfig;
};