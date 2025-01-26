#include <string>
#include <thread>

#include "RecordProcessor.h"

#include "../Utilities/CommonTypes.h"
#include "../MetricsTracker.h"

class JsonDeserializer : public RecordProcessor<std::string, json>
{
private:
    std::shared_ptr<TSQueue<StringRecord>> _sourceQueue;
    std::thread _thread;
    MetricsTracker* _metricsTracker;
public:
    JsonDeserializer();
    json process(std::string in);
    void start(std::shared_ptr<TSQueue<StringRecord>> sourceQueue, std::shared_ptr<TSQueue<JsonRecord>> sinkQueue);
    void stop();
    ~JsonDeserializer();
};