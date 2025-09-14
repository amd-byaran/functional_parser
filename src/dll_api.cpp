/**
 * @file dll_api.cpp
 * @brief C API wrapper for FunctionalCoverageParsers DLL
 * 
 * This file provides C-style API functions that can be exported from the DLL
 * and used by client applications. It wraps the C++ classes and provides
 * a stable ABI for the library.
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "functional_coverage_parser.h"
#include <memory>
#include <map>

using namespace coverage_parser;

// Global handle management
static std::map<void*, std::unique_ptr<BaseParser>> parser_handles;
static std::map<void*, std::unique_ptr<CoverageDatabase>> database_handles;
static uint32_t next_handle_id = 1;

extern "C" {

/**
 * @brief Get library version string
 * @return Version string
 */
COVERAGE_PARSER_API const char* get_version_string() {
    return VERSION_STRING;
}

/**
 * @brief Get library information
 * @return Library info string
 */
COVERAGE_PARSER_API const char* get_library_info() {
    return "FunctionalCoverageParsers Library v1.0 - EDA Coverage File Parser";
}

/**
 * @brief Convert parser result to error string
 * @param result Parser result code
 * @return Error description string
 */
COVERAGE_PARSER_API const char* get_error_string(ParserResult result) {
    switch (result) {
        case ParserResult::SUCCESS:
            return "Success";
        case ParserResult::ERROR_FILE_NOT_FOUND:
            return "File not found";
        case ParserResult::ERROR_PARSE_FAILED:
            return "Parse failed";
        case ParserResult::ERROR_INVALID_FORMAT:
            return "Invalid file format";
        case ParserResult::ERROR_MEMORY_ALLOCATION:
            return "Out of memory";
        case ParserResult::ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        default:
            return "Unknown error";
    }
}

/**
 * @brief Create coverage database
 * @return Database handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_coverage_database() {
    try {
        auto db = std::make_unique<CoverageDatabase>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        database_handles[handle] = std::move(db);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Destroy coverage database
 * @param handle Database handle
 */
COVERAGE_PARSER_API void destroy_coverage_database(void* handle) {
    if (handle && database_handles.find(handle) != database_handles.end()) {
        database_handles.erase(handle);
    }
}

/**
 * @brief Create dashboard parser
 * @return Parser handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_dashboard_parser() {
    try {
        auto parser = std::make_unique<DashboardParser>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        parser_handles[handle] = std::move(parser);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Create groups parser
 * @return Parser handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_groups_parser() {
    try {
        auto parser = std::make_unique<GroupsParser>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        parser_handles[handle] = std::move(parser);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Create hierarchy parser
 * @return Parser handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_hierarchy_parser() {
    try {
        auto parser = std::make_unique<HierarchyParser>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        parser_handles[handle] = std::move(parser);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Create module list parser
 * @return Parser handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_modlist_parser() {
    try {
        auto parser = std::make_unique<ModuleListParser>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        parser_handles[handle] = std::move(parser);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Create assert parser
 * @return Parser handle or nullptr on failure
 */
COVERAGE_PARSER_API void* create_assert_parser() {
    try {
        auto parser = std::make_unique<AssertParser>();
        void* handle = reinterpret_cast<void*>(next_handle_id++);
        parser_handles[handle] = std::move(parser);
        return handle;
    } catch (...) {
        return nullptr;
    }
}

/**
 * @brief Destroy parser
 * @param handle Parser handle
 */
COVERAGE_PARSER_API void destroy_parser(void* handle) {
    if (handle && parser_handles.find(handle) != parser_handles.end()) {
        parser_handles.erase(handle);
    }
}

/**
 * @brief Parse coverage file
 * @param parser_handle Parser handle
 * @param filename File to parse
 * @param db_handle Database handle
 * @return Parser result code
 */
COVERAGE_PARSER_API int parse_coverage_file(void* parser_handle, const char* filename, void* db_handle) {
    if (!parser_handle || !filename || !db_handle) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    auto parser_it = parser_handles.find(parser_handle);
    auto db_it = database_handles.find(db_handle);
    
    if (parser_it == parser_handles.end() || db_it == database_handles.end()) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    try {
        ParserResult result = parser_it->second->parse(filename, *db_it->second);
        return static_cast<int>(result);
    } catch (...) {
        return static_cast<int>(ParserResult::ERROR_PARSE_FAILED);
    }
}

