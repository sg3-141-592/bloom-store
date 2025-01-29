#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Config.h"

std::string GetPathFunc(std::string name, std::string type) {
  // Normalise the folder names, there's some inconsistent casing of them on the dataset
  if (!type.empty()) {
    type[0] = std::toupper(type[0]);
  }
  
  const char first_char = std::tolower(name[0]);

  std::string pathString = "archive/" + (isalnum(first_char) > 0 ? std::string(1, first_char) : "unknown") + "/";
  pathString += (type.empty() ? "" : type + "/");

  return pathString;
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

Config::Config(const std::string &path) {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(path, pt);

  generalConfig.QueueSize = getOptionalConfigValue<int>(
      pt, "General.queue_size", generalConfig.QueueSize);
  generalConfig.NumberProcessingThreads =
      getOptionalConfigValue<int>(pt, "General.number_processing_threads",
                                  generalConfig.NumberProcessingThreads);
  sourceConfig.Path = getMandatoryConfigValue<std::string>(pt, "Source.path");
  sinkConfig.NumberItemsPerBundle = getOptionalConfigValue<unsigned int>(
      pt, "Sink.number_items_per_bundle", sinkConfig.NumberItemsPerBundle);
  sinkConfig.BloomFalsePositiveProbability =
      getOptionalConfigValue<double>(pt, "Sink.bloom_false_positive_probability",
                                    sinkConfig.BloomFalsePositiveProbability);
  sinkConfig.CheckpointFrequency = getOptionalConfigValue<int>(
      pt, "Sink.checkpoint_frequency", sinkConfig.CheckpointFrequency);
}