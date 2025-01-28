#pragma once

#include <string>
#include <variant>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

template <typename TData, typename TCheckpoint> struct Record {
  TData data;
  TCheckpoint checkpoint;
};

typedef Record<std::string, std::streampos> StringRecord;
typedef Record<json, std::streampos> JsonRecord;
typedef Record<std::vector<std::string>, int> SqliteRecord;
typedef std::variant<StringRecord, JsonRecord, SqliteRecord> GenericRecord;