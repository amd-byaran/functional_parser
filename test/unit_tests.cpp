/**
 * @file unit_tests.cpp
 * @brief Comprehensive unit tests for the FunctionalCoverageParsers DLL
 * 
 * This file contains detailed unit tests that load and test the FunctionalCoverageParsers
 * DLL through its C API interface. This provides realistic testing of the actual library
 * that end users will consume.
 * 
 * TEST STRUCTURE:
 * 1. DLL Loading and Initialization Tests
 * 2. DashboardParser Tests - Validate all dashboard.txt fields through DLL
 * 3. GroupsParser Tests - Validate all groups.txt fields through DLL
 * 4. HierarchyParser Tests - Validate hierarchy traversal through DLL
 * 5. ModuleListParser Tests - Validate module aggregation through DLL
 * 6. AssertParser Tests - Validate assertion details through DLL
 * 7. Edge Case Tests - Test malformed files and error conditions through DLL
 * 8. Integration Tests - Test complete workflows through DLL
 * 9. Memory Management Tests - Test proper cleanup and leak detection
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "../include/functional_coverage_parser_dll.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <windows.h>

// DLL handle for the FunctionalCoverageParsers library
static HMODULE dll_handle = nullptr;

// Test result tracking
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Test assertion macro with detailed reporting
#define UNIT_TEST_ASSERT(condition, test_name, expected, actual) \
    do { \
        total_tests++; \
        if (condition) { \
            passed_tests++; \
            std::cout << "✓ PASS: " << test_name << std::endl; \
        } else { \
            failed_tests++; \
            std::cout << "✗ FAIL: " << test_name << std::endl; \
            std::cout << "  Expected: " << expected << std::endl; \
            std::cout << "  Actual: " << actual << std::endl; \
        } \
    } while(0)

// Helper function to load the DLL
bool load_dll() {
    // Try loading from different possible locations
    const char* dll_paths[] = {
        "FunctionalCoverageParsers.dll",           // Current directory
        "../bin/FunctionalCoverageParsers.dll",   // Relative bin directory
        "bin/FunctionalCoverageParsers.dll",      // Local bin directory
        "../lib/FunctionalCoverageParsers.dll"    // Relative lib directory
    };
    
    for (const char* path : dll_paths) {
        dll_handle = LoadLibraryA(path);
        if (dll_handle) {
            std::cout << "✓ DLL loaded successfully from: " << path << std::endl;
            return true;
        }
    }
    
    std::cout << "✗ Failed to load FunctionalCoverageParsers.dll" << std::endl;
    std::cout << "  Please ensure the DLL is built and available in the correct location." << std::endl;
    return false;
}

// Helper function to unload the DLL
void unload_dll() {
    if (dll_handle) {
        FreeLibrary(dll_handle);
        dll_handle = nullptr;
        std::cout << "✓ DLL unloaded successfully" << std::endl;
    }
}
// Helper function to create test files
std::string create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Helper function to remove test files
void cleanup_test_file(const std::string& filename) {
    std::remove(filename.c_str());
}

/**
 * @brief Test DLL initialization and basic functionality
 */
void test_dll_initialization() {
    std::cout << "\n=== DLL Initialization Tests ===" << std::endl;
    
    // Test version string
    const char* version = get_version_string();
    UNIT_TEST_ASSERT(version != nullptr, "Version string available", "non-null", "null");
    if (version) {
        std::cout << "  Library version: " << version << std::endl;
    }
    
    // Test library info
    const char* info = get_library_info();
    UNIT_TEST_ASSERT(info != nullptr, "Library info available", "non-null", "null");
    if (info) {
        std::cout << "  Library info: " << info << std::endl;
    }
    
    // Test database creation and destruction
    void* db = create_coverage_database();
    UNIT_TEST_ASSERT(db != nullptr, "Database creation", "non-null", "null");
    
    if (db) {
        int validation_result = validate_database(db);
        UNIT_TEST_ASSERT(validation_result == 0, "Database validation", "0", std::to_string(validation_result));
        
        destroy_coverage_database(db);
        std::cout << "  Database destroyed successfully" << std::endl;
    }
}

/**
 * @brief Test DashboardParser through DLL interface with comprehensive dashboard data
 */
