#include "RecordProcessor.h"

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonDeserializer : public RecordProcessor<std::string, json> {
public:
    JsonDeserializer();
    json process(std::string in);
};