#include <fstream>
#include <iostream>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "bloom.h"

#include "DataSink/CompressBundle.h"
#include "Config.h"

int main()
{
    std::string testString("Tracy Chapman");

    // Find the path for search term
    std::string path = get_path_func(testString);

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

        // std::cout << "Filter Contains " << testString << ": " << bloom_check(&bloomFilter, testString.c_str(), testString.length()) << std::endl;
        if (bloom_check(&bloomFilter, testString.c_str(), testString.length()))
        {
            // std::cout << "Filter may contain " << testString << std::endl;
            bloomFile.erase(bloomFile.end() - 6, bloomFile.end());
            std::string jsonGzFile = bloomFile + ".json.gz";
            // std::cout << "Trying to open  " << jsonGzFile << std::endl; // Take bloom off the end of screen and add .json.gz
            std::string result = readGzipFileToString(jsonGzFile);
            std::istringstream stream(result);
            std::string line;
            while (std::getline(stream, line))
            {
                json result = json::parse(line);
                if (result["name"] == testString)
                {
                    std::cout << result.dump() << std::endl;
                    break;
                }
            }
        }

        bloom_free(&bloomFilter);
    }

    return 0;
}