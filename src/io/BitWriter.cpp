#include "BitWriter.h"
#include <stdexcept>
#include <iostream>


// Intializing
BitWriter :: BitWriter(std :: ofstream& outStream): outFile(outStream), buffer(0), bitCount(0) {}


BitWriter :: ~BitWriter() {
    // flush remaining bits on destruction
    flush();
}

void BitWriter :: writeBit(int bit) {
    // validate the input
    if (bit != 0 && bit != 1) { // a double protection :)
        throw std :: invalid_argument("writeBit() expected bit = 0 or 1 only");
    }

    // MSB-first packing
    // shift buffer left then insert new bit in least significant position
    buffer = static_cast<uint8_t>((buffer << 1) | (bit & 1)); // (bit & 1) makes sure that it's (0 or 1) a triple protection :)
    // (static_cast<uint8_t>) is the same as (uint8_t) which is used for casting but it's safier
    // it doesn't allow illogical castings
    bitCount++;

    // if buffer contains 8 bits write it to the file
    if (bitCount == 8) {
        outFile.put(static_cast<char>(buffer)); // static_cast<char> is also casting

        if (!outFile) {
            throw std :: runtime_error("writeBit() failed to write byte to file");
        }

        // Reset buffer
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter :: writeBitString(const std::string& bitString) {
    // loop through each character (must be '0' or '1')
    for (char c : bitString) {
        if (c != '0' && c != '1') {
            throw std :: invalid_argument("writeBitString() requires characters '0' or '1' only");
        }
        writeBit(c - '0');
    }
}

void BitWriter :: flush() {
    // if buffer is empty nothing to write
    if (bitCount == 0) return;

    // To finalize incomplete byte:
    // Shift remaining bits to the left so they occupy the MSB positions
    // Fill the remaining lower bits with zeros (padding)
    uint8_t finalByte = static_cast<uint8_t>(buffer << (8 - bitCount));

    outFile.put(static_cast<char>(finalByte)); // (static_cast<char>) is just casting but in a safier way

    if (!outFile) {
        throw std :: runtime_error("flush() failed to write final padded byte");
    }

    // Reset internals
    buffer = 0;
    bitCount = 0;
}