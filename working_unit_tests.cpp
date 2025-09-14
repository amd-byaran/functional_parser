/**
 * @file working_unit_tests.cpp
 * @brief Working unit tests for the FunctionalCoverageParsers DLL
 * 
 * This file contains functional unit tests that properly use the DLL API
 * to validate our coverage parser functionality.
 */

#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <string>
#include <windows.h>

// Test result tracking
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Test assertion macro
#define UNIT_TEST_ASSERT(condition, test_name) \
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

// DLL Function typedefs
typedef const char* (*GetErrorStringFunc)(int);
typedef const char* (*GetVersionStringFunc)();
typedef const char* (*GetLibraryInfoFunc)();
typedef void* (*CreateCoverageDatabaseFunc)();
typedef void (*DestroyCoverageDatabaseFunc)(void*);
typedef int (*ValidateDatabaseFunc)(void*);
typedef void* (*CreateDashboardParserFunc)();
typedef void* (*CreateGroupsParserFunc)();
typedef void* (*CreateHierarchyParserFunc)();
typedef void* (*CreateModlistParserFunc)();
typedef void* (*CreateAssertParserFunc)();
typedef void (*DestroyParserFunc)(void*);
typedef int (*ParseFileFunc)(void*, const char*, void*);

// Global DLL handle and function pointers
static HMODULE dll_handle = nullptr;
static GetErrorStringFunc get_error_string = nullptr;
static GetVersionStringFunc get_version_string = nullptr;
static GetLibraryInfoFunc get_library_info = nullptr;
static CreateCoverageDatabaseFunc create_coverage_database = nullptr;
static DestroyCoverageDatabaseFunc destroy_coverage_database = nullptr;
static ValidateDatabaseFunc validate_database = nullptr;
static CreateDashboardParserFunc create_dashboard_parser = nullptr;
static CreateGroupsParserFunc create_groups_parser = nullptr;
static CreateHierarchyParserFunc create_hierarchy_parser = nullptr;
static CreateModlistParserFunc create_modlist_parser = nullptr;
static CreateAssertParserFunc create_assert_parser = nullptr;
static DestroyParserFunc destroy_parser = nullptr;
static ParseFileFunc parse_coverage_file = nullptr;

bool load_dll_and_functions() {
    // Try loading from bin directory
    dll_handle = LoadLibraryA("bin\\FunctionalCoverageParsers.dll");
    if (!dll_handle) {
        std::cout << "✗ Failed to load FunctionalCoverageParsers.dll" << std::endl;
        return false;
    }
    
    std::cout << "✓ DLL loaded successfully" << std::endl;
    
    // Load function pointers
    get_error_string = (GetErrorStringFunc)GetProcAddress(dll_handle, "get_error_string");
    get_version_string = (GetVersionStringFunc)GetProcAddress(dll_handle, "get_version_string");
    get_library_info = (GetLibraryInfoFunc)GetProcAddress(dll_handle, "get_library_info");
    create_coverage_database = (CreateCoverageDatabaseFunc)GetProcAddress(dll_handle, "create_coverage_database");
    destroy_coverage_database = (DestroyCoverageDatabaseFunc)GetProcAddress(dll_handle, "destroy_coverage_database");
    validate_database = (ValidateDatabaseFunc)GetProcAddress(dll_handle, "validate_database");
    create_dashboard_parser = (CreateDashboardParserFunc)GetProcAddress(dll_handle, "create_dashboard_parser");
    create_groups_parser = (CreateGroupsParserFunc)GetProcAddress(dll_handle, "create_groups_parser");
    create_hierarchy_parser = (CreateHierarchyParserFunc)GetProcAddress(dll_handle, "create_hierarchy_parser");
    create_modlist_parser = (CreateModlistParserFunc)GetProcAddress(dll_handle, "create_modlist_parser");
    create_assert_parser = (CreateAssertParserFunc)GetProcAddress(dll_handle, "create_assert_parser");
    destroy_parser = (DestroyParserFunc)GetProcAddress(dll_handle, "destroy_parser");
    parse_coverage_file = (ParseFileFunc)GetProcAddress(dll_handle, "parse_coverage_file");
    
    bool all_functions_loaded = get_error_string && get_version_string && get_library_info && 
                               create_coverage_database && destroy_coverage_database && 
                               validate_database && create_dashboard_parser && 
                               create_groups_parser && create_hierarchy_parser && 
                               create_modlist_parser && create_assert_parser && 
                               destroy_parser && parse_coverage_file;
    
    UNIT_TEST_ASSERT(all_functions_loaded, "All DLL functions loaded");
    return all_functions_loaded;
}

void unload_dll() {
    if (dll_handle) {
        FreeLibrary(dll_handle);
        dll_handle = nullptr;
        std::cout << "✓ DLL unloaded" << std::endl;
    }
}

void test_dll_basic_functions() {
    std::cout << "\n=== Basic DLL Function Tests ===" << std::endl;
    
    // Test version string
    const char* version = get_version_string();
    UNIT_TEST_ASSERT(version != nullptr, "Version string available");
    if (version) {
        std::cout << "  Version: " << version << std::endl;
    }
    
    // Test library info
    const char* info = get_library_info();
    UNIT_TEST_ASSERT(info != nullptr, "Library info available");
    if (info) {
        std::cout << "  Info: " << info << std::endl;
    }
}

