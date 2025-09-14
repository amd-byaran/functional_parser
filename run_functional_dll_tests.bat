@echo off
echo Compiling and running functional DLL tests...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Compiling functional DLL tests...
cl test_functional_dll.cpp /std:c++17 /EHsc /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\test_functional_dll.exe /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Functional DLL tests compiled successfully!
    echo.
    echo Running functional DLL tests...
    echo.
    cd bin
    test_functional_dll.exe
    set TEST_RESULT=%ERRORLEVEL%
    cd ..
    
    echo.
    if %TEST_RESULT% EQU 0 (
        echo ✅ ALL FUNCTIONAL DLL TESTS PASSED!
    ) else (
        echo ❌ SOME FUNCTIONAL DLL TESTS FAILED!
    )
    
    exit /b %TEST_RESULT%
) else (
    echo ❌ Functional DLL test compilation failed!
    exit /b 1
)