# LoseRAR_Project

Small educational archive + Huffman compressor. This README explains how to build and run the project on Windows PowerShell using the MSYS `c++.exe` provided in the project's task configuration.

Build (PowerShell)
```
Set-Location 'c:\Users\tarek\PycharmProjects\PythonProject9\LoseRAR_Project'
$srcs = Get-ChildItem -Recurse -Filter *.cpp -Path .\src | ForEach-Object { $_.FullName }
& 'c:\msys64\ucrt64\bin\c++.exe' -fdiagnostics-color=always -g .\main.cpp $srcs -I. -std=c++17 -o .\LoseRAR.exe
& 'c:\msys64\ucrt64\bin\c++.exe' -fdiagnostics-color=always -g .\main.cpp $srcs -I. -std=c++17 -o .\LoseRAR.exe
```

Alternative: use CMake if installed
```
Set-Location 'c:\Users\tarek\PycharmProjects\PythonProject9\LoseRAR_Project'
cmake -S . -B cmake-build-debug -G "Ninja"
cmake --build cmake-build-debug --config Debug
```

Run (examples)
```
# Compress a single file
.\LoseRAR.exe -c .\test.txt .\test.ohz

# Extract archive into a directory
.\LoseRAR.exe -x .\test.ohz .\out

GUI wrapper
```
# Requirements: Python 3.x with tkinter (usually included on Windows)
# Run this script from PowerShell or command prompt
python tools\lose_rar_gui.py
```

The GUI will attempt to locate `LoseRAR.exe` in the current working directory and `cmake-build-debug` automatically. If you built the executable elsewhere, click "Browse" and select the executable file.

On Windows you can also run the convenience script:
```
powershell -File tools\run_gui.ps1
```
```

Notes about recent fixes
- Added `src/core/HuffmanNode.h` (correct spelling).
- Fixed `main.cpp` to convert the `std::unordered_map<unsigned char,int>` frequency maps to `std::map<unsigned char,int>` before calling the Huffman tree builder.

The small compatibility shim `src/core/HufmannNode.h` was removed; all includes now reference `HuffmanNode.h`.
