/**
 * @file focused_stub.cpp
 * @brief Focused stub DLL with exact functions needed by .NET tests
 */

extern "C" {

#define COVERAGE_PARSER_API __declspec(dllexport)

// Basic library functions
COVERAGE_PARSER_API const char* __cdecl get_version_string() {
    return "1.0.0-stub";
}

COVERAGE_PARSER_API const char* __cdecl get_library_info() {
    return "FunctionalCoverageParsers Stub DLL v1.0.0";
}

COVERAGE_PARSER_API const char* __cdecl get_error_string(int error_code) {
    return "Stub error occurred";  // Simplified to avoid sprintf issues
}

// Database functions
COVERAGE_PARSER_API void* __cdecl create_coverage_database() {
    return (void*)0x12345678; // Return fake handle
}

COVERAGE_PARSER_API void __cdecl destroy_coverage_database(void* db_handle) {
    // Stub - nothing to destroy
}

COVERAGE_PARSER_API int __cdecl validate_database(void* db_handle) {
    return 0; // Success (0 = valid in .NET code)
}

COVERAGE_PARSER_API double __cdecl calculate_overall_score(void* db_handle) {
    return 85.5; // Mock score
}

// Parser creation functions
COVERAGE_PARSER_API void* __cdecl create_dashboard_parser() {
    return (void*)0x11111111;
}

COVERAGE_PARSER_API void* __cdecl create_groups_parser() {
    return (void*)0x22222222;
}

COVERAGE_PARSER_API void* __cdecl create_hierarchy_parser() {
    return (void*)0x33333333;
}

COVERAGE_PARSER_API void* __cdecl create_assert_parser() {
    return (void*)0x44444444;
}

COVERAGE_PARSER_API void* __cdecl create_modlist_parser() {
    return (void*)0x55555555;
}

COVERAGE_PARSER_API void __cdecl destroy_parser(void* parser_handle) {
    // Stub - nothing to destroy
}

// File parsing functions (corrected signature with 3 parameters)
COVERAGE_PARSER_API int __cdecl parse_coverage_file(void* parser_handle, const char* filename, void* db_handle) {
    return 0; // Success
}

// Query functions
COVERAGE_PARSER_API int __cdecl get_num_groups(void* db_handle) {
    return 42; // Mock count
}

COVERAGE_PARSER_API int __cdecl get_num_hierarchy_instances(void* db_handle) {
    return 17; // Mock count
}

COVERAGE_PARSER_API int __cdecl get_num_modules(void* db_handle) {
    return 8; // Mock count
}

COVERAGE_PARSER_API int __cdecl get_num_asserts(void* db_handle) {
    return 1234; // Mock count
}

// Export functions (corrected function names)
COVERAGE_PARSER_API int __cdecl export_coverage_to_xml(void* db_handle, const char* filename) {
    return 0; // Success
}

COVERAGE_PARSER_API int __cdecl export_coverage_to_json(void* db_handle, const char* filename) {
    return 0; // Success
}

// Memory functions (corrected signature for .NET out parameters)
COVERAGE_PARSER_API int __cdecl get_memory_usage(unsigned int* total_bytes, unsigned int* num_allocations) {
    if (total_bytes) *total_bytes = 1024 * 1024; // 1MB
    if (num_allocations) *num_allocations = 100;
    return 0; // Success
}

// High-performance parser functions
COVERAGE_PARSER_API void* __cdecl create_high_performance_groups_parser() {
    return (void*)0x66666666;
}

COVERAGE_PARSER_API void* __cdecl create_high_performance_hierarchy_parser() {
    return (void*)0x77777777;
}

COVERAGE_PARSER_API void* __cdecl create_high_performance_assert_parser() {
    return (void*)0x88888888;
}

} // extern "C"