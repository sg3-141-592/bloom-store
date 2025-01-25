#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string get_path_func(std::string name) {
    if (!name.empty()) {
        char first_char = std::tolower(name[0]);
        if(isalnum(first_char)) {
            return "archive/" + std::string(1, first_char) + "/";
        } else {
            return "archive/unknown/";
        }
    }
    return "out/unknown";
}

class Config {
public:
    Config() {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini("config.ini", pt);

        generalConfig.QueueSize = pt.get<int>("General.queue_size");
    };

    struct GeneralConfig {
        int QueueSize = 1024;
    } generalConfig;
};