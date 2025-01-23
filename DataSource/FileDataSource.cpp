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

    _thread = std::thread ([this]() {
        std::ifstream infile(_filename);
        std::string sa;
        while (getline(file, sa)) {
            // Push value to the queue
            if(!_queue->push(sa)) {
                // TODO: Add behaviour for dealing with the queue full
                std::cerr << "Error pushing value to the queue\n";
                std::this_thread::yield();
            }
        }
        _queue->push("EOF");

        std::cout << "Finished pushing to queue\n";

        infile.close();
    });
}

FileDataSource::~FileDataSource() {
    file.close();
    _thread.join();
};