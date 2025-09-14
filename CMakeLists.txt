# CMakeLists.txt for FunctionalCoverageParsers Library
# Cross-platform build configuration for DLL/shared library

cmake_minimum_required(VERSION 3.12)
project(FunctionalCoverageParsers VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build configuration
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(PARSER_SOURCES
    src/dashboard_parser.cpp
    src/groups_parser.cpp
    src/hierarchy_parser.cpp
    src/modlist_parser.cpp
    src/assert_parser.cpp
    src/parser_utils.cpp
    src/dll_api.cpp
    src/high_performance_parser.cpp
)

# Header files
set(PARSER_HEADERS
    include/coverage_types.h
    include/functional_coverage_parser.h
    include/functional_coverage_parser_dll.h
    include/high_performance_parser.h
)

# Create shared library (DLL on Windows, .so on Linux, .dylib on macOS)
add_library(FunctionalCoverageParsers SHARED ${PARSER_SOURCES} ${PARSER_HEADERS})

# Set library properties
set_target_properties(FunctionalCoverageParsers PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "FunctionalCoverageParsers"
    PUBLIC_HEADER "${PARSER_HEADERS}"
)

# Compiler definitions
target_compile_definitions(FunctionalCoverageParsers PRIVATE
    BUILDING_COVERAGE_PARSER_DLL
    $<$<PLATFORM_ID:Windows>:_WINDLL>
    $<$<PLATFORM_ID:Windows>:_USRDLL>
)

# Compiler options
target_compile_options(FunctionalCoverageParsers PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /EHsc>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)

# Windows-specific configuration
if(WIN32)
    # Use module definition file for explicit exports
    if(EXISTS ${CMAKE_SOURCE_DIR}/FunctionalCoverageParsers.def)
        set_target_properties(FunctionalCoverageParsers PROPERTIES
            LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/FunctionalCoverageParsers.def"
        )
    endif()
    
    # Set subsystem
    set_target_properties(FunctionalCoverageParsers PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()

# Create static library as well
add_library(FunctionalCoverageParsers_static STATIC ${PARSER_SOURCES} ${PARSER_HEADERS})
set_target_properties(FunctionalCoverageParsers_static PROPERTIES
    OUTPUT_NAME "FunctionalCoverageParsers_static"
    PUBLIC_HEADER "${PARSER_HEADERS}"
)

# Installation configuration
install(TARGETS FunctionalCoverageParsers FunctionalCoverageParsers_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Install module definition file
if(WIN32 AND EXISTS ${CMAKE_SOURCE_DIR}/FunctionalCoverageParsers.def)
    install(FILES ${CMAKE_SOURCE_DIR}/FunctionalCoverageParsers.def
        DESTINATION lib
    )
endif()

# Create package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FunctionalCoverageParsersConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionalCoverageParsersConfig.cmake"
    INSTALL_DESTINATION lib/cmake/FunctionalCoverageParsers
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionalCoverageParsersConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionalCoverageParsersConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionalCoverageParsersConfigVersion.cmake"
    DESTINATION lib/cmake/FunctionalCoverageParsers
)

# Export targets
export(TARGETS FunctionalCoverageParsers FunctionalCoverageParsers_static
    FILE "${CMAKE_CURRENT_BINARY_DIR}/FunctionalCoverageParsersTargets.cmake"
)

install(EXPORT FunctionalCoverageParsersTargets
    FILE FunctionalCoverageParsersTargets.cmake
    DESTINATION lib/cmake/FunctionalCoverageParsers
)

# Create test executable (optional)
option(BUILD_TESTS "Build test executable" ON)
if(BUILD_TESTS)
    add_executable(test_dll_usage usage_example.c)
    target_link_libraries(test_dll_usage FunctionalCoverageParsers)
    target_include_directories(test_dll_usage PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "FunctionalCoverageParsers Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "")

# Build instructions
file(WRITE ${CMAKE_BINARY_DIR}/BUILD_INSTRUCTIONS.txt
"FunctionalCoverageParsers Library Build Instructions
====================================================

To build the library:

1. Create build directory:
   mkdir build && cd build

2. Configure with CMake:
   cmake ..
   
   Optional configuration:
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake -DBUILD_TESTS=OFF ..

3. Build the library:
   cmake --build .
   
   Or on Windows with Visual Studio:
   cmake --build . --config Release

4. Install (optional):
   cmake --install .

Output files will be in:
- DLL/shared library: build/bin/
- Import library: build/lib/
- Headers: include/

Usage:
- Link against: FunctionalCoverageParsers.lib (Windows) or libFunctionalCoverageParsers.so (Linux)
- Include: functional_coverage_parser_dll.h
- Ensure DLL/shared library is in PATH or same directory
"
)