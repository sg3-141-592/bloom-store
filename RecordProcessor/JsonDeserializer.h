#include <string>
#include <thread>

#include "RecordProcessor.h"

#include "../MetricsTracker.h"

class JsonDeserializer : public RecordProcessor<std::string, json>
{
private:
    std::shared_ptr<TSQueue<Record>> _sourceQueue;
    std::thread _thread;
    MetricsTracker* _metricsTracker;
public:
    JsonDeserializer();
    json process(std::string in);
    void start(std::shared_ptr<TSQueue<Record>> sourceQueue, std::shared_ptr<boost::lockfree::spsc_queue<json>> sinkQueue);
    void stop();
    ~JsonDeserializer();
};