/**
 * @file build_dll.bat
 * @brief Build script to create DLL and LIB files for FunctionalCoverageParsers
 * 
 * This script compiles the FunctionalCoverageParsers library into a DLL with
 * an accompanying import library (LIB) file for linking with client applications.
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

@echo off
echo ====================================================
echo Building FunctionalCoverageParsers DLL and LIB
echo ====================================================
echo.

REM Set build configuration
set BUILD_CONFIG=Release
set TARGET_ARCH=x64
set DLL_NAME=FunctionalCoverageParsers
set VERSION=1.0.0

echo Build Configuration: %BUILD_CONFIG%
echo Target Architecture: %TARGET_ARCH%
echo DLL Name: %DLL_NAME%
echo Version: %VERSION%
echo.

REM Create output directories
if not exist "build" mkdir build
if not exist "build\%BUILD_CONFIG%" mkdir build\%BUILD_CONFIG%
if not exist "bin" mkdir bin
if not exist "lib" mkdir lib

echo Setting up Visual Studio environment...
call Import-VisualStudioEnvironment

REM Define source files
set SRC_FILES=^
src\dashboard_parser.cpp ^
src\groups_parser.cpp ^
src\hierarchy_parser.cpp ^
src\modlist_parser.cpp ^
src\assert_parser.cpp ^
src\parser_utils.cpp ^
src\dll_api.cpp

REM Define compiler flags
set CL_FLAGS=^
/std:c++17 ^
/EHsc ^
/MD ^
/O2 ^
/DBUILDING_COVERAGE_PARSER_DLL ^
/D_WINDLL ^
/D_USRDLL ^
/I./include ^
/Fo"build\%BUILD_CONFIG%\\" ^
/Fe"bin\%DLL_NAME%.dll"

REM Define linker flags
set LINK_FLAGS=^
/DLL ^
/DEF:"FunctionalCoverageParsers.def" ^
/IMPLIB:"lib\%DLL_NAME%.lib" ^
/OUT:"bin\%DLL_NAME%.dll" ^
/SUBSYSTEM:WINDOWS ^
/MACHINE:%TARGET_ARCH%

echo.
echo Compiling source files...
cl %CL_FLAGS% %SRC_FILES% /link %LINK_FLAGS%

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ Build failed with error code %ERRORLEVEL%
    goto :error
)

echo.
echo ✅ Build completed successfully!
echo.
echo Output files:
if exist "bin\%DLL_NAME%.dll" (
    echo ✅ DLL: bin\%DLL_NAME%.dll
    for %%F in ("bin\%DLL_NAME%.dll") do echo    Size: %%~zF bytes
) else (
    echo ❌ DLL file not created
)

if exist "lib\%DLL_NAME%.lib" (
    echo ✅ LIB: lib\%DLL_NAME%.lib  
    for %%F in ("lib\%DLL_NAME%.lib") do echo    Size: %%~zF bytes
) else (
    echo ❌ LIB file not created
)

if exist "lib\%DLL_NAME%.exp" (
    echo ✅ EXP: lib\%DLL_NAME%.exp
    for %%F in ("lib\%DLL_NAME%.exp") do echo    Size: %%~zF bytes
)

echo.
echo Build artifacts created in:
echo   DLL: .\bin\
echo   LIB: .\lib\
echo   OBJ: .\build\%BUILD_CONFIG%\
echo.
echo ====================================================
echo FunctionalCoverageParsers DLL Build Complete
echo ====================================================
goto :end

:error
echo.
echo Build failed. Please check the error messages above.
exit /b 1

:end
echo.
echo To use the library:
echo 1. Link against: lib\%DLL_NAME%.lib
echo 2. Include headers from: include\
echo 3. Ensure DLL is in PATH or same directory as executable
echo.