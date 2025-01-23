#include "RecordProcessor.h"

#include <string>
#include <thread>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonDeserializer : public RecordProcessor<std::string, json>
{
private:
    std::shared_ptr<boost::lockfree::spsc_queue<std::string>> _sourceQueue;
    std::thread _thread;

public:
    JsonDeserializer();
    json process(std::string in);
    void start(std::shared_ptr<boost::lockfree::spsc_queue<std::string>> queue);

    ~JsonDeserializer();
};