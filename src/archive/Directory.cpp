#include "Directory.h"
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> DirectoryCrawler::getAllFiles(const std::string& rootPath) {
    std::vector<std::string> files;
    //  Recursive implementation

        // Loop through directory recursively

        for (const auto& entry : fs::recursive_directory_iterator(rootPath)) { 
            if (entry.is_regular_file()) { 
                files.push_back(entry.path().generic_string()); 
            }
        }

    return files;
}
