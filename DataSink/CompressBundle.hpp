#include <zlib.h>
#include <string>
#include <vector>
#include <stdexcept>

bool writeStringToGzipFile(const std::string& inputString, const std::string& outputGzPath) {
    gzFile outputFile = gzopen(outputGzPath.c_str(), "wb");
    if (!outputFile) {
        throw std::runtime_error("Cannot open output gzip file");
    }

    size_t bytesWritten = gzwrite(outputFile, inputString.c_str(), inputString.length());
    gzclose(outputFile);

    return bytesWritten == inputString.length();
}