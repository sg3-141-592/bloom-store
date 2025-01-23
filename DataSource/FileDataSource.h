#pragma once

#include "DataSource.h"

#include <fstream>
#include <optional>
#include <string>

class FileDataSource : public DataSource {
private:
    std::ifstream file;
    std::string _filename;
    std::shared_ptr<boost::lockfree::spsc_queue<std::string>> _queue;
public:
    FileDataSource(std::string filename);
    std::optional<std::string> readNext();
    void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue);
    ~FileDataSource();
};
