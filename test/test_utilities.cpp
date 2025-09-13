/**
 * @file test_utilities.cpp
 * @brief Comprehensive tests for utility functions
 * 
 * This file contains detailed tests for all utility functions in parser_utils.cpp
 * to ensure all helper functions work correctly and handle edge cases properly.
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "../include/functional_coverage_parser.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

using namespace coverage_parser;

// Test result tracking
static int util_tests_total = 0;
static int util_tests_passed = 0;
static int util_tests_failed = 0;

#define UTIL_TEST_ASSERT(condition, test_name, expected, actual) \
    do { \
        util_tests_total++; \
        if (condition) { \
            util_tests_passed++; \
            std::cout << "✓ PASS: " << test_name << std::endl; \
        } else { \
            util_tests_failed++; \
            std::cout << "✗ FAIL: " << test_name << std::endl; \
            std::cout << "  Expected: " << expected << std::endl; \
            std::cout << "  Actual: " << actual << std::endl; \
        } \
    } while(0)

/**
 * @brief Test string utility functions
 */
void test_string_utilities() {
    std::cout << "\n=== String Utility Tests ===" << std::endl;
    
    // Test trim function
    UTIL_TEST_ASSERT(utils::trim("") == "", "Trim empty string", "", utils::trim(""));
    UTIL_TEST_ASSERT(utils::trim("hello") == "hello", "Trim no whitespace", "hello", utils::trim("hello"));
    UTIL_TEST_ASSERT(utils::trim("  hello  ") == "hello", "Trim spaces", "hello", utils::trim("  hello  "));
    UTIL_TEST_ASSERT(utils::trim("\t\nhello\r\n") == "hello", "Trim all whitespace", "hello", utils::trim("\t\nhello\r\n"));
    UTIL_TEST_ASSERT(utils::trim("   ") == "", "Trim only whitespace", "", utils::trim("   "));
    
    // Test split function
    auto tokens = utils::split("a,b,c", ',');
    UTIL_TEST_ASSERT(tokens.size() == 3, "Split basic", "3", std::to_string(tokens.size()));
    UTIL_TEST_ASSERT(tokens[0] == "a", "Split token 0", "a", tokens[0]);
    UTIL_TEST_ASSERT(tokens[1] == "b", "Split token 1", "b", tokens[1]);
    UTIL_TEST_ASSERT(tokens[2] == "c", "Split token 2", "c", tokens[2]);
    
    tokens = utils::split("a,,c", ',');
    UTIL_TEST_ASSERT(tokens.size() == 3, "Split with empty", "3", std::to_string(tokens.size()));
    UTIL_TEST_ASSERT(tokens[1] == "", "Split empty token", "", tokens[1]);
    
    tokens = utils::split("", ',');
    UTIL_TEST_ASSERT(tokens.size() == 1, "Split empty string", "1", std::to_string(tokens.size()));
    
    // Test split whitespace function
    auto ws_tokens = utils::split_whitespace("a  b\tc\nd");
    UTIL_TEST_ASSERT(ws_tokens.size() == 4, "Split whitespace count", "4", std::to_string(ws_tokens.size()));
    UTIL_TEST_ASSERT(ws_tokens[0] == "a", "Split whitespace token 0", "a", ws_tokens[0]);
    UTIL_TEST_ASSERT(ws_tokens[3] == "d", "Split whitespace token 3", "d", ws_tokens[3]);
    
    // Test to_lower function
    UTIL_TEST_ASSERT(utils::to_lower("HELLO") == "hello", "To lower uppercase", "hello", utils::to_lower("HELLO"));
    UTIL_TEST_ASSERT(utils::to_lower("Hello World") == "hello world", "To lower mixed", "hello world", utils::to_lower("Hello World"));
    UTIL_TEST_ASSERT(utils::to_lower("") == "", "To lower empty", "", utils::to_lower(""));
    
    // Test remove_quotes function
    UTIL_TEST_ASSERT(utils::remove_quotes("\"hello\"") == "hello", "Remove double quotes", "hello", utils::remove_quotes("\"hello\""));
    UTIL_TEST_ASSERT(utils::remove_quotes("'hello'") == "hello", "Remove single quotes", "hello", utils::remove_quotes("'hello'"));
    UTIL_TEST_ASSERT(utils::remove_quotes("hello") == "hello", "Remove no quotes", "hello", utils::remove_quotes("hello"));
    UTIL_TEST_ASSERT(utils::remove_quotes("\"hello'") == "\"hello'", "Remove mismatched quotes", "\"hello'", utils::remove_quotes("\"hello'"));
}

