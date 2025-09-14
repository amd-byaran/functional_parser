/**
 * @file simple_dll_test.cpp
 * @brief Simple DLL loading test without complex dependencies
 */

#include <windows.h>
#include <stdio.h>

// Function pointer types for DLL functions
typedef const char* (*GetVersionFunc)();
typedef const char* (*GetLibraryInfoFunc)();
typedef void* (*CreateDatabaseFunc)();
typedef void (*DestroyDatabaseFunc)(void*);
typedef const char* (*GetErrorStringFunc)(int);

int main() {
    printf("FunctionalCoverageParsers DLL Test\n");
    printf("===================================\n\n");
    
    // Try to load the DLL
    HMODULE dll = LoadLibraryA("FunctionalCoverageParsers.dll");
    
    if (dll == NULL) {
        printf("FAILED: Could not load FunctionalCoverageParsers.dll\n");
        printf("Error code: %lu\n", GetLastError());
        return 1;
    }
    
    printf("SUCCESS: DLL loaded successfully\n");
    
    // Test basic functions
    int tests_passed = 0;
    int total_tests = 0;
    
    // Test 1: get_version_string
    total_tests++;
    GetVersionFunc getVersion = (GetVersionFunc)GetProcAddress(dll, "get_version_string");
    if (getVersion) {
        const char* version = getVersion();
        if (version) {
            printf("SUCCESS: get_version_string() - Version: %s\n", version);
            tests_passed++;
        } else {
            printf("FAILED: get_version_string() returned NULL\n");
        }
    } else {
        printf("FAILED: Could not find get_version_string function\n");
    }
    
    // Test 2: get_library_info
    total_tests++;
    GetLibraryInfoFunc getLibraryInfo = (GetLibraryInfoFunc)GetProcAddress(dll, "get_library_info");
    if (getLibraryInfo) {
        const char* info = getLibraryInfo();
        if (info) {
            printf("SUCCESS: get_library_info() - Info: %s\n", info);
            tests_passed++;
        } else {
            printf("FAILED: get_library_info() returned NULL\n");
        }
    } else {
        printf("FAILED: Could not find get_library_info function\n");
    }
    
    // Test 3: Database creation/destruction
    total_tests++;
    CreateDatabaseFunc createDb = (CreateDatabaseFunc)GetProcAddress(dll, "create_coverage_database");
    DestroyDatabaseFunc destroyDb = (DestroyDatabaseFunc)GetProcAddress(dll, "destroy_coverage_database");
    
    if (createDb && destroyDb) {
        void* db = createDb();
        if (db) {
            printf("SUCCESS: create_coverage_database() works\n");
            destroyDb(db);
            printf("SUCCESS: destroy_coverage_database() works\n");
            tests_passed++;
        } else {
            printf("FAILED: create_coverage_database() returned NULL\n");
        }
    } else {
        printf("FAILED: Could not find database functions\n");
    }
    
    // Test 4: Error string function
    total_tests++;
    GetErrorStringFunc getErrorString = (GetErrorStringFunc)GetProcAddress(dll, "get_error_string");
    if (getErrorString) {
        const char* error = getErrorString(1);
        if (error) {
            printf("SUCCESS: get_error_string() - Error: %s\n", error);
            tests_passed++;
        } else {
            printf("FAILED: get_error_string() returned NULL\n");
        }
    } else {
        printf("FAILED: Could not find get_error_string function\n");
    }
    
    // Cleanup
    FreeLibrary(dll);
    printf("SUCCESS: DLL unloaded\n");
    
    // Summary
    printf("\n===================================\n");
    printf("Test Results: %d/%d tests passed\n", tests_passed, total_tests);
    printf("Success Rate: %.1f%%\n", (100.0 * tests_passed) / total_tests);
    
    if (tests_passed == total_tests) {
        printf("ALL TESTS PASSED!\n");
        printf("The DLL is working correctly.\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED!\n");
        printf("The DLL may have issues.\n");
        return 1;
    }
}