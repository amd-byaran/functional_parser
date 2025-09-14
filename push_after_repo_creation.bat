@echo off
echo.
echo ========================================
echo  Pushing to GitHub Repository
echo ========================================
echo.
echo Repository: https://github.com/amd-byaran/functional_parser
echo.

cd "C:\Users\byaran\OneDrive - Advanced Micro Devices Inc\programming\coverage_analyzer\FunctionalParser"

echo Checking git status...
git status

echo.
echo Pushing to GitHub...
git push -u origin master

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  SUCCESS! Code pushed to GitHub
    echo ========================================
    echo.
    echo Repository URL: https://github.com/amd-byaran/functional_parser
    echo.
    echo Your repository now contains:
    echo - Complete C++ functional coverage parser library
    echo - DLL/LIB build system and artifacts
    echo - Comprehensive unit testing suite (46 tests)
    echo - GUI integration documentation and examples
    echo - CMake build configuration
    echo - Usage examples and documentation
    echo.
) else (
    echo.
    echo ========================================
    echo  FAILED! Repository may not exist yet
    echo ========================================
    echo.
    echo Please create the repository first at:
    echo https://github.com/amd-byaran/functional_parser
    echo.
    echo Then run this script again.
)

echo.
pause