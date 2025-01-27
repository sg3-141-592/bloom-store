#pragma once

#include <atomic>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../Config.h"
#include "../Utilities/CommonTypes.h"
#include "../Utilities/TSQueue.h"

class DataSink {
public:
  virtual auto writeNext(json itemIn) -> bool = 0;
  virtual ~DataSink() = default;
  virtual void start(std::shared_ptr<TSQueue<GenericRecord>> queue) = 0;
  virtual auto isCompleted() -> bool { return completed; };
  virtual void stop() = 0;

protected:
  std::atomic<bool> completed = false;
  std::atomic<bool> _stopFlag{false}; // Used for handling cancelling processing
};
