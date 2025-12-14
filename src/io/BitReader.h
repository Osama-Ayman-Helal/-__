#pragma once
#include <fstream>
#include <cstdint>

class BitReader {
private:
    std::ifstream& inFile;
    uint8_t buffer;       // The current byte we are reading bits from
    int bitCount;         // How many bits are left in the current 'buffer'

public:
    // Constructor takes an open file stream
    BitReader(std::ifstream& inStream);

    // Reads a single bit (returns 0 or 1). Returns -1 if EOF.
    int readBit();
};