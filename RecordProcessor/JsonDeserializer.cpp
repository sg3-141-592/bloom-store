#include "JsonDeserializer.h"

#include <iostream>

void JsonDeserializer::extractField(const json& source, json& target, const std::string& fieldName) {
    if (source.contains(fieldName) && !source[fieldName].is_null()) {
        target[fieldName] = source[fieldName];
    }
}

JsonRecord JsonDeserializer::process(const StringRecord& in) {
  auto inParsed = json::parse(in.data);
  json extractedFields;

  // Manually extracting some named fields for now
  extractField(inParsed, extractedFields, "id");
  extractField(inParsed, extractedFields, "name");
  extractField(inParsed, extractedFields, "country");
  extractField(inParsed, extractedFields, "type");

  if (inParsed.contains("genres")) {
    extractedFields["genres"] = json::array();
    for (const auto &genre : inParsed["genres"]) {
      if (genre.contains("name") && !genre["name"].is_null()) {
        extractedFields["genres"].push_back(genre["name"]);
      }
    }
  }

  if(inParsed.contains("life-span") && inParsed["life-span"].contains("begin"))
  {
    extractedFields["begin"] = inParsed["life-span"]["begin"];
  }

  return JsonRecord{extractedFields, in.checkpoint};
}

void JsonDeserializer::start(std::shared_ptr<TSQueue<GenericRecord>> sourceQueue,
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

      // This process can throw so we print the output for debugging
      std::string data_copy;
      try {
        auto processedMessage = process(message);

        while (!sinkQueue->try_push(processedMessage)) {
          std::this_thread::yield();
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
      } catch (const json::exception &e) {
        std::cerr << "Cannot process: " << message.data << " - Error: " << e.what() << std::endl;
      }

      _metricsTracker->recordMessage();
      _metricsTracker->printMetricsIfNeeded();
    }
    // Pushing an end of stream marker
    sinkQueue->push(JsonRecord{json::object(), -1});

    std::cout << "Finished processing messages\n";

    completed.store(true);
  });
}

void JsonDeserializer::stop() {
  _stopFlag.store(true);
  if (_thread.joinable()) {
    _thread.join();
  }
}

JsonDeserializer::~JsonDeserializer() {
  if (_thread.joinable()) {
    _thread.join();
  }
};
