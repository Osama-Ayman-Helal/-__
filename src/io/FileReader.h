#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>

class FileReader {

private:
    std::ifstream inputfile;
    size_t chunksize;

public:
    FileReader(const std::string &filepath, size_t chunk_size);
    ~FileReader();

    std::vector<uint8_t> readchunk();
    void reset();

    bool isEof() const {
        return inputfile.eof();
    }
};