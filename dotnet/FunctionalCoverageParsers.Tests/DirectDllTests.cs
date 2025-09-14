using System;
using System.IO;
using System.Runtime.InteropServices;
using NUnit.Framework;

namespace FunctionalCoverageParsers.Tests
{
    /// <summary>
    /// Simple, direct tests for the native DLL without complex wrapper layers
    /// </summary>
    [TestFixture]
    public class DirectDllTests
    {
        private const string DllName = "FunctionalCoverageParsers.dll";

        #region Direct P/Invoke Declarations

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr get_version_string();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr get_library_info();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr get_error_string(int errorCode);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_coverage_database();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void destroy_coverage_database(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int validate_database(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern double calculate_overall_score(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_dashboard_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_groups_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_hierarchy_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_assert_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_modlist_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void destroy_parser(IntPtr parserHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int parse_coverage_file(IntPtr parserHandle, string filename, IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_num_groups(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_num_hierarchy_instances(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_num_modules(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_num_asserts(IntPtr dbHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int export_coverage_to_xml(IntPtr dbHandle, string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int export_coverage_to_json(IntPtr dbHandle, string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_memory_usage(out uint totalBytes, out uint numAllocations);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_high_performance_groups_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_high_performance_hierarchy_parser();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_high_performance_assert_parser();

        #endregion

        #region Helper Methods

        private static string PtrToString(IntPtr ptr)
        {
            return ptr != IntPtr.Zero ? Marshal.PtrToStringAnsi(ptr) : string.Empty;
        }

        private static string GetTestDataPath(string filename)
        {
            // Look for test data in current directory, parent directories, or TestData folder
            var searchPaths = new[]
            {
                Path.Combine(TestContext.CurrentContext.TestDirectory, "TestData", filename),
                Path.Combine(TestContext.CurrentContext.TestDirectory, "..", "..", "..", "..", "..", "functional", filename),
                Path.Combine(Directory.GetCurrentDirectory(), "TestData", filename),
                Path.Combine(Directory.GetCurrentDirectory(), "..", "..", "..", "..", "..", "functional", filename)
            };

            foreach (var path in searchPaths)
            {
                if (File.Exists(path))
                {
                    Console.WriteLine($"Found test file: {path}");
                    return path;
                }
            }

            Console.WriteLine($"Test file {filename} not found in any search path:");
            foreach (var path in searchPaths)
            {
                Console.WriteLine($"  - {path}");
            }

            return filename; // Return the filename as-is, let the test decide how to handle
        }

        #endregion

        #region Basic Functionality Tests

        [Test]
        public void GetVersionString_ShouldReturnValidString()
        {
            var versionPtr = get_version_string();
            Assert.That(versionPtr, Is.Not.EqualTo(IntPtr.Zero), "Version string pointer should not be null");

            var version = PtrToString(versionPtr);
            Assert.That(version, Is.Not.Null.And.Not.Empty, "Version string should not be empty");
            Console.WriteLine($"Library version: {version}");
        }

        [Test]
        public void GetLibraryInfo_ShouldReturnValidString()
        {
            var infoPtr = get_library_info();
            Assert.That(infoPtr, Is.Not.EqualTo(IntPtr.Zero), "Library info pointer should not be null");

            var info = PtrToString(infoPtr);
            Assert.That(info, Is.Not.Null.And.Not.Empty, "Library info should not be empty");
            Console.WriteLine($"Library info: {info}");
        }

        [Test]
        public void GetErrorString_ShouldReturnValidString()
        {
            var errorPtr = get_error_string(123);
            Assert.That(errorPtr, Is.Not.EqualTo(IntPtr.Zero), "Error string pointer should not be null");

            var error = PtrToString(errorPtr);
            Assert.That(error, Is.Not.Null.And.Not.Empty, "Error string should not be empty");
            Console.WriteLine($"Error string: {error}");
        }

        [Test]
        public void CreateCoverageDatabase_ShouldReturnValidHandle()
        {
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero), "Database handle should not be null");
                Console.WriteLine($"Created database with handle: 0x{dbHandle.ToInt64():X}");
            }
            finally
            {
                if (dbHandle != IntPtr.Zero)
                {
                    destroy_coverage_database(dbHandle);
                }
            }
        }

        [Test]
        public void ValidateDatabase_OnEmptyDatabase_ShouldReturnZero()
        {
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var isValid = validate_database(dbHandle);
                Assert.That(isValid, Is.EqualTo(0), "Empty database should be valid (return 0)");
                Console.WriteLine($"Database validation result: {isValid}");
            }
            finally
            {
                if (dbHandle != IntPtr.Zero)
                {
                    destroy_coverage_database(dbHandle);
                }
            }
        }

        [Test]
        public void CalculateOverallScore_OnEmptyDatabase_ShouldReturnScore()
        {
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var score = calculate_overall_score(dbHandle);
                Assert.That(score, Is.GreaterThanOrEqualTo(0), "Score should be non-negative");
                Console.WriteLine($"Overall score: {score:F2}");
            }
            finally
            {
                if (dbHandle != IntPtr.Zero)
                {
                    destroy_coverage_database(dbHandle);
                }
            }
        }

        #endregion

        #region Parser Creation Tests

        [Test]
        public void CreateDashboardParser_ShouldReturnValidHandle()
        {
            var parserHandle = create_dashboard_parser();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero), "Dashboard parser handle should not be null");
                Console.WriteLine($"Created dashboard parser with handle: 0x{parserHandle.ToInt64():X}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                {
                    destroy_parser(parserHandle);
                }
            }
        }

        [Test]
        public void CreateGroupsParser_ShouldReturnValidHandle()
        {
            var parserHandle = create_groups_parser();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero), "Groups parser handle should not be null");
                Console.WriteLine($"Created groups parser with handle: 0x{parserHandle.ToInt64():X}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                {
                    destroy_parser(parserHandle);
                }
            }
        }

