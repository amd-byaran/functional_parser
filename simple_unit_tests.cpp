/**
 * @file simple_unit_tests.cpp
 * @brief Simple unit tests for the working DLL functionality
 * 
 * This file tests the functionality we know works - our simple_test.dll
 * and validates the basic DLL loading and function calling mechanisms.
 */

#include <iostream>
#include <windows.h>
#include <cassert>

// Function pointer types for our simple DLL
typedef const char* (*GetVersionFunc)();
typedef int (*TestFunc)();
typedef int (*AddFunc)(int, int);

// Test result tracking
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Test assertion macro
#define SIMPLE_TEST_ASSERT(condition, test_name) \
    do { \
        total_tests++; \
        if (condition) { \
            passed_tests++; \
            std::cout << "✓ PASS: " << test_name << std::endl; \
        } else { \
            failed_tests++; \
            std::cout << "✗ FAIL: " << test_name << std::endl; \
        } \
    } while(0)

void test_dll_loading() {
    std::cout << "\n=== Testing DLL Loading ===" << std::endl;
    
    // Test 1: Load the DLL
    HMODULE hDll = LoadLibraryA("simple_test.dll");
    SIMPLE_TEST_ASSERT(hDll != NULL, "Load simple_test.dll");
    
    if (hDll != NULL) {
        // Test 2: Get function addresses
        GetVersionFunc getVersion = (GetVersionFunc)GetProcAddress(hDll, "get_library_version");
        SIMPLE_TEST_ASSERT(getVersion != NULL, "Get get_library_version function address");
        
        TestFunc testFunc = (TestFunc)GetProcAddress(hDll, "test_function");
        SIMPLE_TEST_ASSERT(testFunc != NULL, "Get test_function function address");
        
        AddFunc addFunc = (AddFunc)GetProcAddress(hDll, "add_numbers");
        SIMPLE_TEST_ASSERT(addFunc != NULL, "Get add_numbers function address");
        
        // Test 3: Call functions and validate results
        if (getVersion) {
            const char* version = getVersion();
            SIMPLE_TEST_ASSERT(version != NULL, "get_library_version returns non-null");
            SIMPLE_TEST_ASSERT(strcmp(version, "1.0.0") == 0, "get_library_version returns correct version");
        }
        
        if (testFunc) {
            int result = testFunc();
            SIMPLE_TEST_ASSERT(result == 42, "test_function returns 42");
        }
        
        if (addFunc) {
            int result1 = addFunc(5, 3);
            SIMPLE_TEST_ASSERT(result1 == 8, "add_numbers(5, 3) returns 8");
            
            int result2 = addFunc(10, -5);
            SIMPLE_TEST_ASSERT(result2 == 5, "add_numbers(10, -5) returns 5");
            
            int result3 = addFunc(0, 0);
            SIMPLE_TEST_ASSERT(result3 == 0, "add_numbers(0, 0) returns 0");
        }
        
        // Test 4: Free the library
        BOOL freed = FreeLibrary(hDll);
        SIMPLE_TEST_ASSERT(freed != 0, "Free DLL successfully");
    }
}

void test_error_handling() {
    std::cout << "\n=== Testing Error Handling ===" << std::endl;
    
    // Test 1: Try to load non-existent DLL
    HMODULE hDll = LoadLibraryA("non_existent_dll.dll");
    SIMPLE_TEST_ASSERT(hDll == NULL, "Loading non-existent DLL returns NULL");
    
    // Test 2: Try to get non-existent function from valid DLL
    hDll = LoadLibraryA("simple_test.dll");
    if (hDll != NULL) {
        FARPROC badFunc = GetProcAddress(hDll, "non_existent_function");
        SIMPLE_TEST_ASSERT(badFunc == NULL, "Getting non-existent function returns NULL");
        
        FreeLibrary(hDll);
    }
}

void test_multiple_dll_loads() {
    std::cout << "\n=== Testing Multiple DLL Loads ===" << std::endl;
    
    // Test 1: Load same DLL multiple times
    HMODULE hDll1 = LoadLibraryA("simple_test.dll");
    HMODULE hDll2 = LoadLibraryA("simple_test.dll");
    
    SIMPLE_TEST_ASSERT(hDll1 != NULL, "First DLL load succeeds");
    SIMPLE_TEST_ASSERT(hDll2 != NULL, "Second DLL load succeeds");
    
    if (hDll1 && hDll2) {
        // Both handles should work
        GetVersionFunc getVersion1 = (GetVersionFunc)GetProcAddress(hDll1, "get_library_version");
        GetVersionFunc getVersion2 = (GetVersionFunc)GetProcAddress(hDll2, "get_library_version");
        
        SIMPLE_TEST_ASSERT(getVersion1 != NULL, "Function from first handle works");
        SIMPLE_TEST_ASSERT(getVersion2 != NULL, "Function from second handle works");
        
        if (getVersion1 && getVersion2) {
            SIMPLE_TEST_ASSERT(strcmp(getVersion1(), getVersion2()) == 0, 
                              "Both handles return same result");
        }
        
        FreeLibrary(hDll1);
        FreeLibrary(hDll2);
    }
}

void print_test_summary() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "UNIT TEST SUMMARY" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Total Tests: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (total_tests > 0 ? (100.0 * passed_tests / total_tests) : 0.0) << "%" << std::endl;
    
    if (failed_tests == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED! 🎉" << std::endl;
    } else {
        std::cout << "\n❌ " << failed_tests << " TESTS FAILED!" << std::endl;
    }
}

int main() {
    std::cout << "Simple DLL Unit Tests" << std::endl;
    std::cout << "=====================" << std::endl;
    
    test_dll_loading();
    test_error_handling();
    test_multiple_dll_loads();
    
    print_test_summary();
    
    return (failed_tests == 0) ? 0 : 1;
}