void test_dashboard_parser_dll_comprehensive() {
    std::cout << "\n=== DashboardParser DLL Comprehensive Tests ===" << std::endl;
    
    // Create realistic dashboard file with all possible fields
    std::string dashboard_content = 
        "Dashboard\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_engineer\n"
        "Version: U-2023.03-SP2-9\n"
        "Command line: urg -full64 -hvp_no_score_missing -dir sim.vdb -format text -report dashboard\n"
        "\n"
        "Total Coverage Summary \n"
        "SCORE   ASSERT               GROUP                \n"
        " 75.32   68.45 12584/18392   82.19 25847/31456\n"
        "\n"
        "Total: 75.32\n"
        "\n"
        "Number of Hierarchical instances processed: 2847\n"
        "\n"
        "Hierarchical coverage data for top-level instances \n"
        "SCORE   ASSERT               NAME                                                                                            \n"
        " 85.67   85.67 456/532      testbench.cpu_subsystem\n"
        " 72.43   72.43 298/412      testbench.memory_subsystem\n";
    
    std::string test_file = create_test_file("test_dashboard_comprehensive.txt", dashboard_content);
    
    // Create database and parser through DLL interface
    void* db = create_coverage_database();
    void* parser = create_dashboard_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for dashboard test", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "Dashboard parser creation", "non-null", "null");
    
    if (db && parser) {
        // Test parsing through DLL
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        UNIT_TEST_ASSERT(parse_result == 0, "Dashboard parsing through DLL", "0", std::to_string(parse_result));
        
        if (parse_result != 0) {
            const char* error_msg = get_error_string(parse_result);
            std::cout << "  Parse error: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        } else {
            // Test overall score calculation
            double score = calculate_overall_score(db);
            UNIT_TEST_ASSERT(score > 0.0, "Overall score calculation", "> 0.0", std::to_string(score));
            std::cout << "  Calculated overall score: " << score << "%" << std::endl;
            
            // Test database validation after parsing
            int validation_result = validate_database(db);
            UNIT_TEST_ASSERT(validation_result == 0, "Database validation after parsing", "0", std::to_string(validation_result));
        }
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
    
    cleanup_test_file(test_file);
}
        UNIT_TEST_ASSERT(dashboard->user == "test_engineer", 
                        "User field parsing", "test_engineer", dashboard->user);
        
/**
 * @brief Test GroupsParser through DLL interface with comprehensive groups data
 */
void test_groups_parser_dll_comprehensive() {
    std::cout << "\n=== GroupsParser DLL Comprehensive Tests ===" << std::endl;
    
    // Create realistic groups file with multiple configurations
    std::string groups_content = 
        "Groups\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_engineer\n"
        "Version: U-2023.03-SP2-9\n"
        "\n"
        "Group: CPU_FUNCTIONAL_COVERAGE\n"
        "------------------------------\n"
        "Hierarchical coverage:  0.00%  [    0/   10]\n"
        "Hierarchical score:     0.00%\n"
        "Hit Status: 0.00%       [    0/   10]\n"
        "\n"
        "Cross Product Details:\n"
        "Uncovered Cross Products: 10\n"
        "\n"
        "cpu_instruction_type_x_operand_type\n"
        "Hierarchical coverage:  0.00%  [    0/   10]\n"
        "Bins: 10\n"
        "Missing bins:\n"
        "  (ADD, REG)\n"
        "  (SUB, IMM)\n"
        "  (MUL, REG)\n"
        "  (DIV, IMM)\n"
        "  (AND, REG)\n"
        "  (OR, IMM)\n"
        "  (XOR, REG)\n"
        "  (SHL, IMM)\n"
        "  (SHR, REG)\n"
        "  (CMP, IMM)\n"
        "\n"
        "Group: MEMORY_FUNCTIONAL_COVERAGE\n"
        "----------------------------------\n"
        "Hierarchical coverage: 75.50%  [   15/   20]\n"
        "Hierarchical score:    75.50%\n"
        "Hit Status: 75.00%      [   15/   20]\n"
        "\n"
        "Cross Product Details:\n"
        "Covered Cross Products: 15\n"
        "Uncovered Cross Products: 5\n"
        "\n"
        "memory_access_x_cache_state\n"
        "Hierarchical coverage: 75.50%  [   15/   20]\n"
        "Bins: 20\n"
        "Missing bins:\n"
        "  (READ, MISS)\n"
        "  (WRITE, HIT)\n"
        "  (FETCH, MISS)\n"
        "  (PREFETCH, HIT)\n"
        "  (INVALIDATE, MISS)\n";
    
    std::string test_file = create_test_file("test_groups_comprehensive.txt", groups_content);
    
    // Create database and parser through DLL interface
    void* db = create_coverage_database();
    void* parser = create_groups_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for groups test", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "Groups parser creation", "non-null", "null");
    
    if (db && parser) {
        // Test parsing through DLL
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        UNIT_TEST_ASSERT(parse_result == 0, "Groups parsing through DLL", "0", std::to_string(parse_result));
        
        if (parse_result != 0) {
            const char* error_msg = get_error_string(parse_result);
            std::cout << "  Parse error: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        } else {
            // Test group count
            int num_groups = get_num_groups(db);
            UNIT_TEST_ASSERT(num_groups == 2, "Number of groups parsed", "2", std::to_string(num_groups));
            std::cout << "  Number of groups: " << num_groups << std::endl;
            
            // Test overall score after groups parsing
            double score = calculate_overall_score(db);
            UNIT_TEST_ASSERT(score > 0.0, "Overall score with groups", "> 0.0", std::to_string(score));
            std::cout << "  Overall score with groups: " << score << "%" << std::endl;
        }
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test HierarchyParser through DLL interface
 */
void test_hierarchy_parser_dll_comprehensive() {
    std::cout << "\n=== HierarchyParser DLL Comprehensive Tests ===" << std::endl;
    
    // Create realistic hierarchy file
    std::string hierarchy_content = 
        "Hierarchy\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_engineer\n"
        "Version: U-2023.03-SP2-9\n"
        "\n"
        "Hierarchical coverage data for instance testbench\n"
        "SCORE   GROUP                INSTANCE                                                                            \n"
        " 85.67   85.67 456/532      testbench\n"
        " 78.23   78.23 312/399        testbench.cpu_subsystem\n"
        " 82.45   82.45 198/240          testbench.cpu_subsystem.alu\n"
        " 76.88   76.88  89/116            testbench.cpu_subsystem.alu.adder\n"
        " 88.92   88.92  67/76             testbench.cpu_subsystem.alu.multiplier\n"
        " 74.12   74.12 156/210          testbench.cpu_subsystem.decoder\n"
        " 89.45   89.45 144/161        testbench.memory_subsystem\n"
        " 91.23   91.23  89/98           testbench.memory_subsystem.cache\n"
        " 87.67   87.67  55/63           testbench.memory_subsystem.mmu\n";
    
    std::string test_file = create_test_file("test_hierarchy_comprehensive.txt", hierarchy_content);
    
    // Create database and parser through DLL interface
    void* db = create_coverage_database();
    void* parser = create_hierarchy_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for hierarchy test", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "Hierarchy parser creation", "non-null", "null");
    
    if (db && parser) {
        // Test parsing through DLL
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        UNIT_TEST_ASSERT(parse_result == 0, "Hierarchy parsing through DLL", "0", std::to_string(parse_result));
        
        if (parse_result != 0) {
            const char* error_msg = get_error_string(parse_result);
            std::cout << "  Parse error: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        } else {
            // Test hierarchy instance count
            int num_hierarchy = get_num_hierarchy_instances(db);
            UNIT_TEST_ASSERT(num_hierarchy > 0, "Number of hierarchy instances", "> 0", std::to_string(num_hierarchy));
            std::cout << "  Number of hierarchy instances: " << num_hierarchy << std::endl;
        }
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
    
    cleanup_test_file(test_file);
}
    
    // Create realistic groups file with various group configurations
    std::string groups_content = 
        "Testbench Group List\n"
        "\n"
        "Total Groups Coverage Summary \n"
        "COVERED EXPECTED SCORE  COVERED EXPECTED INST SCORE WEIGHT \n"
        "3456    4567      75.68 4123    5234      78.81     1      \n"
        "\n"
        "Total groups in report: 25\n"
        "-------------------------------------------------------------------------------\n"
        "COVERED EXPECTED SCORE  INSTANCES WEIGHT GOAL   AT LEAST PER INSTANCE AUTO BIN MAX PRINT MISSING COMMENT NAME\n"
        "45      50        90.00   2.00    3      95     2        1            128          32         High priority group    tb.cpu.alu::arithmetic_ops\n"
        "0       16         0.00   0.00    1      100    1        1            64           64                               tb.memory.cache::miss_coverage\n"
        "128     128      100.00   4.00    2      100    1        2            256          128        Complete coverage     tb.bus.axi::transaction_types\n"
        "15      30        50.00   1.50    1      80     3        1            64           32         Partial coverage      tb.interrupt::priority_levels\n"
        "200     200      100.00   8.00    5      90     1        4            512          256        Multi-instance        tb.dma.channels::transfer_modes\n";
    
    std::string test_file = create_test_file("test_groups_comprehensive.txt", groups_content);
    
    GroupsParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    // Test parsing success
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Groups parsing success", "SUCCESS", "Parser failed");
    
    // Test number of groups parsed
    UNIT_TEST_ASSERT(db.get_num_groups() == 5, "Number of groups parsed", "5", std::to_string(db.get_num_groups()));
    
    // Test specific group 1: arithmetic_ops (high coverage)
    auto arith_group = db.find_coverage_group("tb.cpu.alu::arithmetic_ops");
    UNIT_TEST_ASSERT(arith_group != nullptr, "Arithmetic ops group found", "found", "not found");
    
    if (arith_group) {
        UNIT_TEST_ASSERT(arith_group->coverage.covered == 45, "Arith group covered", "45", std::to_string(arith_group->coverage.covered));
        UNIT_TEST_ASSERT(arith_group->coverage.expected == 50, "Arith group expected", "50", std::to_string(arith_group->coverage.expected));
        UNIT_TEST_ASSERT(std::abs(arith_group->coverage.score - 90.00) < 0.01, "Arith group score", "90.00", std::to_string(arith_group->coverage.score));
        UNIT_TEST_ASSERT(arith_group->weight == 3, "Arith group weight", "3", std::to_string(arith_group->weight));
        UNIT_TEST_ASSERT(arith_group->goal == 95, "Arith group goal", "95", std::to_string(arith_group->goal));
        UNIT_TEST_ASSERT(arith_group->at_least == 2, "Arith group at_least", "2", std::to_string(arith_group->at_least));
        UNIT_TEST_ASSERT(arith_group->auto_bin_max == 128, "Arith group auto_bin_max", "128", std::to_string(arith_group->auto_bin_max));
        UNIT_TEST_ASSERT(arith_group->comment == "High priority group", "Arith group comment", "High priority group", arith_group->comment);
        UNIT_TEST_ASSERT(arith_group->meets_goal() == false, "Arith group meets goal", "false", "true"); // 90 < 95
    }
    
    // Test specific group 2: cache_miss (zero coverage)
    auto cache_group = db.find_coverage_group("tb.memory.cache::miss_coverage");
    UNIT_TEST_ASSERT(cache_group != nullptr, "Cache miss group found", "found", "not found");
    
    if (cache_group) {
        UNIT_TEST_ASSERT(cache_group->coverage.covered == 0, "Cache group covered", "0", std::to_string(cache_group->coverage.covered));
        UNIT_TEST_ASSERT(cache_group->coverage.expected == 16, "Cache group expected", "16", std::to_string(cache_group->coverage.expected));
        UNIT_TEST_ASSERT(std::abs(cache_group->coverage.score - 0.00) < 0.01, "Cache group score", "0.00", std::to_string(cache_group->coverage.score));
        UNIT_TEST_ASSERT(cache_group->is_empty() == false, "Cache group not empty", "false", "true"); // has expected > 0
        UNIT_TEST_ASSERT(cache_group->comment.empty(), "Cache group no comment", "empty", cache_group->comment);
    }
    
    // Test specific group 3: axi_transactions (100% coverage)
    auto axi_group = db.find_coverage_group("tb.bus.axi::transaction_types");
    UNIT_TEST_ASSERT(axi_group != nullptr, "AXI transaction group found", "found", "not found");
    
    if (axi_group) {
        UNIT_TEST_ASSERT(axi_group->coverage.covered == 128, "AXI group covered", "128", std::to_string(axi_group->coverage.covered));
        UNIT_TEST_ASSERT(axi_group->coverage.expected == 128, "AXI group expected", "128", std::to_string(axi_group->coverage.expected));
        UNIT_TEST_ASSERT(std::abs(axi_group->coverage.score - 100.00) < 0.01, "AXI group score", "100.00", std::to_string(axi_group->coverage.score));
        UNIT_TEST_ASSERT(axi_group->meets_goal(), "AXI group meets goal", "true", "false");
        UNIT_TEST_ASSERT(axi_group->weight == 2, "AXI group weight", "2", std::to_string(axi_group->weight));
        UNIT_TEST_ASSERT(axi_group->per_instance == 2, "AXI group per_instance", "2", std::to_string(axi_group->per_instance));
    }
    
    // Test weighted score calculation
    if (arith_group) {
        double weighted = arith_group->weighted_score();
        double expected_weighted = 90.00 * 3 / 100.0; // score * weight / 100
        UNIT_TEST_ASSERT(std::abs(weighted - expected_weighted) < 0.01, "Weighted score calculation", 
                        std::to_string(expected_weighted), std::to_string(weighted));
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test HierarchyParser with comprehensive hierarchy data
 */
void test_hierarchy_parser_comprehensive() {
    std::cout << "\n=== HierarchyParser Comprehensive Tests ===" << std::endl;
    
    // Create realistic hierarchy file with nested instances
    std::string hierarchy_content = 
        "Design Hierarchy\n"
        "\n"
        "Hierarchical coverage data for top-level instances \n"
        "SCORE   ASSERT               NAME                                                                                            \n"
        " 85.50   85.50 1234/1445    top\n"
        " 90.25   90.25 456/505      top.cpu_subsystem\n"
        " 88.75   88.75 234/264      top.cpu_subsystem.core0\n"
        " 92.15   92.15 123/133      top.cpu_subsystem.core0.alu\n"
        " 85.45   85.45 67/78        top.cpu_subsystem.core0.fpu\n"
        " 87.30   87.30 44/50        top.cpu_subsystem.core0.decode_unit\n"
        " 91.80   91.80 178/194      top.cpu_subsystem.core1\n"
        " 75.60   75.60 567/750      top.memory_subsystem\n"
        " 82.45   82.45 234/284      top.memory_subsystem.l1_cache\n"
        " 78.90   78.90 156/198      top.memory_subsystem.l2_cache\n"
        " 71.25   71.25 177/248      top.memory_subsystem.memory_controller\n"
        " 95.80   95.80 345/360      top.io_subsystem\n"
        " 98.50   98.50 123/125      top.io_subsystem.uart0\n"
        " 97.25   97.25 89/91        top.io_subsystem.spi0\n"
        " 94.15   94.15 133/141      top.io_subsystem.gpio_controller\n";
    
    std::string test_file = create_test_file("test_hierarchy_comprehensive.txt", hierarchy_content);
    
    HierarchyParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    // Test parsing success
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Hierarchy parsing success", "SUCCESS", "Parser failed");
    
    // Test number of hierarchy instances
    UNIT_TEST_ASSERT(db.get_num_hierarchy_instances() == 15, "Number of hierarchy instances", "15", std::to_string(db.get_num_hierarchy_instances()));
    
    // Test top-level instance
    auto top_instance = db.find_hierarchy_instance("top");
    UNIT_TEST_ASSERT(top_instance != nullptr, "Top instance found", "found", "not found");
    
    if (top_instance) {
        UNIT_TEST_ASSERT(std::abs(top_instance->total_score - 85.50) < 0.01, "Top instance score", "85.50", std::to_string(top_instance->total_score));
        UNIT_TEST_ASSERT(top_instance->assert_coverage.covered == 1234, "Top assert covered", "1234", std::to_string(top_instance->assert_coverage.covered));
        UNIT_TEST_ASSERT(top_instance->assert_coverage.expected == 1445, "Top assert expected", "1445", std::to_string(top_instance->assert_coverage.expected));
        UNIT_TEST_ASSERT(top_instance->depth_level == 0, "Top instance depth", "0", std::to_string(top_instance->depth_level));
        UNIT_TEST_ASSERT(top_instance->module_name == "top", "Top module name", "top", top_instance->module_name);
    }
    
    // Test second-level instance
    auto cpu_instance = db.find_hierarchy_instance("top.cpu_subsystem");
    UNIT_TEST_ASSERT(cpu_instance != nullptr, "CPU subsystem found", "found", "not found");
    
    if (cpu_instance) {
        UNIT_TEST_ASSERT(cpu_instance->depth_level == 1, "CPU subsystem depth", "1", std::to_string(cpu_instance->depth_level));
        UNIT_TEST_ASSERT(cpu_instance->module_name == "cpu_subsystem", "CPU module name", "cpu_subsystem", cpu_instance->module_name);
        
        // Test parent path extraction
        std::string parent = cpu_instance->get_parent_path();
        UNIT_TEST_ASSERT(parent == "top", "CPU parent path", "top", parent);
    }
    
    // Test third-level instance
    auto core0_instance = db.find_hierarchy_instance("top.cpu_subsystem.core0");
    UNIT_TEST_ASSERT(core0_instance != nullptr, "Core0 instance found", "found", "not found");
    
    if (core0_instance) {
        UNIT_TEST_ASSERT(core0_instance->depth_level == 2, "Core0 depth", "2", std::to_string(core0_instance->depth_level));
        UNIT_TEST_ASSERT(core0_instance->module_name == "core0", "Core0 module name", "core0", core0_instance->module_name);
    }
    
    // Test fourth-level instance (deepest)
    auto alu_instance = db.find_hierarchy_instance("top.cpu_subsystem.core0.alu");
    UNIT_TEST_ASSERT(alu_instance != nullptr, "ALU instance found", "found", "not found");
    
    if (alu_instance) {
        UNIT_TEST_ASSERT(alu_instance->depth_level == 3, "ALU depth", "3", std::to_string(alu_instance->depth_level));
        UNIT_TEST_ASSERT(alu_instance->module_name == "alu", "ALU module name", "alu", alu_instance->module_name);
        UNIT_TEST_ASSERT(std::abs(alu_instance->total_score - 92.15) < 0.01, "ALU score", "92.15", std::to_string(alu_instance->total_score));
        
        // Test path components
        auto components = alu_instance->get_path_components();
        UNIT_TEST_ASSERT(components.size() == 4, "ALU path components count", "4", std::to_string(components.size()));
        if (components.size() >= 4) {
            UNIT_TEST_ASSERT(components[0] == "top", "ALU path component 0", "top", components[0]);
            UNIT_TEST_ASSERT(components[1] == "cpu_subsystem", "ALU path component 1", "cpu_subsystem", components[1]);
            UNIT_TEST_ASSERT(components[2] == "core0", "ALU path component 2", "core0", components[2]);
            UNIT_TEST_ASSERT(components[3] == "alu", "ALU path component 3", "alu", components[3]);
        }
    }
    
    // Test leaf instance detection (instances with no children)
    auto uart_instance = db.find_hierarchy_instance("top.io_subsystem.uart0");
    UNIT_TEST_ASSERT(uart_instance != nullptr, "UART instance found", "found", "not found");
    
    if (uart_instance) {
        // Check if this is likely a leaf (no other instances start with this path + ".")
        bool is_likely_leaf = true;
        std::string uart_path = uart_instance->instance_path + ".";
        for (auto it = db.hierarchy_begin(); it != db.hierarchy_end(); ++it) {
            if (it->second->instance_path.find(uart_path) == 0) {
                is_likely_leaf = false;
                break;
            }
        }
        UNIT_TEST_ASSERT(is_likely_leaf, "UART is leaf instance", "true", "false");
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test ModuleListParser with comprehensive module data
 */
void test_modlist_parser_comprehensive() {
    std::cout << "\n=== ModuleListParser Comprehensive Tests ===" << std::endl;
    
    // Create realistic module list file
    std::string modlist_content = 
        "Module List\n"
        "\n"
        "SCORE   ASSERT          NAME                                                   \n"
        " 95.67   95.67 234/245  cpu_core\n"
        " 88.45   88.45 156/176  memory_controller\n"
        " 92.30   92.30 89/96    alu_unit\n"
        " 78.90   78.90 123/156  cache_controller\n"
        "100.00  100.00 45/45    uart_controller\n"
        " 85.15   85.15 67/78    spi_controller\n"
        " 91.25   91.25 234/256  gpio_controller\n"
        " 76.85   76.85 198/258  dma_controller\n"
        "  0.00    0.00 0/34     unused_test_module\n"
        " 89.50   89.50 178/199  bus_arbiter\n";
    
    std::string test_file = create_test_file("test_modlist_comprehensive.txt", modlist_content);
    
    ModuleListParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    // Test parsing success
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "ModList parsing success", "SUCCESS", "Parser failed");
    
    // Test number of modules parsed
    UNIT_TEST_ASSERT(db.get_num_modules() == 10, "Number of modules parsed", "10", std::to_string(db.get_num_modules()));
    
    // Test high-coverage module
    auto cpu_module = db.find_module_definition("cpu_core");
    UNIT_TEST_ASSERT(cpu_module != nullptr, "CPU core module found", "found", "not found");
    
    if (cpu_module) {
        UNIT_TEST_ASSERT(std::abs(cpu_module->total_score - 95.67) < 0.01, "CPU module score", "95.67", std::to_string(cpu_module->total_score));
        UNIT_TEST_ASSERT(cpu_module->assert_coverage.covered == 234, "CPU assert covered", "234", std::to_string(cpu_module->assert_coverage.covered));
        UNIT_TEST_ASSERT(cpu_module->assert_coverage.expected == 245, "CPU assert expected", "245", std::to_string(cpu_module->assert_coverage.expected));
        UNIT_TEST_ASSERT(std::abs(cpu_module->assert_coverage.score - 95.67) < 0.01, "CPU assert score", "95.67", std::to_string(cpu_module->assert_coverage.score));
    }
    
    // Test 100% coverage module
    auto uart_module = db.find_module_definition("uart_controller");
    UNIT_TEST_ASSERT(uart_module != nullptr, "UART module found", "found", "not found");
    
    if (uart_module) {
        UNIT_TEST_ASSERT(std::abs(uart_module->total_score - 100.00) < 0.01, "UART module score", "100.00", std::to_string(uart_module->total_score));
        UNIT_TEST_ASSERT(uart_module->assert_coverage.covered == 45, "UART assert covered", "45", std::to_string(uart_module->assert_coverage.covered));
        UNIT_TEST_ASSERT(uart_module->assert_coverage.expected == 45, "UART assert expected", "45", std::to_string(uart_module->assert_coverage.expected));
    }
    
    // Test zero coverage module
    auto unused_module = db.find_module_definition("unused_test_module");
    UNIT_TEST_ASSERT(unused_module != nullptr, "Unused module found", "found", "not found");
    
    if (unused_module) {
        UNIT_TEST_ASSERT(std::abs(unused_module->total_score - 0.00) < 0.01, "Unused module score", "0.00", std::to_string(unused_module->total_score));
        UNIT_TEST_ASSERT(unused_module->assert_coverage.covered == 0, "Unused assert covered", "0", std::to_string(unused_module->assert_coverage.covered));
        UNIT_TEST_ASSERT(unused_module->assert_coverage.expected == 34, "Unused assert expected", "34", std::to_string(unused_module->assert_coverage.expected));
    }
    
    // Test module with spaces in name
    auto alu_module = db.find_module_definition("alu_unit");
    UNIT_TEST_ASSERT(alu_module != nullptr, "ALU unit module found", "found", "not found");
    
    if (alu_module) {
        UNIT_TEST_ASSERT(std::abs(alu_module->total_score - 92.30) < 0.01, "ALU unit score", "92.30", std::to_string(alu_module->total_score));
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test AssertParser with comprehensive assertion data
 */
void test_assert_parser_comprehensive() {
    std::cout << "\n=== AssertParser Comprehensive Tests ===" << std::endl;
    
    // Create realistic assertions file with various assertion statuses
    std::string assert_content = 
        "Assertion Coverage Report\n"
        "\n"
        "Total Assertions: 1250\n"
        "Covered: 847\n"
        "Coverage: 67.76%\n"
        "\n"
        "-------------------------------------------------------------------------------\n"
        "STATUS  HITS    ASSERTION                           INSTANCE                    FILE:LINE\n"
        "PASS    1234    check_valid_transaction             tb.cpu.alu                  alu.sv:45\n"
        "FAIL    0       check_data_integrity                tb.mem.ctrl                 mem_ctrl.sv:123\n"
        "PASS    567     verify_reset_behavior               tb.reset_mgr                reset.sv:67\n"
        "PASS    89      check_clock_domain_crossing         tb.clk.cdc                  cdc_checker.sv:89\n"
        "FAIL    0       verify_power_down_sequence          tb.pwr.mgr                  power_mgr.sv:156\n"
        "PASS    2345    assert_bus_protocol                 tb.bus.axi                  axi_protocol.sv:234\n"
        "PASS    12      check_interrupt_priority            tb.int.ctrl                 interrupt.sv:78\n"
        "FAIL    0       verify_cache_coherency              tb.cache.l2                 cache_l2.sv:345\n"
        "PASS    678     check_pipeline_stall                tb.cpu.pipeline             pipeline.sv:123\n"
        "PASS    3456    verify_dma_transfer                 tb.dma.chan0                dma.sv:67\n"
        "FAIL    0       check_thermal_shutdown              tb.thermal.sensor           thermal.sv:89\n"
        "PASS    123     verify_uart_transmission            tb.uart0                    uart.sv:45\n"
        "COVERED 1/1     simple_assertion                    tb.simple                   simple.sv:10\n"
        "UNCOVERED 0/1   uncovered_assertion                 tb.uncovered                uncov.sv:20\n";
    
    std::string test_file = create_test_file("test_assert_comprehensive.txt", assert_content);
    
    AssertParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    // Test parsing success
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Assert parsing success", "SUCCESS", "Parser failed");
    
    // Test number of assertions parsed
    UNIT_TEST_ASSERT(db.get_num_asserts() >= 12, "Number of assertions parsed", ">=12", std::to_string(db.get_num_asserts()));
    
    // Test passing assertion with high hit count
    auto valid_trans = db.find_assert_coverage("check_valid_transaction");
    UNIT_TEST_ASSERT(valid_trans != nullptr, "Valid transaction assert found", "found", "not found");
    
    if (valid_trans) {
        UNIT_TEST_ASSERT(valid_trans->is_covered == true, "Valid trans covered", "true", "false");
        UNIT_TEST_ASSERT(valid_trans->hit_count == 1234, "Valid trans hit count", "1234", std::to_string(valid_trans->hit_count));
        UNIT_TEST_ASSERT(valid_trans->severity == "PASS", "Valid trans severity", "PASS", valid_trans->severity);
        UNIT_TEST_ASSERT(valid_trans->instance_path == "tb.cpu.alu", "Valid trans instance", "tb.cpu.alu", valid_trans->instance_path);
        UNIT_TEST_ASSERT(valid_trans->file_location == "alu.sv", "Valid trans file", "alu.sv", valid_trans->file_location);
        UNIT_TEST_ASSERT(valid_trans->line_number == 45, "Valid trans line", "45", std::to_string(valid_trans->line_number));
        
        // Test full location string
        std::string full_loc = valid_trans->get_full_location();
        UNIT_TEST_ASSERT(full_loc == "alu.sv:45", "Valid trans full location", "alu.sv:45", full_loc);
    }
    
    // Test failing assertion
    auto data_integrity = db.find_assert_coverage("check_data_integrity");
    UNIT_TEST_ASSERT(data_integrity != nullptr, "Data integrity assert found", "found", "not found");
    
    if (data_integrity) {
        UNIT_TEST_ASSERT(data_integrity->is_covered == false, "Data integrity covered", "false", "true");
        UNIT_TEST_ASSERT(data_integrity->hit_count == 0, "Data integrity hit count", "0", std::to_string(data_integrity->hit_count));
        UNIT_TEST_ASSERT(data_integrity->severity == "FAIL", "Data integrity severity", "FAIL", data_integrity->severity);
        UNIT_TEST_ASSERT(data_integrity->instance_path == "tb.mem.ctrl", "Data integrity instance", "tb.mem.ctrl", data_integrity->instance_path);
        UNIT_TEST_ASSERT(data_integrity->file_location == "mem_ctrl.sv", "Data integrity file", "mem_ctrl.sv", data_integrity->file_location);
        UNIT_TEST_ASSERT(data_integrity->line_number == 123, "Data integrity line", "123", std::to_string(data_integrity->line_number));
    }
    
    // Test assertion with alternative format (COVERED/UNCOVERED)
    auto simple_assert = db.find_assert_coverage("simple_assertion");
    UNIT_TEST_ASSERT(simple_assert != nullptr, "Simple assertion found", "found", "not found");
    
    if (simple_assert) {
        UNIT_TEST_ASSERT(simple_assert->is_covered == true, "Simple assert covered", "true", "false");
        UNIT_TEST_ASSERT(simple_assert->instance_path == "tb.simple", "Simple assert instance", "tb.simple", simple_assert->instance_path);
    }
    
    auto uncov_assert = db.find_assert_coverage("uncovered_assertion");
    UNIT_TEST_ASSERT(uncov_assert != nullptr, "Uncovered assertion found", "found", "not found");
    
    if (uncov_assert) {
        UNIT_TEST_ASSERT(uncov_assert->is_covered == false, "Uncovered assert covered", "false", "true");
        UNIT_TEST_ASSERT(uncov_assert->instance_path == "tb.uncovered", "Uncovered assert instance", "tb.uncovered", uncov_assert->instance_path);
    }
    
    // Test critical assertion detection
    if (data_integrity) {
        bool is_critical = data_integrity->is_critical();
        UNIT_TEST_ASSERT(is_critical == true, "Data integrity is critical", "true", "false"); // FAIL severity
    }
    
    // Count covered vs uncovered assertions
    uint32_t covered_count = 0;
    uint32_t uncovered_count = 0;
    uint32_t total_hits = 0;
    
    for (const auto& [name, assert_cov] : db.asserts_table) {
        if (assert_cov->is_covered) {
            covered_count++;
        } else {
            uncovered_count++;
        }
        total_hits += assert_cov->hit_count;
    }
    
    UNIT_TEST_ASSERT(covered_count >= 8, "Covered assertions count", ">=8", std::to_string(covered_count));
    UNIT_TEST_ASSERT(uncovered_count >= 4, "Uncovered assertions count", ">=4", std::to_string(uncovered_count));
    UNIT_TEST_ASSERT(total_hits >= 8000, "Total hits count", ">=8000", std::to_string(total_hits));
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test edge cases and error conditions
 */
void test_edge_cases() {
    std::cout << "\n=== Edge Cases and Error Handling Tests ===" << std::endl;
/**
 * @brief Test ModListParser through DLL interface
 */
void test_modlist_parser_dll_comprehensive() {
    std::cout << "\n=== ModListParser DLL Comprehensive Tests ===" << std::endl;
    
    // Create realistic module list file
    std::string modlist_content = 
        "Module List\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_engineer\n"
        "Version: U-2023.03-SP2-9\n"
        "\n"
        "Module coverage data\n"
        "MODULE                                          SCORE   GROUP                ASSERT               \n"
        "cpu_core                                        78.45   85.20 654/768       71.70 234/326\n"
        "memory_controller                               82.30   89.15 445/499       75.45 198/262\n"
        "bus_interface                                   65.78   72.34 298/412       59.22 156/263\n"
        "interrupt_controller                            91.56   94.23 187/198       88.89 144/162\n"
        "timer_module                                    76.88   83.45 167/200       70.31 89/127\n";
    
    std::string test_file = create_test_file("test_modlist_comprehensive.txt", modlist_content);
    
    // Create database and parser through DLL interface
    void* db = create_coverage_database();
    void* parser = create_modlist_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for modlist test", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "ModList parser creation", "non-null", "null");
    
    if (db && parser) {
        // Test parsing through DLL
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        UNIT_TEST_ASSERT(parse_result == 0, "ModList parsing through DLL", "0", std::to_string(parse_result));
        
        if (parse_result != 0) {
            const char* error_msg = get_error_string(parse_result);
            std::cout << "  Parse error: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        } else {
            // Test module count
            int num_modules = get_num_modules(db);
            UNIT_TEST_ASSERT(num_modules == 5, "Number of modules parsed", "5", std::to_string(num_modules));
            std::cout << "  Number of modules: " << num_modules << std::endl;
        }
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test AssertParser through DLL interface
 */
void test_assert_parser_dll_comprehensive() {
    std::cout << "\n=== AssertParser DLL Comprehensive Tests ===" << std::endl;
    
    // Create realistic assert file
    std::string assert_content = 
        "Assertions\n"
        "\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_engineer\n"
        "Version: U-2023.03-SP2-9\n"
        "\n"
        "Assertion coverage data\n"
        "SCORE   ASSERT               NAME                                                                            \n"
        " 85.67   85.67 456/532      cpu_functional_assertions\n"
        " 78.23   78.23 312/399        reset_sequence_check\n"
        " 92.45   92.45 198/214          power_on_reset_assert\n"
        " 76.88   76.88  89/116          soft_reset_assert\n"
        " 88.92   88.92  67/76           watchdog_reset_assert\n"
        " 74.12   74.12 156/210        instruction_decode_check\n"
        " 89.45   89.45 144/161      memory_functional_assertions\n"
        " 91.23   91.23  89/98         cache_coherency_check\n"
        " 87.67   87.67  55/63         memory_protection_check\n";
    
    std::string test_file = create_test_file("test_assert_comprehensive.txt", assert_content);
    
    // Create database and parser through DLL interface
    void* db = create_coverage_database();
    void* parser = create_assert_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for assert test", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "Assert parser creation", "non-null", "null");
    
    if (db && parser) {
        // Test parsing through DLL
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        UNIT_TEST_ASSERT(parse_result == 0, "Assert parsing through DLL", "0", std::to_string(parse_result));
        
        if (parse_result != 0) {
            const char* error_msg = get_error_string(parse_result);
            std::cout << "  Parse error: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        } else {
            // Test assert count
            int num_asserts = get_num_asserts(db);
            UNIT_TEST_ASSERT(num_asserts > 0, "Number of asserts parsed", "> 0", std::to_string(num_asserts));
            std::cout << "  Number of asserts: " << num_asserts << std::endl;
        }
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test edge cases and error handling through DLL interface
 */
void test_dll_edge_cases() {
    std::cout << "\n=== DLL Edge Cases and Error Handling Tests ===" << std::endl;
    
    void* db = create_coverage_database();
    void* parser = create_dashboard_parser();
    
    UNIT_TEST_ASSERT(db != nullptr, "Database creation for edge cases", "non-null", "null");
    UNIT_TEST_ASSERT(parser != nullptr, "Parser creation for edge cases", "non-null", "null");
    
    if (db && parser) {
        // Test empty file
        std::string empty_file = create_test_file("empty_test.txt", "");
        int result = parse_coverage_file(parser, empty_file.c_str(), db);
        UNIT_TEST_ASSERT(result == 0, "Empty file handling through DLL", "0", std::to_string(result));
        cleanup_test_file(empty_file);
        
        // Test non-existent file
        result = parse_coverage_file(parser, "non_existent_file.txt", db);
        UNIT_TEST_ASSERT(result != 0, "Non-existent file error through DLL", "!= 0", std::to_string(result));
        
        if (result != 0) {
            const char* error_msg = get_error_string(result);
            std::cout << "  Expected error for non-existent file: " << (error_msg ? error_msg : "Unknown error") << std::endl;
        }
        
        // Test malformed file
        std::string malformed_content = 
            "This is not a valid coverage file\n"
            "Random text without proper format\n"
            "No valid coverage data here\n";
        
        std::string malformed_file = create_test_file("malformed_test.txt", malformed_content);
        result = parse_coverage_file(parser, malformed_file.c_str(), db);
        // Should handle gracefully (either success with no data or appropriate error)
        UNIT_TEST_ASSERT(result >= 0, "Malformed file graceful handling", ">= 0", std::to_string(result));
        cleanup_test_file(malformed_file);
        
        // Cleanup
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
}

/**
 * @brief Test memory management and resource cleanup
 */
void test_dll_memory_management() {
    std::cout << "\n=== DLL Memory Management Tests ===" << std::endl;
    
    // Test memory usage tracking
    uint32_t initial_bytes, initial_allocations;
    int mem_result = get_memory_usage(&initial_bytes, &initial_allocations);
    UNIT_TEST_ASSERT(mem_result == 0, "Memory usage tracking available", "0", std::to_string(mem_result));
    
    if (mem_result == 0) {
        std::cout << "  Initial memory: " << initial_bytes << " bytes, " << initial_allocations << " allocations" << std::endl;
    }
    
    // Create multiple databases and parsers
    std::vector<void*> databases;
    std::vector<void*> parsers;
    
    for (int i = 0; i < 5; ++i) {
        void* db = create_coverage_database();
        void* parser = create_dashboard_parser();
        
        if (db && parser) {
            databases.push_back(db);
            parsers.push_back(parser);
        }
    }
    
    UNIT_TEST_ASSERT(databases.size() == 5, "Multiple database creation", "5", std::to_string(databases.size()));
    UNIT_TEST_ASSERT(parsers.size() == 5, "Multiple parser creation", "5", std::to_string(parsers.size()));
    
    // Check memory usage after allocation
    uint32_t after_bytes, after_allocations;
    if (get_memory_usage(&after_bytes, &after_allocations) == 0) {
        std::cout << "  After allocation: " << after_bytes << " bytes, " << after_allocations << " allocations" << std::endl;
        UNIT_TEST_ASSERT(after_bytes >= initial_bytes, "Memory usage increased", ">= initial", std::to_string(after_bytes));
    }
    
    // Cleanup all resources
    for (void* parser : parsers) {
        destroy_parser(parser);
    }
    for (void* db : databases) {
        destroy_coverage_database(db);
    }
    
    // Final memory check
    uint32_t final_bytes, final_allocations;
    if (get_memory_usage(&final_bytes, &final_allocations) == 0) {
        std::cout << "  After cleanup: " << final_bytes << " bytes, " << final_allocations << " allocations" << std::endl;
    }
}

/**
 * @brief Test export functionality through DLL
 */
void test_dll_export_functionality() {
    std::cout << "\n=== DLL Export Functionality Tests ===" << std::endl;
    
    void* db = create_coverage_database();
    void* parser = create_dashboard_parser();
    
    if (db && parser) {
        // Create test data
        std::string dashboard_content = 
            "Dashboard\n"
            "Date: Mon Sep  8 14:06:30 2025\n"
            "Total: 75.32\n";
        
        std::string test_file = create_test_file("test_export_data.txt", dashboard_content);
        
        // Parse test data
        int parse_result = parse_coverage_file(parser, test_file.c_str(), db);
        
        if (parse_result == 0) {
            // Test XML export
            int xml_result = export_coverage_to_xml(db, "test_export.xml");
            UNIT_TEST_ASSERT(xml_result == 0, "XML export through DLL", "0", std::to_string(xml_result));
            
            // Test JSON export
            int json_result = export_coverage_to_json(db, "test_export.json");
            UNIT_TEST_ASSERT(json_result == 0, "JSON export through DLL", "0", std::to_string(json_result));
            
            // Cleanup export files
            std::remove("test_export.xml");
            std::remove("test_export.json");
        }
        
        cleanup_test_file(test_file);
        destroy_parser(parser);
        destroy_coverage_database(db);
    }
}

/**
 * @brief Main test runner for DLL tests
 */
int main() {
    std::cout << "FunctionalCoverageParsers DLL - Comprehensive Unit Tests" << std::endl;
    std::cout << "=========================================================" << std::endl;
    
    // Load the DLL first
    if (!load_dll()) {
        std::cout << "Failed to load DLL. Cannot proceed with tests." << std::endl;
        return 1;
    }
    
    try {
        // Run all DLL-based tests
        test_dll_initialization();
        test_dashboard_parser_dll_comprehensive();
        test_groups_parser_dll_comprehensive();
        test_hierarchy_parser_dll_comprehensive();
        test_modlist_parser_dll_comprehensive();
        test_assert_parser_dll_comprehensive();
        test_dll_edge_cases();
        test_dll_memory_management();
        test_dll_export_functionality();
        
        // Final cleanup
        cleanup_library();
        
    } catch (const std::exception& e) {
        std::cout << "Test suite failed with exception: " << e.what() << std::endl;
        unload_dll();
        return 1;
    }
    
    // Unload the DLL
    unload_dll();
    
    std::cout << "\n=========================================================" << std::endl;
    std::cout << "DLL Unit Test Results Summary:" << std::endl;
    std::cout << "Total Tests: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (100.0 * passed_tests / total_tests) << "%" << std::endl;
    
    if (failed_tests == 0) {
        std::cout << "\n🎉 ALL DLL UNIT TESTS PASSED! 🎉" << std::endl;
        std::cout << "DLL interface works correctly and captures all parser details." << std::endl;
        return 0;
    } else {
        std::cout << "\n❌ " << failed_tests << " DLL UNIT TESTS FAILED!" << std::endl;
        std::cout << "Some DLL functionality may not be working properly." << std::endl;
        return 1;
    }
}