#include "Directory.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::vector<std::string> DirectoryCrawler::getAllFiles(const std::string& rootPath) {
    std::vector<std::string> files;
    //  Recursive implementation
    try {           //عشان لو اتبعت pathمش موجود بالفعل(مشاكل)

        // Loop through directory recursively

        for (const auto& entry : fs::recursive_directory_iterator(rootPath)) {    //ميثود جاهزةوبطبعها recursive وهتاخد الباث المبعوت ليها منا اللي بيظبط ال main
            if (entry.is_regular_file()) {   //لو لقي فايل (مش فولدر او شورتكات
                files.push_back(entry.path().generic_string());  //هيضيفه للvector بتاعي علي هيئة path (to string->totally)
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Directory Error: " << e.what() << std::endl;   //.what-> to return characters
    }

    return files;
}
