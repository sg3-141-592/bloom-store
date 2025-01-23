#include "FileDataSource.h"

#include <iostream>

FileDataSource::FileDataSource(std::string filename) {
    _filename = filename;
};

std::optional<std::string> FileDataSource::readNext() {
    std::string sa;
    if (getline(file, sa)) {
        // Push value to the queue
        _queue->push(sa);
        //
        return sa;
    } else {
        return std::nullopt;
    }
}

void FileDataSource::start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue) {
    _queue = queue;
    //
    file.open(_filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
    }
}

FileDataSource::~FileDataSource() {
    file.close();
};