#include "Checkpoint.h"

#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

Checkpoint::Checkpoint(const std::string &filename)
{
    // Open the file with read and write permissions
    _fileLockDescriptor = open(filename.c_str(), O_RDWR | O_CREAT, 0644);

    if (_fileLockDescriptor == -1)
    {
        throw std::runtime_error("Failed to open checkpoint file");
    }

    // Attempt to acquire an exclusive lock
    if (flock(_fileLockDescriptor, LOCK_EX | LOCK_NB) == -1)
    {
        close(_fileLockDescriptor);
        throw std::runtime_error("Unable to acquire exclusive lock on checkpoint file");
    }

    // Open ofstream after getting the lock
    _checkpointFile.open(filename, std::ios::out | std::ios::app);
    if (!_checkpointFile)
    {
        flock(_fileLockDescriptor, LOCK_UN);
        close(_fileLockDescriptor);
        throw std::runtime_error("Failed to open checkpoint file stream");
    }
}

Checkpoint::~Checkpoint()
{
    // Release the lock and close file descriptors
    if (_fileLockDescriptor != -1)
    {
        flock(_fileLockDescriptor, LOCK_UN);
        close(_fileLockDescriptor);
    }

    if (_checkpointFile.is_open())
    {
        _checkpointFile.close();
    }
}