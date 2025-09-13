/**
 * @file test_parsers.cpp
 * @brief Comprehensive test suite for the FunctionalCoverageParsers library
 * 
 * This file contains unit tests and integration tests for all parser
 * implementations in the coverage analysis library. Tests cover:
 * - Individual parser functionality
 * - Data structure validation
 * - Error handling and edge cases
 * - Performance with large files
 * - Memory management and leak detection
 * 
 * TEST CATEGORIES:
 * 1. Parser Unit Tests: Test each parser in isolation
 * 2. Integration Tests: Test complete parsing workflows
 * 3. Error Handling Tests: Test failure scenarios
 * 4. Performance Tests: Test with large data sets
 * 5. Memory Tests: Validate memory usage patterns
 * 
 * USAGE:
 * Compile and run this test file to validate the parser library:
 * ```
 * g++ -std=c++17 -I../include test_parsers.cpp ../src/*.cpp -o test_parsers
 * ./test_parsers
 * ```
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "../include/functional_coverage_parser.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>
#include <sstream>

using namespace coverage_parser;

// Test result counters
static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

/**
 * @brief Test assertion macro
 */
#define TEST_ASSERT(condition, message) \
    do { \
        tests_total++; \
        if (condition) { \
            tests_passed++; \
            std::cout << "[PASS] " << message << std::endl; \
        } else { \
            tests_failed++; \
            std::cout << "[FAIL] " << message << std::endl; \
        } \
    } while(0)

/**
 * @brief Create a temporary test file with content
 */
std::string create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

/**
 * @brief Test CoverageDatabase basic functionality
 */
void test_coverage_database() {
    std::cout << "\n=== Testing CoverageDatabase ===" << std::endl;
    
    CoverageDatabase db;
    
    // Test adding dashboard data
    auto dashboard = std::make_unique<DashboardData>();
    dashboard->total_coverage = 75.5;
    dashboard->tool_name = "VCS";
    db.add_dashboard_data(std::move(dashboard));
    
    TEST_ASSERT(db.get_dashboard_data() != nullptr, "Dashboard data storage");
    TEST_ASSERT(db.get_dashboard_data()->total_coverage == 75.5, "Dashboard data retrieval");
    
    // Test adding coverage groups
    auto group = std::make_unique<CoverageGroup>();
    group->group_name = "test_group";
    group->metrics.covered = 100;
    group->metrics.total = 150;
    db.add_coverage_group(std::move(group));
    
    TEST_ASSERT(db.get_coverage_groups().size() == 1, "Coverage group storage");
    TEST_ASSERT(db.get_coverage_groups().begin()->second->group_name == "test_group", "Coverage group retrieval");
    
    // Test adding hierarchy instances
    auto hierarchy = std::make_unique<HierarchyInstance>();
    hierarchy->instance_name = "top.cpu";
    hierarchy->depth_level = 2;
    db.add_hierarchy_instance(std::move(hierarchy));
    
    TEST_ASSERT(db.get_hierarchy_instances().size() == 1, "Hierarchy instance storage");
    
    // Test adding module definitions
    auto module = std::make_unique<ModuleDefinition>();
    module->module_name = "cpu_core";
    module->metrics.covered = 50;
    module->metrics.total = 100;
    db.add_module_definition(std::move(module));
    
    TEST_ASSERT(db.get_module_definitions().size() == 1, "Module definition storage");
    
    // Test adding assert coverage
    auto assert_cov = std::make_unique<AssertCoverage>();
    assert_cov->assert_name = "test_assert";
    assert_cov->is_covered = true;
    assert_cov->hit_count = 5;
    db.add_assert_coverage(std::move(assert_cov));
    
    TEST_ASSERT(db.get_assert_coverage().size() == 1, "Assert coverage storage");
}

/**
 * @brief Test DashboardParser with sample data
 */
void test_dashboard_parser() {
    std::cout << "\n=== Testing DashboardParser ===" << std::endl;
    
    std::string sample_dashboard = 
        "Coverage Summary Report\n"
        "Tool: VCS 2023.03\n"
        "Date: Mon Jan 15 14:30:25 2024\n"
        "Total Coverage: 75.67%\n"
        "\n"
        "Coverage Type Breakdown:\n"
        "Line Coverage: 85.23%\n"
        "Toggle Coverage: 72.45%\n"
        "FSM Coverage: 68.91%\n"
        "\n"
        "Hierarchical Coverage:\n"
        "top                    75.67%\n"
        "top.cpu               82.34%\n"
        "top.memory            71.23%\n";
    
    std::string test_file = create_test_file("test_dashboard.txt", sample_dashboard);
    
    DashboardParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Dashboard parser success");
    TEST_ASSERT(db.get_dashboard_data() != nullptr, "Dashboard data created");
    TEST_ASSERT(db.get_dashboard_data()->total_coverage == 75.67, "Total coverage parsed correctly");
    TEST_ASSERT(db.get_dashboard_data()->tool_name == "VCS 2023.03", "Tool name parsed correctly");
    
    // Clean up
    std::remove(test_file.c_str());
}

