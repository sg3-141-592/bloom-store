#include <fstream>
#include <iostream>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <argparse/argparse.hpp>
#include "bloom.h"

#include "Config.h"
#include "DataSink/CompressBundle.h"
#include "Search/SearchRecords.h"

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

    auto results = SearchRecords(program.get<std::string>("name"));

    for (auto &result : results)
    {
        std::cout << result.dump() << std::endl;
    }

    return 0;
}