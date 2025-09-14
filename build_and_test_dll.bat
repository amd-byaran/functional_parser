@echo off
echo ========================================
echo  Building and Testing DLL Unit Tests
echo ========================================
echo.

:: Set paths
set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%build"
set "BIN_DIR=%PROJECT_DIR%bin"
set "TEST_DIR=%PROJECT_DIR%test"

:: Create directories if they don't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

echo Step 1: Building the DLL...
echo.

:: First, build the DLL using our existing script
call "%PROJECT_DIR%build_dll.ps1"

if %ERRORLEVEL% NEQ 0 (
    echo ❌ DLL build failed!
    pause
    exit /b 1
)

echo.
echo ✅ DLL built successfully!
echo.

echo Step 2: Compiling DLL unit tests...
echo.

:: Try to set up Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 2>nul
if %ERRORLEVEL% NEQ 0 (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 2>nul
)

:: Compile the DLL unit tests
cl /std:c++17 /EHsc /MD /O2 ^
   /I"%PROJECT_DIR%include" ^
   "%TEST_DIR%\unit_tests.cpp" ^
   /Fe:"%BIN_DIR%\unit_tests_dll.exe" ^
   /link "%PROJECT_DIR%lib\FunctionalCoverageParsers.lib"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Unit test compilation failed!
    echo.
    echo Note: This requires Visual Studio build tools.
    echo If Visual Studio is not available, this is expected.
    echo The DLL has been built successfully and can be used.
    echo.
    pause
    exit /b 1
)

echo ✅ Unit tests compiled successfully!
echo.

echo Step 3: Running DLL unit tests...
echo.

:: Change to bin directory to ensure DLL is found
cd /d "%BIN_DIR%"

:: Run the tests
unit_tests_dll.exe

set TEST_RESULT=%ERRORLEVEL%

:: Return to original directory
cd /d "%PROJECT_DIR%"

echo.
if %TEST_RESULT% EQU 0 (
    echo ========================================
    echo  🎉 ALL DLL UNIT TESTS PASSED! 🎉
    echo ========================================
    echo.
    echo The FunctionalCoverageParsers DLL is working correctly!
    echo All parser functionality is accessible through the DLL interface.
) else (
    echo ========================================
    echo  ❌ SOME DLL UNIT TESTS FAILED!
    echo ========================================
    echo.
    echo Please check the test output above for details.
)

echo.
echo Built artifacts:
echo - DLL: %BIN_DIR%\FunctionalCoverageParsers.dll
echo - LIB: %PROJECT_DIR%lib\FunctionalCoverageParsers.lib
echo - Tests: %BIN_DIR%\unit_tests_dll.exe
echo.

pause
exit /b %TEST_RESULT%