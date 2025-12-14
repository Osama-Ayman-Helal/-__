#include "Directory.h"
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> DirectoryCrawler::getAllFiles(const std::string& rootPath) {
    std::vector<std::string> files;
    //  Recursive implementation

        // Loop through directory recursively

        for (const auto& entry : fs::recursive_directory_iterator(rootPath)) {    //ميثود جاهزةوبطبعها recursive وهتاخد الباث المبعوت ليها منا اللي بيظبط ال main
            if (entry.is_regular_file()) {   //لو لقي فايل (مش فولدر او شورتكات
                files.push_back(entry.path().generic_string());  //هيضيفه للvector بتاعي علي هيئة path (to string->totally)
            }
        }

    return files;
}
