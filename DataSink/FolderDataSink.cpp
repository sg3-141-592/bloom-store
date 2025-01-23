#include "FolderDataSink.h"

FolderDataSink::FolderDataSink(std::function<std::string(std::string)> getPathFunc)
{
    _getPathFunc = getPathFunc;
}

bool FolderDataSink::writeNext(std::string in)
{
    return true;
}

FolderDataSink::~FolderDataSink()
{

}