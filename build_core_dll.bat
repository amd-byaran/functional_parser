@echo off
echo Building FunctionalCoverageParsers DLL without problematic dll_api...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Building core parsers DLL...
cl /LD src\assert_parser.cpp src\dashboard_parser.cpp src\groups_parser.cpp src\hierarchy_parser.cpp src\modlist_parser.cpp src\parser_utils.cpp /std:c++17 /EHsc /DBUILDING_COVERAGE_PARSER_DLL /I include /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\FunctionalCoverageParsers.dll /DEF:FunctionalCoverageParsers.def /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Core DLL built successfully!
    echo.
    echo Checking DLL and LIB sizes:
    dir bin\FunctionalCoverageParsers.*
) else (
    echo ❌ Core DLL build failed!
)