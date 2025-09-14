#include <stdio.h>

// Simple C API without C++ dependencies
extern "C" {
    __declspec(dllexport) const char* get_library_version() {
        return "1.0.0";
    }
    
    __declspec(dllexport) int test_function() {
        return 42;
    }
    
    __declspec(dllexport) int add_numbers(int a, int b) {
        return a + b;
    }
}