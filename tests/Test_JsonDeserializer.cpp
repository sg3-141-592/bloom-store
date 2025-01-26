#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/lockfree/spsc_queue.hpp>

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
    // Create a test queue of records
    auto sourceQueue = std::make_shared<TSQueue<Record<std::string, std::streampos>>>(128);
    auto sinkQueue = std::make_shared<TSQueue<Record<json, std::streampos>>>(128);

    JsonDeserializer deserializer;
    deserializer.start(sourceQueue, sinkQueue);

    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::seconds(5);

    // Push test messages to the queue
    Record<std::string, std::streampos> record{R"({"key": "value"})", 0};
    sourceQueue->push(record);
    Record<std::string, std::streampos> record2{"EOF", -1};
    sourceQueue->push(record2);

    while (!deserializer.isCompleted())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (std::chrono::steady_clock::now() - start > timeout)
        {
            FAIL() << "Test timed out waiting for deserializer to complete.";
            break;
        }
    }
}