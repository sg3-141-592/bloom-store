#include <stdexcept>
#include <string>
#include <vector>
#include <zlib.h>

bool writeStringToGzipFile(const std::string &inputString,
                           const std::string &outputGzPath);

std::string readGzipFileToString(const std::string &inputGzPath);
