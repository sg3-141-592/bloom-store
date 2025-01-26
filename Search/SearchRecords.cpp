#include "SearchRecords.h"

#include <iostream>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "bloom.h"

#include "../Config.h"
#include "../DataSink/CompressBundle.h"

void SearchRecords(std::string name) {
    std::string path = get_path_func(name);

    // Iterate over all the bloom filters in the directory for matches
    std::vector<std::string> bloomFiles;
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".bloom")
        {
            bloomFiles.push_back(entry.path().string());
        }
    }

    for (auto &bloomFile : bloomFiles)
    {
        bloom bloomFilter;
        bloom_load(&bloomFilter, const_cast<char *>(bloomFile.c_str()));

        if (bloom_check(&bloomFilter, name.c_str(), name.length()))
        {
            bloomFile.erase(bloomFile.end() - 6, bloomFile.end());
            std::string jsonGzFile = bloomFile + ".json.gz";
            std::string result = readGzipFileToString(jsonGzFile);
            std::istringstream stream(result);
            std::string line;
            while (std::getline(stream, line))
            {
                json result = json::parse(line);
                if (result["name"] == name)
                {
                    std::cout << result.dump() << std::endl;
                }
            }
        }

        bloom_free(&bloomFilter);
    }
}