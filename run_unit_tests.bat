@echo off
echo Compiling and running unit tests...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Compiling unit tests...
cl test\unit_tests.cpp /std:c++17 /EHsc /I include /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\unit_tests.exe /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Unit tests compiled successfully!
    echo.
    echo Running unit tests...
    echo.
    cd bin
    unit_tests.exe
    set TEST_RESULT=%ERRORLEVEL%
    cd ..
    
    echo.
    if %TEST_RESULT% EQU 0 (
        echo ✅ ALL UNIT TESTS PASSED!
    ) else (
        echo ❌ SOME UNIT TESTS FAILED!
    )
    
    exit /b %TEST_RESULT%
) else (
    echo ❌ Unit test compilation failed!
    exit /b 1
)