/**
 * @brief Calculate overall coverage score
 * @param db_handle Database handle
 * @return Overall score or -1.0 on error
 */
COVERAGE_PARSER_API double calculate_overall_score(void* db_handle) {
    if (!db_handle) {
        return -1.0;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1.0;
    }
    
    try {
        return db_it->second->calculate_overall_score();
    } catch (...) {
        return -1.0;
    }
}

/**
 * @brief Validate database
 * @param db_handle Database handle
 * @return 1 if valid, 0 if invalid, -1 on error
 */
COVERAGE_PARSER_API int validate_database(void* db_handle) {
    if (!db_handle) {
        return -1;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1;
    }
    
    try {
        return db_it->second->validate() ? 1 : 0;
    } catch (...) {
        return -1;
    }
}

/**
 * @brief Get number of coverage groups
 * @param db_handle Database handle
 * @return Number of groups or -1 on error
 */
COVERAGE_PARSER_API int get_num_groups(void* db_handle) {
    if (!db_handle) {
        return -1;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1;
    }
    
    try {
        return static_cast<int>(db_it->second->get_num_groups());
    } catch (...) {
        return -1;
    }
}

/**
 * @brief Get number of hierarchy instances
 * @param db_handle Database handle
 * @return Number of instances or -1 on error
 */
COVERAGE_PARSER_API int get_num_hierarchy_instances(void* db_handle) {
    if (!db_handle) {
        return -1;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1;
    }
    
    try {
        return static_cast<int>(db_it->second->get_num_hierarchy_instances());
    } catch (...) {
        return -1;
    }
}

/**
 * @brief Get number of modules
 * @param db_handle Database handle
 * @return Number of modules or -1 on error
 */
COVERAGE_PARSER_API int get_num_modules(void* db_handle) {
    if (!db_handle) {
        return -1;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1;
    }
    
    try {
        return static_cast<int>(db_it->second->get_num_modules());
    } catch (...) {
        return -1;
    }
}

/**
 * @brief Get number of assertions
 * @param db_handle Database handle
 * @return Number of assertions or -1 on error
 */
COVERAGE_PARSER_API int get_num_asserts(void* db_handle) {
    if (!db_handle) {
        return -1;
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return -1;
    }
    
    try {
        return static_cast<int>(db_it->second->get_num_asserts());
    } catch (...) {
        return -1;
    }
}

/**
 * @brief Export coverage to XML
 * @param db_handle Database handle
 * @param filename Output filename
 * @return Parser result code
 */
COVERAGE_PARSER_API int export_coverage_to_xml(void* db_handle, const char* filename) {
    if (!db_handle || !filename) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    try {
        return static_cast<int>(coverage_parser::export_coverage_to_xml(*db_it->second, filename));
    } catch (...) {
        return static_cast<int>(ParserResult::ERROR_PARSE_FAILED);
    }
}

/**
 * @brief Export coverage to JSON
 * @param db_handle Database handle
 * @param filename Output filename
 * @return Parser result code
 */
COVERAGE_PARSER_API int export_coverage_to_json(void* db_handle, const char* filename) {
    if (!db_handle || !filename) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    auto db_it = database_handles.find(db_handle);
    if (db_it == database_handles.end()) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    try {
        return static_cast<int>(coverage_parser::export_coverage_to_json(*db_it->second, filename));
    } catch (...) {
        return static_cast<int>(ParserResult::ERROR_PARSE_FAILED);
    }
}

/**
 * @brief Cleanup library resources
 */
COVERAGE_PARSER_API void cleanup_library() {
    parser_handles.clear();
    database_handles.clear();
    next_handle_id = 1;
}

/**
 * @brief Get memory usage statistics
 * @param total_bytes Output parameter for total bytes
 * @param num_allocations Output parameter for number of allocations
 * @return Parser result code
 */
COVERAGE_PARSER_API int get_memory_usage(uint32_t* total_bytes, uint32_t* num_allocations) {
    if (!total_bytes || !num_allocations) {
        return static_cast<int>(ParserResult::ERROR_INVALID_PARAMETER);
    }
    
    try {
        std::size_t total;
        uint32_t allocs;
        ParserResult result = coverage_parser::get_memory_usage(total, allocs);
        *total_bytes = static_cast<uint32_t>(total);
        *num_allocations = allocs;
        return static_cast<int>(result);
    } catch (...) {
        return static_cast<int>(ParserResult::ERROR_PARSE_FAILED);
    }
}

} // extern "C"