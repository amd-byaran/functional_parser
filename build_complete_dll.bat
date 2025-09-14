@echo off
echo Building complete FunctionalCoverageParsers DLL...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Building complete DLL with all source files...
cl /LD src\*.cpp /std:c++17 /EHsc /DBUILDING_COVERAGE_PARSER_DLL /I include /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\FunctionalCoverageParsers.dll /DEF:FunctionalCoverageParsers.def /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Complete DLL built successfully!
    echo.
    echo Checking DLL and LIB sizes:
    dir bin\FunctionalCoverageParsers.*
    echo.
    echo Moving LIB file to lib directory:
    if exist bin\FunctionalCoverageParsers.lib (
        move bin\FunctionalCoverageParsers.lib lib\
        echo ✅ LIB file moved to lib directory
    )
    echo.
    echo Final file listing:
    dir bin\FunctionalCoverageParsers.dll
    dir lib\FunctionalCoverageParsers.lib
) else (
    echo ❌ Complete DLL build failed!
)