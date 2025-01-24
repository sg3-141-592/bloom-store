#include "FolderDataSink.h"

#include "CompressBundle.hpp"

#include <filesystem>
#include <iostream>

FolderDataSink::FolderDataSink(std::function<std::string(json)> getPathFunc)
{
    _getPathFunc = getPathFunc;
}

bool FolderDataSink::writeNext(json in) 
{
    std::filesystem::path dir = std::filesystem::path(_getPathFunc(in)).parent_path();
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }
    
    const std::string path = _getPathFunc(in);

    _pathToHook[path].buffer.append(in.dump() + "\n");
    _pathToHook[path].recordCount++;

    // Check if we should move to the next bundle
    if (_pathToHook[path].recordCount >= BUNDLE_SIZE) {
        _pathToHook[path].recordCount = 0;
        _pathToHook[path].bundleId++;

        // Write the buffer to the file
        writeStringToGzipFile(_pathToHook[path].buffer, path + std::to_string(_pathToHook[path].bundleId) + ".json.gz");
        
        _pathToHook[path].buffer.clear();
    }
    
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
            writeStringToGzipFile(hook.buffer, path + std::to_string(hook.bundleId) + ".json.gz");
        }

        std::cout << "Finished writing messages\n";
    });
}

FolderDataSink::~FolderDataSink()
{
    _thread.join();
}