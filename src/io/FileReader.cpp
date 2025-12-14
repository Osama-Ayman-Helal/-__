#include "FileReader.h"
#include <filesystem>
FileReader::FileReader(const std::string &filepath, size_t chunk_size) {
    chunksize = chunk_size;
    inputfile.open(filepath, std::ios::binary | std::ios::in);
    if (!inputfile.is_open()) {
        throw std::runtime_error("Cannot open the file: " + filepath);
    }
}

FileReader::~FileReader() {
    if (inputfile.is_open()) {
        inputfile.close();
    }
}

std::vector<uint8_t> FileReader::readchunk() {
    std::vector<uint8_t> buffer(chunksize);
    inputfile.read(reinterpret_cast<char*>(buffer.data()), chunksize);
    std::streamsize bytesread = inputfile.gcount();

    if (bytesread < chunksize) {
        buffer.resize(bytesread);
    }
    return buffer;
}

void FileReader::reset() {
    inputfile.clear();
    inputfile.seekg(0, std::ios::beg);
}
