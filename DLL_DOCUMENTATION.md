# FunctionalCoverageParsers DLL and Library Documentation

## Overview

The FunctionalCoverageParsers library is available as both a Dynamic Link Library (DLL) for Windows and shared libraries for other platforms. This document describes how to build, distribute, and use the library.

## Build Artifacts

### Windows (MSVC)
- **DLL**: `FunctionalCoverageParsers.dll` - Runtime library containing the implementation
- **LIB**: `FunctionalCoverageParsers.lib` - Import library for linking
- **EXP**: `FunctionalCoverageParsers.exp` - Export file (generated automatically)
- **DEF**: `FunctionalCoverageParsers.def` - Module definition file

### Linux/Unix
- **Shared Library**: `libFunctionalCoverageParsers.so` - Shared object library
- **Static Library**: `libFunctionalCoverageParsers_static.a` - Static library

### macOS
- **Shared Library**: `libFunctionalCoverageParsers.dylib` - Dynamic library
- **Static Library**: `libFunctionalCoverageParsers_static.a` - Static library

## Building the Library

### Method 1: Using CMake (Recommended)

```bash
# Create build directory
mkdir build && cd build

# Configure build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build library
cmake --build . --config Release

# Install (optional)
cmake --install . --prefix /path/to/install
```

### Method 2: Using Visual Studio (Windows)

```batch
# Run the PowerShell build script
PowerShell -ExecutionPolicy Bypass -File build_dll.ps1

# Or use the batch file
build_dll.bat
```

### Method 3: Manual compilation

```bash
# Windows (MSVC)
cl /std:c++17 /EHsc /MD /O2 /DBUILDING_COVERAGE_PARSER_DLL ^
   /I./include src/*.cpp /link /DLL /DEF:FunctionalCoverageParsers.def ^
   /OUT:FunctionalCoverageParsers.dll /IMPLIB:FunctionalCoverageParsers.lib

# Linux (GCC)
g++ -std=c++17 -fPIC -shared -O2 -DBUILDING_COVERAGE_PARSER_DLL ^
    -I./include src/*.cpp -o libFunctionalCoverageParsers.so

# macOS (Clang)
clang++ -std=c++17 -fPIC -shared -O2 -DBUILDING_COVERAGE_PARSER_DLL ^
         -I./include src/*.cpp -o libFunctionalCoverageParsers.dylib
```

## API Reference

### Core Functions

```c
// Library information
const char* get_version_string();
const char* get_library_info();
const char* get_error_string(int result);

// Database management
void* create_coverage_database();
void destroy_coverage_database(void* handle);
int validate_database(void* db_handle);
double calculate_overall_score(void* db_handle);

// Parser creation
void* create_dashboard_parser();
void* create_groups_parser();
void* create_hierarchy_parser();
void* create_modlist_parser();
void* create_assert_parser();
void destroy_parser(void* handle);

// Parsing operations
int parse_coverage_file(void* parser_handle, const char* filename, void* db_handle);

// Query functions
int get_num_groups(void* db_handle);
int get_num_hierarchy_instances(void* db_handle);
int get_num_modules(void* db_handle);
int get_num_asserts(void* db_handle);

// Export functions
int export_coverage_to_xml(void* db_handle, const char* filename);
int export_coverage_to_json(void* db_handle, const char* filename);

// Resource management
int get_memory_usage(uint32_t* total_bytes, uint32_t* num_allocations);
void cleanup_library();
```

## Usage Examples

### C Application

```c
#include "functional_coverage_parser_dll.h"
#include <stdio.h>

int main() {
    // Initialize library
    printf("FunctionalCoverageParsers %s\n", get_version_string());
    
    // Create database and parsers
    void* db = create_coverage_database();
    void* dashboard_parser = create_dashboard_parser();
    void* groups_parser = create_groups_parser();
    
    if (!db || !dashboard_parser || !groups_parser) {
        printf("Failed to create parsers\n");
        return 1;
    }
    
    // Parse files
    int result = parse_coverage_file(dashboard_parser, "dashboard.txt", db);
    if (result != 0) {
        printf("Dashboard parse failed: %s\n", get_error_string(result));
    }
    
    result = parse_coverage_file(groups_parser, "groups.txt", db);
    if (result != 0) {
        printf("Groups parse failed: %s\n", get_error_string(result));
    }
    
    // Get statistics
    double score = calculate_overall_score(db);
    printf("Overall coverage: %.2f%%\n", score);
    
    printf("Groups: %d\n", get_num_groups(db));
    printf("Hierarchy instances: %d\n", get_num_hierarchy_instances(db));
    printf("Modules: %d\n", get_num_modules(db));
    printf("Assertions: %d\n", get_num_asserts(db));
    
    // Export results
    export_coverage_to_xml(db, "coverage_report.xml");
    export_coverage_to_json(db, "coverage_report.json");
    
    // Cleanup
    destroy_parser(dashboard_parser);
    destroy_parser(groups_parser);
    destroy_coverage_database(db);
    cleanup_library();
    
    return 0;
}
```

### C++ Application

