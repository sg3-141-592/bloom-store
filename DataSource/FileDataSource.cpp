#include "FileDataSource.h"

#include <iostream>

FileDataSource::FileDataSource(std::string filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
    }
};

std::optional<std::string> FileDataSource::readNext() {
    std::string sa;
    if (getline(file, sa)) {
        return sa;
    } else {
        return std::nullopt;
    }
}

FileDataSource::~FileDataSource() {
    file.close();
};