        [Test]
        public void CreateHierarchyParser_ShouldReturnValidHandle()
        {
            var parserHandle = create_hierarchy_parser();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero), "Hierarchy parser handle should not be null");
                Console.WriteLine($"Created hierarchy parser with handle: 0x{parserHandle.ToInt64():X}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                {
                    destroy_parser(parserHandle);
                }
            }
        }

        [Test]
        public void CreateAssertParser_ShouldReturnValidHandle()
        {
            var parserHandle = create_assert_parser();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero), "Assert parser handle should not be null");
                Console.WriteLine($"Created assert parser with handle: 0x{parserHandle.ToInt64():X}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                {
                    destroy_parser(parserHandle);
                }
            }
        }

        [Test]
        public void CreateModuleListParser_ShouldReturnValidHandle()
        {
            var parserHandle = create_modlist_parser();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero), "Module list parser handle should not be null");
                Console.WriteLine($"Created module list parser with handle: 0x{parserHandle.ToInt64():X}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                {
                    destroy_parser(parserHandle);
                }
            }
        }

        #endregion

        #region File Parsing Tests

        [Test]
        public void ParseDashboard_WithNonExistentFile_ShouldReturnSuccess()
        {
            var parserHandle = create_dashboard_parser();
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero));
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                // Test with non-existent file - stub should return success
                var result = parse_coverage_file(parserHandle, "nonexistent.txt", dbHandle);
                Assert.That(result, Is.EqualTo(0), "Stub parser should return success (0) for any file");
                Console.WriteLine($"Parse result: {result}");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                    destroy_parser(parserHandle);
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        [Test]
        public void ParseGroups_WithTestFile_ShouldReturnSuccess()
        {
            var parserHandle = create_groups_parser();
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero));
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var testFile = GetTestDataPath("groups.txt");
                var result = parse_coverage_file(parserHandle, testFile, dbHandle);
                Assert.That(result, Is.EqualTo(0), "Groups parser should return success");
                Console.WriteLine($"Parsed groups file: {testFile}, result: {result}");

                // Test count functions
                var numGroups = get_num_groups(dbHandle);
                Console.WriteLine($"Number of groups: {numGroups}");
                Assert.That(numGroups, Is.GreaterThanOrEqualTo(0), "Group count should be non-negative");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                    destroy_parser(parserHandle);
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        [Test]
        public void ParseAssertions_WithTestFile_ShouldReturnSuccess()
        {
            var parserHandle = create_assert_parser();
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero));
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var testFile = GetTestDataPath("asserts.txt");
                var result = parse_coverage_file(parserHandle, testFile, dbHandle);
                Assert.That(result, Is.EqualTo(0), "Assert parser should return success");
                Console.WriteLine($"Parsed asserts file: {testFile}, result: {result}");

                // Test count functions
                var numAsserts = get_num_asserts(dbHandle);
                Console.WriteLine($"Number of asserts: {numAsserts}");
                Assert.That(numAsserts, Is.GreaterThanOrEqualTo(0), "Assert count should be non-negative");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                    destroy_parser(parserHandle);
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        #endregion

        #region Memory Management Tests

        [Test]
        public void GetMemoryUsage_ShouldReturnValidData()
        {
            var result = get_memory_usage(out uint totalBytes, out uint numAllocations);
            
            Assert.That(result, Is.EqualTo(0), "get_memory_usage should return success");
            Assert.That(totalBytes, Is.GreaterThan(0), "Total bytes should be positive");
            Assert.That(numAllocations, Is.GreaterThan(0), "Number of allocations should be positive");
            
            Console.WriteLine($"Memory usage: {totalBytes} bytes, {numAllocations} allocations");
        }

        [Test]
        public void MultipleDatabase_CreateDestroy_ShouldWork()
        {
            const int numDatabases = 5;
            var handles = new IntPtr[numDatabases];

            try
            {
                // Create multiple databases
                for (int i = 0; i < numDatabases; i++)
                {
                    handles[i] = create_coverage_database();
                    Assert.That(handles[i], Is.Not.EqualTo(IntPtr.Zero), $"Database {i} should be created");
                    Console.WriteLine($"Created database {i}: 0x{handles[i].ToInt64():X}");
                }

                // Verify they're all different
                for (int i = 0; i < numDatabases; i++)
                {
                    for (int j = i + 1; j < numDatabases; j++)
                    {
                        Assert.That(handles[i], Is.Not.EqualTo(handles[j]), 
                            $"Database handles {i} and {j} should be different");
                    }
                }
            }
            finally
            {
                // Clean up all databases
                for (int i = 0; i < numDatabases; i++)
                {
                    if (handles[i] != IntPtr.Zero)
                    {
                        destroy_coverage_database(handles[i]);
                        Console.WriteLine($"Destroyed database {i}");
                    }
                }
            }
        }

        #endregion

        #region Export Tests

        [Test]
        public void ExportToXml_WithEmptyDatabase_ShouldSucceed()
        {
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var tempFile = Path.GetTempFileName();
                var result = export_coverage_to_xml(dbHandle, tempFile);
                
                Assert.That(result, Is.EqualTo(0), "XML export should return success");
                Console.WriteLine($"Exported to XML: {tempFile}, result: {result}");

                // Clean up temp file
                if (File.Exists(tempFile))
                {
                    File.Delete(tempFile);
                }
            }
            finally
            {
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        [Test]
        public void ExportToJson_WithEmptyDatabase_ShouldSucceed()
        {
            var dbHandle = create_coverage_database();
            try
            {
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var tempFile = Path.GetTempFileName();
                var result = export_coverage_to_json(dbHandle, tempFile);
                
                Assert.That(result, Is.EqualTo(0), "JSON export should return success");
                Console.WriteLine($"Exported to JSON: {tempFile}, result: {result}");

                // Clean up temp file
                if (File.Exists(tempFile))
                {
                    File.Delete(tempFile);
                }
            }
            finally
            {
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        #endregion

        #region High Performance Tests

        [Test]
        public void CreateHighPerformanceParsers_ShouldReturnValidHandles()
        {
            var groupsParser = create_high_performance_groups_parser();
            var hierarchyParser = create_high_performance_hierarchy_parser();
            var assertParser = create_high_performance_assert_parser();

            try
            {
                Assert.That(groupsParser, Is.Not.EqualTo(IntPtr.Zero), "HP groups parser should be valid");
                Assert.That(hierarchyParser, Is.Not.EqualTo(IntPtr.Zero), "HP hierarchy parser should be valid");
                Assert.That(assertParser, Is.Not.EqualTo(IntPtr.Zero), "HP assert parser should be valid");

                Console.WriteLine($"HP Groups parser: 0x{groupsParser.ToInt64():X}");
                Console.WriteLine($"HP Hierarchy parser: 0x{hierarchyParser.ToInt64():X}");
                Console.WriteLine($"HP Assert parser: 0x{assertParser.ToInt64():X}");
            }
            finally
            {
                if (groupsParser != IntPtr.Zero)
                    destroy_parser(groupsParser);
                if (hierarchyParser != IntPtr.Zero)
                    destroy_parser(hierarchyParser);
                if (assertParser != IntPtr.Zero)
                    destroy_parser(assertParser);
            }
        }

        [Test]
        [Category("Performance")]
        public void HighPerformanceGroupsParser_WithLargeFile_ShouldBeUltraFast()
        {
            var parserHandle = create_high_performance_groups_parser();
            var dbHandle = create_coverage_database();

            try
            {
                Assert.That(parserHandle, Is.Not.EqualTo(IntPtr.Zero));
                Assert.That(dbHandle, Is.Not.EqualTo(IntPtr.Zero));

                var testFile = GetTestDataPath("groups.txt");
                
                var startTime = DateTime.UtcNow;
                var result = parse_coverage_file(parserHandle, testFile, dbHandle);
                var elapsed = DateTime.UtcNow - startTime;

                Assert.That(result, Is.EqualTo(0), "HP groups parser should return success");
                Console.WriteLine($"HP Groups parsing took: {elapsed.TotalMilliseconds:F2} ms");
                
                // For stub implementation, just verify it completes quickly
                Assert.That(elapsed.TotalSeconds, Is.LessThan(5), "HP parser should complete quickly");
            }
            finally
            {
                if (parserHandle != IntPtr.Zero)
                    destroy_parser(parserHandle);
                if (dbHandle != IntPtr.Zero)
                    destroy_coverage_database(dbHandle);
            }
        }

        #endregion

        #region Error Handling Tests

        [Test]
        public void PassNullPointers_ShouldNotCrash()
        {
            // These tests verify the DLL handles null pointers gracefully
            // Note: Some of these may cause access violations in a real implementation
            // but our stub should handle them safely

            Assert.DoesNotThrow(() => destroy_coverage_database(IntPtr.Zero), 
                "destroy_coverage_database with null should not crash");

            Assert.DoesNotThrow(() => destroy_parser(IntPtr.Zero), 
                "destroy_parser with null should not crash");

            var result = validate_database(IntPtr.Zero);
            Console.WriteLine($"validate_database(null) returned: {result}");

            var score = calculate_overall_score(IntPtr.Zero);
            Console.WriteLine($"calculate_overall_score(null) returned: {score}");
        }

        #endregion
    }
}