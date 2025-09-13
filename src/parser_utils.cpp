/**
 * @file parser_utils.cpp
 * @brief Common utility functions for coverage parsers
 * 
 * This file contains shared utility functions used across all parser
 * implementations in the FunctionalCoverageParsers library. These utilities
 * handle common parsing tasks such as:
 * - String manipulation and cleaning
 * - File validation and reading
 * - Date/time parsing from coverage reports
 * - Number parsing and validation
 * - Path normalization and cleanup
 * 
 * UTILITY CATEGORIES:
 * 1. String utilities: trimming, cleaning, splitting
 * 2. File utilities: validation, reading, path handling
 * 3. Data utilities: number parsing, date parsing, validation
 * 4. Coverage utilities: percentage calculation, status determination
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "functional_coverage_parser.h"
#include <sstream>
#include <algorithm>
#include <regex>
#include <iomanip>

namespace coverage_parser {
namespace utils {

/**
 * @brief Trim whitespace from both ends of a string
 * 
 * Removes leading and trailing whitespace, tabs, newlines, and carriage returns.
 * This is essential for parsing coverage files which often have inconsistent
 * whitespace formatting.
 * 
 * @param str String to trim
 * @return Trimmed string
 */
std::string trim(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

/**
 * @brief Split a string by delimiter
 * 
 * Splits a string into tokens using the specified delimiter.
 * Empty tokens are preserved to maintain field alignment in CSV-like formats.
 * 
 * @param str String to split
 * @param delimiter Character to split on
 * @return Vector of string tokens
 */
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    // Handle trailing delimiter
    if (!str.empty() && str.back() == delimiter) {
        tokens.push_back("");
    }
    
    return tokens;
}

/**
 * @brief Split a string by whitespace
 * 
 * Splits a string into tokens using any whitespace as delimiter.
 * Multiple consecutive whitespace characters are treated as a single delimiter.
 * 
 * @param str String to split
 * @return Vector of string tokens
 */
std::vector<std::string> split_whitespace(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

/**
 * @brief Convert string to lowercase
 * 
 * Converts all characters in a string to lowercase for case-insensitive
 * comparisons in coverage data parsing.
 * 
 * @param str String to convert
 * @return Lowercase string
 */
std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

/**
 * @brief Remove quotes from a string
 * 
 * Removes surrounding single or double quotes from a string.
 * Coverage tools sometimes output quoted field values.
 * 
 * @param str String to unquote
 * @return Unquoted string
 */
std::string remove_quotes(const std::string& str) {
    if (str.length() < 2) {
        return str;
    }
    
    char first = str.front();
    char last = str.back();
    
    if ((first == '"' && last == '"') || (first == '\'' && last == '\'')) {
        return str.substr(1, str.length() - 2);
    }
    
    return str;
}

/**
 * @brief Check if a file exists and is readable
 * 
 * Validates that a file exists and can be opened for reading.
 * 
 * @param filename Path to the file
 * @return true if file exists and is readable, false otherwise
 */
bool file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

/**
 * @brief Get file size in bytes
 * 
 * Returns the size of a file in bytes, useful for progress reporting
 * and memory allocation for large coverage files.
 * 
 * @param filename Path to the file
 * @return File size in bytes, 0 if file doesn't exist
 */
std::size_t get_file_size(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return 0;
    }
    
    return static_cast<std::size_t>(file.tellg());
}

/**
 * @brief Parse a percentage string
 * 
 * Parses percentage values from coverage reports. Handles various formats:
 * - "45.67%"
 * - "45.67"
 * - "45%"
 * 
 * @param percentage_str String containing percentage value
 * @return Percentage as double (0.0 to 100.0), -1.0 on error
 */
double parse_percentage(const std::string& percentage_str) {
    if (percentage_str.empty()) {
        return -1.0;
    }
    
    std::string clean_str = trim(percentage_str);
    
    // Remove % symbol if present
    if (clean_str.back() == '%') {
        clean_str.pop_back();
    }
    
    try {
        return std::stod(clean_str);
    } catch (const std::exception&) {
        return -1.0;
    }
}

/**
 * @brief Parse an integer with error handling
 * 
 * Safely parses integer values from coverage data with proper error handling.
 * 
 * @param int_str String containing integer value
 * @param default_value Default value to return on parse error
 * @return Parsed integer or default value
 */
std::int32_t parse_int(const std::string& int_str, std::int32_t default_value) {
    if (int_str.empty()) {
        return default_value;
    }
    
    try {
        return std::stoi(trim(int_str));
    } catch (const std::exception&) {
        return default_value;
    }
}

/**
 * @brief Parse an unsigned integer with error handling
 * 
 * Safely parses unsigned integer values from coverage data.
 * 
 * @param uint_str String containing unsigned integer value
 * @param default_value Default value to return on parse error
 * @return Parsed unsigned integer or default value
 */
std::uint32_t parse_uint(const std::string& uint_str, std::uint32_t default_value) {
    if (uint_str.empty()) {
        return default_value;
    }
    
    try {
        return std::stoul(trim(uint_str));
    } catch (const std::exception&) {
        return default_value;
    }
}

/**
 * @brief Parse a floating-point number with error handling
 * 
 * Safely parses floating-point values from coverage data.
 * 
 * @param float_str String containing floating-point value
 * @param default_value Default value to return on parse error
 * @return Parsed double or default value
 */
double parse_double(const std::string& float_str, double default_value) {
    if (float_str.empty()) {
        return default_value;
    }
    
    try {
        return std::stod(trim(float_str));
    } catch (const std::exception&) {
        return default_value;
    }
}

