#include <fstream>
#include <iostream>
#include <vector>

#include "bloom.h"

int main()
{
    bloom bloomFilter;
    bloom_load(&bloomFilter, const_cast<char*>("archive/s/0.bloom"));

    bloom_print(&bloomFilter);

    std::string testString("Shark Island");
    std::cout << "Filter Contains " << testString << ": " << bloom_check(&bloomFilter, testString.c_str(), testString.length()) << std::endl;

    return 0;
}