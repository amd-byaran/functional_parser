using System;
using System.Runtime.InteropServices;

namespace FunctionalCoverageParsers.Native
{
    /// <summary>
    /// Native P/Invoke declarations for FunctionalCoverageParsers.dll
    /// This class provides direct access to the native C API.
    /// </summary>
    internal static class NativeMethods
    {
        private const string DllName = "FunctionalCoverageParsers.dll";

        #region Library Information Functions

        /// <summary>
        /// Gets the version string of the native library
        /// </summary>
        /// <returns>Pointer to version string</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr get_version_string();

        /// <summary>
        /// Gets the library information string
        /// </summary>
        /// <returns>Pointer to library info string</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr get_library_info();

        /// <summary>
        /// Gets the error string for a given error code
        /// </summary>
        /// <param name="errorCode">Error code to get string for</param>
        /// <returns>Pointer to error string</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr get_error_string(int errorCode);

        #endregion

        #region Database Management Functions

        /// <summary>
        /// Creates a new coverage database
        /// </summary>
        /// <returns>Handle to the database</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_coverage_database();

        /// <summary>
        /// Destroys a coverage database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void destroy_coverage_database(IntPtr dbHandle);

        /// <summary>
        /// Validates a coverage database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int validate_database(IntPtr dbHandle);

        /// <summary>
        /// Calculates the overall coverage score
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>Overall coverage score as percentage</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern double calculate_overall_score(IntPtr dbHandle);

        #endregion

        #region Parser Creation Functions

        /// <summary>
        /// Creates a dashboard parser
        /// </summary>
        /// <returns>Handle to the parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_dashboard_parser();

        /// <summary>
        /// Creates a groups parser
        /// </summary>
        /// <returns>Handle to the parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_groups_parser();

        /// <summary>
        /// Creates a hierarchy parser
        /// </summary>
        /// <returns>Handle to the parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_hierarchy_parser();

        /// <summary>
        /// Creates a module list parser
        /// </summary>
        /// <returns>Handle to the parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_modlist_parser();

        /// <summary>
        /// Creates an assert parser
        /// </summary>
        /// <returns>Handle to the parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_assert_parser();

        /// <summary>
        /// Destroys a parser
        /// </summary>
        /// <param name="parserHandle">Handle to the parser</param>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void destroy_parser(IntPtr parserHandle);

        #endregion

        #region Parsing Functions

        /// <summary>
        /// Parses a coverage file using the specified parser
        /// </summary>
        /// <param name="parserHandle">Handle to the parser</param>
        /// <param name="filename">Path to the file to parse</param>
        /// <param name="dbHandle">Handle to the database to store results</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        internal static extern int parse_coverage_file(IntPtr parserHandle, string filename, IntPtr dbHandle);

        #endregion

        #region Query Functions

        /// <summary>
        /// Gets the number of coverage groups in the database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>Number of groups</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_num_groups(IntPtr dbHandle);

        /// <summary>
        /// Gets the number of hierarchy instances in the database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>Number of hierarchy instances</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_num_hierarchy_instances(IntPtr dbHandle);

        /// <summary>
        /// Gets the number of modules in the database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>Number of modules</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_num_modules(IntPtr dbHandle);

        /// <summary>
        /// Gets the number of assertions in the database
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>Number of assertions</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_num_asserts(IntPtr dbHandle);

        #endregion

        #region Export Functions

        /// <summary>
        /// Exports coverage data to XML format
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <param name="filename">Output filename</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        internal static extern int export_coverage_to_xml(IntPtr dbHandle, string filename);

        /// <summary>
        /// Exports coverage data to JSON format
        /// </summary>
        /// <param name="dbHandle">Handle to the database</param>
        /// <param name="filename">Output filename</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        internal static extern int export_coverage_to_json(IntPtr dbHandle, string filename);

        #endregion

        #region High-Performance Parser Functions

        /// <summary>
        /// Performance statistics structure for high-performance parsers
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        internal struct PerformanceStats
        {
            public double ParseTimeSeconds;      // Total parsing time in seconds
            public uint FileSizeBytes;          // File size in bytes
            public uint LinesProcessed;         // Number of lines processed
            public uint GroupsParsed;           // Number of groups parsed
            public uint MemoryAllocated;        // Memory allocated in bytes
            public uint ThreadsUsed;            // Number of threads used
            public double ThroughputMBPerSec;   // Throughput in MB/s
        }

        /// <summary>
        /// Creates a high-performance groups parser for huge files
        /// </summary>
        /// <returns>Handle to the high-performance parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_high_performance_groups_parser();

        /// <summary>
        /// Creates a high-performance hierarchy parser for huge files
        /// </summary>
        /// <returns>Handle to the high-performance parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_high_performance_hierarchy_parser();

        /// <summary>
        /// Creates a high-performance assert parser for huge files
        /// </summary>
        /// <returns>Handle to the high-performance parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr create_high_performance_assert_parser();

        /// <summary>
        /// Parses a file using high-performance optimizations
        /// </summary>
        /// <param name="parserHandle">Handle to the high-performance parser</param>
        /// <param name="filename">Path to the file to parse</param>
        /// <param name="dbHandle">Handle to the database</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        internal static extern int parse_coverage_file_high_performance(IntPtr parserHandle, string filename, IntPtr dbHandle);

        /// <summary>
        /// Gets performance statistics from the last parse operation
        /// </summary>
        /// <param name="parserHandle">Handle to the high-performance parser</param>
        /// <param name="stats">Output structure for performance statistics</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_performance_stats(IntPtr parserHandle, out PerformanceStats stats);

        /// <summary>
        /// Auto-selects optimal parser based on file size and type
        /// </summary>
        /// <param name="filename">Path to the file to analyze</param>
        /// <param name="parserType">Type of parser ("groups", "hierarchy", "assert")</param>
        /// <returns>Handle to the optimal parser</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        internal static extern IntPtr create_optimal_parser(string filename, string parserType);

        #endregion

        #region Memory Management Functions

        /// <summary>
        /// Gets current memory usage statistics
        /// </summary>
        /// <param name="totalBytes">Output: total bytes allocated</param>
        /// <param name="numAllocations">Output: number of allocations</param>
        /// <returns>0 on success, error code on failure</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int get_memory_usage(out uint totalBytes, out uint numAllocations);

        /// <summary>
        /// Cleans up library resources
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void cleanup_library();

        #endregion

        #region Helper Methods

        /// <summary>
        /// Converts a native string pointer to a managed string
        /// </summary>
        /// <param name="ptr">Pointer to native string</param>
        /// <returns>Managed string or null if pointer is invalid</returns>
        internal static string? PtrToString(IntPtr ptr)
        {
            return ptr != IntPtr.Zero ? Marshal.PtrToStringAnsi(ptr) : null;
        }

        /// <summary>
        /// Checks if a handle is valid (non-zero)
        /// </summary>
        /// <param name="handle">Handle to check</param>
        /// <returns>True if handle is valid</returns>
        internal static bool IsValidHandle(IntPtr handle)
        {
            return handle != IntPtr.Zero;
        }

        #endregion
    }
}