/**
 * @brief Parse a date/time string from coverage reports
 * 
 * Parses common date/time formats found in coverage reports:
 * - "Mon Jan 15 14:30:25 2024"
 * - "2024-01-15 14:30:25"
 * - "15/01/2024 14:30:25"
 * 
 * @param datetime_str String containing date/time
 * @return Parsed time_point, epoch time on error
 */
std::chrono::system_clock::time_point parse_datetime(const std::string& datetime_str) {
    if (datetime_str.empty()) {
        return std::chrono::system_clock::time_point{};
    }
    
    // Try to parse common formats
    std::tm tm = {};
    
    // Format: "Mon Jan 15 14:30:25 2024"
    std::regex format1(R"(\w+\s+\w+\s+(\d+)\s+(\d+):(\d+):(\d+)\s+(\d+))");
    std::smatch match;
    
    if (std::regex_search(datetime_str, match, format1)) {
        tm.tm_mday = std::stoi(match[1]);
        tm.tm_hour = std::stoi(match[2]);
        tm.tm_min = std::stoi(match[3]);
        tm.tm_sec = std::stoi(match[4]);
        tm.tm_year = std::stoi(match[5]) - 1900;
        
        // Parse month name
        if (datetime_str.find("Jan") != std::string::npos) tm.tm_mon = 0;
        else if (datetime_str.find("Feb") != std::string::npos) tm.tm_mon = 1;
        else if (datetime_str.find("Mar") != std::string::npos) tm.tm_mon = 2;
        else if (datetime_str.find("Apr") != std::string::npos) tm.tm_mon = 3;
        else if (datetime_str.find("May") != std::string::npos) tm.tm_mon = 4;
        else if (datetime_str.find("Jun") != std::string::npos) tm.tm_mon = 5;
        else if (datetime_str.find("Jul") != std::string::npos) tm.tm_mon = 6;
        else if (datetime_str.find("Aug") != std::string::npos) tm.tm_mon = 7;
        else if (datetime_str.find("Sep") != std::string::npos) tm.tm_mon = 8;
        else if (datetime_str.find("Oct") != std::string::npos) tm.tm_mon = 9;
        else if (datetime_str.find("Nov") != std::string::npos) tm.tm_mon = 10;
        else if (datetime_str.find("Dec") != std::string::npos) tm.tm_mon = 11;
        
        std::time_t time = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(time);
    }
    
    return std::chrono::system_clock::time_point{};
}

/**
 * @brief Calculate coverage percentage
 * 
 * Calculates coverage percentage from covered and total counts.
 * Handles division by zero gracefully.
 * 
 * @param covered Number of covered items
 * @param total Total number of items
 * @return Coverage percentage (0.0 to 100.0)
 */
double calculate_coverage_percentage(std::uint32_t covered, std::uint32_t total) {
    if (total == 0) {
        return 0.0;
    }
    
    return (static_cast<double>(covered) / static_cast<double>(total)) * 100.0;
}

/**
 * @brief Determine coverage status from percentage
 * 
 * Converts a coverage percentage to a human-readable status string.
 * 
 * @param percentage Coverage percentage (0.0 to 100.0)
 * @return Status string ("Excellent", "Good", "Fair", "Poor", "None")
 */
std::string get_coverage_status(double percentage) {
    if (percentage >= 95.0) {
        return "Excellent";
    } else if (percentage >= 80.0) {
        return "Good";
    } else if (percentage >= 60.0) {
        return "Fair";
    } else if (percentage > 0.0) {
        return "Poor";
    } else {
        return "None";
    }
}

/**
 * @brief Normalize a file path
 * 
 * Normalizes file paths by converting backslashes to forward slashes
 * and removing redundant path separators.
 * 
 * @param path File path to normalize
 * @return Normalized path
 */
std::string normalize_path(const std::string& path) {
    std::string normalized = path;
    
    // Convert backslashes to forward slashes
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // Remove double slashes
    size_t pos = 0;
    while ((pos = normalized.find("//", pos)) != std::string::npos) {
        normalized.replace(pos, 2, "/");
    }
    
    return normalized;
}

/**
 * @brief Extract filename from a path
 * 
 * Extracts the filename (with extension) from a full file path.
 * 
 * @param path Full file path
 * @return Filename only
 */
std::string get_filename(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

/**
 * @brief Extract directory from a path
 * 
 * Extracts the directory portion from a full file path.
 * 
 * @param path Full file path
 * @return Directory path
 */
std::string get_directory(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(0, pos);
    }
    return "";
}

/**
 * @brief Check if a string represents a valid number
 * 
 * Determines if a string contains a valid numeric value (integer or float).
 * 
 * @param str String to check
 * @return true if string is a valid number, false otherwise
 */
bool is_number(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    std::regex number_pattern(R"(^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$)");
    return std::regex_match(trim(str), number_pattern);
}

/**
 * @brief Format a number with thousands separators
 * 
 * Formats large numbers with comma separators for better readability
 * in reports and output.
 * 
 * @param number Number to format
 * @return Formatted string with thousands separators
 */
std::string format_number(std::uint32_t number) {
    std::string str = std::to_string(number);
    std::string formatted;
    
    int count = 0;
    for (int i = str.length() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            formatted = ',' + formatted;
        }
        formatted = str[i] + formatted;
        count++;
    }
    
    return formatted;
}

} // namespace utils
} // namespace coverage_parser