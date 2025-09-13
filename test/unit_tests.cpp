/**
 * @file unit_tests.cpp
 * @brief Comprehensive unit tests for all parsers in the FunctionalCoverageParsers library
 * 
 * This file contains detailed unit tests for each parser to ensure that all data
 * from coverage files is properly captured in the data structures. Each test
 * validates specific aspects of parsing and data extraction.
 * 
 * TEST STRUCTURE:
 * 1. DashboardParser Tests - Validate all dashboard.txt fields
 * 2. GroupsParser Tests - Validate all groups.txt fields and configurations
 * 3. HierarchyParser Tests - Validate hierarchy traversal and depth calculation
 * 4. ModuleListParser Tests - Validate module aggregation and instance counting
 * 5. AssertParser Tests - Validate assertion details and coverage status
 * 6. Edge Case Tests - Test malformed files, empty files, and error conditions
 * 7. Integration Tests - Test complete parsing workflows
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
#include <iomanip>

using namespace coverage_parser;

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
 * @brief Test DashboardParser with comprehensive dashboard data
 */
void test_dashboard_parser_comprehensive() {
    std::cout << "\n=== DashboardParser Comprehensive Tests ===" << std::endl;
    
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
    
    DashboardParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(test_file, db);
    
    // Test parsing success
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Dashboard parsing success", "SUCCESS", "Parser failed");
    
    // Test dashboard data exists
    auto dashboard = db.dashboard_data.get();
    UNIT_TEST_ASSERT(dashboard != nullptr, "Dashboard data created", "non-null", "null");
    
    if (dashboard) {
        // Test date parsing
        UNIT_TEST_ASSERT(dashboard->date == "Mon Sep  8 14:06:30 2025", 
                        "Date field parsing", "Mon Sep  8 14:06:30 2025", dashboard->date);
        
        // Test user parsing
        UNIT_TEST_ASSERT(dashboard->user == "test_engineer", 
                        "User field parsing", "test_engineer", dashboard->user);
        
        // Test version parsing
        UNIT_TEST_ASSERT(dashboard->version == "U-2023.03-SP2-9", 
                        "Version field parsing", "U-2023.03-SP2-9", dashboard->version);
        
        // Test command line parsing
        std::string expected_cmd = "urg -full64 -hvp_no_score_missing -dir sim.vdb -format text -report dashboard";
        UNIT_TEST_ASSERT(dashboard->command_line == expected_cmd,
                        "Command line parsing", expected_cmd, dashboard->command_line);
        
        // Test total score parsing
        UNIT_TEST_ASSERT(std::abs(dashboard->total_score - 75.32) < 0.01, 
                        "Total score parsing", "75.32", std::to_string(dashboard->total_score));
        
        // Test assertion coverage parsing
        UNIT_TEST_ASSERT(dashboard->assert_coverage.is_valid, 
                        "Assert coverage validity", "true", "false");
        UNIT_TEST_ASSERT(dashboard->assert_coverage.covered == 12584, 
                        "Assert coverage covered", "12584", std::to_string(dashboard->assert_coverage.covered));
        UNIT_TEST_ASSERT(dashboard->assert_coverage.expected == 18392, 
                        "Assert coverage expected", "18392", std::to_string(dashboard->assert_coverage.expected));
        UNIT_TEST_ASSERT(std::abs(dashboard->assert_coverage.score - 68.45) < 0.01, 
                        "Assert coverage score", "68.45", std::to_string(dashboard->assert_coverage.score));
        
        // Test group coverage parsing
        UNIT_TEST_ASSERT(dashboard->group_coverage.is_valid, 
                        "Group coverage validity", "true", "false");
        UNIT_TEST_ASSERT(dashboard->group_coverage.covered == 25847, 
                        "Group coverage covered", "25847", std::to_string(dashboard->group_coverage.covered));
        UNIT_TEST_ASSERT(dashboard->group_coverage.expected == 31456, 
                        "Group coverage expected", "31456", std::to_string(dashboard->group_coverage.expected));
        UNIT_TEST_ASSERT(std::abs(dashboard->group_coverage.score - 82.19) < 0.01, 
                        "Group coverage score", "82.19", std::to_string(dashboard->group_coverage.score));
        
        // Test hierarchical instances count
        UNIT_TEST_ASSERT(dashboard->num_hierarchical_instances == 2847, 
                        "Hierarchical instances count", "2847", std::to_string(dashboard->num_hierarchical_instances));
        
        // Test validity flag
        UNIT_TEST_ASSERT(dashboard->is_valid(), 
                        "Dashboard validity check", "true", "false");
    }
    
    cleanup_test_file(test_file);
}

/**
 * @brief Test GroupsParser with comprehensive groups data
 */
