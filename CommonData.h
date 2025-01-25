#include <string>

// Message used to capture checkpoint of incoming data so we can record when its sinked
struct Record {
    std::string data;
    std::streampos checkpoint;
};