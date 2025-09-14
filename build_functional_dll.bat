@echo off
echo Building FunctionalCoverageParsers DLL with correct export macro...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Building main FunctionalCoverageParsers DLL with correct macro...
cl /LD src\*.cpp src\dll_api.cpp /std:c++17 /EHsc /DBUILDING_COVERAGE_PARSER_DLL /I include /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\FunctionalCoverageParsers.dll /DEF:FunctionalCoverageParsers.def /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Main DLL built successfully!
    echo.
    echo Checking DLL and LIB sizes:
    dir bin\FunctionalCoverageParsers.*
    if exist lib\FunctionalCoverageParsers.lib (
        dir lib\FunctionalCoverageParsers.lib
    ) else (
        echo Looking for LIB file in bin directory:
        dir bin\FunctionalCoverageParsers.lib
    )
) else (
    echo ❌ Main DLL build failed!
)