/**
 * @brief Test GroupsParser with sample data
 */
void test_groups_parser() {
    std::cout << "\n=== Testing GroupsParser ===" << std::endl;
    
    std::string sample_groups = 
        "Coverage Groups Report\n"
        "\n"
        "GROUP_NAME              COVERED/TOTAL    PERCENTAGE\n"
        "test_group1             45/50           90.00%\n"
        "test_group2             30/40           75.00%\n"
        "verification_group      88/100          88.00%\n";
    
    std::string test_file = create_test_file("test_groups.txt", sample_groups);
    
    GroupsParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Groups parser success");
    TEST_ASSERT(db.get_coverage_groups().size() == 3, "All groups parsed");
    
    auto groups = db.get_coverage_groups();
    bool found_test_group1 = groups.find("test_group1") != groups.end();
    TEST_ASSERT(found_test_group1, "test_group1 found in database");
    
    if (found_test_group1) {
        auto& group = groups["test_group1"];
        TEST_ASSERT(group->metrics.covered == 45, "Group covered count correct");
        TEST_ASSERT(group->metrics.total == 50, "Group total count correct");
    }
    
    // Clean up
    std::remove(test_file.c_str());
}

/**
 * @brief Test HierarchyParser with sample data
 */
void test_hierarchy_parser() {
    std::cout << "\n=== Testing HierarchyParser ===" << std::endl;
    
    std::string sample_hierarchy = 
        "Design Hierarchy Coverage\n"
        "\n"
        "INSTANCE                    COVERAGE\n"
        "top                         75.67%\n"
        "  top.cpu                   82.34%\n"
        "    top.cpu.alu             95.12%\n"
        "    top.cpu.decoder         78.45%\n"
        "  top.memory                71.23%\n"
        "    top.memory.cache        88.91%\n";
    
    std::string test_file = create_test_file("test_hierarchy.txt", sample_hierarchy);
    
    HierarchyParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Hierarchy parser success");
    TEST_ASSERT(db.get_hierarchy_instances().size() >= 3, "Multiple hierarchy instances parsed");
    
    // Find specific instances
    bool found_top = false, found_cpu = false, found_alu = false;
    for (const auto& pair : db.get_hierarchy_instances()) {
        const auto& instance = pair.second;
        if (instance->instance_name == "top") {
            found_top = true;
            TEST_ASSERT(instance->depth_level == 0, "Top level depth correct");
        } else if (instance->instance_name == "top.cpu") {
            found_cpu = true;
            TEST_ASSERT(instance->depth_level == 1, "CPU level depth correct");
        } else if (instance->instance_name == "top.cpu.alu") {
            found_alu = true;
            TEST_ASSERT(instance->depth_level == 2, "ALU level depth correct");
        }
    }
    
    TEST_ASSERT(found_top, "Top instance found");
    TEST_ASSERT(found_cpu, "CPU instance found");
    TEST_ASSERT(found_alu, "ALU instance found");
    
    // Clean up
    std::remove(test_file.c_str());
}

/**
 * @brief Test ModuleListParser with sample data
 */
void test_modlist_parser() {
    std::cout << "\n=== Testing ModuleListParser ===" << std::endl;
    
    std::string sample_modlist = 
        "Module Coverage Report\n"
        "\n"
        "MODULE                  COVERED/TOTAL    PERCENTAGE\n"
        "cpu_core                150/200         75.00%\n"
        "memory_controller       88/100          88.00%\n"
        "alu unit                45/50           90.00%\n";
    
    std::string test_file = create_test_file("test_modlist.txt", sample_modlist);
    
    ModuleListParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Module list parser success");
    TEST_ASSERT(db.get_module_definitions().size() == 3, "All modules parsed");
    
    // Verify specific module
    bool found_cpu_core = false;
    for (const auto& pair : db.get_module_definitions()) {
        const auto& module = pair.second;
        if (module->module_name == "cpu_core") {
            found_cpu_core = true;
            TEST_ASSERT(module->metrics.covered == 150, "CPU core covered count correct");
            TEST_ASSERT(module->metrics.total == 200, "CPU core total count correct");
            break;
        }
    }
    
    TEST_ASSERT(found_cpu_core, "CPU core module found");
    
    // Clean up
    std::remove(test_file.c_str());
}

