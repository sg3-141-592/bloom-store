#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Config.h"

std::string get_path_func(std::string name, std::string type) {
  char first_char = std::tolower(name[0]);
  if (isalnum(first_char)) {
    if (type != "") {
      return "archive/" + std::string(1, first_char) + "/" + type + "/";
    } else {
      return "archive/" + std::string(1, first_char) + "/";
    }

  } else {
    if (type != "") {
      return "archive/unknown/" + type + "/";
    } else {
      return "archive/unknown/";
    }
  }
}

template <typename T>
T getOptionalConfigValue(const boost::property_tree::ptree &pt,
                         const std::string &key, const T &defaultValue) {
  try {
    return pt.get<T>(key);
  } catch (const boost::property_tree::ptree_bad_path &e) {
    return defaultValue;
  }
}

template <typename T>
T getMandatoryConfigValue(const boost::property_tree::ptree &pt,
                          const std::string &key) {
  try {
    return pt.get<T>(key);
  } catch (const boost::property_tree::ptree_bad_path &e) {
    std::cerr << "Fatal error: Missing '" << key << "' in configuration file."
              << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

Config::Config(std::string path) {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(path, pt);

  generalConfig.QueueSize = getOptionalConfigValue<int>(
      pt, "General.queue_size", generalConfig.QueueSize);
  generalConfig.NumberProcessingThreads =
      getOptionalConfigValue<int>(pt, "General.number_processing_threads",
                                  generalConfig.NumberProcessingThreads);
  sourceConfig.Path = getMandatoryConfigValue<std::string>(pt, "Source.path");
  sinkConfig.NumberItemsPerBundle = getOptionalConfigValue<int>(
      pt, "Sink.number_items_per_bundle", sinkConfig.NumberItemsPerBundle);
  sinkConfig.BloomFalsePositiveProbability =
      getOptionalConfigValue<float>(pt, "Sink.bloom_false_positive_probability",
                                    sinkConfig.BloomFalsePositiveProbability);
  sinkConfig.CheckpointFrequency = getOptionalConfigValue<int>(
      pt, "Sink.checkpoint_frequency", sinkConfig.CheckpointFrequency);
}