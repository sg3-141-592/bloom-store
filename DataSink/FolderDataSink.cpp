#include "FolderDataSink.h"

#include <iostream>

FolderDataSink::FolderDataSink(std::function<std::string(json)> getPathFunc)
{
    _getPathFunc = getPathFunc;
}

bool FolderDataSink::writeNext(json in) 
{
    const std::string path = _getPathFunc(in);
    // Check to see if we already have a hook to the current path
    // if not create one
    if (_pathToHook.find(path) == _pathToHook.end()) {
        _pathToHook[path] = std::ofstream(path, std::ios::app);
    }

    _pathToHook[path] << in.dump() << std::endl;
    
    return true;
}

FolderDataSink::~FolderDataSink()
{

}