@echo off
echo Setting up Visual Studio x64 environment with manual paths...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Building simple test DLL with manual paths...
cl /LD simple_dll.cpp /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\simple_test.dll /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Simple DLL built successfully!
    echo.
    echo Building main FunctionalCoverageParsers DLL...
    cl /LD src\*.cpp src\dll_api.cpp /std:c++17 /DCOVERAGE_PARSER_EXPORTS /I include /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\FunctionalCoverageParsers.dll /DEF:FunctionalCoverageParsers.def /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"
    
    if %ERRORLEVEL% EQU 0 (
        echo ✅ Main DLL built successfully!
        echo.
        echo Checking DLL sizes:
        dir bin\*.dll
        echo.
        echo Creating LIB file listing:
        dir lib\*.lib
    ) else (
        echo ❌ Main DLL build failed!
    )
) else (
    echo ❌ Simple DLL build failed!
)