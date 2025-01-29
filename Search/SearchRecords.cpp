#include "SearchRecords.h"

#include <atomic>
#include <iostream>
#include <vector>
#include <future>
#include <filesystem>
namespace fs = std::filesystem;

#include "bloom.h"

#include "../Config.h"
#include "../DataSink/CompressBundle.h"
#include "../Utilities/CommonTypes.h"

json processBloomFile(std::string filename, std::string name) {
    
    bloom bloomFilter;
    bloom_load(&bloomFilter, const_cast<char *>(filename.c_str()));

    if (bloom_check(&bloomFilter, name.c_str(),
                    static_cast<int>(name.length()))) {
        
        // TODO: Can use a shared pointer in future to ensure bloom_free is called
        bloom_free(&bloomFilter);

        try {
            auto jsonPath = filename.substr(0, filename.length() - 6) + ".json.gz";
            auto jsonContent = readGzipFileToString(jsonPath);
            
            std::istringstream stream(jsonContent);
            std::string line;
            while (std::getline(stream, line)) {
                auto result = json::parse(line);
                if (result["name"] == name) {
                    return result;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing file " << filename << ": " << e.what() << std::endl;
        }
    }

    bloom_free(&bloomFilter);

    return nullptr;
}

std::vector<json> SearchRecords(std::string name, std::string type) {
    std::string path = GetPathFunc(name, type);

    // Iterate over all the bloom filters in the directory for matches
    std::vector<std::string> bloomFiles;
    for (const auto &entry : fs::recursive_directory_iterator(path)) {
        if (type.empty()) {
            if (entry.path().extension() == ".bloom" &&
                entry.path().string().find(type) != std::string::npos) {
                bloomFiles.push_back(entry.path().string());
            }
        } else {
            if (entry.path().extension() == ".bloom") {
                bloomFiles.push_back(entry.path().string());
            }
        }
    }

    // Launch async tasks for each bloom file
    std::vector<std::future<json>> futures;
    for (const auto &bloomFile : bloomFiles) {
        futures.push_back(
            std::async(std::launch::async,
                      processBloomFile,
                      bloomFile,
                      name)
        );
    }

    // Collect results
    std::vector<json> results;
    for (auto &future : futures) {
        auto result = future.get();
        if (result != nullptr) {
            results.push_back(std::move(result));
        }
    }

    return results;
}