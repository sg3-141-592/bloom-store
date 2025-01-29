#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::vector<json> SearchRecords(const std::string &name, const std::string &type);