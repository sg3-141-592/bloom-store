#include <fstream>
#include <iostream>
#include <sys/file.h>
#include <unistd.h>

#include <filesystem>
namespace fs = std::filesystem;

#include "ProgramLock.h"

ProgramLock::ProgramLock(const std::string &lockFileName)
    : lockFilePath(fs::temp_directory_path() / lockFileName),
      lockFileDescriptor(-1){};

bool ProgramLock::tryLock() {
  // Open or create the lock file
  lockFileDescriptor = open(lockFilePath.c_str(), O_CREAT | O_RDWR, 0644);
  if (lockFileDescriptor == -1) {
    std::cerr << "Failed to open lock file" << std::endl;
    return false;
  }

  // Try to acquire an exclusive lock
  int lockResult = flock(lockFileDescriptor, LOCK_EX | LOCK_NB);
  if (lockResult == -1) {
    close(lockFileDescriptor);
    return false; // Another instance is running
  }

  return true;
};

ProgramLock::~ProgramLock() {
  // Release and close the lock file
  if (lockFileDescriptor != -1) {
    flock(lockFileDescriptor, LOCK_UN);
    close(lockFileDescriptor);
    fs::remove(lockFilePath);
  }
};