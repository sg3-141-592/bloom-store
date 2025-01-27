#include <string>
#include <thread>

#include "RecordProcessor.h"

#include "../MetricsTracker.h"
#include "../Utilities/CommonTypes.h"

class JsonDeserializer : public AbstractProcessor {
public:
  JsonDeserializer();
  JsonRecord process(StringRecord in);
  void start(std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
             std::shared_ptr<TSQueue<GenericRecord>> sinkQueue);
  void stop();
  ~JsonDeserializer();

private:
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker;
};