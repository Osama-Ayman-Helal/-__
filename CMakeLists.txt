cmake_minimum_required(VERSION 3.10)

project(LoseRAR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CORE_SOURCES
        src/core/FrequencyMap.cpp
        src/core/Huffman.cpp
)

set(IO_SOURCES
        src/io/BitWriter.cpp
        src/io/BitReader.cpp
        src/io/HeaderManager.cpp
        src/io/FileReader.cpp
)

set(ARCHIVE_SOURCES
        src/archive/Directory.cpp
        src/archive/Packer.cpp
)

add_executable(LoseRAR
        main.cpp
        ${CORE_SOURCES}
        ${IO_SOURCES}
        ${ARCHIVE_SOURCES}
)

target_include_directories(LoseRAR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_libraries(LoseRAR stdc++fs)
endif()