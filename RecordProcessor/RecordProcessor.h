#pragma once

#include <atomic>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../DataSource/DataSource.h"
#include "../Utilities/CommonTypes.h"
#include "../Utilities/TSQueue.h"

class AbstractProcessor {
public:
  virtual void start(std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
                     std::shared_ptr<TSQueue<GenericRecord>> sinkQueue) = 0;
  virtual bool isCompleted() { return completed; };
  virtual void stop() = 0;
  virtual ~AbstractProcessor() = default;

protected:
  std::atomic<bool> completed = false;
  std::atomic<bool> _stopFlag{false}; // Used for handling cancelling processing
};