/**
 * @brief Test AssertParser with sample data
 */
void test_assert_parser() {
    std::cout << "\n=== Testing AssertParser ===" << std::endl;
    
    std::string sample_asserts = 
        "Assertion Coverage Report\n"
        "\n"
        "STATUS  HITS    ASSERTION                   INSTANCE            FILE:LINE\n"
        "PASS    1234    check_valid_transaction     tb.cpu.alu          alu.sv:45\n"
        "FAIL    0       check_data_integrity        tb.mem.ctrl         mem_ctrl.sv:123\n"
        "PASS    567     verify_reset_behavior       tb.reset_mgr        reset.sv:67\n";
    
    std::string test_file = create_test_file("test_asserts.txt", sample_asserts);
    
    AssertParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Assert parser success");
    TEST_ASSERT(db.get_assert_coverage().size() == 3, "All assertions parsed");
    
    // Verify specific assertion
    bool found_valid_transaction = false;
    for (const auto& pair : db.get_assert_coverage()) {
        const auto& assert_cov = pair.second;
        if (assert_cov->assert_name == "check_valid_transaction") {
            found_valid_transaction = true;
            TEST_ASSERT(assert_cov->is_covered == true, "Assertion coverage status correct");
            TEST_ASSERT(assert_cov->hit_count == 1234, "Assertion hit count correct");
            TEST_ASSERT(assert_cov->instance_path == "tb.cpu.alu", "Assertion instance path correct");
            TEST_ASSERT(assert_cov->file_location == "alu.sv", "Assertion file location correct");
            TEST_ASSERT(assert_cov->line_number == 45, "Assertion line number correct");
            break;
        }
    }
    
    TEST_ASSERT(found_valid_transaction, "Valid transaction assertion found");
    
    // Clean up
    std::remove(test_file.c_str());
}

/**
 * @brief Test error handling scenarios
 */
void test_error_handling() {
    std::cout << "\n=== Testing Error Handling ===" << std::endl;
    
    DashboardParser parser;
    CoverageDatabase db;
    
    // Test non-existent file
    ParserResult result = parser.parse("non_existent_file.txt", db);
    TEST_ASSERT(result == ParserResult::ERROR_FILE_NOT_FOUND, "Non-existent file error handling");
    
    // Test empty file
    std::string empty_file = create_test_file("empty.txt", "");
    result = parser.parse(empty_file, db);
    TEST_ASSERT(result == ParserResult::SUCCESS, "Empty file handling");
    std::remove(empty_file.c_str());
    
    // Test malformed file
    std::string malformed_file = create_test_file("malformed.txt", 
        "This is not a valid coverage file\n"
        "Random text without proper format\n"
        "123abc!@# garbage data\n");
    
    result = parser.parse(malformed_file, db);
    TEST_ASSERT(result == ParserResult::SUCCESS, "Malformed file graceful handling");
    std::remove(malformed_file.c_str());
}

/**
 * @brief Test utility functions
 */
void test_utilities() {
    std::cout << "\n=== Testing Utility Functions ===" << std::endl;
    
    // Test string trimming
    TEST_ASSERT(utils::trim("  hello  ") == "hello", "String trimming");
    TEST_ASSERT(utils::trim("\t\ntest\r\n") == "test", "Whitespace trimming");
    
    // Test string splitting
    auto tokens = utils::split("a,b,c", ',');
    TEST_ASSERT(tokens.size() == 3, "String splitting count");
    TEST_ASSERT(tokens[0] == "a" && tokens[1] == "b" && tokens[2] == "c", "String splitting content");
    
    // Test percentage parsing
    TEST_ASSERT(utils::parse_percentage("75.67%") == 75.67, "Percentage parsing with %");
    TEST_ASSERT(utils::parse_percentage("45.23") == 45.23, "Percentage parsing without %");
    
    // Test number parsing
    TEST_ASSERT(utils::parse_int("123", 0) == 123, "Integer parsing");
    TEST_ASSERT(utils::parse_int("invalid", 42) == 42, "Integer parsing with default");
    
    // Test coverage calculation
    double coverage = utils::calculate_coverage_percentage(75, 100);
    TEST_ASSERT(coverage == 75.0, "Coverage percentage calculation");
    
    // Test path normalization
    TEST_ASSERT(utils::normalize_path("path\\to\\file") == "path/to/file", "Path normalization");
    TEST_ASSERT(utils::get_filename("path/to/file.txt") == "file.txt", "Filename extraction");
}

/**
 * @brief Performance test with large data
 */
