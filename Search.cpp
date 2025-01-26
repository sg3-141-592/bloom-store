#include <fstream>
#include <iostream>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <argparse/argparse.hpp>
#include "bloom.h"

#include "DataSink/CompressBundle.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("Search");
    program.add_argument("--name")
        .help("Artist name to search for")
        .required();
    program.add_argument("--type")
        .help("Artist type to search for, e.g. 'Person', 'Group'")
        .default_value(std::string(""));
    
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::string testString = program.get<std::string>("name");

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