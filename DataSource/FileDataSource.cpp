#include "FileDataSource.h"

#include "../Utilities/TSQueue.h"

#include <iostream>

#define SLEEP_TIME 100 // how long to sleep when queues are full

FileDataSource::FileDataSource(std::string filename) {
    _filename = filename; // TODO: Get rid of this unnecessary copy (std::move)
    _metricsTracker = new MetricsTracker("FileDataSource");
};

void FileDataSource::start(std::shared_ptr<TSQueue<Record>> queue) {
    _thread = std::thread ([this, queue]() {
        std::cout << "Starting loading messages\n";

        std::ifstream infile(_filename);
        std::string nextline;
        while (getline(infile, nextline) && !_stopFlag) {

            // Get a checkpoint for the file
            std::streampos checkpoint = infile.tellg();
            
            // Push value to the queue
            if(!queue->try_push(Record{ nextline, checkpoint })) {
                // If can't push to the queue, then pause and try again
                while (!queue->try_push(Record{ nextline, checkpoint })) {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
                }
            }

            _metricsTracker->recordMessage();
            _metricsTracker->printMetricsIfNeeded();
        }

        // Push EOF to terminate the queue
        // TODO: Add handling for number of thread for EOF
        queue->push(Record{ "EOF", -1 });

        std::cout << "Finished loading messages\n";

        infile.close();

        _completed.store(true);
    });
}

FileDataSource::~FileDataSource() {
    _thread.join();
    delete _metricsTracker;
};

void FileDataSource::stop() {
    _stopFlag = true;
    _thread.join();
}