#include "JsonDeserializer.h"

#include <iostream>

JsonDeserializer::JsonDeserializer()
{
    _metricsTracker = std::make_unique<MetricsTracker>("JsonDeserializer");
};

inline json JsonDeserializer::process(std::string in)
{
    auto result = json::parse(in);

    json extracted;
    if (result.contains("id"))
    {
        extracted["id"] = result["id"];
    }
    if (result.contains("name"))
    {
        extracted["name"] = result["name"];
    }
    if (result.contains("country"))
    {
        extracted["country"] = result["country"];
    }
    if (result.contains("type"))
    {
        extracted["type"] = result["type"];
    }
    if (result.contains("genres"))
    {
        extracted["genres"] = json::array();
        for (const auto &genre : result["genres"])
        {
            if (genre.contains("name"))
            {
                extracted["genres"].push_back(genre["name"]);
            }
        }
    }

    return extracted;
};

void JsonDeserializer::start(std::shared_ptr<TSQueue<StringRecord>> sourceQueue,
                             std::shared_ptr<TSQueue<JsonRecord>> sinkQueue)
{
    // Ensure the thread is not already running
    if (_thread.joinable()) {
        throw std::runtime_error("Already running!");
    }

    _thread = std::thread([this, sourceQueue, sinkQueue]()
                          {
        std::cout << "Starting processing messages\n";

        
        while (!_stopFlag)
        {
            Record message = sourceQueue->pop();
            if (message.data == "EOF" && message.checkpoint == -1) {
                break;
            }

            json processedMessage = process(std::move(message.data));
            
            while (!sinkQueue->try_push(JsonRecord{std::move(processedMessage), message.checkpoint}))
            {
                std::this_thread::yield();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            _metricsTracker->recordMessage();
            _metricsTracker->printMetricsIfNeeded();
        }
        // Pushing an end of stream marker
        sinkQueue->push(JsonRecord{json::object(), -1});

        std::cout << "Finished processing messages\n";

        completed.store(true); });
}

void JsonDeserializer::stop()
{
    _stopFlag.store(true);
    if (_thread.joinable())
    {
        _thread.join();
    }
}

JsonDeserializer::~JsonDeserializer()
{
    stop();
};
