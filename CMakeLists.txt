cmake_minimum_required(VERSION 3.12)
project(7zip C CXX ASM_MASM RC)

set(USE_LTO YES CACHE BOOL "Use LTO/LTCG")
set(ENABLE_NSIS_DECOMPILE NO CACHE BOOL "Enable NSIS script decompiling")

set(COMMON_DIR "CPP/Common")
set(WIN_DIR "CPP/Windows")
set(7ZIP_COMMON_DIR "CPP/7zip/Common")
set(AR_DIR "CPP/7zip/Archive")
set(AR_COMMON_DIR "CPP/7zip/Archive/Common")
set(COMPRESS_DIR "CPP/7zip/Compress")
set(CRYPTO_DIR "CPP/7zip/Crypto")
set(UI_DIR "CPP/7zip/UI ")
set(C_DIR "C")

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)

add_definitions(-DUNICODE -D_UNICODE)

if(MSVC OR "${CLANG_CXX_COMPILER}" MATCHES "clang-cl")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Gy -Gw")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Gy -Gw")
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
else()
  add_library(htmlhelp STATIC contrib/htmlhelp.c)
endif()

if("${CMAKE_CXX_COMPILER}" MATCHES "clang-cl")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  add_definitions(-Dx64)
  add_definitions(-D_LZMA_DEC_OPT)
  set(LZMA_DEC_FILES Asm/x86/LzmaDecOpt.asm)
else()
  if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")
  else()
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -Wl,--large-address-aware")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--large-address-aware")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--large-address-aware")
  endif()
endif()

if(USE_LTO AND ipo_supported)
  message(STATUS "Using LTO")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "Not using LTO")
endif()

if(ENABLE_NSIS_DECOMPILE)
  add_definitions(-DNSIS_SCRIPT)
endif()

add_library(CRC OBJECT Asm/x86/7zCrcOpt.asm ${C_DIR}/7zCrc.c)

add_library(Aes OBJECT Asm/x86/AesOpt.asm ${C_DIR}/Aes.c)

add_library(COMMON OBJECT
${COMMON_DIR}/IntToString
${COMMON_DIR}/MyString
${COMMON_DIR}/MyVector
${COMMON_DIR}/NewHandler
${COMMON_DIR}/StringConvert
${COMMON_DIR}/StringToInt
${COMMON_DIR}/UTFConvert
${COMMON_DIR}/Wildcard
)


# add_library(Format7z)
# set_target_properties(Format7z PROPERTIES OUTPUT_NAME "7za")

# add_library(Format7zExtract)
# set_target_properties(Format7zExtract PROPERTIES OUTPUT_NAME "7zxa")

# add_library(Format7zR)
# set_target_properties(Format7zR PROPERTIES OUTPUT_NAME "7zr")

# add_library(Format7zExtractR)
# set_target_properties(Format7zExtractR PROPERTIES OUTPUT_NAME "7zxr")