```cpp
// You can also use the C++ API directly
#include "functional_coverage_parser.h"

int main() {
    using namespace coverage_parser;
    
    auto db = std::make_unique<CoverageDatabase>();
    
    DashboardParser dashboard_parser;
    GroupsParser groups_parser;
    
    ParserResult result = dashboard_parser.parse("dashboard.txt", *db);
    if (result == ParserResult::SUCCESS) {
        std::cout << "Dashboard parsed successfully\n";
    }
    
    result = groups_parser.parse("groups.txt", *db);
    if (result == ParserResult::SUCCESS) {
        std::cout << "Groups parsed successfully\n";
    }
    
    double score = db->calculate_overall_score();
    std::cout << "Overall coverage: " << score << "%\n";
    
    return 0;
}
```

## Linking Instructions

### Windows (Visual Studio)

1. **Include the header**:
   ```c
   #include "functional_coverage_parser_dll.h"
   ```

2. **Link against the import library**:
   - Add `FunctionalCoverageParsers.lib` to your project's Additional Dependencies
   - Or use pragma: `#pragma comment(lib, "FunctionalCoverageParsers.lib")`

3. **Ensure DLL is available at runtime**:
   - Copy `FunctionalCoverageParsers.dll` to your executable's directory
   - Or add the DLL's directory to your PATH

### Linux (GCC/Clang)

1. **Compile with headers**:
   ```bash
   gcc -I/path/to/headers your_app.c -L/path/to/lib -lFunctionalCoverageParsers -o your_app
   ```

2. **Set library path**:
   ```bash
   export LD_LIBRARY_PATH=/path/to/lib:$LD_LIBRARY_PATH
   ./your_app
   ```

### macOS (Clang)

1. **Compile with headers**:
   ```bash
   clang -I/path/to/headers your_app.c -L/path/to/lib -lFunctionalCoverageParsers -o your_app
   ```

2. **Set library path**:
   ```bash
   export DYLD_LIBRARY_PATH=/path/to/lib:$DYLD_LIBRARY_PATH
   ./your_app
   ```

## CMake Integration

If the library is installed with CMake, you can use it in your project:

```cmake
find_package(FunctionalCoverageParsers REQUIRED)

add_executable(your_app your_app.c)
target_link_libraries(your_app FunctionalCoverageParsers::FunctionalCoverageParsers)
```

## Distribution

### Windows Package Contents
```
FunctionalCoverageParsers/
├── bin/
│   └── FunctionalCoverageParsers.dll
├── lib/
│   ├── FunctionalCoverageParsers.lib
│   ├── FunctionalCoverageParsers.exp
│   └── FunctionalCoverageParsers.def
├── include/
│   ├── coverage_types.h
│   ├── functional_coverage_parser.h
│   └── functional_coverage_parser_dll.h
├── examples/
│   ├── usage_example.c
│   └── CMakeLists.txt
└── docs/
    └── README.md
```

### Linux/Unix Package Contents
```
FunctionalCoverageParsers/
├── lib/
│   ├── libFunctionalCoverageParsers.so
│   └── libFunctionalCoverageParsers_static.a
├── include/
│   ├── coverage_types.h
│   ├── functional_coverage_parser.h
│   └── functional_coverage_parser_dll.h
├── examples/
│   ├── usage_example.c
│   └── Makefile
└── docs/
    └── README.md
```

## Error Handling

All functions return error codes that can be converted to human-readable strings:

```c
int result = parse_coverage_file(parser, "file.txt", db);
if (result != 0) {
    printf("Error: %s\n", get_error_string(result));
}
```

Error codes:
- `0` (PARSER_SUCCESS): Operation successful
- `1` (PARSER_ERROR_FILE_NOT_FOUND): File not found
- `2` (PARSER_ERROR_FILE_ACCESS): File access denied
- `3` (PARSER_ERROR_PARSE_FAILED): Parsing failed
- `4` (PARSER_ERROR_INVALID_FORMAT): Invalid file format
- `5` (PARSER_ERROR_OUT_OF_MEMORY): Out of memory
- `6` (PARSER_ERROR_INVALID_PARAMETER): Invalid parameter

## Thread Safety

The library is not thread-safe by default. If you need to use it from multiple threads:

1. Use separate parser and database instances per thread
2. Or add your own synchronization (mutexes, etc.)
3. The `cleanup_library()` function should only be called once at program exit

## Memory Management

- Always call `destroy_parser()` and `destroy_coverage_database()` for created objects
- Call `cleanup_library()` before program exit to ensure proper cleanup
- Use `get_memory_usage()` to monitor memory consumption
- The library uses smart pointers internally for automatic memory management

## Performance Considerations

- The library is optimized for parsing large coverage files efficiently
- Use the appropriate parser type for each file format
- Consider using static linking for better performance in some scenarios
- The library supports streaming large files without loading everything into memory

## Troubleshooting

### Common Issues

1. **DLL not found**: Ensure the DLL is in PATH or same directory as executable
2. **Import library not found**: Check that the .lib file is in the linker's library path
3. **Version mismatch**: Ensure the DLL and headers are from the same version
4. **Memory leaks**: Always call cleanup functions in proper order

### Debug Build

For debugging, build with debug symbols:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

This enables additional error checking and debug output.