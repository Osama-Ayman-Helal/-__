#pragma once
#include <vector>
#include <string>

class Packer {
public:
    void packFiles(const std::vector<std::string>& files, const std::string& outputFile);

    void unpackFiles(const std::string& inputFile, const std::string& outputDir);
};
