#include "FolderDataSink.h"

#include "CompressBundle.hpp"

#include <filesystem>
#include <iostream>

FolderDataSink::FolderDataSink(std::function<std::string(std::string)> getPathFunc)
{
    _getPathFunc = getPathFunc;
}

bool FolderDataSink::writeNext(json in)
{
    std::filesystem::path dir = std::filesystem::path(_getPathFunc(in["name"])).parent_path();
    if (!std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    const std::string path = _getPathFunc(in["name"]);

    // Extract index value from the json object and insert it into the bloom filter
    // _pathToHook[path].filter.insert(in["id"]);
    // std::cout << "Inserting " << in["name"].get<std::string>() << " into bloom filter" << std::endl;
    bloom_add(&_pathToHook[path].bloomFilter, in["name"].get<std::string>().c_str(), in["name"].get<std::string>().length());
    // _pathToHook[path].bloomFilter.insert(in["id"].get<int>());

    _pathToHook[path].buffer.append(in.dump() + "\n");
    _pathToHook[path].recordCount++;

    // Check if we should move to the next bundle
    if (_pathToHook[path].recordCount >= BUNDLE_SIZE)
    {
        _pathToHook[path].recordCount = 0;
        _pathToHook[path].bundleId++;

        // Write the buffer to the file
        writeStringToGzipFile(_pathToHook[path].buffer, path + std::to_string(_pathToHook[path].bundleId) + ".json.gz");

        bloom_save(&_pathToHook[path].bloomFilter, const_cast<char *>((path + std::to_string(_pathToHook[path].bundleId) + ".bloom").c_str()));
        bloom_reset(&_pathToHook[path].bloomFilter);
    }

    return true;
}

void FolderDataSink::start(std::shared_ptr<boost::lockfree::spsc_queue<json>> queue)
{
    _thread = std::thread([this, queue]()
                          {
        std::cout << "Starting writing messages\n";

        json sa;
        while (_stopFlag == false) {
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
            bloom_save(&hook.bloomFilter, const_cast<char*>((path + std::to_string(hook.bundleId) + ".bloom").c_str()));
            bloom_reset(&hook.bloomFilter);
            bloom_free(&hook.bloomFilter);
        }

        std::cout << "Finished writing messages\n"; });

        completed = true;
}

void FolderDataSink::stop()
{
    _stopFlag = true;
    _thread.join();
}

FolderDataSink::~FolderDataSink()
{
    _thread.join();
}