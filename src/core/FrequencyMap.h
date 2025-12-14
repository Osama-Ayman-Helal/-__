#pragma once
#include <unordered_map>
#include <string>

class FrequencyMap {
public:
    // TODO: Student 1 - Implement this function
    // 1. Open the file at filePath.
    // 2. Read byte by byte.
    // 3. Update the map count.
    std::unordered_map<unsigned char, int> buildFrequencyMap(const std::string& filePath);
};
