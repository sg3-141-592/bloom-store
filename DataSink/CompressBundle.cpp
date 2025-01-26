#include "CompressBundle.h"

bool writeStringToGzipFile(const std::string &inputString,
                           const std::string &outputGzPath) {
  gzFile outputFile = gzopen(outputGzPath.c_str(), "wb");
  if (!outputFile) {
    throw std::runtime_error("Cannot open output gzip file");
  }

  size_t bytesWritten =
      gzwrite(outputFile, inputString.c_str(), inputString.length());
  gzclose(outputFile);

  return bytesWritten == inputString.length();
}

std::string readGzipFileToString(const std::string &inputGzPath) {
  gzFile inputFile = gzopen(inputGzPath.c_str(), "rb");
  if (!inputFile) {
    throw std::runtime_error("Cannot open input gzip file");
  }

  std::string result;
  char buffer[4096];
  int bytesRead;

  while ((bytesRead = gzread(inputFile, buffer, sizeof(buffer))) > 0) {
    result.append(buffer, bytesRead);
  }

  gzclose(inputFile);
  return result;
}