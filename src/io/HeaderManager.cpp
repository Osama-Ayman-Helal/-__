#include "HeaderManager.h"
#include "BitWriter.h"
#include <filesystem>
void HeaderManager::writeHeader(std::ofstream& outFile, const std::unordered_map<unsigned char, int>& freqMap) {
    // TODO: Student 6 - Serialize the map
    size_t freqMap_size = freqMap.size();
    outFile.write(reinterpret_cast<const char*>(&freqMap_size),sizeof(freqMap_size));

    for (auto& x : freqMap){
        unsigned char  c = x.first;
        int freq = x.second;

        outFile.write(reinterpret_cast<const char*>(&c),sizeof(c));
        outFile.write(reinterpret_cast<const char*>(&freq),sizeof(freq));

    }
}

std::unordered_map<unsigned char, int> HeaderManager::readHeader(std::ifstream& inFile) {
    std::unordered_map<unsigned char, int> map;
    // TODO: Student 6 - Deserialize the map
    size_t freqMap_size ;
    inFile.read(reinterpret_cast<char*>(&freqMap_size),sizeof(freqMap_size));

    for (size_t i = 0 ; i < freqMap_size ; i++){
        unsigned char c ;
        int freq;

        inFile.read(reinterpret_cast<char*>(&c),sizeof(c));
        inFile.read(reinterpret_cast<char*>(&freq),sizeof(freq));
        map[c] = freq;
    }

    return map;
}
