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

int main(int argc, char *argv[])
{
    std::string testString;

    // Check if the command line arguments contain --name
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--name" && i + 1 < argc)
        {
            testString = argv[i + 1];
            break;
        }
    }

    if (testString.empty())
    {
        std::cerr << "Usage: " << argv[0] << " --name <name>" << std::endl;
        return 1;
    }

    // Rest of the code
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

        if (bloom_check(&bloomFilter, testString.c_str(), testString.length()))
        {
            bloomFile.erase(bloomFile.end() - 6, bloomFile.end());
            std::string jsonGzFile = bloomFile + ".json.gz";
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