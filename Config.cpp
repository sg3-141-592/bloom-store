#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Config.h"

std::string get_path_func(std::string name)
{
    if (!name.empty())
    {
        char first_char = std::tolower(name[0]);
        if (isalnum(first_char))
        {
            return "archive/" + std::string(1, first_char) + "/";
        }
        else
        {
            return "archive/unknown/";
        }
    }
    return "out/unknown";
}

Config::Config(std::string path)
{
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(path, pt);

    try {
        generalConfig.QueueSize = pt.get<int>("General.queue_size");
    } catch (const boost::property_tree::ptree_bad_path& e) {
        // Take the default value off the struct
    }

    try {
        sourceConfig.Path = pt.get<std::string>("Source.path");
    } catch (const boost::property_tree::ptree_bad_path& e) {
        std::cerr << "Fatal error: Missing 'Source.path' in configuration file." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    try {
        sinkConfig.NumberItemsPerBundle = pt.get<int>("Sink.number_items_per_bundle");
    } catch (const boost::property_tree::ptree_bad_path& e) {
        // Take the default value off the struct
    }

    try {
        sinkConfig.BloomFalsePositiveProbability = pt.get<float>("Sink.bloom_false_positive_probability");
    } catch (const boost::property_tree::ptree_bad_path& e) {
        // Take the default value off the struct
    }
};