/**
 * @brief Test number parsing utilities
 */
void test_number_utilities() {
    std::cout << "\n=== Number Utility Tests ===" << std::endl;
    
    // Test parse_percentage function
    UTIL_TEST_ASSERT(std::abs(utils::parse_percentage("75.5%") - 75.5) < 0.01, "Parse percentage with %", "75.5", std::to_string(utils::parse_percentage("75.5%")));
    UTIL_TEST_ASSERT(std::abs(utils::parse_percentage("75.5") - 75.5) < 0.01, "Parse percentage without %", "75.5", std::to_string(utils::parse_percentage("75.5")));
    UTIL_TEST_ASSERT(std::abs(utils::parse_percentage("100%") - 100.0) < 0.01, "Parse 100%", "100.0", std::to_string(utils::parse_percentage("100%")));
    UTIL_TEST_ASSERT(utils::parse_percentage("invalid") < 0, "Parse invalid percentage", "<0", std::to_string(utils::parse_percentage("invalid")));
    UTIL_TEST_ASSERT(utils::parse_percentage("") < 0, "Parse empty percentage", "<0", std::to_string(utils::parse_percentage("")));
    
    // Test parse_int function
    UTIL_TEST_ASSERT(utils::parse_int("123", 0) == 123, "Parse int valid", "123", std::to_string(utils::parse_int("123", 0)));
    UTIL_TEST_ASSERT(utils::parse_int("-456", 0) == -456, "Parse int negative", "-456", std::to_string(utils::parse_int("-456", 0)));
    UTIL_TEST_ASSERT(utils::parse_int("invalid", 42) == 42, "Parse int invalid", "42", std::to_string(utils::parse_int("invalid", 42)));
    UTIL_TEST_ASSERT(utils::parse_int("", 99) == 99, "Parse int empty", "99", std::to_string(utils::parse_int("", 99)));
    
    // Test parse_uint function
    UTIL_TEST_ASSERT(utils::parse_uint("123", 0) == 123, "Parse uint valid", "123", std::to_string(utils::parse_uint("123", 0)));
    UTIL_TEST_ASSERT(utils::parse_uint("0", 42) == 0, "Parse uint zero", "0", std::to_string(utils::parse_uint("0", 42)));
    UTIL_TEST_ASSERT(utils::parse_uint("invalid", 42) == 42, "Parse uint invalid", "42", std::to_string(utils::parse_uint("invalid", 42)));
    
    // Test parse_double function
    UTIL_TEST_ASSERT(std::abs(utils::parse_double("123.45", 0.0) - 123.45) < 0.01, "Parse double valid", "123.45", std::to_string(utils::parse_double("123.45", 0.0)));
    UTIL_TEST_ASSERT(std::abs(utils::parse_double("-67.89", 0.0) - (-67.89)) < 0.01, "Parse double negative", "-67.89", std::to_string(utils::parse_double("-67.89", 0.0)));
    UTIL_TEST_ASSERT(std::abs(utils::parse_double("invalid", 99.9) - 99.9) < 0.01, "Parse double invalid", "99.9", std::to_string(utils::parse_double("invalid", 99.9)));
    
    // Test is_number function
    UTIL_TEST_ASSERT(utils::is_number("123"), "Is number integer", "true", "false");
    UTIL_TEST_ASSERT(utils::is_number("123.45"), "Is number float", "true", "false");
    UTIL_TEST_ASSERT(utils::is_number("-67.89"), "Is number negative", "true", "false");
    UTIL_TEST_ASSERT(utils::is_number("1.23e-4"), "Is number scientific", "true", "false");
    UTIL_TEST_ASSERT(!utils::is_number("invalid"), "Is number invalid", "false", "true");
    UTIL_TEST_ASSERT(!utils::is_number(""), "Is number empty", "false", "true");
    UTIL_TEST_ASSERT(!utils::is_number("12.34.56"), "Is number multiple dots", "false", "true");
}

/**
 * @brief Test coverage calculation utilities
 */
