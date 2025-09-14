@echo off
echo Compiling simple DLL test...

:: Set up Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Compile the simple test
cl simple_dll_test.cpp /Fe:bin\simple_dll_test.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful! Running test...
    echo.
    
    :: Change to bin directory where DLL is located
    cd bin
    
    :: Run the test
    simple_dll_test.exe
    
    :: Save the test result
    set TEST_RESULT=%ERRORLEVEL%
    
    :: Return to project directory
    cd ..
    
    echo.
    if %TEST_RESULT% EQU 0 (
        echo ✅ ALL TESTS PASSED!
    ) else (
        echo ❌ SOME TESTS FAILED!
    )
    
    exit /b %TEST_RESULT%
) else (
    echo.
    echo ❌ Compilation failed!
    exit /b 1
)