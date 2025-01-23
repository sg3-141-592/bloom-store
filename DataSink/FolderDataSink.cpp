#include "FolderDataSink.h"

#include <iostream>

FolderDataSink::FolderDataSink(std::function<std::string(json)> getPathFunc)
{
    _getPathFunc = getPathFunc;
}

bool FolderDataSink::writeNext(json in) 
{
    const std::string path = _getPathFunc(in);
    // Check to see if we already have a hook to the current path
    // if not create one
    if (_pathToHook.find(path) == _pathToHook.end()) {
        _pathToHook[path] = std::ofstream(path, std::ios::app);
        if (!_pathToHook[path].is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return false;
        }
    }

    _pathToHook[path] << in.dump() << std::endl;
    
    return true;
}

void FolderDataSink::start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue)
{
    _thread = std::thread([this, queue]() {
        std::cout << "Starting writing messages\n";

        json sa;
        while (true) {
            if (queue->pop(sa)) {
                if (sa.empty()) {  // Use empty() instead of comparing to empty object
                    break;
                }
                writeNext(std::move(sa));  // Use move semantics
            }
        }

        // Finish writing all of the files    
        for (auto& [path, hook] : _pathToHook) {
            hook.close();
        }

        std::cout << "Finished writing messages\n";
    });
}

FolderDataSink::~FolderDataSink()
{
    _thread.join();
}