@echo off
echo Testing simple DLL...
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvarsall.bat" x64

set UCRT_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set SHARED_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
set UM_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
set UCRT_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64
set UM_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64

echo Compiling simple DLL test...
cl test_simple_dll.cpp /I"%UCRT_INCLUDE%" /I"%SHARED_INCLUDE%" /I"%UM_INCLUDE%" /Fe:bin\test_simple_dll.exe /link /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%UM_LIB%"

if %ERRORLEVEL% EQU 0 (
    echo ✅ Test compiled successfully!
    echo.
    echo Running test...
    cd bin
    test_simple_dll.exe
    cd ..
) else (
    echo ❌ Test compilation failed!
)