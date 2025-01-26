#include <string>

class ProgramLock {
private:
  std::string lockFilePath;
  int lockFileDescriptor;

public:
  ProgramLock(const std::string &lockFileName);
  bool tryLock();
  ~ProgramLock();
};
