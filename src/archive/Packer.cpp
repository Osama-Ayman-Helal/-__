#include "Packer.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../io/ui.h"

using namespace std;
namespace fs = std::filesystem;

void Packer::packFiles(const vector<string> &files, const string &outputFile)
{
    ofstream out(outputFile, ios::binary);
    if (!out.is_open())
        throw runtime_error("Could not create archive file " + outputFile);


    for (const auto &filePath : files)
    {
        fs::path p(filePath);

        if (!fs::exists(p))
        {
            cout << RED_BACKGROUND ;
            cout << "Warning: File skipped (not found): " << filePath << RESET << endl;
            continue;
        }

        // Use generic_string() to normalize slashes (Windows vs Linux)
        string storedPath = p.generic_string();
        uint32_t nameLen = static_cast<uint32_t>(storedPath.size());
        uint32_t fileSize = static_cast<uint32_t>(fs::file_size(p));

        // Write Metadata
        out.write(reinterpret_cast<const char *>(&nameLen), sizeof(nameLen));
        out.write(storedPath.c_str(), nameLen);
        out.write(reinterpret_cast<const char *>(&fileSize), sizeof(fileSize));

        // Write Content
        ifstream in(filePath, ios::binary);
        if (in.is_open())
        {
            out << in.rdbuf();
            in.close();
        }
        else
        {
             cout << RED_BACKGROUND;
             cout << "Warning: Could not read content of: " << filePath << RESET << endl;
        }
    }

    out.close();
    cout << GREEN_BACKGROUND << BLACK;
    cout << "Packing complete: " << outputFile << RESET << endl;
}

void Packer::unpackFiles(const string &inputFile, const string &outputDir)
{
    ifstream in(inputFile, ios::binary);
    if (!in.is_open())
        throw runtime_error("Could not open archive " + inputFile);


    // Ensure the output root directory exists
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    // Peek to check for EOF before starting a new read cycle
    while (in.peek() != EOF)
    {
        // 1. Read Name Length
        uint32_t nameLen = 0;
        in.read(reinterpret_cast<char *>(&nameLen), sizeof(nameLen));
        if (in.gcount() != sizeof(nameLen)) break; // Safety break

        // 2. Read Filename
        string filename(nameLen, '\0');
        in.read(&filename[0], nameLen);

        // 3. Read File Size
        uint32_t fileSize = 0;
        in.read(reinterpret_cast<char *>(&fileSize), sizeof(fileSize));

        // 4. Construct Path safely using filesystem
        fs::path destPath = fs::path(outputDir) / filename;

        // --- FIX START: Create parent directories if they don't exist ---
        if (destPath.has_parent_path()) {
            fs::create_directories(destPath.parent_path());
        }
        // --- FIX END ---

        ofstream out(destPath, ios::binary);
        if (!out.is_open()) {
            cout << RED_BACKGROUND ;
            cout << "Error: Could not create output file: " << destPath << RESET << endl;
            // Skip the bytes of this file so we can try the next one
            in.seekg(fileSize, ios::cur);
            continue;
        }

        // 5. Write Content in chunks
        const size_t bufferSize = 5120; // 5KB
        char buffer[bufferSize];
        uint64_t remaining = fileSize;

        while (remaining > 0)
        {
            size_t toRead = (remaining < bufferSize) ? remaining : bufferSize;
            in.read(buffer, toRead);

            // Check if we actually read what we expected
            size_t actuallyRead = in.gcount();
            out.write(buffer, actuallyRead);

            remaining -= actuallyRead;

            if(actuallyRead == 0 && in.eof()) break; // Prevent infinite loop on corrupted files
        }

        out.close();
        cout << "Extracted: " << destPath.string() << endl;
    }

    in.close();
}
