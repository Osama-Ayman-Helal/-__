#include "BitReader.h"
#include <filesystem>
BitReader::BitReader(std::ifstream& inStream) 
    : inFile(inStream), buffer(0), bitCount(0) {}

int BitReader::readBit() {
    // If we have run out of bits in the current buffer, read the next byte from file
    if (bitCount == 0) {
        char c;
        if (!inFile.get(c)) {
            return -1; // End of file
        }
        buffer = static_cast<uint8_t>(c);
        bitCount = 8; // We now have 8 fresh bits
    }

    // Extract the Most Significant Bit (MSB) first
    // Logic: Shift the buffer right so the target bit is at position 0, then mask with 1.
    // Example: buffer = 10100000. 
    // First read (bitCount=8): (10100000 >> 7) & 1 = 1
    bitCount--;
    int bit = (buffer >> bitCount) & 1;
    
    return bit;
}