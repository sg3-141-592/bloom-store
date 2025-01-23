#include "RecordProcessor.h"

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ExtractFields : public RecordProcessor<json, std::string> {
public:
    ExtractFields();
    std::string process(json in);
};