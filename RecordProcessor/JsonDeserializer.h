#include <string>
#include <thread>

#include "RecordProcessor.h"

#include "../MetricsTracker.h"

class JsonDeserializer : public RecordProcessor<std::string, json>
{
private:
    std::shared_ptr<TSQueue<Record<std::string, std::streampos>>> _sourceQueue;
    std::thread _thread;
    MetricsTracker* _metricsTracker;
public:
    JsonDeserializer();
    json process(std::string in);
    void start(std::shared_ptr<TSQueue<Record<std::string, std::streampos>>> sourceQueue, std::shared_ptr<TSQueue<Record<json, std::streampos>>> sinkQueue);
    void stop();
    ~JsonDeserializer();
};