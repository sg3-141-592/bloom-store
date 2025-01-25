#include <fstream>
#include <iostream>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include "bloom.h"

#include "config.hpp"

int main()
{
    std::string testString("Shark Island");

    // Find the path for search term
    std::string path = get_path_func(testString);

    // Iterate over all the bloom filters in the directory for matches
    std::vector<std::string> bloomFiles;
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".bloom")
        {
            bloomFiles.push_back(entry.path().string());
        }
    }

    for (auto &bloomFile : bloomFiles)
    {
        bloom bloomFilter;
        bloom_load(&bloomFilter, const_cast<char*>(bloomFile.c_str()));

        bloom_print(&bloomFilter);

        std::cout << "Filter Contains " << testString << ": " << bloom_check(&bloomFilter, testString.c_str(), testString.length()) << std::endl;

        bloom_free(&bloomFilter);
    }

    return 0;
}