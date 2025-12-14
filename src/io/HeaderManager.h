#pragma once
#include <fstream>
#include <unordered_map>

class HeaderManager {
public:
    // TODO: Student 6 - Design the Header Format
    // Suggestion: Write Int(MapSize) -> Then Loop [Char][Int(Freq)]
    void writeHeader(std::ofstream& outFile, const std::unordered_map<unsigned char, int>& freqMap);

    // TODO: Student 6 - Read the header back to rebuild the map
    std::unordered_map<unsigned char, int> readHeader(std::ifstream& inFile);
};
