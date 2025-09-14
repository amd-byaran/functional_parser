# Build and Test DLL Unit Tests PowerShell Script
Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Building and Testing DLL Unit Tests" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Set paths
$ProjectDir = $PSScriptRoot
$BuildDir = Join-Path $ProjectDir "build"
$BinDir = Join-Path $ProjectDir "bin"
$TestDir = Join-Path $ProjectDir "test"
$LibDir = Join-Path $ProjectDir "lib"

# Create directories if they don't exist
if (!(Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir | Out-Null }
if (!(Test-Path $BinDir)) { New-Item -ItemType Directory -Path $BinDir | Out-Null }

Write-Host "Step 1: Building the DLL..." -ForegroundColor Yellow
Write-Host ""

# Build the DLL using our existing script
$buildDllScript = Join-Path $ProjectDir "build_dll.ps1"
if (Test-Path $buildDllScript) {
    & $buildDllScript
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ DLL build failed!" -ForegroundColor Red
        Read-Host "Press Enter to continue"
        exit 1
    }
} else {
    Write-Host "❌ build_dll.ps1 not found!" -ForegroundColor Red
    Read-Host "Press Enter to continue"
    exit 1
}

Write-Host ""
Write-Host "✅ DLL built successfully!" -ForegroundColor Green
Write-Host ""

Write-Host "Step 2: Compiling DLL unit tests..." -ForegroundColor Yellow
Write-Host ""

# Check if DLL and LIB files exist
$dllPath = Join-Path $BinDir "FunctionalCoverageParsers.dll"
$libPath = Join-Path $LibDir "FunctionalCoverageParsers.lib"

if (!(Test-Path $dllPath)) {
    Write-Host "❌ DLL not found at: $dllPath" -ForegroundColor Red
    Read-Host "Press Enter to continue"
    exit 1
}

if (!(Test-Path $libPath)) {
    Write-Host "❌ LIB not found at: $libPath" -ForegroundColor Red
    Read-Host "Press Enter to continue"
    exit 1
}

# Try to find Visual Studio
$vsPath = $null
$vsPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat"
)

foreach ($path in $vsPaths) {
    if (Test-Path $path) {
        $vsPath = $path
        break
    }
}

if ($vsPath) {
    Write-Host "✅ Found Visual Studio at: $vsPath" -ForegroundColor Green
    
    # Compile unit tests
    $unitTestsSource = Join-Path $TestDir "unit_tests.cpp"
    $unitTestsExe = Join-Path $BinDir "unit_tests_dll.exe"
    $includeDir = Join-Path $ProjectDir "include"
    
    $compileCmd = @"
    & "$vsPath" x64 && cl /std:c++17 /EHsc /MD /O2 /I"$includeDir" "$unitTestsSource" /Fe:"$unitTestsExe" /link "$libPath"
"@
    
    try {
        cmd /c "`"$vsPath`" x64 && cl /std:c++17 /EHsc /MD /O2 /I`"$includeDir`" `"$unitTestsSource`" /Fe:`"$unitTestsExe`" /link `"$libPath`""
        
        if ($LASTEXITCODE -eq 0 -and (Test-Path $unitTestsExe)) {
            Write-Host "✅ Unit tests compiled successfully!" -ForegroundColor Green
            Write-Host ""
            
            Write-Host "Step 3: Running DLL unit tests..." -ForegroundColor Yellow
            Write-Host ""
            
            # Change to bin directory and run tests
            Push-Location $BinDir
            try {
                & ".\unit_tests_dll.exe"
                $testResult = $LASTEXITCODE
            } finally {
                Pop-Location
            }
            
            Write-Host ""
            if ($testResult -eq 0) {
                Write-Host "========================================" -ForegroundColor Green
                Write-Host " 🎉 ALL DLL UNIT TESTS PASSED! 🎉" -ForegroundColor Green
                Write-Host "========================================" -ForegroundColor Green
                Write-Host ""
                Write-Host "The FunctionalCoverageParsers DLL is working correctly!" -ForegroundColor Green
                Write-Host "All parser functionality is accessible through the DLL interface." -ForegroundColor Green
            } else {
                Write-Host "========================================" -ForegroundColor Red
                Write-Host " ❌ SOME DLL UNIT TESTS FAILED!" -ForegroundColor Red
                Write-Host "========================================" -ForegroundColor Red
                Write-Host ""
                Write-Host "Please check the test output above for details." -ForegroundColor Red
            }
        } else {
            Write-Host "❌ Unit test compilation failed!" -ForegroundColor Red
            $testResult = 1
        }
    } catch {
        Write-Host "❌ Compilation error: $($_.Exception.Message)" -ForegroundColor Red
        $testResult = 1
    }
} else {
    Write-Host "❌ Visual Studio not found!" -ForegroundColor Red
    Write-Host "Cannot compile unit tests without Visual Studio build tools." -ForegroundColor Yellow
    Write-Host "However, the DLL has been built successfully and can be used." -ForegroundColor Green
    $testResult = 1
}

Write-Host ""
Write-Host "Built artifacts:" -ForegroundColor Cyan
Write-Host "- DLL: $dllPath" -ForegroundColor Gray
Write-Host "- LIB: $libPath" -ForegroundColor Gray
if (Test-Path (Join-Path $BinDir "unit_tests_dll.exe")) {
    Write-Host "- Tests: $(Join-Path $BinDir "unit_tests_dll.exe")" -ForegroundColor Gray
}
Write-Host ""

Read-Host "Press Enter to continue"
exit $testResult