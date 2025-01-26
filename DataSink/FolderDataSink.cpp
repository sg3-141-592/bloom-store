#include "FolderDataSink.h"

#include "CompressBundle.h"

#include <filesystem>
#include <iostream>

FolderDataSink::FolderDataSink(std::function<std::string(std::string)> getPathFunc)
{
    _getPathFunc = getPathFunc;
    _metricsTracker = std::make_unique<MetricsTracker>("FolderDataSink");
}

auto FolderDataSink::writeNext(json itemIn) -> bool
{
    std::filesystem::path dir = std::filesystem::path(_getPathFunc(itemIn["name"])).parent_path();
    if (!std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    const std::string path = _getPathFunc(itemIn["name"]);

    // Extract index value from the json object and insert it into the bloom filter
    bloom_add(&_pathToHook[path].bloomFilter, itemIn["name"].get<std::string>().c_str(), itemIn["name"].get<std::string>().length());

    _pathToHook[path].buffer.append(itemIn.dump() + "\n");
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

void FolderDataSink::start(std::shared_ptr<TSQueue<JsonRecord>> queue)
{
    _thread = std::thread([this, queue]()
                          {
        std::cout << "Starting writing messages\n";

        JsonRecord itemIn;
        while (!_stopFlag) {
            itemIn = queue->pop();
            if (itemIn.data.empty()) {  // Use empty() instead of comparing to empty object
                break;
            }
            writeNext(std::move(itemIn.data));  // Use move semantics
            _metricsTracker->recordMessage();
            _metricsTracker->printMetricsIfNeeded();
        }

        // Finish writing all of the files    
        for (auto& [path, hook] : _pathToHook) {
            writeStringToGzipFile(hook.buffer, path + std::to_string(hook.bundleId) + ".json.gz");
            bloom_save(&hook.bloomFilter, const_cast<char*>((path + std::to_string(hook.bundleId) + ".bloom").c_str()));
        }

        std::cout << "Finished writing messages\n"; });

        completed.store(true);
}

void FolderDataSink::stop()
{
    _stopFlag.store(true);
    if (_thread.joinable())
    {
        _thread.join();
    }
}

FolderDataSink::~FolderDataSink()
{
    stop();
}