void test_coverage_utilities() {
    std::cout << "\n=== Coverage Utility Tests ===" << std::endl;
    
    // Test calculate_coverage_percentage function
    UTIL_TEST_ASSERT(std::abs(utils::calculate_coverage_percentage(50, 100) - 50.0) < 0.01, "Coverage 50/100", "50.0", std::to_string(utils::calculate_coverage_percentage(50, 100)));
    UTIL_TEST_ASSERT(std::abs(utils::calculate_coverage_percentage(100, 100) - 100.0) < 0.01, "Coverage 100/100", "100.0", std::to_string(utils::calculate_coverage_percentage(100, 100)));
    UTIL_TEST_ASSERT(std::abs(utils::calculate_coverage_percentage(0, 100) - 0.0) < 0.01, "Coverage 0/100", "0.0", std::to_string(utils::calculate_coverage_percentage(0, 100)));
    UTIL_TEST_ASSERT(std::abs(utils::calculate_coverage_percentage(75, 150) - 50.0) < 0.01, "Coverage 75/150", "50.0", std::to_string(utils::calculate_coverage_percentage(75, 150)));
    UTIL_TEST_ASSERT(std::abs(utils::calculate_coverage_percentage(0, 0) - 0.0) < 0.01, "Coverage 0/0", "0.0", std::to_string(utils::calculate_coverage_percentage(0, 0)));
    
    // Test get_coverage_status function
    UTIL_TEST_ASSERT(utils::get_coverage_status(100.0) == "Excellent", "Status 100%", "Excellent", utils::get_coverage_status(100.0));
    UTIL_TEST_ASSERT(utils::get_coverage_status(95.0) == "Excellent", "Status 95%", "Excellent", utils::get_coverage_status(95.0));
    UTIL_TEST_ASSERT(utils::get_coverage_status(85.0) == "Good", "Status 85%", "Good", utils::get_coverage_status(85.0));
    UTIL_TEST_ASSERT(utils::get_coverage_status(70.0) == "Fair", "Status 70%", "Fair", utils::get_coverage_status(70.0));
    UTIL_TEST_ASSERT(utils::get_coverage_status(50.0) == "Poor", "Status 50%", "Poor", utils::get_coverage_status(50.0));
    UTIL_TEST_ASSERT(utils::get_coverage_status(0.0) == "None", "Status 0%", "None", utils::get_coverage_status(0.0));
}

/**
 * @brief Test path utilities
 */
void test_path_utilities() {
    std::cout << "\n=== Path Utility Tests ===" << std::endl;
    
    // Test normalize_path function
    UTIL_TEST_ASSERT(utils::normalize_path("path\\to\\file") == "path/to/file", "Normalize backslashes", "path/to/file", utils::normalize_path("path\\to\\file"));
    UTIL_TEST_ASSERT(utils::normalize_path("path//to//file") == "path/to/file", "Normalize double slashes", "path/to/file", utils::normalize_path("path//to//file"));
    UTIL_TEST_ASSERT(utils::normalize_path("path/to/file") == "path/to/file", "Normalize already good", "path/to/file", utils::normalize_path("path/to/file"));
    UTIL_TEST_ASSERT(utils::normalize_path("") == "", "Normalize empty path", "", utils::normalize_path(""));
    
    // Test get_filename function
    UTIL_TEST_ASSERT(utils::get_filename("path/to/file.txt") == "file.txt", "Get filename unix", "file.txt", utils::get_filename("path/to/file.txt"));
    UTIL_TEST_ASSERT(utils::get_filename("path\\to\\file.txt") == "file.txt", "Get filename windows", "file.txt", utils::get_filename("path\\to\\file.txt"));
    UTIL_TEST_ASSERT(utils::get_filename("file.txt") == "file.txt", "Get filename no path", "file.txt", utils::get_filename("file.txt"));
    UTIL_TEST_ASSERT(utils::get_filename("") == "", "Get filename empty", "", utils::get_filename(""));
    
    // Test get_directory function
    UTIL_TEST_ASSERT(utils::get_directory("path/to/file.txt") == "path/to", "Get directory unix", "path/to", utils::get_directory("path/to/file.txt"));
    UTIL_TEST_ASSERT(utils::get_directory("path\\to\\file.txt") == "path\\to", "Get directory windows", "path\\to", utils::get_directory("path\\to\\file.txt"));
    UTIL_TEST_ASSERT(utils::get_directory("file.txt") == "", "Get directory no path", "", utils::get_directory("file.txt"));
    UTIL_TEST_ASSERT(utils::get_directory("") == "", "Get directory empty", "", utils::get_directory(""));
}