add_library(Format7zF MODULE
$<TARGET_OBJECTS:COMMON>
${COMMON_DIR}/CRC
${COMMON_DIR}/CrcReg
${COMMON_DIR}/DynLimBuf
${COMMON_DIR}/MyMap
${COMMON_DIR}/MyXml
${COMMON_DIR}/Sha1Reg
${COMMON_DIR}/Sha256Reg
${COMMON_DIR}/XzCrc64Init
${COMMON_DIR}/XzCrc64Reg
${WIN_DIR}/FileDir
${WIN_DIR}/FileFind
${WIN_DIR}/FileIO
${WIN_DIR}/FileName
${WIN_DIR}/PropVariant
${WIN_DIR}/PropVariantUtils
${WIN_DIR}/Synchronization
${WIN_DIR}/System
${WIN_DIR}/TimeUtils
${7ZIP_COMMON_DIR}/CreateCoder
${7ZIP_COMMON_DIR}/CWrappers
${7ZIP_COMMON_DIR}/InBuffer
${7ZIP_COMMON_DIR}/InOutTempBuffer
${7ZIP_COMMON_DIR}/FilterCoder
${7ZIP_COMMON_DIR}/LimitedStreams
${7ZIP_COMMON_DIR}/LockedStream
${7ZIP_COMMON_DIR}/MemBlocks
${7ZIP_COMMON_DIR}/MethodId
${7ZIP_COMMON_DIR}/MethodProps
${7ZIP_COMMON_DIR}/OffsetStream
${7ZIP_COMMON_DIR}/OutBuffer
${7ZIP_COMMON_DIR}/OutMemStream
${7ZIP_COMMON_DIR}/ProgressMt
${7ZIP_COMMON_DIR}/ProgressUtils
${7ZIP_COMMON_DIR}/PropId
${7ZIP_COMMON_DIR}/StreamBinder
${7ZIP_COMMON_DIR}/StreamObjects
${7ZIP_COMMON_DIR}/StreamUtils
${7ZIP_COMMON_DIR}/UniqBlocks
${7ZIP_COMMON_DIR}/VirtThread
${AR_DIR}/ApmHandler
${AR_DIR}/ArHandler
${AR_DIR}/ArjHandler
${AR_DIR}/Bz2Handler
${AR_DIR}/ComHandler
${AR_DIR}/CpioHandler
${AR_DIR}/CramfsHandler
${AR_DIR}/DeflateProps
${AR_DIR}/DmgHandler
${AR_DIR}/ElfHandler
${AR_DIR}/ExtHandler
${AR_DIR}/FatHandler
${AR_DIR}/FlvHandler
${AR_DIR}/GzHandler
${AR_DIR}/GptHandler
${AR_DIR}/HandlerCont
${AR_DIR}/HfsHandler
${AR_DIR}/IhexHandler
${AR_DIR}/LzhHandler
${AR_DIR}/LzmaHandler
${AR_DIR}/MachoHandler
${AR_DIR}/MbrHandler
${AR_DIR}/MslzHandler
${AR_DIR}/MubHandler
${AR_DIR}/NtfsHandler
${AR_DIR}/PeHandler
${AR_DIR}/PpmdHandler
${AR_DIR}/QcowHandler
${AR_DIR}/RpmHandler
${AR_DIR}/SplitHandler
${AR_DIR}/SquashfsHandler
${AR_DIR}/SwfHandler
${AR_DIR}/UefiHandler
${AR_DIR}/VdiHandler
${AR_DIR}/VhdHandler
${AR_DIR}/VmdkHandler
${AR_DIR}/XarHandler
${AR_DIR}/XzHandler
${AR_DIR}/ZHandler
${AR_DIR}/Common/CoderMixer2
${AR_DIR}/Common/DummyOutStream
${AR_DIR}/Common/FindSignature
${AR_DIR}/Common/InStreamWithCRC
${AR_DIR}/Common/ItemNameUtils
${AR_DIR}/Common/MultiStream
${AR_DIR}/Common/OutStreamWithCRC
${AR_DIR}/Common/OutStreamWithSha1
${AR_DIR}/Common/HandlerOut
${AR_DIR}/Common/ParseProperties
${AR_DIR}/7z/7zCompressionMode
${AR_DIR}/7z/7zDecode
${AR_DIR}/7z/7zEncode
${AR_DIR}/7z/7zExtract
${AR_DIR}/7z/7zFolderInStream
${AR_DIR}/7z/7zHandler
${AR_DIR}/7z/7zHandlerOut
${AR_DIR}/7z/7zHeader
${AR_DIR}/7z/7zIn
${AR_DIR}/7z/7zOut
${AR_DIR}/7z/7zProperties
${AR_DIR}/7z/7zSpecStream
${AR_DIR}/7z/7zUpdate
${AR_DIR}/7z/7zRegister
${AR_DIR}/Cab/CabBlockInStream
${AR_DIR}/Cab/CabHandler
${AR_DIR}/Cab/CabHeader
${AR_DIR}/Cab/CabIn
${AR_DIR}/Cab/CabRegister
${AR_DIR}/Chm/ChmHandler
${AR_DIR}/Chm/ChmIn
${AR_DIR}/Iso/IsoHandler
${AR_DIR}/Iso/IsoHeader
${AR_DIR}/Iso/IsoIn
${AR_DIR}/Iso/IsoRegister
${AR_DIR}/Nsis/NsisDecode
${AR_DIR}/Nsis/NsisHandler
${AR_DIR}/Nsis/NsisIn
${AR_DIR}/Nsis/NsisRegister
${AR_DIR}/Rar/RarHandler
${AR_DIR}/Rar/Rar5Handler
${AR_DIR}/Tar/TarHandler
${AR_DIR}/Tar/TarHandlerOut
${AR_DIR}/Tar/TarHeader
${AR_DIR}/Tar/TarIn
${AR_DIR}/Tar/TarOut
${AR_DIR}/Tar/TarUpdate
${AR_DIR}/Tar/TarRegister
${AR_DIR}/Udf/UdfHandler
${AR_DIR}/Udf/UdfIn
${AR_DIR}/Wim/WimHandler
${AR_DIR}/Wim/WimHandlerOut
${AR_DIR}/Wim/WimIn
${AR_DIR}/Wim/WimRegister
${AR_DIR}/Zip/ZipAddCommon
${AR_DIR}/Zip/ZipHandler
${AR_DIR}/Zip/ZipHandlerOut
${AR_DIR}/Zip/ZipIn
${AR_DIR}/Zip/ZipItem
${AR_DIR}/Zip/ZipOut
${AR_DIR}/Zip/ZipUpdate
${AR_DIR}/Zip/ZipRegister
${COMPRESS_DIR}/Bcj2Coder
${COMPRESS_DIR}/Bcj2Register
${COMPRESS_DIR}/BcjCoder
${COMPRESS_DIR}/BcjRegister
${COMPRESS_DIR}/BitlDecoder
${COMPRESS_DIR}/BranchMisc
${COMPRESS_DIR}/BranchRegister
${COMPRESS_DIR}/ByteSwap
${COMPRESS_DIR}/BZip2Crc
${COMPRESS_DIR}/BZip2Decoder
${COMPRESS_DIR}/BZip2Encoder
${COMPRESS_DIR}/BZip2Register
${COMPRESS_DIR}/CopyCoder
${COMPRESS_DIR}/CopyRegister
${COMPRESS_DIR}/Deflate64Register
${COMPRESS_DIR}/DeflateDecoder
${COMPRESS_DIR}/DeflateEncoder
${COMPRESS_DIR}/DeflateRegister
${COMPRESS_DIR}/DeltaFilter
${COMPRESS_DIR}/ImplodeDecoder
${COMPRESS_DIR}/LzfseDecoder
${COMPRESS_DIR}/LzhDecoder
${COMPRESS_DIR}/Lzma2Decoder
${COMPRESS_DIR}/Lzma2Encoder
${COMPRESS_DIR}/Lzma2Register
${COMPRESS_DIR}/LzmaDecoder
${COMPRESS_DIR}/LzmaEncoder
${COMPRESS_DIR}/LzmaRegister
${COMPRESS_DIR}/LzmsDecoder
${COMPRESS_DIR}/LzOutWindow
${COMPRESS_DIR}/LzxDecoder
${COMPRESS_DIR}/PpmdDecoder
${COMPRESS_DIR}/PpmdEncoder
${COMPRESS_DIR}/PpmdRegister
${COMPRESS_DIR}/PpmdZip
${COMPRESS_DIR}/QuantumDecoder
${COMPRESS_DIR}/Rar1Decoder
${COMPRESS_DIR}/Rar2Decoder
${COMPRESS_DIR}/Rar3Decoder
${COMPRESS_DIR}/Rar3Vm
${COMPRESS_DIR}/Rar5Decoder
${COMPRESS_DIR}/RarCodecsRegister
${COMPRESS_DIR}/ShrinkDecoder
${COMPRESS_DIR}/XpressDecoder
${COMPRESS_DIR}/XzDecoder
${COMPRESS_DIR}/XzEncoder
${COMPRESS_DIR}/ZlibDecoder
${COMPRESS_DIR}/ZlibEncoder
${COMPRESS_DIR}/ZDecoder
${CRYPTO_DIR}/7zAes
${CRYPTO_DIR}/7zAesRegister
${CRYPTO_DIR}/HmacSha1
${CRYPTO_DIR}/HmacSha256
${CRYPTO_DIR}/MyAes
${CRYPTO_DIR}/MyAesReg
${CRYPTO_DIR}/Pbkdf2HmacSha1
${CRYPTO_DIR}/RandGen
${CRYPTO_DIR}/Rar20Crypto
${CRYPTO_DIR}/Rar5Aes
${CRYPTO_DIR}/RarAes
${CRYPTO_DIR}/WzAes
${CRYPTO_DIR}/ZipCrypto
${CRYPTO_DIR}/ZipStrong
${C_DIR}/7zBuf2
${C_DIR}/7zStream
${C_DIR}/Alloc
${C_DIR}/Bcj2
${C_DIR}/Bcj2Enc
${C_DIR}/Blake2s
${C_DIR}/Bra
${C_DIR}/Bra86
${C_DIR}/BraIA64
${C_DIR}/BwtSort
${C_DIR}/CpuArch
${C_DIR}/Delta
${C_DIR}/HuffEnc
${C_DIR}/LzFind
${C_DIR}/LzFindMt
${C_DIR}/Lzma2Dec
${C_DIR}/Lzma2DecMt
${C_DIR}/Lzma2Enc
${C_DIR}/LzmaDec
${C_DIR}/LzmaEnc
${C_DIR}/MtCoder
${C_DIR}/MtDec
${C_DIR}/Ppmd7
${C_DIR}/Ppmd7Dec
${C_DIR}/Ppmd7Enc
${C_DIR}/Ppmd8
${C_DIR}/Ppmd8Dec
${C_DIR}/Ppmd8Enc
${C_DIR}/Sha1
${C_DIR}/Sha256
${C_DIR}/Sort
${C_DIR}/Threads
${C_DIR}/Xz
${C_DIR}/XzDec
${C_DIR}/XzEnc
${C_DIR}/XzIn
${C_DIR}/XzCrc64

${COMPRESS_DIR}/CodecExports
${AR_DIR}/ArchiveExports
${AR_DIR}/DllExports2

${AR_DIR}/Archive2.def

Asm/x86/XzCrc64Opt.asm

$<TARGET_OBJECTS:CRC>
$<TARGET_OBJECTS:Aes>

${LZMA_DEC_FILES}

CPP/7zip/Bundles/Format7zF/resource.rc
${AR_DIR}/Archive2.def
)
set_target_properties(Format7zF PROPERTIES OUTPUT_NAME "7z" PREFIX "")
target_compile_definitions(Format7zF PRIVATE -DEXTERNAL_CODECS -D_7ZIP_LARGE_PAGES)

