/**
 * @file high_performance_parser.h
 * @brief High-performance optimized coverage file parser for ultra-fast processing of huge files
 * 
 * This header provides optimized parsing capabilities for handling multi-gigabyte coverage files
 * with maximum performance through:
 * 
 * 1. Memory-mapped file I/O - Direct memory access without syscall overhead
 * 2. SIMD vectorized operations - SSE/AVX for fast string operations  
 * 3. Parallel processing - Multi-threaded parsing of file sections
 * 4. Custom memory management - Memory pools and optimized allocators
 * 5. Zero-copy parsing - Minimize string allocations and copies
 * 
 * PERFORMANCE TARGETS:
 * - 100MB+ files: Parse in under 5 seconds
 * - 1GB+ files: Parse in under 30 seconds
 * - Memory usage: <10% of file size during parsing
 * - CPU utilization: Scale to all available cores
 * 
 * @author FunctionalCoverageParsers Library - Performance Team
 * @version 2.0 - Ultra-Fast Edition
 * @date 2025
 */

#pragma once

#include "functional_coverage_parser.h"
#include <memory>
#include <thread>
#include <vector>
#include <string_view>
#include <mutex>
#include <atomic>
#include <immintrin.h>  // SIMD intrinsics

namespace coverage_parser {
namespace performance {

/**
 * @brief Memory-mapped file wrapper for zero-copy file access
 * 
 * Provides efficient access to file contents through memory mapping,
 * eliminating the need for read() syscalls and buffer copies.
 */
class MemoryMappedFile {
public:
    MemoryMappedFile(const std::string& filename);
    ~MemoryMappedFile();
    
    bool is_valid() const { return data_ != nullptr; }
    const char* data() const { return data_; }
    std::size_t size() const { return size_; }
    
    // Get a string_view to a specific range (zero-copy)
    std::string_view get_view(std::size_t offset, std::size_t length) const;
    
private:
    const char* data_ = nullptr;
    std::size_t size_ = 0;
    void* file_handle_ = nullptr;
    void* mapping_handle_ = nullptr;
};

/**
 * @brief High-performance string utilities using SIMD instructions
 * 
 * Vectorized string operations for maximum parsing speed on large datasets.
 */
namespace simd {
    
    /**
     * @brief Find first occurrence of character using SIMD (SSE/AVX)
     * Up to 16x faster than standard strchr for large buffers
     */
    const char* find_char_simd(const char* data, std::size_t length, char target);
    
    /**
     * @brief Find newline characters using vectorized search
     * Optimized for finding line boundaries in huge files
     */
    std::vector<std::size_t> find_newlines_simd(const char* data, std::size_t length);
    
    /**
     * @brief Skip whitespace using SIMD instructions
     * Fast whitespace trimming for parsing operations
     */
    const char* skip_whitespace_simd(const char* start, const char* end);
    
    /**
     * @brief Parse numbers from string using vectorized operations
     * Optimized integer/float parsing for coverage metrics
     */
    bool parse_uint_simd(const char* start, const char* end, uint32_t& result);
    bool parse_double_simd(const char* start, const char* end, double& result);
}

/**
 * @brief Memory pool allocator for reduced allocation overhead
 * 
 * Custom allocator that pre-allocates large chunks and sub-allocates
 * from them to minimize malloc/free overhead during parsing.
 */
class MemoryPool {
public:
    MemoryPool(std::size_t chunk_size = 64 * 1024); // 64KB chunks
    ~MemoryPool();
    
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    void reset(); // Reset pool for reuse
    
    // Statistics
    std::size_t total_allocated() const { return total_allocated_; }
    std::size_t chunks_count() const { return chunks_.size(); }
    
private:
    struct Chunk {
        void* memory;
        std::size_t size;
        std::size_t used;
    };
    
    std::vector<Chunk> chunks_;
    std::size_t chunk_size_;
    std::size_t total_allocated_ = 0;
    std::mutex mutex_;
};

/**
 * @brief Parallel processing utilities for multi-threaded parsing
 * 
 * Divides large files into chunks and processes them concurrently
 * across multiple CPU cores for maximum throughput.
 */
class ParallelProcessor {
public:
    struct FileChunk {
        std::size_t start_offset;
        std::size_t end_offset;
        std::size_t line_start;  // Adjusted to line boundaries
        std::size_t line_end;    // Adjusted to line boundaries
    };
    
