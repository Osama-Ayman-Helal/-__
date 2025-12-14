#pragma once
#include <vector>
#include <string>

class DirectoryCrawler {
public:
    // TODO: Student 8 - Use std::filesystem (C++17) or dirent.h (legacy)
    // to recursively find all files in a folder.
    std::vector<std::string> getAllFiles(const std::string& rootPath);
};
