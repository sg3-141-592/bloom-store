#pragma once

#include <string>

std::string get_path_func(std::string name);

class Config {
public:
    Config();

    struct GeneralConfig {
        int QueueSize = 1024;
    } generalConfig;

    struct SourceConfig {
        std::string Path;
    } sourceConfig;
};