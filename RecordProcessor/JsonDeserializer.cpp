#include "JsonDeserializer.h"

#include <iostream>

JsonDeserializer::JsonDeserializer() {
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

    return extracted;
};

void JsonDeserializer::start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> sourceQueue, 
                              std::shared_ptr<boost::lockfree::spsc_queue<json>> sinkQueue)
{
    _thread = std::thread([this, sourceQueue, sinkQueue]()
    {
        std::cout << "Starting processing messages\n";

        std::string message;
        while (true)
        {
            if (sourceQueue->pop(message))
            {
                if (message == "EOF") {
                    break;
                }

                json processedMessage = process(message);
                
                while (!sinkQueue->push(std::move(processedMessage)))
                {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        sinkQueue->push(json::object());

        std::cout << "Finished processing messages\n";
    }); 
}

JsonDeserializer::~JsonDeserializer()
{
    _thread.join();
};
