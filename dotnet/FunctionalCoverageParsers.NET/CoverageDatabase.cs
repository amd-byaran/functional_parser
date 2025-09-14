using System;
using System.Diagnostics;
using System.IO;
using FunctionalCoverageParsers.Native;

namespace FunctionalCoverageParsers
{
    /// <summary>
    /// Managed wrapper for the native coverage database
    /// Provides .NET-friendly interface for working with EDA coverage data
    /// </summary>
    public sealed class CoverageDatabase : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed;

        /// <summary>
        /// Gets a value indicating whether this database instance is valid
        /// </summary>
        public bool IsValid => _handle != IntPtr.Zero && !_disposed;

        /// <summary>
        /// Gets the native handle for this database (for internal use)
        /// </summary>
        internal IntPtr Handle => _handle;

        /// <summary>
        /// Gets the version string of the native library
        /// </summary>
        public static string Version
        {
            get
            {
                var ptr = NativeMethods.get_version_string();
                return NativeMethods.PtrToString(ptr) ?? "Unknown";
            }
        }

        /// <summary>
        /// Gets the library information string
        /// </summary>
        public static string LibraryInfo
        {
            get
            {
                var ptr = NativeMethods.get_library_info();
                return NativeMethods.PtrToString(ptr) ?? "Unknown";
            }
        }

