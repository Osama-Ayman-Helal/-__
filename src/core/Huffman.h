#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "HuffmanNode.h"
#include <map>
#include <string>
#include <utility>

// Calculate frequency of each character in the string
std::map<unsigned char, int> calculateFrequency(const std::string &s);

// Build Huffman tree from a frequency map
HuffmanNode *buildTree(std::map<unsigned char, int> freq);

// Build code table (mapping from characters to their binary codes)
void buildCodeTable(HuffmanNode *root, const std::string &code,
                    std::map<unsigned char, std::string> &chCodes);

// Delete the Huffman tree to free memory
void deleteTree(HuffmanNode *root);

// Encode a string and return the encoded string along with the frequency map
std::pair<std::string, std::map<unsigned char, int>> encodeString(const std::string &s);

// Decode an encoded string using a frequency map
std::string decodedString(const std::string &encodedString,
                          std::map<unsigned char, int> freq);

#endif // HUFFMAN_H