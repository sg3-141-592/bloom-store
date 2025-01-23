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

void JsonDeserializer::start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue)
{
    _sourceQueue = queue;

    _thread = std::thread([this]() {
        // Read from the queue while there's still messages in it
        std::cout << "Starting processing messages\n";
        std::string message;
        while (true)
        {
            if (_sourceQueue->pop(message))
            {
                if (message != "EOF")
                {
                    // std::cout << "Processing message: " << message << std::endl;
                    // Process the message
                    json processedMessage = process(message);
                    // std::cout << "Processed message: " << processedMessage.dump() << std::endl;
                } else {
                    break;
                }
            }
        }
        std::cout << "Finished processing messages\n"; 
    });
};

JsonDeserializer::~JsonDeserializer() {
    _thread.join();
};
