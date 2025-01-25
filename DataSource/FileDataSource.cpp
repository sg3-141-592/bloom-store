#include "FileDataSource.h"

#include <iostream>

#define SLEEP_TIME 100 // how long to sleep when queues are full

FileDataSource::FileDataSource(std::string filename) {
    // TODO: Get rid of this unnecessary copy
    _filename = filename;
};

void FileDataSource::start(std::shared_ptr<boost::lockfree::spsc_queue<Record>> queue) {
    _thread = std::thread ([this, queue]() {
        std::cout << "Starting loading messages\n";

        std::ifstream infile(_filename);
        std::string nextline;
        while (getline(infile, nextline) && !_stopFlag) {

            // Get a checkpoint for the file
            std::streampos checkpoint = infile.tellg();
            
            // Push value to the queue
            if(!queue->push(Record{ nextline, checkpoint })) {
                // If can't push to the queue, then pause and try again
                while (!queue->push(Record{ nextline, checkpoint })) {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
                }
            }
        }
        queue->push(Record{ nextline, -1 });

        std::cout << "Finished loading messages\n";

        infile.close();

        _completed.store(true);
    });
}

FileDataSource::~FileDataSource() {
    _thread.join();
};

void FileDataSource::stop() {
    _stopFlag = true;
    _thread.join();
}