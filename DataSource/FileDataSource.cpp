#include "FileDataSource.h"

#include <iostream>

FileDataSource::FileDataSource(std::string filename) {
    _filename = filename;
};

void FileDataSource::start(std::shared_ptr<boost::lockfree::spsc_queue<Record>> queue) {
    _thread = std::thread ([this, queue]() {
        std::cout << "Starting loading messages\n";

        std::ifstream infile(_filename);
        std::string sa;
        while (getline(infile, sa) && !_stopFlag) {

            // Get a checkpoint for the file
            std::streampos checkpoint = infile.tellg();
            
            // Push value to the queue
            if(!queue->push(Record{ sa, checkpoint })) {
                // If can't push to the queue, then pause and try again
                while (!queue->push(Record{ sa, checkpoint })) {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        queue->push(Record{ sa, -1 });

        std::cout << "Finished loading messages\n";

        infile.close();

        completed.store(true);
    });
}

FileDataSource::~FileDataSource() {
    _thread.join();
};

void FileDataSource::stop() {
    _stopFlag = true;
    _thread.join();
}