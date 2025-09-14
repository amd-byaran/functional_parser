# PowerShell script to test DLL loading
Write-Host "Testing DLL Loading with PowerShell..."

# Add the necessary .NET types for P/Invoke
Add-Type -TypeDefinition @"
    using System;
    using System.Runtime.InteropServices;
    
    public class DllTest {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibrary(string lpFileName);
        
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FreeLibrary(IntPtr hModule);
        
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);
    }
"@

try {
    # Get current directory
    $currentDir = Get-Location
    $dllPath = Join-Path $currentDir "bin\FunctionalCoverageParsers.dll"
    
    Write-Host "Attempting to load DLL: $dllPath"
    
    if (-not (Test-Path $dllPath)) {
        Write-Host "❌ DLL file not found at: $dllPath"
        exit 1
    }
    
    # Load the DLL
    $hModule = [DllTest]::LoadLibrary($dllPath)
    
    if ($hModule -eq [IntPtr]::Zero) {
        Write-Host "❌ Failed to load DLL"
        exit 1
    }
    
    Write-Host "✅ DLL loaded successfully!"
    
    # Test getting proc addresses for our exported functions
    $functions = @(
        "parse_asserts",
        "parse_groups", 
        "parse_grpinfo",
        "parse_hierarchy",
        "parse_modinfo",
        "parse_modlist",
        "parse_dashboard"
    )
    
    $successCount = 0
    foreach ($func in $functions) {
        $procAddr = [DllTest]::GetProcAddress($hModule, $func)
        if ($procAddr -ne [IntPtr]::Zero) {
            Write-Host "✅ Found function: $func"
            $successCount++
        } else {
            Write-Host "❌ Function not found: $func"
        }
    }
    
    # Free the library
    [DllTest]::FreeLibrary($hModule) | Out-Null
    
    Write-Host ""
    Write-Host "Test Results:"
    Write-Host "- DLL Loading: ✅ SUCCESS"
    Write-Host "- Functions Found: $successCount / $($functions.Count)"
    
    if ($successCount -eq $functions.Count) {
        Write-Host "🎉 ALL TESTS PASSED!"
        exit 0
    } else {
        Write-Host "⚠️  Some functions not found"
        exit 1
    }
    
} catch {
    Write-Host "❌ Error during testing: $($_.Exception.Message)"
    exit 1
}