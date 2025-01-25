#include <zlib.h>
#include <string>
#include <vector>
#include <stdexcept>

bool writeStringToGzipFile(const std::string &inputString, const std::string &outputGzPath);

std::string readGzipFileToString(const std::string &inputGzPath);
