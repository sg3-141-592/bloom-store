#pragma once

#include "DataSource.h"

#include <fstream>
#include <optional>
#include <string>

class FileDataSource : public DataSource {
private:
    std::ifstream file;
public:
    FileDataSource(std::string filename);
    std::optional<std::string> readNext();
    ~FileDataSource();
};
