#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>

// CORE
#include "src/core/FrequencyMap.h"
#include "src/core/Huffman.h"

// IO
#include "src/io/BitWriter.h"
#include "src/io/BitReader.h"
#include "src/io/HeaderManager.h"
#include "src/io/FileReader.h"
#include "src/io/ui.h"

// ARCHIVE
#include "src/archive/Packer.h"
#include "src/archive/Directory.h"

using namespace std;
namespace fs = std::filesystem;

// --- Helper to clean up temp files ---
void removeFile(const string& path) {
    if (fs::exists(path)) {
        fs::remove(path);
    }
}

// --- Huffman Compression Logic ---
void compressRawFile(const string& inputFile, const string& outputFile) {
    cout << YELLOW;
    cout << "  [Step 2] Frequency Analysis..." << RESET << endl;
    FrequencyMap fMap;
    auto freqs = fMap.buildFrequencyMap(inputFile);
    // Huffman functions expect a std::map; convert unordered_map -> map
    std::map<unsigned char, int> freqMap(freqs.begin(), freqs.end());

    cout << YELLOW;
    cout << "  [Step 3] Building Huffman Tree..." << RESET << endl;
    HuffmanNode* root = buildTree(freqMap);

    map<unsigned char, string> codes;
    buildCodeTable(root, "", codes);

    cout << YELLOW;
    cout << "  [Step 4] Writing Compressed File..." << RESET << endl;
    ofstream outFile(outputFile, ios::binary);
    if (!outFile)
        throw runtime_error("Cannot create output file " + outputFile);


    // A. Write Header (Frequency Map)
    HeaderManager headerMgr;
    headerMgr.writeHeader(outFile, freqs);

    // B. Write Original File Size (Crucial for decoding loop)
    uint64_t fileSize = fs::file_size(inputFile);
    outFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

    // C. Compress Body
    BitWriter bWriter(outFile);
    FileReader reader(inputFile, 1024 * 1024); // Read in 1MB chunks

    while (!reader.isEof()) {
        vector<uint8_t> chunk = reader.readchunk();
        if (chunk.empty()) break;

        for (uint8_t byte : chunk) {
            // Write the bit string for this byte (e.g., "10010")
            bWriter.writeBitString(codes[byte]);
        }
    }

    bWriter.flush(); // Write remaining bits and close byte
    outFile.close();
    deleteTree(root);
}

// --- Huffman Decompression Logic ---
void decompressRawFile(const string& inputFile, const string& outputFile) {
    cout << YELLOW;
    cout << "  [Step 1] Reading Header & Rebuilding Tree..." << RESET << endl;
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) 
        throw runtime_error("Cannot open input file " + inputFile);
 

    // A. Read Header
    HeaderManager headerMgr;
    auto freqs = headerMgr.readHeader(inFile);
    // Convert to std::map for Huffman tree rebuilding
    std::map<unsigned char, int> freqMap(freqs.begin(), freqs.end());
    HuffmanNode* root = buildTree(freqMap);

    // B. Read Expected Size
    uint64_t expectedSize = 0;
    inFile.read(reinterpret_cast<char*>(&expectedSize), sizeof(expectedSize));

    // C. Decode Body
    cout << YELLOW;
    cout << "  [Step 2] Decoding..." << RESET << endl;
    ofstream outFile(outputFile, ios::binary);
    BitReader bReader(inFile);

    HuffmanNode* curr = root;
    uint64_t writtenBytes = 0;

    while (writtenBytes < expectedSize) {
        int bit = bReader.readBit();
        if (bit == -1) break; // Error or Unexpected EOF

        // Traverse Tree
        if (bit == 0) curr = curr->left;
        else          curr = curr->right;

        // Found Leaf
        if (!curr->left && !curr->right) {
            outFile.put(curr->character);
            curr = root; // Reset to top
            writtenBytes++;
        }
    }

    outFile.close();
    inFile.close();
    deleteTree(root);
}

// --- Main Integration ---

void runCompress(const string& inputPath, const string& outputPath) {
    string tempArchive = "temp_pack.blob";

    // 1. ARCHIVE (Packer)
    // Always pack, even if it's a single file. This preserves filenames.
    cout << YELLOW ;
    cout << "Phase 1: Archiving..." << RESET << endl;
    Packer packer;
    vector<string> files;

    if (fs::is_directory(inputPath)) {
        DirectoryCrawler crawler;
        files = crawler.getAllFiles(inputPath);
    } else {
        files.push_back(inputPath);
    }

    packer.packFiles(files, tempArchive);

    // 2. COMPRESS (Huffman)
    cout << YELLOW;
    cout << "Phase 2: Compressing..." << RESET << endl;
    compressRawFile(tempArchive, outputPath);

    // Cleanup
    removeFile(tempArchive);
    cout << GREEN_BACKGROUND << BLACK;
    cout << "Success! Created " << outputPath << RESET << endl;
}

void runDecompress(const string& inputPath, const string& outputDir) {
    string tempArchive = "temp_unpack.blob";

    // 1. DECOMPRESS (Huffman)
    cout << YELLOW;
    cout << "Phase 1: Decompressing..." << RESET << endl;
    decompressRawFile(inputPath, tempArchive);

    // 2. UNPACK (Packer)
    cout << YELLOW;
    cout << "Phase 2: Unpacking..." << RESET << endl;
    Packer packer;
    // Ensure output directory exists
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }
    packer.unpackFiles(tempArchive, outputDir);

    // Cleanup
    removeFile(tempArchive);
    cout << GREEN_BACKGROUND << BLACK;
    cout << "Success! Extracted to " << outputDir << RESET << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: ./LoseRAR [flag] [input] [output]" << endl;
        cout << "Flags: -c (Compress), -x (Extract)" << endl;
        return 1;
    }

    string flag = argv[1];
    string input = argv[2];
    string output = argv[3];

    try {
        if (flag == "-c") {
            runCompress(input, output);
        } else if (flag == "-x") {
            runDecompress(input, output);
        } else {
            cerr << RED_BACKGROUND ;
            cerr << "Unknown flag: " << flag << RESET << endl;
        }
    } catch (const exception& e) {
        cerr << RED_BACKGROUND ;
        cerr << "Runtime Error: " << e.what() << RESET << endl;
        return 1;
    }

    return 0;
}
