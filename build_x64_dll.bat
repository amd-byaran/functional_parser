@echo off
echo Setting up Visual Studio x64 environment...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

echo Building simple test DLL...
cl /LD simple_dll.cpp /Fe:bin\simple_test.dll

if %ERRORLEVEL% EQU 0 (
    echo ✅ Simple DLL built successfully!
    echo.
    echo Building main FunctionalCoverageParsers DLL...
    cl /LD src\*.cpp src\dll_api.cpp /std:c++17 /DCOVERAGE_PARSER_EXPORTS /I include /Fe:bin\FunctionalCoverageParsers.dll /DEF:FunctionalCoverageParsers.def
    
    if %ERRORLEVEL% EQU 0 (
        echo ✅ Main DLL built successfully!
        echo.
        echo Checking DLL sizes:
        dir bin\*.dll
    ) else (
        echo ❌ Main DLL build failed!
    )
) else (
    echo ❌ Simple DLL build failed!
)