# add_executable(LzmaCon )
# set_target_properties(LzmaCon PROPERTIES OUTPUT_NAME "lzma")

# add_executable(SFXCon )
# set_target_properties(SFXCon PROPERTIES OUTPUT_NAME "7zCon" SUFFIX ".sfx")

# add_executable(SFXWin )
# set_target_properties(SFXWin PROPERTIES OUTPUT_NAME "7z" SUFFIX ".sfx")

# add_executable(SFXSetup )
# set_target_properties(SFXSetup PROPERTIES OUTPUT_NAME "7zS" SUFFIX ".sfx")

add_executable(Console 

  $<TARGET_OBJECTS:COMMON>
${COMMON_DIR}/CRC
  ${COMMON_DIR}/CommandLineParser
  ${COMMON_DIR}/ListFileUtils
  ${COMMON_DIR}/StdInStream
  ${COMMON_DIR}/StdOutStream
  
  ${WIN_DIR}/DLL
  ${WIN_DIR}/ErrorMsg
  ${WIN_DIR}/FileDir
  ${WIN_DIR}/FileFind
  ${WIN_DIR}/FileIO
  ${WIN_DIR}/FileLink
  ${WIN_DIR}/FileName
  ${WIN_DIR}/FileSystem
  ${WIN_DIR}/MemoryLock
  ${WIN_DIR}/PropVariant
  ${WIN_DIR}/PropVariantConv
  ${WIN_DIR}/Registry
  ${WIN_DIR}/System
  ${WIN_DIR}/TimeUtils
  
  ${7ZIP_COMMON_DIR}/CreateCoder
  ${7ZIP_COMMON_DIR}/FilePathAutoRename
  ${7ZIP_COMMON_DIR}/FileStreams
  ${7ZIP_COMMON_DIR}/FilterCoder
  ${7ZIP_COMMON_DIR}/LimitedStreams
  ${7ZIP_COMMON_DIR}/MethodProps
  ${7ZIP_COMMON_DIR}/ProgressUtils
  ${7ZIP_COMMON_DIR}/PropId
  ${7ZIP_COMMON_DIR}/StreamObjects
  ${7ZIP_COMMON_DIR}/StreamUtils
  ${7ZIP_COMMON_DIR}/UniqBlocks

  ${AR_DIR}/Common/OutStreamWithCRC

  ${COMPRESS_DIR}/CopyCoder
  
  ${C_DIR}/Alloc
  ${C_DIR}/CpuArch
  ${C_DIR}/Sort
  ${C_DIR}/Threads

  ${UI_DIR}/Console/BenchCon
  ${UI_DIR}/Console/ConsoleClose
  ${UI_DIR}/Console/ExtractCallbackConsole
  ${UI_DIR}/Console/HashCon
  ${UI_DIR}/Console/List
  ${UI_DIR}/Console/Main
  ${UI_DIR}/Console/MainAr
  ${UI_DIR}/Console/OpenCallbackConsole
  ${UI_DIR}/Console/PercentPrinter
  ${UI_DIR}/Console/UpdateCallbackConsole
  ${UI_DIR}/Console/UserInputUtils

  ${UI_DIR}/Common/ArchiveCommandLine
  ${UI_DIR}/Common/ArchiveExtractCallback
  ${UI_DIR}/Common/ArchiveOpenCallback
  ${UI_DIR}/Common/Bench
  ${UI_DIR}/Common/DefaultName
  ${UI_DIR}/Common/EnumDirItems
  ${UI_DIR}/Common/Extract
  ${UI_DIR}/Common/ExtractingFilePath
  ${UI_DIR}/Common/HashCalc
  ${UI_DIR}/Common/LoadCodecs
  ${UI_DIR}/Common/OpenArchive
  ${UI_DIR}/Common/PropIDUtils
  ${UI_DIR}/Common/SetProperties
  ${UI_DIR}/Common/SortUtils
  ${UI_DIR}/Common/TempFiles
  ${UI_DIR}/Common/Update
  ${UI_DIR}/Common/UpdateAction
  ${UI_DIR}/Common/UpdateCallback
  ${UI_DIR}/Common/UpdatePair
  ${UI_DIR}/Common/UpdateProduce
  
  $<TARGET_OBJECTS:CRC>
)
set_target_properties(Console PROPERTIES OUTPUT_NAME "7z")
target_compile_definitions(Console PRIVATE -DEXTERNAL_CODECS -DWIN_LONG_PATH -D_7ZIP_LARGE_PAGES -DSUPPORT_DEVICE_FILE)

