#include <fstream>
#include <string>

class Checkpoint {
public:
  Checkpoint(const std::string &filename);
  ~Checkpoint();

private:
  std::ifstream _checkpointFile;
  int _fileLockDescriptor;
};