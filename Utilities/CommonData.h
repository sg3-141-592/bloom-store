#pragma once

#include <string>

template <typename TData, typename TCheckpoint>
struct Record {
    TData data;
    TCheckpoint checkpoint;
};