# add_library(Explorer MODULE)
# set_target_properties(Explorer PROPERTIES OUTPUT_NAME "7-zip")

# if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#   add_library(Explorer32 MODULE)
#   set_target_properties(Explorer PROPERTIES OUTPUT_NAME "7-zip32")
# endif()

add_executable(FileManager WIN32
$<TARGET_OBJECTS:COMMON>
${COMMON_DIR}/Lang
${COMMON_DIR}/Random

${WIN_DIR}/Clipboard
${WIN_DIR}/CommonDialog
${WIN_DIR}/DLL
${WIN_DIR}/ErrorMsg
${WIN_DIR}/FileDir
${WIN_DIR}/FileFind
${WIN_DIR}/FileIO
${WIN_DIR}/FileLink
${WIN_DIR}/FileName
${WIN_DIR}/MemoryGlobal
${WIN_DIR}/MemoryLock
${WIN_DIR}/Menu
${WIN_DIR}/ProcessUtils
${WIN_DIR}/PropVariant
${WIN_DIR}/PropVariantConv
${WIN_DIR}/Registry
${WIN_DIR}/ResourceString
${WIN_DIR}/Shell
${WIN_DIR}/Synchronization
${WIN_DIR}/System
${WIN_DIR}/TimeUtils
${WIN_DIR}/Window

${WIN_DIR}/FileSystem
${WIN_DIR}/Net
${WIN_DIR}/SecurityUtils

${WIN_DIR}/Control/ComboBox
${WIN_DIR}/Control/Dialog
${WIN_DIR}/Control/ListView
${WIN_DIR}/Control/PropertyPage
${WIN_DIR}/Control/Window2

${7ZIP_COMMON_DIR}/CreateCoder
${7ZIP_COMMON_DIR}/FilePathAutoRename
${7ZIP_COMMON_DIR}/FileStreams
${7ZIP_COMMON_DIR}/FilterCoder
${7ZIP_COMMON_DIR}/LimitedStreams
${7ZIP_COMMON_DIR}/MethodProps
${7ZIP_COMMON_DIR}/ProgressUtils
${7ZIP_COMMON_DIR}/PropId
${7ZIP_COMMON_DIR}/StreamObjects
${7ZIP_COMMON_DIR}/StreamUtils
${7ZIP_COMMON_DIR}/UniqBlocks

${UI_DIR}/Common/ArchiveExtractCallback
${UI_DIR}/Common/ArchiveName
${UI_DIR}/Common/ArchiveOpenCallback
${UI_DIR}/Common/CompressCall
${UI_DIR}/Common/DefaultName
${UI_DIR}/Common/EnumDirItems
${UI_DIR}/Common/ExtractingFilePath
${UI_DIR}/Common/HashCalc
${UI_DIR}/Common/LoadCodecs
${UI_DIR}/Common/OpenArchive
${UI_DIR}/Common/PropIDUtils
${UI_DIR}/Common/SetProperties
${UI_DIR}/Common/SortUtils
${UI_DIR}/Common/UpdateAction
${UI_DIR}/Common/UpdateCallback
${UI_DIR}/Common/UpdatePair
${UI_DIR}/Common/UpdateProduce
${UI_DIR}/Common/WorkDir
${UI_DIR}/Common/ZipRegistry

${UI_DIR}/Explorer/ContextMenu
${UI_DIR}/Explorer/RegistryContextMenu

${UI_DIR}/GUI/HashGUI
${UI_DIR}/GUI/UpdateCallbackGUI2

${COMPRESS_DIR}/CopyCoder

${C_DIR}/Alloc
${C_DIR}/Sort
${C_DIR}/Threads

${UI_DIR}/FileManager/App
${UI_DIR}/FileManager/BrowseDialog
${UI_DIR}/FileManager/ClassDefs
${UI_DIR}/FileManager/EnumFormatEtc
${UI_DIR}/FileManager/ExtractCallback
${UI_DIR}/FileManager/FileFolderPluginOpen
${UI_DIR}/FileManager/FilePlugins
${UI_DIR}/FileManager/FM
${UI_DIR}/FileManager/FoldersPage
${UI_DIR}/FileManager/FormatUtils
${UI_DIR}/FileManager/FSFolder
${UI_DIR}/FileManager/FSFolderCopy
${UI_DIR}/FileManager/HelpUtils
${UI_DIR}/FileManager/LangUtils
${UI_DIR}/FileManager/MenuPage
${UI_DIR}/FileManager/MyLoadMenu
${UI_DIR}/FileManager/OpenCallback
${UI_DIR}/FileManager/OptionsDialog
${UI_DIR}/FileManager/Panel
${UI_DIR}/FileManager/PanelCopy
${UI_DIR}/FileManager/PanelCrc
${UI_DIR}/FileManager/PanelDrag
${UI_DIR}/FileManager/PanelFolderChange
${UI_DIR}/FileManager/PanelItemOpen
${UI_DIR}/FileManager/PanelItems
${UI_DIR}/FileManager/PanelKey
${UI_DIR}/FileManager/PanelListNotify
${UI_DIR}/FileManager/PanelMenu
${UI_DIR}/FileManager/PanelOperations
${UI_DIR}/FileManager/PanelSelect
${UI_DIR}/FileManager/PanelSort
${UI_DIR}/FileManager/PanelSplitFile
${UI_DIR}/FileManager/ProgramLocation
${UI_DIR}/FileManager/PropertyName
${UI_DIR}/FileManager/RegistryAssociations
${UI_DIR}/FileManager/RegistryPlugins
${UI_DIR}/FileManager/RegistryUtils
${UI_DIR}/FileManager/RootFolder
${UI_DIR}/FileManager/SplitUtils
${UI_DIR}/FileManager/StringUtils
${UI_DIR}/FileManager/SysIconUtils
${UI_DIR}/FileManager/TextPairs
${UI_DIR}/FileManager/UpdateCallback100
${UI_DIR}/FileManager/ViewSettings
${UI_DIR}/FileManager/AboutDialog
${UI_DIR}/FileManager/ComboDialog
${UI_DIR}/FileManager/CopyDialog
${UI_DIR}/FileManager/EditDialog
${UI_DIR}/FileManager/EditPage
${UI_DIR}/FileManager/LangPage
${UI_DIR}/FileManager/ListViewDialog
${UI_DIR}/FileManager/MessagesDialog
${UI_DIR}/FileManager/OverwriteDialog
${UI_DIR}/FileManager/PasswordDialog
${UI_DIR}/FileManager/ProgressDialog2
${UI_DIR}/FileManager/SettingsPage
${UI_DIR}/FileManager/SplitDialog
${UI_DIR}/FileManager/SystemPage

${UI_DIR}/FileManager/AltStreamsFolder
${UI_DIR}/FileManager/FSDrives
${UI_DIR}/FileManager/LinkDialog
${UI_DIR}/FileManager/NetFolder

${UI_DIR}/FileManager/resource.rc

${UI_DIR}/Agent/Agent
${UI_DIR}/Agent/AgentOut
${UI_DIR}/Agent/AgentProxy
${UI_DIR}/Agent/ArchiveFolder
${UI_DIR}/Agent/ArchiveFolderOpen
${UI_DIR}/Agent/ArchiveFolderOut
${UI_DIR}/Agent/UpdateCallbackAgent


)
set_target_properties(FileManager PROPERTIES OUTPUT_NAME "7zFM")
target_compile_definitions(FileManager PRIVATE -DLANG -DNEW_FOLDER_INTERFACE -DEXTERNAL_CODECS -DWIN_LONG_PATH -DSUPPORT_DEVICE_FILE)
target_link_libraries(FileManager comctl32 comdlg32 mpr gdi32 htmlhelp)