void test_groups_parser_comprehensive() {
    std::cout << "\n=== GroupsParser Comprehensive Tests ===" << std::endl;
    
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
    
    // Test empty file
    std::string empty_file = create_test_file("empty_test.txt", "");
    DashboardParser parser;
    CoverageDatabase db;
    
    ParserResult result = parser.parse(empty_file, db);
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Empty file handling", "SUCCESS", "Failed");
    cleanup_test_file(empty_file);
    
    // Test non-existent file
    result = parser.parse("non_existent_file.txt", db);
    UNIT_TEST_ASSERT(result == ParserResult::ERROR_FILE_NOT_FOUND, "Non-existent file error", "ERROR_FILE_NOT_FOUND", "Different error");
    
    // Test malformed dashboard file
    std::string malformed_content = 
        "This is not a valid dashboard file\n"
        "Random text without proper format\n"
        "No valid coverage data here\n";
    
    std::string malformed_file = create_test_file("malformed_test.txt", malformed_content);
    result = parser.parse(malformed_file, db);
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Malformed file graceful handling", "SUCCESS", "Failed");
    cleanup_test_file(malformed_file);
    
    // Test file with partial data
    std::string partial_content = 
        "Dashboard\n"
        "Date: Mon Sep  8 14:06:30 2025\n"
        "User: test_user\n"
        "# Missing version and other fields\n";
    
    std::string partial_file = create_test_file("partial_test.txt", partial_content);
    result = parser.parse(partial_file, db);
    UNIT_TEST_ASSERT(result == ParserResult::SUCCESS, "Partial file handling", "SUCCESS", "Failed");
    
    // Verify partial data was captured
    auto dashboard = db.dashboard_data.get();
    if (dashboard) {
        UNIT_TEST_ASSERT(dashboard->date == "Mon Sep  8 14:06:30 2025", "Partial date parsed", "Mon Sep  8 14:06:30 2025", dashboard->date);
        UNIT_TEST_ASSERT(dashboard->user == "test_user", "Partial user parsed", "test_user", dashboard->user);
        UNIT_TEST_ASSERT(dashboard->version.empty(), "Partial version empty", "empty", dashboard->version);
    }
    
    cleanup_test_file(partial_file);
}

/**
 * @brief Test database operations and statistics
 */
void test_database_operations() {
    std::cout << "\n=== Database Operations Tests ===" << std::endl;
    
    CoverageDatabase db;
    
    // Create test dashboard data
    auto dashboard = std::make_unique<DashboardData>();
    dashboard->date = "Test Date";
    dashboard->user = "Test User";
    dashboard->total_score = 85.5;
    db.dashboard_data = std::move(dashboard);
    
    // Create test coverage groups
    auto group1 = std::make_unique<CoverageGroup>("test_group_1");
    group1->coverage.covered = 45;
    group1->coverage.expected = 50;
    group1->coverage.score = 90.0;
    group1->weight = 2;
    db.add_coverage_group(std::move(group1));
    
    auto group2 = std::make_unique<CoverageGroup>("test_group_2");
    group2->coverage.covered = 0;
    group2->coverage.expected = 25;
    group2->coverage.score = 0.0;
    group2->weight = 1;
    db.add_coverage_group(std::move(group2));
    
    // Test database validation
    bool is_valid = db.validate();
    UNIT_TEST_ASSERT(is_valid, "Database validation", "true", "false");
    
    // Test overall score calculation
    double overall_score = db.calculate_overall_score();
    UNIT_TEST_ASSERT(overall_score > 0.0, "Overall score calculation", ">0", std::to_string(overall_score));
    
    // Test uncovered groups
    auto uncovered = db.get_uncovered_groups();
    UNIT_TEST_ASSERT(uncovered.size() == 1, "Uncovered groups count", "1", std::to_string(uncovered.size()));
    
    if (!uncovered.empty()) {
        UNIT_TEST_ASSERT(uncovered[0]->name == "test_group_2", "Uncovered group name", "test_group_2", uncovered[0]->name);
    }
    
    // Test statistics generation
    auto stats = db.generate_statistics();
    UNIT_TEST_ASSERT(stats != nullptr, "Statistics generation", "non-null", "null");
    
    if (stats) {
        UNIT_TEST_ASSERT(stats->num_zero_coverage_groups == 1, "Zero coverage groups", "1", std::to_string(stats->num_zero_coverage_groups));
        UNIT_TEST_ASSERT(stats->covered_points > 0, "Covered points", ">0", std::to_string(stats->covered_points));
        UNIT_TEST_ASSERT(stats->total_coverage_points > 0, "Total coverage points", ">0", std::to_string(stats->total_coverage_points));
    }
}

/**
 * @brief Main test runner
 */
int main() {
    std::cout << "FunctionalCoverageParsers Library - Comprehensive Unit Tests" << std::endl;
    std::cout << "============================================================" << std::endl;
    
    try {
        test_dashboard_parser_comprehensive();
        test_groups_parser_comprehensive();
        test_hierarchy_parser_comprehensive();
        test_modlist_parser_comprehensive();
        test_assert_parser_comprehensive();
        test_edge_cases();
        test_database_operations();
    } catch (const std::exception& e) {
        std::cout << "Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n============================================================" << std::endl;
    std::cout << "Unit Test Results Summary:" << std::endl;
    std::cout << "Total Tests: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (100.0 * passed_tests / total_tests) << "%" << std::endl;
    
    if (failed_tests == 0) {
        std::cout << "\n🎉 ALL UNIT TESTS PASSED! 🎉" << std::endl;
        std::cout << "All parser details are properly captured in data structures." << std::endl;
        return 0;
    } else {
        std::cout << "\n❌ " << failed_tests << " UNIT TESTS FAILED!" << std::endl;
        std::cout << "Some parser details may not be properly captured." << std::endl;
        return 1;
    }
}