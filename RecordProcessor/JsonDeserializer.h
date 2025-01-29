#include <string>
#include <thread>

#include "RecordProcessor.h"

#include "../MetricsTracker.h"
#include "../Utilities/CommonTypes.h"

class JsonDeserializer : public AbstractProcessor {
public:
  void start(std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
             std::shared_ptr<TSQueue<GenericRecord>> sinkQueue) override;
  void stop() override;
  ~JsonDeserializer() override;

private:
  static void extractField(const json& source, json& target, const std::string& fieldName);
  JsonRecord process(const StringRecord& in);
  
  std::thread _thread;
  std::unique_ptr<MetricsTracker> _metricsTracker = std::make_unique<MetricsTracker>("JsonDeserializer");
};