add_executable(GUI WIN32
${UI_DIR}/GUI/BenchmarkDialog
${UI_DIR}/GUI/CompressDialog
${UI_DIR}/GUI/ExtractDialog
${UI_DIR}/GUI/ExtractGUI
${UI_DIR}/GUI/GUI
${UI_DIR}/GUI/HashGUI
${UI_DIR}/GUI/UpdateCallbackGUI
${UI_DIR}/GUI/UpdateCallbackGUI2
${UI_DIR}/GUI/UpdateGUI

$<TARGET_OBJECTS:COMMON>

${COMMON_DIR}/CRC
  ${COMMON_DIR}/CommandLineParser
  ${COMMON_DIR}/Lang
  ${COMMON_DIR}/ListFileUtils
  
  ${WIN_DIR}/Clipboard
  ${WIN_DIR}/CommonDialog
  ${WIN_DIR}/DLL
  ${WIN_DIR}/ErrorMsg
  ${WIN_DIR}/FileDir
  ${WIN_DIR}/FileFind
  ${WIN_DIR}/FileIO
  ${WIN_DIR}/FileLink
  ${WIN_DIR}/FileName
  ${WIN_DIR}/FileSystem
  ${WIN_DIR}/MemoryGlobal
  ${WIN_DIR}/MemoryLock
  ${WIN_DIR}/PropVariant
  ${WIN_DIR}/PropVariantConv
  ${WIN_DIR}/Registry
  ${WIN_DIR}/ResourceString
  ${WIN_DIR}/Shell
  ${WIN_DIR}/Synchronization
  ${WIN_DIR}/System
  ${WIN_DIR}/TimeUtils
  ${WIN_DIR}/Window
  
  ${WIN_DIR}/Control/ComboBox
  ${WIN_DIR}/Control/Dialog
  ${WIN_DIR}/Control/ListView
  
  ${7ZIP_COMMON_DIR}/CreateCoder
  ${7ZIP_COMMON_DIR}/FilePathAutoRename
  ${7ZIP_COMMON_DIR}/FileStreams
  ${7ZIP_COMMON_DIR}/FilterCoder
  ${7ZIP_COMMON_DIR}/LimitedStreams
  ${7ZIP_COMMON_DIR}/MethodProps
  ${7ZIP_COMMON_DIR}/ProgressUtils
  ${7ZIP_COMMON_DIR}/PropId
  ${7ZIP_COMMON_DIR}/StreamObjects
  ${7ZIP_COMMON_DIR}/StreamUtils
  ${7ZIP_COMMON_DIR}/UniqBlocks
  
  ${UI_DIR}/Common/ArchiveCommandLine
  ${UI_DIR}/Common/ArchiveExtractCallback
  ${UI_DIR}/Common/ArchiveOpenCallback
  ${UI_DIR}/Common/Bench
  ${UI_DIR}/Common/DefaultName
  ${UI_DIR}/Common/EnumDirItems
  ${UI_DIR}/Common/Extract
  ${UI_DIR}/Common/ExtractingFilePath
  ${UI_DIR}/Common/HashCalc
  ${UI_DIR}/Common/LoadCodecs
  ${UI_DIR}/Common/OpenArchive
  ${UI_DIR}/Common/PropIDUtils
  ${UI_DIR}/Common/SetProperties
  ${UI_DIR}/Common/SortUtils
  ${UI_DIR}/Common/TempFiles
  ${UI_DIR}/Common/Update
  ${UI_DIR}/Common/UpdateAction
  ${UI_DIR}/Common/UpdateCallback
  ${UI_DIR}/Common/UpdatePair
  ${UI_DIR}/Common/UpdateProduce
  ${UI_DIR}/Common/WorkDir
  ${UI_DIR}/Common/ZipRegistry
  
  ${AR_DIR}/Common/OutStreamWithCRC
  
  ${UI_DIR}/FileManager/EditDialog
  ${UI_DIR}/FileManager/ExtractCallback
  ${UI_DIR}/FileManager/FormatUtils
  ${UI_DIR}/FileManager/HelpUtils
  ${UI_DIR}/FileManager/LangUtils
  ${UI_DIR}/FileManager/ListViewDialog
  ${UI_DIR}/FileManager/OpenCallback
  ${UI_DIR}/FileManager/ProgramLocation
  ${UI_DIR}/FileManager/PropertyName
  ${UI_DIR}/FileManager/RegistryUtils
  ${UI_DIR}/FileManager/SplitUtils
  ${UI_DIR}/FileManager/StringUtils
  ${UI_DIR}/FileManager/OverwriteDialog
  ${UI_DIR}/FileManager/PasswordDialog
  ${UI_DIR}/FileManager/ProgressDialog2

  ${UI_DIR}/FileManager/BrowseDialog
  ${UI_DIR}/FileManager/ComboDialog
  ${UI_DIR}/FileManager/SysIconUtils
  
  ${UI_DIR}/Explorer/MyMessages
  
  ${COMPRESS_DIR}/CopyCoder
  
  ${C_DIR}/Alloc
  ${C_DIR}/CpuArch
  ${C_DIR}/Sort
  ${C_DIR}/Threads
  
  $<TARGET_OBJECTS:CRC>
  
  ${UI_DIR}/GUI/resource.rc
)
set_target_properties(GUI PROPERTIES OUTPUT_NAME "7zG")
target_compile_definitions(GUI PRIVATE -DLANG -DEXTERNAL_CODECS -DWIN_LONG_PATH -DSUPPORT_DEVICE_FILE -D_7ZIP_LARGE_PAGES)
target_link_libraries(GUI comctl32 comdlg32 gdi32 htmlhelp)

# add_executable(BundleAlone )
# set_target_properties(BundleAlone PROPERTIES OUTPUT_NAME "7za")

# add_executable(BundleAlone7z )
# set_target_properties(BundleAlone7z PROPERTIES OUTPUT_NAME "7zr")

# add_executable(BundleFM WIN32)
# set_target_properties(BundleFM PROPERTIES OUTPUT_NAME "7zFM")