void test_database_operations() {
    std::cout << "\n=== Database Operation Tests ===" << std::endl;
    
    // Test database creation
    void* db = create_coverage_database();
    UNIT_TEST_ASSERT(db != nullptr, "Database creation");
    
    if (db) {
        // Test database validation
        int validation_result = validate_database(db);
        UNIT_TEST_ASSERT(validation_result == 0, "Database validation");
        
        // Clean up
        destroy_coverage_database(db);
        std::cout << "  Database destroyed" << std::endl;
    }
}

void test_parser_creation() {
    std::cout << "\n=== Parser Creation Tests ===" << std::endl;
    
    // Test dashboard parser creation
    void* dashboard_parser = create_dashboard_parser();
    UNIT_TEST_ASSERT(dashboard_parser != nullptr, "Dashboard parser creation");
    if (dashboard_parser) {
        destroy_parser(dashboard_parser);
    }
    
    // Test groups parser creation
    void* groups_parser = create_groups_parser();
    UNIT_TEST_ASSERT(groups_parser != nullptr, "Groups parser creation");
    if (groups_parser) {
        destroy_parser(groups_parser);
    }
    
    // Test hierarchy parser creation
    void* hierarchy_parser = create_hierarchy_parser();
    UNIT_TEST_ASSERT(hierarchy_parser != nullptr, "Hierarchy parser creation");
    if (hierarchy_parser) {
        destroy_parser(hierarchy_parser);
    }
    
    // Test modlist parser creation
    void* modlist_parser = create_modlist_parser();
    UNIT_TEST_ASSERT(modlist_parser != nullptr, "Modlist parser creation");
    if (modlist_parser) {
        destroy_parser(modlist_parser);
    }
    
    // Test assert parser creation
    void* assert_parser = create_assert_parser();
    UNIT_TEST_ASSERT(assert_parser != nullptr, "Assert parser creation");
    if (assert_parser) {
        destroy_parser(assert_parser);
    }
}

std::string create_test_dashboard() {
    std::string content = 
        "Dashboard\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "\n"
        "User: test_engineer\n"
        "\n"
        "Version: U-2023.03-SP2-9\n"
        "Command line: urg -full64 -hvp_no_score_missing -dir sim.vdb -format text -report dashboard\n"
        "\n"
        "\n"
        "-------------------------------------------------------------------------------\n"
        "Total Coverage Summary \n"
        "SCORE   ASSERT               GROUP                \n"
        " 31.56   22.11 60148/272087   41.01 134908/328987 \n"
        "\n"
        "\n"
        "-------------------------------------------------------------------------------\n"
        "Hierarchical coverage data for top-level instances \n"
        "SCORE   ASSERT               NAME                                                                                                                                                            \n"
        "  0.00    0.00 0/66          test_instance.cpu.alu                                 \n"
        "  0.00    0.00 0/66          test_instance.cpu.mem                                \n";
    
    std::ofstream file("test_dashboard.txt");
    file << content;
    file.close();
    return "test_dashboard.txt";
}

void test_dashboard_parsing() {
    std::cout << "\n=== Dashboard Parsing Tests ===" << std::endl;
    
    // Try simple test file first
    std::string test_file = "simple_dashboard_test.txt";
    
    // Create database and parser
    void* db = create_coverage_database();
    void* parser = create_dashboard_parser();
    
    if (db && parser) {
        // Test parsing
        std::cout << "  Attempting to parse file: " << test_file << std::endl;
        int result = parse_coverage_file(parser, test_file.c_str(), db);
        std::cout << "  Parse result: " << result << std::endl;
        if (result != 0) {
            const char* error_msg = get_error_string(result);
            if (error_msg) {
                std::cout << "  Error message: " << error_msg << std::endl;
            }
        }
        UNIT_TEST_ASSERT(result == 0, "Dashboard file parsing");
        
        // Clean up
        destroy_parser(parser);
        destroy_coverage_database(db);
    } else {
        std::cout << "  Failed to create database or parser" << std::endl;
        UNIT_TEST_ASSERT(false, "Dashboard file parsing");
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
              << (total_tests > 0 ? (double)passed_tests / total_tests * 100.0 : 0.0) << "%" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    if (failed_tests == 0) {
        std::cout << "🎉 ALL TESTS PASSED! 🎉" << std::endl;
    } else {
        std::cout << "❌ " << failed_tests << " TEST(S) FAILED!" << std::endl;
    }
}

int main() {
    std::cout << "FunctionalCoverageParsers DLL Unit Tests" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Load DLL and functions
    if (!load_dll_and_functions()) {
        std::cout << "Failed to load DLL or functions. Exiting." << std::endl;
        return 1;
    }
    
    // Run tests
    test_dll_basic_functions();
    test_database_operations();
    test_parser_creation();
    test_dashboard_parsing();
    
    // Print summary
    print_test_summary();
    
    // Clean up
    unload_dll();
    
    return (failed_tests == 0) ? 0 : 1;
}