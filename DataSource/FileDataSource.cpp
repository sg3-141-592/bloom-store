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
    _thread = std::thread ([this, queue]() {
        std::cout << "Starting loading messages\n";

        std::ifstream infile(_filename);
        std::string sa;
        while (getline(infile, sa)) {

            // Get a checkpoint for the file
            std::streampos checkpoint = infile.tellg();
            
            // Push value to the queue
            if(!queue->push(sa)) {
                // If can't push to the queue, then pause and try again
                while (!queue->push(sa)) {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        queue->push("EOF");

        std::cout << "Finished loading messages\n";

        infile.close();
    });
}

FileDataSource::~FileDataSource() {
    _thread.join();
};