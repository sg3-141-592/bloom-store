#include "JsonDeserializer.h"

#include <iostream>

JsonDeserializer::JsonDeserializer() {
  _metricsTracker = std::make_unique<MetricsTracker>("JsonDeserializer");
};

JsonRecord JsonDeserializer::process(StringRecord in) {
  auto result = json::parse(in.data);
  json extracted;

  if (result.contains("id")) {
    if (!result["id"].is_null()) {
      extracted["id"] = result["id"];
    }
  }
  if (result.contains("name")) {
    if (!result["name"].is_null()) {
      extracted["name"] = result["name"];
    }
  }
  if (result.contains("country")) {
    if (!result["country"].is_null()) {
      extracted["country"] = result["country"];
    }
  }
  if (result.contains("type")) {
    if (!result["type"].is_null()) {
      extracted["type"] = result["type"];
    }
  }
  if (result.contains("genres")) {
    extracted["genres"] = json::array();
    for (const auto &genre : result["genres"]) {
      if (genre.contains("name") && !genre["name"].is_null()) {
        extracted["genres"].push_back(genre["name"]);
      }
    }
  }

  return JsonRecord{extracted, in.checkpoint};
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

JsonDeserializer::~JsonDeserializer() { stop(); };
