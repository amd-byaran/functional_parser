/**
 * @file test_functional_dll.cpp
 * @brief Simple test for the FunctionalCoverageParsers DLL
 * 
 * This file tests the basic functionality of our newly built
 * FunctionalCoverageParsers DLL to ensure it loads and functions work.
 */

#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cassert>

// Function pointer types based on the DLL API
typedef const char* (*GetVersionFunc)();
typedef const char* (*GetLibraryInfoFunc)();
typedef void* (*CreateCoverageDatabaseFunc)();
typedef void (*DestroyCoverageDatabaseFunc)(void*);
typedef int (*ValidateDatabaseFunc)(void*);

// Test result tracking
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Test assertion macro
#define FUNCTIONAL_TEST_ASSERT(condition, test_name) \
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
    std::cout << "\n=== Testing FunctionalCoverageParsers DLL Loading ===" << std::endl;
    
    // Test 1: Load the DLL
    HMODULE hDll = LoadLibraryA("FunctionalCoverageParsers.dll");
    FUNCTIONAL_TEST_ASSERT(hDll != NULL, "Load FunctionalCoverageParsers.dll");
    
    if (hDll != NULL) {
        // Test 2: Get basic function addresses
        GetVersionFunc getVersion = (GetVersionFunc)GetProcAddress(hDll, "get_version_string");
        FUNCTIONAL_TEST_ASSERT(getVersion != NULL, "Get get_version_string function address");
        
        GetLibraryInfoFunc getLibraryInfo = (GetLibraryInfoFunc)GetProcAddress(hDll, "get_library_info");
        FUNCTIONAL_TEST_ASSERT(getLibraryInfo != NULL, "Get get_library_info function address");
        
        CreateCoverageDatabaseFunc createDb = (CreateCoverageDatabaseFunc)GetProcAddress(hDll, "create_coverage_database");
        FUNCTIONAL_TEST_ASSERT(createDb != NULL, "Get create_coverage_database function address");
        
        DestroyCoverageDatabaseFunc destroyDb = (DestroyCoverageDatabaseFunc)GetProcAddress(hDll, "destroy_coverage_database");
        FUNCTIONAL_TEST_ASSERT(destroyDb != NULL, "Get destroy_coverage_database function address");
        
        ValidateDatabaseFunc validateDb = (ValidateDatabaseFunc)GetProcAddress(hDll, "validate_database");
        FUNCTIONAL_TEST_ASSERT(validateDb != NULL, "Get validate_database function address");
        
        // Test 3: Call basic functions
        if (getVersion) {
            const char* version = getVersion();
            FUNCTIONAL_TEST_ASSERT(version != NULL, "get_version_string returns non-null");
            std::cout << "    Version: " << version << std::endl;
        }
        
        if (getLibraryInfo) {
            const char* info = getLibraryInfo();
            FUNCTIONAL_TEST_ASSERT(info != NULL, "get_library_info returns non-null");
            std::cout << "    Library Info: " << info << std::endl;
        }
        
        // Test 4: Test database creation and destruction
        if (createDb && destroyDb) {
            void* db = createDb();
            FUNCTIONAL_TEST_ASSERT(db != NULL, "create_coverage_database returns valid handle");
            
            if (db) {
                // Test database validation
                if (validateDb) {
                    int result = validateDb(db);
                    // Empty database should be valid for basic testing
                    std::cout << "    Database validation result: " << result << std::endl;
                }
                
                destroyDb(db);
                std::cout << "    Database destroyed successfully" << std::endl;
            }
        }
        
        // Test 5: Free the library
        BOOL freed = FreeLibrary(hDll);
        FUNCTIONAL_TEST_ASSERT(freed != 0, "Free DLL successfully");
    }
}

void test_parser_functions() {
    std::cout << "\n=== Testing Parser Function Availability ===" << std::endl;
    
    HMODULE hDll = LoadLibraryA("FunctionalCoverageParsers.dll");
    if (hDll != NULL) {
        // Test parser creation functions
        const char* parser_functions[] = {
            "create_dashboard_parser",
            "create_groups_parser", 
            "create_hierarchy_parser",
            "create_modlist_parser",
            "create_assert_parser",
            "destroy_parser"
        };
        
        for (int i = 0; i < 6; i++) {
            FARPROC func = GetProcAddress(hDll, parser_functions[i]);
            FUNCTIONAL_TEST_ASSERT(func != NULL, std::string("Function available: ") + parser_functions[i]);
        }
        
        FreeLibrary(hDll);
    }
}

void print_test_summary() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "FUNCTIONAL DLL TEST SUMMARY" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Total Tests: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (total_tests > 0 ? (100.0 * passed_tests / total_tests) : 0.0) << "%" << std::endl;
    
    if (failed_tests == 0) {
        std::cout << "\n🎉 ALL FUNCTIONAL TESTS PASSED! 🎉" << std::endl;
    } else {
        std::cout << "\n❌ " << failed_tests << " TESTS FAILED!" << std::endl;
    }
}

int main() {
    std::cout << "FunctionalCoverageParsers DLL Tests" << std::endl;
    std::cout << "===================================" << std::endl;
    
    test_dll_loading();
    test_parser_functions();
    
    print_test_summary();
    
    return (failed_tests == 0) ? 0 : 1;
}