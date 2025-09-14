#include <windows.h>
#include <stdio.h>

// Function pointer types for our simple DLL
typedef const char* (*GetVersionFunc)();
typedef int (*TestFunc)();
typedef int (*AddFunc)(int, int);

int main() {
    printf("Simple DLL Test\n");
    printf("================\n\n");
    
    // Load the DLL
    HMODULE hDll = LoadLibraryA("simple_test.dll");
    if (hDll == NULL) {
        printf("FAILED: Could not load simple_test.dll\n");
        printf("Error code: %d\n", GetLastError());
        return 1;
    }
    
    printf("✅ DLL loaded successfully!\n\n");
    
    // Get function addresses
    GetVersionFunc getVersion = (GetVersionFunc)GetProcAddress(hDll, "get_library_version");
    TestFunc testFunc = (TestFunc)GetProcAddress(hDll, "test_function");
    AddFunc addFunc = (AddFunc)GetProcAddress(hDll, "add_numbers");
    
    if (getVersion) {
        printf("✅ get_library_version: %s\n", getVersion());
    } else {
        printf("❌ get_library_version not found\n");
    }
    
    if (testFunc) {
        printf("✅ test_function returned: %d\n", testFunc());
    } else {
        printf("❌ test_function not found\n");
    }
    
    if (addFunc) {
        printf("✅ add_numbers(5, 3) = %d\n", addFunc(5, 3));
    } else {
        printf("❌ add_numbers not found\n");
    }
    
    // Free the library
    FreeLibrary(hDll);
    
    printf("\n🎉 Simple DLL test completed successfully!\n");
    return 0;
}