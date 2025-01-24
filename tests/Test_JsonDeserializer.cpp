#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../RecordProcessor/JsonDeserializer.h"

class JsonDeserializerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(JsonDeserializerTest, DeserializeJson)
{
    const std::string test_json = "{\"name\": \"John\", \"age\": 30, \"car\": null}";
    JsonDeserializer deserializer;
    try {
        json result =  json::parse(test_json);
        EXPECT_EQ(result["name"], "John");
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown: " << e.what();
    }
}