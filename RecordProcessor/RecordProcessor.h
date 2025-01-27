#pragma once

#include <atomic>
#include <iostream>
#include <typeinfo>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/lockfree/spsc_queue.hpp>

#include "../DataSource/DataSource.h"
#include "../Utilities/CommonTypes.h"
#include "../Utilities/TSQueue.h"

class AbstractProcessor {
public:
  virtual ~AbstractProcessor() = default;
  virtual void start(std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
                     std::shared_ptr<TSQueue<GenericRecord>> sinkQueue) = 0;
  virtual bool isCompleted() { return completed; };
  virtual void stop() = 0;

protected:
  std::atomic<bool> completed = false;
  std::atomic<bool> _stopFlag{false}; // Used for handling cancelling processing
};