        /// <summary>
        /// Gets the number of coverage groups in the database
        /// </summary>
        public int GroupCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.get_num_groups(_handle);
            }
        }

        /// <summary>
        /// Gets the number of hierarchy instances in the database
        /// </summary>
        public int HierarchyInstanceCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.get_num_hierarchy_instances(_handle);
            }
        }

        /// <summary>
        /// Gets the number of modules in the database
        /// </summary>
        public int ModuleCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.get_num_modules(_handle);
            }
        }

        /// <summary>
        /// Gets the number of assertions in the database
        /// </summary>
        public int AssertCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.get_num_asserts(_handle);
            }
        }

        /// <summary>
        /// Gets the overall coverage score as a percentage
        /// </summary>
        public double OverallScore
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.calculate_overall_score(_handle);
            }
        }

        /// <summary>
        /// Initializes a new instance of the CoverageDatabase class
        /// </summary>
        public CoverageDatabase()
        {
            _handle = NativeMethods.create_coverage_database();
            if (_handle == IntPtr.Zero)
            {
                throw new CoverageMemoryException("Failed to create coverage database");
            }
        }

        /// <summary>
        /// Validates the database structure and data integrity
        /// </summary>
        /// <returns>True if the database is valid</returns>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public bool Validate()
        {
            ThrowIfDisposed();
            int result = NativeMethods.validate_database(_handle);
            return result == 0;
        }

        /// <summary>
        /// Parses a dashboard coverage file
        /// </summary>
        /// <param name="filename">Path to the dashboard.txt file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="InvalidCoverageFormatException">Thrown when the file format is invalid</exception>
        /// <exception cref="ParseFailedException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ParseDashboard(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            using var parser = new DashboardParser();
            ParseWithParser(parser, filename);
        }

        /// <summary>
        /// Parses a groups coverage file
        /// </summary>
        /// <param name="filename">Path to the groups.txt file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="InvalidCoverageFormatException">Thrown when the file format is invalid</exception>
        /// <exception cref="ParseFailedException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ParseGroups(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            using var parser = new GroupsParser();
            ParseWithParser(parser, filename);
        }

        /// <summary>
        /// Parses a hierarchy coverage file
        /// </summary>
        /// <param name="filename">Path to the hierarchy.txt file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="InvalidCoverageFormatException">Thrown when the file format is invalid</exception>
        /// <exception cref="ParseFailedException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ParseHierarchy(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            using var parser = new HierarchyParser();
            ParseWithParser(parser, filename);
        }

        /// <summary>
        /// Parses a module list coverage file
        /// </summary>
        /// <param name="filename">Path to the modlist.txt file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="InvalidCoverageFormatException">Thrown when the file format is invalid</exception>
        /// <exception cref="ParseFailedException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ParseModuleList(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            using var parser = new ModuleListParser();
            ParseWithParser(parser, filename);
        }

        /// <summary>
        /// Parses an assertions coverage file
        /// </summary>
        /// <param name="filename">Path to the asserts.txt file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="InvalidCoverageFormatException">Thrown when the file format is invalid</exception>
        /// <exception cref="ParseFailedException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ParseAssertions(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            using var parser = new AssertParser();
            ParseWithParser(parser, filename);
        }

        #region High-Performance Parsing Methods

        /// <summary>
        /// Parses a groups coverage file with high-performance optimizations
        /// 
        /// Uses advanced optimization techniques for huge files:
        /// - Memory-mapped file I/O for zero-copy access
        /// - SIMD vectorized string operations
        /// - Parallel processing across multiple CPU cores
        /// - Custom memory allocators to reduce overhead
        /// 
        /// Recommended for files larger than 10MB.
        /// </summary>
        /// <param name="filename">Path to the groups.txt file</param>
        /// <returns>Performance statistics from the parsing operation</returns>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="CoverageParserException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public PerformanceStatistics ParseGroupsHighPerformance(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            try
            {
                using var parser = HighPerformanceParser.CreateOptimal(filename, "groups");
                if (!parser.Parse(filename, this))
                    throw new ParseFailedException($"High-performance parsing of groups file failed: {filename}");

                return parser.LastPerformanceStats ?? 
                    throw new CoverageParserException("Performance statistics not available");
            }
            catch (System.EntryPointNotFoundException)
            {
                // High-performance parser not available, fall back to standard parser
                var stopwatch = Stopwatch.StartNew();
                var initialMemory = GC.GetTotalMemory(false);
                
                ParseGroups(filename);
                
                stopwatch.Stop();
                var finalMemory = GC.GetTotalMemory(false);
                
                return new PerformanceStatistics
                {
                    ParseTimeSeconds = stopwatch.Elapsed.TotalSeconds,
                    FileSizeBytes = GetFileSize(filename),
                    MemoryAllocated = Math.Max(0, finalMemory - initialMemory),
                    ThreadsUsed = 1,
                    ItemsParsed = 0, // Can't determine count easily
                    LinesProcessed = 0,
                    ThroughputMBPerSec = CalculateThroughput(GetFileSize(filename), stopwatch.Elapsed.TotalSeconds)
                };
            }
        }

        /// <summary>
        /// Parses a hierarchy coverage file with high-performance optimizations
        /// </summary>
        /// <param name="filename">Path to the hierarchy.txt file</param>
        /// <returns>Performance statistics from the parsing operation</returns>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="CoverageParserException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public PerformanceStatistics ParseHierarchyHighPerformance(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            try
            {
                using var parser = HighPerformanceParser.CreateOptimal(filename, "hierarchy");
                if (!parser.Parse(filename, this))
                    throw new ParseFailedException($"High-performance parsing of hierarchy file failed: {filename}");

                return parser.LastPerformanceStats ?? 
                    throw new CoverageParserException("Performance statistics not available");
            }
            catch (System.EntryPointNotFoundException)
            {
                // High-performance parser not available, fall back to standard parser
                var stopwatch = Stopwatch.StartNew();
                var initialMemory = GC.GetTotalMemory(false);
                
                ParseHierarchy(filename);
                
                stopwatch.Stop();
                var finalMemory = GC.GetTotalMemory(false);
                
                return new PerformanceStatistics
                {
                    ParseTimeSeconds = stopwatch.Elapsed.TotalSeconds,
                    FileSizeBytes = GetFileSize(filename),
                    MemoryAllocated = Math.Max(0, finalMemory - initialMemory),
                    ThreadsUsed = 1,
                    ItemsParsed = 0, // Can't determine count easily
                    LinesProcessed = 0,
                    ThroughputMBPerSec = CalculateThroughput(GetFileSize(filename), stopwatch.Elapsed.TotalSeconds)
                };
            }
        }

        /// <summary>
        /// Parses an assertions coverage file with high-performance optimizations
        /// </summary>
        /// <param name="filename">Path to the asserts.txt file</param>
        /// <returns>Performance statistics from the parsing operation</returns>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageFileNotFoundException">Thrown when the file is not found</exception>
        /// <exception cref="CoverageParserException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public PerformanceStatistics ParseAssertionsHighPerformance(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            if (!File.Exists(filename))
                throw new CoverageFileNotFoundException(filename);

            try
            {
                using var parser = HighPerformanceParser.CreateOptimal(filename, "assert");
                if (!parser.Parse(filename, this))
                    throw new ParseFailedException($"High-performance parsing of assertions file failed: {filename}");

                return parser.LastPerformanceStats ?? 
                    throw new CoverageParserException("Performance statistics not available");
            }
            catch (System.EntryPointNotFoundException)
            {
                // High-performance parser not available, fall back to standard parser
                var stopwatch = Stopwatch.StartNew();
                var initialMemory = GC.GetTotalMemory(false);
                
                ParseAssertions(filename);
                
                stopwatch.Stop();
                var finalMemory = GC.GetTotalMemory(false);
                
                return new PerformanceStatistics
                {
                    ParseTimeSeconds = stopwatch.Elapsed.TotalSeconds,
                    FileSizeBytes = GetFileSize(filename),
                    MemoryAllocated = Math.Max(0, finalMemory - initialMemory),
                    ThreadsUsed = 1,
                    ItemsParsed = 0, // Can't determine count easily
                    LinesProcessed = 0,
                    ThroughputMBPerSec = CalculateThroughput(GetFileSize(filename), stopwatch.Elapsed.TotalSeconds)
                };
            }
        }

        /// <summary>
        /// Parses multiple coverage files in parallel with high-performance optimizations
        /// </summary>
        /// <param name="groupsFile">Path to groups.txt file (optional)</param>
        /// <param name="hierarchyFile">Path to hierarchy.txt file (optional)</param>
        /// <param name="assertsFile">Path to asserts.txt file (optional)</param>
        /// <returns>Combined performance statistics from all parsing operations</returns>
        /// <exception cref="CoverageFileNotFoundException">Thrown when a specified file is not found</exception>
        /// <exception cref="CoverageParserException">Thrown when parsing fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public PerformanceStatistics ParseMultipleFilesHighPerformance(
            string? groupsFile = null, 
            string? hierarchyFile = null, 
            string? assertsFile = null)
        {
            ThrowIfDisposed();

            var totalStats = new PerformanceStatistics();
            double totalTime = 0;
            long totalSize = 0;
            long totalItems = 0;
            long totalLines = 0;
            long totalMemory = 0;
            int maxThreads = 0;

            if (!string.IsNullOrEmpty(groupsFile))
            {
                var stats = ParseGroupsHighPerformance(groupsFile!);
                totalTime += stats.ParseTimeSeconds;
                totalSize += stats.FileSizeBytes;
                totalItems += stats.ItemsParsed;
                totalLines += stats.LinesProcessed;
                totalMemory += stats.MemoryAllocated;
                maxThreads = Math.Max(maxThreads, stats.ThreadsUsed);
            }

            if (!string.IsNullOrEmpty(hierarchyFile))
            {
                var stats = ParseHierarchyHighPerformance(hierarchyFile!);
                totalTime += stats.ParseTimeSeconds;
                totalSize += stats.FileSizeBytes;
                totalItems += stats.ItemsParsed;
                totalLines += stats.LinesProcessed;
                totalMemory += stats.MemoryAllocated;
                maxThreads = Math.Max(maxThreads, stats.ThreadsUsed);
            }

            if (!string.IsNullOrEmpty(assertsFile))
            {
                var stats = ParseAssertionsHighPerformance(assertsFile!);
                totalTime += stats.ParseTimeSeconds;
                totalSize += stats.FileSizeBytes;
                totalItems += stats.ItemsParsed;
                totalLines += stats.LinesProcessed;
                totalMemory += stats.MemoryAllocated;
                maxThreads = Math.Max(maxThreads, stats.ThreadsUsed);
            }

            totalStats.ParseTimeSeconds = totalTime;
            totalStats.FileSizeBytes = totalSize;
            totalStats.ItemsParsed = totalItems;
            totalStats.LinesProcessed = totalLines;
            totalStats.MemoryAllocated = totalMemory;
            totalStats.ThreadsUsed = maxThreads;
            
            if (totalTime > 0)
                totalStats.ThroughputMBPerSec = (totalSize / (1024.0 * 1024.0)) / totalTime;

            return totalStats;
        }

        #endregion

        /// <summary>
        /// Exports coverage data to XML format
        /// </summary>
        /// <param name="filename">Output filename for the XML file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageParserException">Thrown when export fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ExportToXml(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            int result = NativeMethods.export_coverage_to_xml(_handle, filename);
            if (result != 0)
            {
                string error = GetErrorString(result);
                throw new CoverageParserException(result, $"Failed to export to XML: {error}");
            }
        }

        /// <summary>
        /// Exports coverage data to JSON format
        /// </summary>
        /// <param name="filename">Output filename for the JSON file</param>
        /// <exception cref="ArgumentNullException">Thrown when filename is null</exception>
        /// <exception cref="CoverageParserException">Thrown when export fails</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the database has been disposed</exception>
        public void ExportToJson(string filename)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            
            ThrowIfDisposed();
            
            int result = NativeMethods.export_coverage_to_json(_handle, filename);
            if (result != 0)
            {
                string error = GetErrorString(result);
                throw new CoverageParserException(result, $"Failed to export to JSON: {error}");
            }
        }

        /// <summary>
        /// Gets memory usage statistics for the native library
        /// </summary>
        /// <returns>A tuple containing total bytes allocated and number of allocations</returns>
        public static (uint TotalBytes, uint NumAllocations) GetMemoryUsage()
        {
            int result = NativeMethods.get_memory_usage(out uint totalBytes, out uint numAllocations);
            if (result != 0)
            {
                return (0, 0);
            }
            return (totalBytes, numAllocations);
        }

        /// <summary>
        /// Cleans up all library resources (should be called at application shutdown)
        /// </summary>
        public static void CleanupLibrary()
        {
            NativeMethods.cleanup_library();
        }

        #region Private Methods

        private void ParseWithParser(IDisposable parser, string filename)
        {
            var parserHandle = (parser as ICoverageParser)?.Handle ?? IntPtr.Zero;
            if (parserHandle == IntPtr.Zero)
                throw new InvalidOperationException("Invalid parser handle");

            // Get memory usage before parsing
            var (initialBytes, initialAllocs) = GetMemoryUsage();

            int result = NativeMethods.parse_coverage_file(parserHandle, filename, _handle);
            if (result != 0)
            {
                // Get memory usage after failed parsing
                var (finalBytes, finalAllocs) = GetMemoryUsage();
                string error = GetErrorString(result);
                
                var fileInfo = new FileInfo(filename);
                string diagnostics = $"File size: {fileInfo.Length:N0} bytes, " +
                                   $"Memory before: {initialBytes:N0} bytes ({initialAllocs} allocs), " +
                                   $"Memory after: {finalBytes:N0} bytes ({finalAllocs} allocs)";
                
                throw result switch
                {
                    1 => new CoverageFileNotFoundException(filename),
                    2 => new InvalidCoverageFormatException(filename, $"{error} [{diagnostics}]"),
                    3 => new CoverageMemoryException($"Memory allocation failed while parsing {filename}. {diagnostics}"),
                    4 => new InvalidParameterException("filename", $"{error} [{diagnostics}]"),
                    5 => new ParseFailedException(filename, $"{error} [{diagnostics}]"),
                    _ => new CoverageParserException(result, $"Parse failed for {filename}: {error} [{diagnostics}]")
                };
            }
        }

        private static string GetErrorString(int errorCode)
        {
            var ptr = NativeMethods.get_error_string(errorCode);
            return NativeMethods.PtrToString(ptr) ?? $"Unknown error code: {errorCode}";
        }

        private static long GetFileSize(string filename)
        {
            try
            {
                return new FileInfo(filename).Length;
            }
            catch
            {
                return 0;
            }
        }

        private static double CalculateThroughput(long fileSizeBytes, double timeSeconds)
        {
            if (timeSeconds <= 0 || fileSizeBytes <= 0)
                return 0.0;
            
            return (fileSizeBytes / (1024.0 * 1024.0)) / timeSeconds;
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(CoverageDatabase));
        }

        #endregion

        #region IDisposable Implementation

        /// <summary>
        /// Releases all resources used by the CoverageDatabase
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!_disposed && _handle != IntPtr.Zero)
            {
                NativeMethods.destroy_coverage_database(_handle);
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer for the CoverageDatabase
        /// </summary>
        ~CoverageDatabase()
        {
            Dispose(false);
        }

        #endregion
    }
}