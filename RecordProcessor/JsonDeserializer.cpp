#include "JsonDeserializer.h"

#include <iostream>

JsonDeserializer::JsonDeserializer()
{
};

json JsonDeserializer::process(std::string in)
{
    auto result = json::parse(in);

    json extracted;
    if (result.contains("id")) {
        extracted["id"] = result["id"];
    }
    if (result.contains("name")) {
        extracted["name"] = result["name"];
    }
    if (result.contains("country")) {
        extracted["country"] = result["country"];
    }
    if (result.contains("type")) {
        extracted["type"] = result["type"];
    }
    
    return extracted;
};
