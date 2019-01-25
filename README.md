# 7-Zip
Please check original README [here](/DOC/readme.txt)

## CMake Support
This allows you to build 7-zip with another cursed compiler.

We add options to control LTO/LTCG and NSIS script decompiling.

Note: You still need MSVC assembler (ml/ml64) for assembly code. <br>
e. g. `cmake -DCMAKE_ASM_MASM_COMPILER='/c/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/ml64.exe'`

Currently we have the following targets in CMake:

[ ] Alone (7za.exe)
[ ] Alone7z (7zr.exe)
[ ] Fm (Standalone 7zFM)
[ ] Format7z (7za.dll)
[ ] Format7zExtract (7zxa.dll)
[ ] Format7zR (7zr.dll)
[ ] Format7zExtractR (7zxr.dll)
[x] Format7zF (7z.dll)
[ ] LzmaCon (lzma.exe)
[ ] SFXCon (7zCon.sfx)
[ ] SFXWin (7z.sfx)
[ ] SFXSetup (7zS.sfx)
[x] Console (7z.exe)
[ ] Explorer (7-zip(32).dll)
[x] FileManager (7zFM.exe)
[x] GUI (7zG.exe)

### Known Issue
* (Currently) only supports x86 (MSVC and probably Clang-MSVC)* / amd64 (MSVC/MinGW-w64/Clang-MSVC)
* Common objects will be compiled multiple times (each target has different macro definitions)

*: our x86 MinGW-w64 used wrong calling convention
