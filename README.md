# LoseRAR_Project

Small educational archive + Huffman compressor. This README explains how to build and run the project on Linux and Windows PowerShell using the MSYS `c++.exe` provided in the project's task configuration.

# Build 

## On Windows:

```powershell
Set-Location 'c:\Users\tarek\PycharmProjects\PythonProject9\LoseRAR_Project'
$srcs = Get-ChildItem -Recurse -Filter *.cpp -Path .\src | ForEach-Object { $_.FullName }
& 'c:\msys64\ucrt64\bin\c++.exe' -fdiagnostics-color=always -g .\main.cpp $srcs -I. -std=c++17 -o .\LoseRAR.exe
& 'c:\msys64\ucrt64\bin\c++.exe' -fdiagnostics-color=always -g .\main.cpp $srcs -I. -std=c++17 -o .\LoseRAR.exe
```

Alternative: use CMake if installed

```powershell
Set-Location 'c:\Users\tarek\PycharmProjects\PythonProject9\LoseRAR_Project'
cmake -S . -B cmake-build-debug -G "Ninja"
cmake --build cmake-build-debug --config Debug
```

## On Linux:

```bash
cd LoseRAR_Project/
g++ -g -std=c++17 main.cpp src/*/*.cpp -o LoseRAR
```

# Run (examples)

## Compress a single file

On Windows:
```powershell
.\LoseRAR.exe -c .\test.txt .\test.ohz
```

On Linux:
```bash
./LoseRAR -c ./test.txt ./test.ohz
```

## Extract archive into a directory

On Windows:
```powershell
.\LoseRAR.exe -x .\test.ohz .\out
```

On Linux:
```bash
./LoseRAR -x ./test.ohz ./out
```

# Notes about recent fixes
- Added `src/core/HuffmanNode.h` (correct spelling).
- Fixed `main.cpp` to convert the `std::unordered_map<unsigned char,int>` frequency maps to `std::map<unsigned char,int>` before calling the Huffman tree builder.

The small compatibility shim `src/core/HufmannNode.h` was removed; all includes now reference `HuffmanNode.h`.
