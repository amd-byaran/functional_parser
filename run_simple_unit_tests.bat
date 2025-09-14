@echo off
echo Compiling and running simple unit tests...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Compiling simple unit tests...
cl simple_unit_tests.cpp /std:c++17 /EHsc /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\simple_unit_tests.exe /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Simple unit tests compiled successfully!
    echo.
    echo Running simple unit tests...
    echo.
    cd bin
    simple_unit_tests.exe
    set TEST_RESULT=%ERRORLEVEL%
    cd ..
    
    echo.
    if %TEST_RESULT% EQU 0 (
        echo ✅ ALL SIMPLE UNIT TESTS PASSED!
    ) else (
        echo ❌ SOME SIMPLE UNIT TESTS FAILED!
    )
    
    exit /b %TEST_RESULT%
) else (
    echo ❌ Simple unit test compilation failed!
    exit /b 1
)