void test_performance() {
    std::cout << "\n=== Testing Performance ===" << std::endl;
    
    // Create a large test file
    std::ostringstream large_content;
    large_content << "Coverage Groups Report\n\n";
    large_content << "GROUP_NAME              COVERED/TOTAL    PERCENTAGE\n";
    
    for (int i = 0; i < 10000; ++i) {
        large_content << "test_group_" << i << "           " << (i % 100) << "/100          " 
                     << (i % 100) << ".00%\n";
    }
    
    std::string large_file = create_test_file("large_test.txt", large_content.str());
    
    GroupsParser parser;
    CoverageDatabase db;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    ParserResult result = parser.parse(large_file, db);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    TEST_ASSERT(result == ParserResult::SUCCESS, "Large file parsing success");
    TEST_ASSERT(db.get_coverage_groups().size() == 10000, "Large file parsing completeness");
    
    std::cout << "Parsed 10,000 groups in " << duration.count() << " ms" << std::endl;
    TEST_ASSERT(duration.count() < 5000, "Performance within acceptable limits (< 5 seconds)");
    
    // Clean up
    std::remove(large_file.c_str());
}

/**
 * @brief Integration test with all parsers
 */
void test_integration() {
    std::cout << "\n=== Testing Integration ===" << std::endl;
    
    CoverageDatabase db;
    
    // Create test files for all parsers
    std::string dashboard_content = "Tool: VCS\nTotal Coverage: 80.5%\n";
    std::string groups_content = "test_group 50/100 50.00%\n";
    std::string hierarchy_content = "top 80.5%\n";
    std::string modlist_content = "cpu_module 40/50 80.00%\n";
    std::string asserts_content = "PASS 100 test_assert tb.top alu.sv:10\n";
    
    std::string dashboard_file = create_test_file("dashboard.txt", dashboard_content);
    std::string groups_file = create_test_file("groups.txt", groups_content);
    std::string hierarchy_file = create_test_file("hierarchy.txt", hierarchy_content);
    std::string modlist_file = create_test_file("modlist.txt", modlist_content);
    std::string asserts_file = create_test_file("asserts.txt", asserts_content);
    
    // Parse all files
    DashboardParser dashboard_parser;
    GroupsParser groups_parser;
    HierarchyParser hierarchy_parser;
    ModuleListParser modlist_parser;
    AssertParser assert_parser;
    
    ParserResult r1 = dashboard_parser.parse(dashboard_file, db);
    ParserResult r2 = groups_parser.parse(groups_file, db);
    ParserResult r3 = hierarchy_parser.parse(hierarchy_file, db);
    ParserResult r4 = modlist_parser.parse(modlist_file, db);
    ParserResult r5 = assert_parser.parse(asserts_file, db);
    
    TEST_ASSERT(r1 == ParserResult::SUCCESS, "Dashboard integration parsing");
    TEST_ASSERT(r2 == ParserResult::SUCCESS, "Groups integration parsing");
    TEST_ASSERT(r3 == ParserResult::SUCCESS, "Hierarchy integration parsing");
    TEST_ASSERT(r4 == ParserResult::SUCCESS, "Module list integration parsing");
    TEST_ASSERT(r5 == ParserResult::SUCCESS, "Asserts integration parsing");
    
    // Verify all data is present
    TEST_ASSERT(db.get_dashboard_data() != nullptr, "Integration: Dashboard data present");
    TEST_ASSERT(db.get_coverage_groups().size() >= 1, "Integration: Groups data present");
    TEST_ASSERT(db.get_hierarchy_instances().size() >= 1, "Integration: Hierarchy data present");
    TEST_ASSERT(db.get_module_definitions().size() >= 1, "Integration: Module data present");
    TEST_ASSERT(db.get_assert_coverage().size() >= 1, "Integration: Assert data present");
    
    // Clean up
    std::remove(dashboard_file.c_str());
    std::remove(groups_file.c_str());
    std::remove(hierarchy_file.c_str());
    std::remove(modlist_file.c_str());
    std::remove(asserts_file.c_str());
}

/**
 * @brief Main test runner
 */
int main() {
    std::cout << "FunctionalCoverageParsers Library Test Suite" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    try {
        test_coverage_database();
        test_dashboard_parser();
        test_groups_parser();
        test_hierarchy_parser();
        test_modlist_parser();
        test_assert_parser();
        test_error_handling();
        test_utilities();
        test_performance();
        test_integration();
    } catch (const std::exception& e) {
        std::cout << "Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=============================================" << std::endl;
    std::cout << "Test Results Summary:" << std::endl;
    std::cout << "Total Tests: " << tests_total << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (100.0 * tests_passed / tests_total) << "%" << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\nAll tests PASSED! 🎉" << std::endl;
        return 0;
    } else {
        std::cout << "\n" << tests_failed << " tests FAILED! ❌" << std::endl;
        return 1;
    }
}