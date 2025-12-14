#pragma once

#include <fstream>
#include <string>
#include <cstdint>

/*
    BitWriter
    ---------
    Responsible for writing individual bits (0/1) into a binary file.
    Bits are accumulated into an 8-bit buffer. Once the buffer is full,
    a complete byte is written to the output file.

    - Uses MSB-first packing: (MSB -> Most Significant Bit) {left most bit}
        Each incoming bit is shifted into the left side of the buffer
        (buffer = (buffer << 1) | bit)
        Example:
        buffer = 10101010
        bit = 1
        buffer = (buffer << 1) | bit
                    0101010'0' | 0000000'1'
        so "bit"'s value will go to the left of the buffer


    - Remaining bits on flush() are padded with zeros
        Example:
        if buffer = 1011 (not 8 bits yet so can't be added)
        flush makes it = 10110000 (to be a full byte and you can add it to the file)

    - Note:
        the output file stream is provided externally and must be opened
        in binary mode. BitWriter does NOT close the stream.
*/

/*
    Run flow
    --------
    func. writeBitString(string& bitString) is being called with a string
    it sends them to func. writeBit(int bit) as an integers to be added in the buffer one by one
    if number of bits in the buffer reaches 8 that means you have a full byte so it's written in the file
    it keeps going until you reach the end of the string (bitString) in func. writeBitString(string& bitString)
    then the destructor calls func. flush() as if there's still some bits in the buffer it shifts them left
    (buffer << (8 - bitCount))
    until they make a full byte that you can add in the file

    - Example:
    bitString = 10101010101010101010 (20 bits)
    buffer = 0
    bitCount = 0

    writeBit(1)
    buffer = 1
    bitCount = 1

    writeBit(0)
    buffer = 10
    bitCount = 2

    ...

    writeBit(1)
    buffer = 1010101
    bitCount = 7

    writeBit(0)
    buffer = 10101010
    bitCount = 8
    as bitCount = 8 then add the buffer to the file..and it keeps going

    ...

    final state:

    writeBit(0)
    buffer = 1010
    bitCount = 4
    as the string is done so it will flush the buffer as we explained
    buffer will equal 1010'0000' to make it a full byte
*/



class BitWriter {
private:
    std :: ofstream& outFile;   // Reference to external output file
    uint8_t buffer;             // a temp. bits storage max 8 (uint8_t -> as you only need 8 bits Unsigned integer 8 bits)
    int bitCount;               // number of bits stored currently in the buffer variable (max 8)

    // disabling copying (copying is unsafe)
    // having two or more objects writing in the same file will make alot of errors
    // you have only a refrence for the file which makes ut unsafe
    BitWriter(const BitWriter&) = delete;
    BitWriter& operator=(const BitWriter&) = delete;

public:
    // Constructor: requires an already opened binary output stream
    BitWriter(std :: ofstream& outStream);

    // Destructor: automatically flushes remaining bits (if any)
    ~BitWriter();

    // Writes a single bit (0 or 1)
    // If buffer becomes full (8 bits) one byte is written to file
    void writeBit(int bit);

    // Writes a series of bits encoded in a "010101" string
    void writeBitString(const std :: string& bitString);

    // Writes the last incomplete byte (if any), padded with zeros
    void flush();
};