#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::vector<json> SearchRecords(std::string name);