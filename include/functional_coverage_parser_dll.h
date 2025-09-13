/**
 * @file functional_coverage_parser_dll.h
 * @brief C API header for FunctionalCoverageParsers DLL
 * 
 * This header provides the C API for using the FunctionalCoverageParsers library
 * from a DLL. It defines the exported functions and data structures that can be
 * used by client applications.
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#ifndef FUNCTIONAL_COVERAGE_PARSER_DLL_H
#define FUNCTIONAL_COVERAGE_PARSER_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Windows DLL export/import macros */
#ifdef _WIN32
    #ifdef BUILDING_COVERAGE_PARSER_DLL
        #define COVERAGE_PARSER_API __declspec(dllexport)
    #else
        #define COVERAGE_PARSER_API __declspec(dllimport)
    #endif
#else
    #define COVERAGE_PARSER_API
#endif

/* Standard integer types for compatibility */
typedef unsigned int uint32_t;

/**
 * @brief Parser result codes
 */
typedef enum {
    PARSER_SUCCESS = 0,
    PARSER_ERROR_FILE_NOT_FOUND = 1,
    PARSER_ERROR_FILE_ACCESS = 2,
    PARSER_ERROR_PARSE_FAILED = 3,
    PARSER_ERROR_INVALID_FORMAT = 4,
    PARSER_ERROR_OUT_OF_MEMORY = 5,
    PARSER_ERROR_INVALID_PARAMETER = 6
} ParserResultCode;

/**
 * @defgroup Version Version Information
 * @brief Library version and information functions
 * @{
 */

/**
 * @brief Get library version string
 * @return Version string (e.g., "1.0.0")
 */
COVERAGE_PARSER_API const char* get_version_string();

/**
 * @brief Get library information
 * @return Library description string
 */
COVERAGE_PARSER_API const char* get_library_info();

/**
 * @brief Convert parser result to error string
 * @param result Parser result code
 * @return Human-readable error description
 */
COVERAGE_PARSER_API const char* get_error_string(int result);

/** @} */

/**
 * @defgroup Database Database Management
 * @brief Coverage database creation and management functions
 * @{
 */

/**
 * @brief Create coverage database
 * @return Database handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_coverage_database();

/**
 * @brief Destroy coverage database
 * @param handle Database handle
 */
COVERAGE_PARSER_API void destroy_coverage_database(void* handle);

/**
 * @brief Validate database integrity
 * @param db_handle Database handle
 * @return 1 if valid, 0 if invalid, -1 on error
 */
COVERAGE_PARSER_API int validate_database(void* db_handle);

/**
 * @brief Calculate overall coverage score
 * @param db_handle Database handle
 * @return Overall score (0-100) or -1.0 on error
 */
COVERAGE_PARSER_API double calculate_overall_score(void* db_handle);

/** @} */

/**
 * @defgroup Parsers Parser Creation and Management
 * @brief Parser creation and destruction functions
 * @{
 */

/**
 * @brief Create dashboard parser
 * @return Parser handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_dashboard_parser();

/**
 * @brief Create groups parser
 * @return Parser handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_groups_parser();

/**
 * @brief Create hierarchy parser
 * @return Parser handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_hierarchy_parser();

/**
 * @brief Create module list parser
 * @return Parser handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_modlist_parser();

/**
 * @brief Create assert parser
 * @return Parser handle or NULL on failure
 */
COVERAGE_PARSER_API void* create_assert_parser();

/**
 * @brief Destroy parser
 * @param handle Parser handle
 */
COVERAGE_PARSER_API void destroy_parser(void* handle);

/** @} */

/**
 * @defgroup Parsing Parsing Functions
 * @brief File parsing functions
 * @{
 */

/**
 * @brief Parse coverage file
 * @param parser_handle Parser handle
 * @param filename Path to coverage file
 * @param db_handle Database handle to store results
 * @return Parser result code (0 = success)
 */
COVERAGE_PARSER_API int parse_coverage_file(void* parser_handle, const char* filename, void* db_handle);

/** @} */

/**
 * @defgroup Query Query Functions
 * @brief Database query and statistics functions
 * @{
 */

/**
 * @brief Get number of coverage groups
 * @param db_handle Database handle
 * @return Number of groups or -1 on error
 */
COVERAGE_PARSER_API int get_num_groups(void* db_handle);

/**
 * @brief Get number of hierarchy instances
 * @param db_handle Database handle
 * @return Number of instances or -1 on error
 */
COVERAGE_PARSER_API int get_num_hierarchy_instances(void* db_handle);

/**
 * @brief Get number of modules
 * @param db_handle Database handle
 * @return Number of modules or -1 on error
 */
COVERAGE_PARSER_API int get_num_modules(void* db_handle);

/**
 * @brief Get number of assertions
 * @param db_handle Database handle
 * @return Number of assertions or -1 on error
 */
COVERAGE_PARSER_API int get_num_asserts(void* db_handle);

/** @} */

/**
 * @defgroup Export Export Functions
 * @brief Coverage data export functions
 * @{
 */

/**
 * @brief Export coverage data to XML format
 * @param db_handle Database handle
 * @param filename Output XML filename
 * @return Parser result code (0 = success)
 */
COVERAGE_PARSER_API int export_coverage_to_xml(void* db_handle, const char* filename);

/**
 * @brief Export coverage data to JSON format
 * @param db_handle Database handle
 * @param filename Output JSON filename
 * @return Parser result code (0 = success)
 */
COVERAGE_PARSER_API int export_coverage_to_json(void* db_handle, const char* filename);

/** @} */

/**
 * @defgroup Memory Memory Management
 * @brief Memory and resource management functions
 * @{
 */

/**
 * @brief Get memory usage statistics
 * @param total_bytes Output parameter for total allocated bytes
 * @param num_allocations Output parameter for number of active allocations
 * @return Parser result code (0 = success)
 */
COVERAGE_PARSER_API int get_memory_usage(uint32_t* total_bytes, uint32_t* num_allocations);

/**
 * @brief Cleanup library resources
 * 
 * Call this function before unloading the DLL to ensure proper cleanup
 * of all allocated resources.
 */
COVERAGE_PARSER_API void cleanup_library();

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* FUNCTIONAL_COVERAGE_PARSER_DLL_H */