/**
 * @brief Test file utilities
 */
void test_file_utilities() {
    std::cout << "\n=== File Utility Tests ===" << std::endl;
    
    // Create a test file
    std::ofstream test_file("test_util_file.txt");
    test_file << "Test content for file utilities";
    test_file.close();
    
    // Test file_exists function
    UTIL_TEST_ASSERT(utils::file_exists("test_util_file.txt"), "File exists check existing", "true", "false");
    UTIL_TEST_ASSERT(!utils::file_exists("non_existent_file.txt"), "File exists check non-existent", "false", "true");
    
    // Test get_file_size function
    std::size_t file_size = utils::get_file_size("test_util_file.txt");
    UTIL_TEST_ASSERT(file_size > 0, "File size check", ">0", std::to_string(file_size));
    UTIL_TEST_ASSERT(utils::get_file_size("non_existent_file.txt") == 0, "File size non-existent", "0", std::to_string(utils::get_file_size("non_existent_file.txt")));
    
    // Clean up
    std::remove("test_util_file.txt");
}

/**
 * @brief Test number formatting utilities
 */
void test_formatting_utilities() {
    std::cout << "\n=== Formatting Utility Tests ===" << std::endl;
    
    // Test format_number function
    UTIL_TEST_ASSERT(utils::format_number(1234) == "1,234", "Format number thousands", "1,234", utils::format_number(1234));
    UTIL_TEST_ASSERT(utils::format_number(1234567) == "1,234,567", "Format number millions", "1,234,567", utils::format_number(1234567));
    UTIL_TEST_ASSERT(utils::format_number(123) == "123", "Format number hundreds", "123", utils::format_number(123));
    UTIL_TEST_ASSERT(utils::format_number(0) == "0", "Format number zero", "0", utils::format_number(0));
    UTIL_TEST_ASSERT(utils::format_number(1000000000) == "1,000,000,000", "Format number billions", "1,000,000,000", utils::format_number(1000000000));
}

/**
 * @brief Test date/time parsing utilities
 */
void test_datetime_utilities() {
    std::cout << "\n=== DateTime Utility Tests ===" << std::endl;
    
    // Test parse_datetime function with common format
    auto time_point = utils::parse_datetime("Mon Jan 15 14:30:25 2024");
    auto epoch_time = std::chrono::system_clock::to_time_t(time_point);
    
    // Check if parsing returned a valid time (not epoch)
    UTIL_TEST_ASSERT(epoch_time > 0, "Parse datetime valid", ">0", std::to_string(epoch_time));
    
    // Test empty datetime
    auto empty_time = utils::parse_datetime("");
    auto empty_epoch = std::chrono::system_clock::to_time_t(empty_time);
    UTIL_TEST_ASSERT(empty_epoch == 0, "Parse datetime empty", "0", std::to_string(empty_epoch));
    
    // Test invalid datetime
    auto invalid_time = utils::parse_datetime("invalid date string");
    auto invalid_epoch = std::chrono::system_clock::to_time_t(invalid_time);
    UTIL_TEST_ASSERT(invalid_epoch == 0, "Parse datetime invalid", "0", std::to_string(invalid_epoch));
}

/**
 * @brief Main utility test runner
 */
int main() {
    std::cout << "FunctionalCoverageParsers Library - Utility Function Tests" << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    try {
        test_string_utilities();
        test_number_utilities();
        test_coverage_utilities();
        test_path_utilities();
        test_file_utilities();
        test_formatting_utilities();
        test_datetime_utilities();
    } catch (const std::exception& e) {
        std::cout << "Utility test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "Utility Test Results Summary:" << std::endl;
    std::cout << "Total Tests: " << util_tests_total << std::endl;
    std::cout << "Passed: " << util_tests_passed << std::endl;
    std::cout << "Failed: " << util_tests_failed << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (100.0 * util_tests_passed / util_tests_total) << "%" << std::endl;
    
    if (util_tests_failed == 0) {
        std::cout << "\n🎉 ALL UTILITY TESTS PASSED! 🎉" << std::endl;
        std::cout << "All utility functions work correctly." << std::endl;
        return 0;
    } else {
        std::cout << "\n❌ " << util_tests_failed << " UTILITY TESTS FAILED!" << std::endl;
        std::cout << "Some utility functions need attention." << std::endl;
        return 1;
    }
}