    static std::vector<FileChunk> create_chunks(
        const MemoryMappedFile& file, 
        std::size_t num_threads = std::thread::hardware_concurrency()
    );
    
    template<typename ParseFunc>
    static ParserResult process_parallel(
        const MemoryMappedFile& file,
        const std::vector<FileChunk>& chunks,
        ParseFunc parse_func,
        CoverageDatabase& db
    );
    
private:
    static std::size_t find_line_boundary(const char* data, std::size_t start, std::size_t file_size, bool find_start);
};

/**
 * @brief High-performance groups parser using all optimizations
 * 
 * Ultra-fast implementation of groups.txt parsing using:
 * - Memory mapping for zero-copy access
 * - SIMD operations for string processing
 * - Parallel processing across multiple cores
 * - Memory pools for allocation efficiency
 */
class HighPerformanceGroupsParser {
public:
    HighPerformanceGroupsParser();
    ~HighPerformanceGroupsParser() = default;
    
    /**
     * @brief Parse groups file with maximum performance
     * Target: 100MB file in <5 seconds on modern CPU
     */
    ParserResult parse(const std::string& filename, CoverageDatabase& db);
    
    // Performance monitoring
    struct PerformanceStats {
        double parse_time_seconds = 0.0;
        std::size_t file_size_bytes = 0;
        std::size_t lines_processed = 0;
        std::size_t groups_parsed = 0;
        std::size_t memory_allocated = 0;
        uint32_t threads_used = 0;
        double throughput_mb_per_sec = 0.0;
    };
    
    const PerformanceStats& get_stats() const { return stats_; }
    
private:
    MemoryPool memory_pool_;
    PerformanceStats stats_;
    
    // Chunk processing functions
    ParserResult parse_chunk(
        const MemoryMappedFile& file,
        const ParallelProcessor::FileChunk& chunk,
        std::vector<std::unique_ptr<CoverageGroup>>& groups
    );
    
    ParserResult parse_group_line_optimized(
        std::string_view line,
        std::unique_ptr<CoverageGroup>& group
    );
    
    // Fast tokenization using SIMD
    std::vector<std::string_view> tokenize_line_simd(std::string_view line);
};

/**
 * @brief High-performance hierarchy parser
 * Optimized for processing large hierarchy.txt files
 */
class HighPerformanceHierarchyParser {
public:
    ParserResult parse(const std::string& filename, CoverageDatabase& db);
    const PerformanceGroupsParser::PerformanceStats& get_stats() const { return stats_; }
    
private:
    MemoryPool memory_pool_;
    HighPerformanceGroupsParser::PerformanceStats stats_;
};

/**
 * @brief High-performance assert parser
 * Optimized for processing huge asserts.txt files (100MB+)
 */
class HighPerformanceAssertParser {
public:
    ParserResult parse(const std::string& filename, CoverageDatabase& db);
    const HighPerformanceGroupsParser::PerformanceStats& get_stats() const { return stats_; }
    
private:
    MemoryPool memory_pool_;
    HighPerformanceGroupsParser::PerformanceStats stats_;
};

/**
 * @brief Factory for creating high-performance parsers
 * 
 * Automatically selects optimized parser based on file size and type.
 * Falls back to standard parsers for small files where optimization overhead
 * would exceed the benefits.
 */
class PerformanceParserFactory {
public:
    static std::unique_ptr<GroupsParser> create_groups_parser(const std::string& filename);
    static std::unique_ptr<HierarchyParser> create_hierarchy_parser(const std::string& filename);
    static std::unique_ptr<AssertParser> create_assert_parser(const std::string& filename);
    
private:
    static constexpr std::size_t OPTIMIZATION_THRESHOLD = 10 * 1024 * 1024; // 10MB
};

} // namespace performance
} // namespace coverage_parser