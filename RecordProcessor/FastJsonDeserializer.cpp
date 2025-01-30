#include "FastJsonDeserializer.h"

#include <iostream>

#include "simdjson.h"
using namespace simdjson;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

JsonRecord FastJsonDeserializer::process(const StringRecord &in) {

  // simdjson requires the input JSON string to have at least SIMDJSON_PADDING
  // extra bytes allocated beyond the string's length
  padded_string padded_json(in.data);
  ondemand::parser parser;
  ondemand::document doc = parser.iterate(padded_json);

  // simdjson is only a parser so we still need to use nlohmann::json for
  // passing objects
  json extractedFields;

  auto id = doc["id"].get_string();
  if (!id.error()) {
    extractedFields["id"] = id.value();
  }

  auto name = doc["name"].get_string();
  if (!name.error()) {
    extractedFields["name"] = name.value();
  }

  auto country = doc["country"].get_string();
  if (!country.error()) {
    extractedFields["country"] = country.value();
  }

  auto type = doc["type"].get_string();
  if (!type.error()) {
    extractedFields["type"] = type.value();
  }

  if (doc["genres"].error() != NO_SUCH_FIELD) {
    extractedFields["genres"] = json::array();

    for (auto genre : doc["genres"]) {
      auto name_result = genre["name"];
      if (name_result.error() != NO_SUCH_FIELD && !name_result.is_null()) {
        extractedFields["genres"].push_back(name_result.get_string().value());
      }
    }
  }

  if (doc["life-span"].error() != NO_SUCH_FIELD && doc["life-span"]["begin"].error() != NO_SUCH_FIELD) {
    auto begin_result = doc["life-span"]["begin"];
    if (begin_result.error() != NO_SUCH_FIELD && !begin_result.is_null()) {
        extractedFields["begin"] = begin_result.get_string().value();
    }
  } 

  return JsonRecord{extractedFields, in.checkpoint};
}

void FastJsonDeserializer::start(
    std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
    std::shared_ptr<TSQueue<GenericRecord>> sinkQueue) {
  // Ensure the thread is not already running
  if (_thread.joinable()) {
    throw std::runtime_error("Already running!");
  }

  _thread = std::thread([this, sourceQueue, sinkQueue]() {
    std::cout << "Starting processing messages\n";

    while (!_stopFlag) {
      StringRecord message = std::get<StringRecord>(sourceQueue->pop());
      if (message.data == "EOF" && message.checkpoint == -1) {
        break;
      }

      auto processedMessage = process(message);

      while (!sinkQueue->try_push(processedMessage)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      _metricsTracker->recordMessage();
      _metricsTracker->printMetricsIfNeeded();
    };

    // Pushing an end of stream marker
    sinkQueue->push(JsonRecord{json::object(), -1});

    std::cout << "Finished processing messages\n";

    completed.store(true);
  });
}

void FastJsonDeserializer::stop() {
  _stopFlag.store(true);
  if (_thread.joinable()) {
    _thread.join();
  }
}

FastJsonDeserializer::~FastJsonDeserializer() {
  if (_thread.joinable()) {
    _thread.join();
  }
};