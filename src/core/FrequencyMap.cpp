#include "FrequencyMap.h"
#include <fstream>
#include <iostream>
#include <vector> // Safer than raw arrays

std::unordered_map<unsigned char, int>
FrequencyMap::buildFrequencyMap(const std::string& filePath) {
    std::unordered_map<unsigned char, int> map;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "[Error] Cannot open file: " << filePath << std::endl;
        return map;
    }

    const size_t CHUNK_SIZE = 8192;
    std::vector<char> buffer(CHUNK_SIZE);

    while (true) {
        file.read(buffer.data(), CHUNK_SIZE);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead == 0) break;

        for (std::streamsize i = 0; i < bytesRead; ++i) {
            // Cast to unsigned char to correctly map bytes 128-255
            map[static_cast<unsigned char>(buffer[i])]++;
        }

        if (!file) break; // Break if EOF or error occurred during read
    }

    file.close();
    return map;
}