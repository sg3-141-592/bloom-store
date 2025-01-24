#include <zlib.h>
#include <string>
#include <vector>
#include <stdexcept>

std::string compressString(const std::string& input) {
    // Prepare zlib compression parameters
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = input.size();
    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));

    // Initialize deflate compression
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
        throw std::runtime_error("Compression initialization failed");
    }

    // Output buffer
    std::vector<char> outBuffer(input.size() * 2);
    zs.avail_out = outBuffer.size();
    zs.next_out = reinterpret_cast<Bytef*>(outBuffer.data());

    // Compress data
    if (deflate(&zs, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&zs);
        throw std::runtime_error("Compression failed");
    }

    // Clean up
    deflateEnd(&zs);

    // Return compressed string
    return std::string(outBuffer.data(), zs.total_out);
}