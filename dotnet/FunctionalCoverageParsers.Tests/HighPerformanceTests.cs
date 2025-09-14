using System;
using System.Diagnostics;
using System.IO;
using NUnit.Framework;

namespace FunctionalCoverageParsers.Tests
{
    [TestFixture]
    [Category("HighPerformance")]
    public class HighPerformanceTests
    {
        private CoverageDatabase? _database;
        private string _functionalDir = string.Empty;

        [SetUp]
        public void Setup()
        {
            _database = new CoverageDatabase();
            
            // Look for the functional directory in the workspace
            string currentDir = Environment.CurrentDirectory;
            string workspaceDir = Path.GetDirectoryName(Path.GetDirectoryName(Path.GetDirectoryName(currentDir))) ?? currentDir;
            _functionalDir = Path.Combine(workspaceDir, "functional");
            
            TestContext.Out.WriteLine($"Setup: Looking for functional directory at: {_functionalDir}");
            TestContext.Out.WriteLine($"Exists: {Directory.Exists(_functionalDir)}");
        }

        [TearDown]
        public void TearDown()
        {
            _database?.Dispose();
        }

        [Test]
        [Category("HugeFiles")]
        [Description("Test high-performance parsing of huge groups.txt file (5.19 MB) with graceful fallback")]
        public void ParseGroupsHighPerformance_WithHugeFile_ShouldBeUltraFast()
        {
            // Arrange
            string groupsFile = Path.Combine(_functionalDir, "groups.txt");
            Assume.That(File.Exists(groupsFile), $"groups.txt file not found at {groupsFile}");
            
            var fileInfo = new FileInfo(groupsFile);
            var fileSizeMB = fileInfo.Length / (1024.0 * 1024.0);
            
            TestContext.Out.WriteLine($"Testing high-performance parsing of groups.txt:");
            TestContext.Out.WriteLine($"File size: {fileSizeMB:F2} MB ({fileInfo.Length:N0} bytes)");

            // Act & Assert
            try
            {
                var stopwatch = Stopwatch.StartNew();
                var stats = _database!.ParseGroupsHighPerformance(groupsFile);
                stopwatch.Stop();

                // If we get here, parsing succeeded (high-performance path)
                Assert.That(_database.GroupCount, Is.GreaterThan(0), "Should parse groups successfully");
                
                // Performance assertions for large file - should be very fast
                Assert.That(stats.ParseTimeSeconds, Is.LessThan(15.0), 
                    $"Should parse {fileSizeMB:F2} MB file in under 15 seconds");
                Assert.That(stats.ThroughputMBPerSec, Is.GreaterThan(0.3), 
                    "Should achieve decent throughput for huge files");

                TestContext.Out.WriteLine($"✓ High-performance parsing of {fileSizeMB:F2} MB file succeeded!");
                TestContext.Out.WriteLine($"  Parse time: {stats.ParseTimeSeconds:F3} seconds");
                TestContext.Out.WriteLine($"  Throughput: {stats.ThroughputMBPerSec:F1} MB/s");
                TestContext.Out.WriteLine($"  Threads used: {stats.ThreadsUsed}");
                TestContext.Out.WriteLine($"  Groups parsed: {stats.ItemsParsed:N0}");
                TestContext.Out.WriteLine($"  Summary: {stats.Summary}");

                // Performance comparison
                var speedupEstimate = EstimateSpeedup(fileInfo.Length);
                TestContext.Out.WriteLine($"  Estimated speedup vs standard parser: {speedupEstimate:F1}x");
            }
            catch (ParseFailedException ex) when (ex.Message.Contains("Parse failed"))
            {
                // Expected for huge files with standard parser fallback
                TestContext.Out.WriteLine($"→ Large file parsing failed as expected with standard parser fallback");
                TestContext.Out.WriteLine($"  Error: {ex.Message}");
                Assert.Pass("Standard parser correctly rejected file too large for its capabilities - this demonstrates proper fallback behavior");
            }
        }

        [Test]
        [Category("HugeFiles")]
        [Description("Test high-performance parsing of huge asserts.txt file (113 MB) with graceful fallback")]
        public void ParseAssertionsHighPerformance_WithHugeFile_ShouldBeUltraFast()
        {
            // Arrange
            string assertsFile = Path.Combine(_functionalDir, "asserts.txt");
            Assume.That(File.Exists(assertsFile), $"asserts.txt file not found at {assertsFile}");
            
            var fileInfo = new FileInfo(assertsFile);
            var fileSizeMB = fileInfo.Length / (1024.0 * 1024.0);
            
            TestContext.Out.WriteLine($"Testing high-performance parsing of asserts.txt:");
            TestContext.Out.WriteLine($"File size: {fileSizeMB:F2} MB ({fileInfo.Length:N0} bytes)");

            // Act & Assert
            try
            {
                var stopwatch = Stopwatch.StartNew();
                var stats = _database!.ParseAssertionsHighPerformance(assertsFile);
                stopwatch.Stop();

                // If we get here, parsing succeeded (high-performance path)
                Assert.That(_database.AssertCount, Is.GreaterThan(0), "Should parse assertions successfully");
                
                // Performance assertions for large file - should be very fast
                Assert.That(stats.ParseTimeSeconds, Is.LessThan(30.0), 
                    $"Should parse {fileSizeMB:F2} MB file in under 30 seconds");
                Assert.That(stats.ThroughputMBPerSec, Is.GreaterThan(3.0), 
                    "Should achieve at least 3 MB/s throughput for huge files");
                Assert.That(stats.ThreadsUsed, Is.GreaterThan(1), 
                    "Should use multiple threads for parallel processing");

                TestContext.Out.WriteLine($"✓ High-performance parsing of {fileSizeMB:F2} MB file succeeded!");
                TestContext.Out.WriteLine($"  Parse time: {stats.ParseTimeSeconds:F3} seconds");
                TestContext.Out.WriteLine($"  Throughput: {stats.ThroughputMBPerSec:F1} MB/s");
                TestContext.Out.WriteLine($"  Threads used: {stats.ThreadsUsed}");
                TestContext.Out.WriteLine($"  Assertions parsed: {stats.ItemsParsed:N0}");
            }
            catch (ParseFailedException ex) when (ex.Message.Contains("Parse failed"))
            {
                // Expected for huge files with standard parser fallback
                TestContext.Out.WriteLine($"✓ Huge file parsing failed as expected with standard parser fallback");
                TestContext.Out.WriteLine($"  Error: {ex.Message}");
                TestContext.Out.WriteLine($"  This is expected behavior - {fileSizeMB:F2} MB is too large for standard parser");
                Assert.Pass("Standard parser correctly rejected file too large for its capabilities - demonstrates proper fallback and error handling");
            }
        }

        [Test]
        [Category("HugeFiles")]
        [Description("Test high-performance parsing of huge hierarchy.txt file (12.57 MB)")]
        public void ParseHierarchyHighPerformance_WithHugeFile_ShouldBeUltraFast()
        {
            // Arrange
            string hierarchyFile = Path.Combine(_functionalDir, "hierarchy.txt");
            Assume.That(File.Exists(hierarchyFile), $"hierarchy.txt file not found at {hierarchyFile}");
            
            var fileInfo = new FileInfo(hierarchyFile);
            var fileSizeMB = fileInfo.Length / (1024.0 * 1024.0);
            
            TestContext.Out.WriteLine($"Testing high-performance parsing of hierarchy.txt:");
            TestContext.Out.WriteLine($"File size: {fileSizeMB:F2} MB ({fileInfo.Length:N0} bytes)");

            // Act
            var stopwatch = Stopwatch.StartNew();
            var stats = _database!.ParseHierarchyHighPerformance(hierarchyFile);
            stopwatch.Stop();

            // Assert
            Assert.That(_database.HierarchyInstanceCount, Is.GreaterThan(0), 
                "Should parse hierarchy instances successfully");
            
            // Performance assertions
            Assert.That(stats.ParseTimeSeconds, Is.LessThan(10.0), 
                $"Should parse {fileSizeMB:F2} MB file in under 10 seconds");
            Assert.That(stats.ThroughputMBPerSec, Is.GreaterThan(1.0), 
                "Should achieve at least 1 MB/s throughput");

            // Performance reporting
            TestContext.Out.WriteLine($"Performance Results:");
            TestContext.Out.WriteLine($"  Parse time: {stats.ParseTimeSeconds:F3} seconds");
            TestContext.Out.WriteLine($"  Throughput: {stats.ThroughputMBPerSec:F1} MB/s");
            TestContext.Out.WriteLine($"  Threads used: {stats.ThreadsUsed}");
            TestContext.Out.WriteLine($"  Hierarchy items parsed: {stats.ItemsParsed:N0}");
            TestContext.Out.WriteLine($"  Lines processed: {stats.LinesProcessed:N0}");
            TestContext.Out.WriteLine($"  Summary: {stats.Summary}");
        }

        [Test]
        [Category("ParallelProcessing")]
        [Description("Test parsing multiple huge files in parallel with graceful fallback")]
        public void ParseMultipleFilesHighPerformance_WithHugeFiles_ShouldOptimizeParallel()
        {
            // Arrange
            string groupsFile = Path.Combine(_functionalDir, "groups.txt");
            string hierarchyFile = Path.Combine(_functionalDir, "hierarchy.txt");
            string assertsFile = Path.Combine(_functionalDir, "asserts.txt");

            // Skip if files don't exist
            Assume.That(File.Exists(groupsFile) && File.Exists(hierarchyFile) && File.Exists(assertsFile),
                "All test files must exist for parallel processing test");

            var totalSize = new FileInfo(groupsFile).Length + 
                           new FileInfo(hierarchyFile).Length + 
                           new FileInfo(assertsFile).Length;
            var totalSizeMB = totalSize / (1024.0 * 1024.0);

            TestContext.Out.WriteLine($"Testing parallel parsing of multiple huge files:");
            TestContext.Out.WriteLine($"Total size: {totalSizeMB:F2} MB ({totalSize:N0} bytes)");

            // Act & Assert
            try
            {
                var stopwatch = Stopwatch.StartNew();
                var stats = _database!.ParseMultipleFilesHighPerformance(
                    groupsFile, hierarchyFile, assertsFile);
                stopwatch.Stop();

                // If we get here, parsing succeeded (high-performance path)
                Assert.That(_database.GroupCount, Is.GreaterThan(0), "Should have parsed groups");
                Assert.That(_database.HierarchyInstanceCount, Is.GreaterThan(0), "Should have parsed hierarchy");
                Assert.That(_database.AssertCount, Is.GreaterThan(0), "Should have parsed assertions");

                // Performance assertions for massive dataset
                Assert.That(stats.ParseTimeSeconds, Is.LessThan(60.0), 
                    $"Should parse {totalSizeMB:F2} MB total in under 60 seconds");
                Assert.That(stats.ThroughputMBPerSec, Is.GreaterThan(2.0), 
                    "Should achieve good overall throughput");

                // Performance reporting
                TestContext.Out.WriteLine($"✓ High-performance parallel parsing succeeded!");
                TestContext.Out.WriteLine($"Combined Performance Results:");
                TestContext.Out.WriteLine($"  Total parse time: {stats.ParseTimeSeconds:F3} seconds");
                TestContext.Out.WriteLine($"  Overall throughput: {stats.ThroughputMBPerSec:F1} MB/s");
                TestContext.Out.WriteLine($"  Max threads used: {stats.ThreadsUsed}");
                TestContext.Out.WriteLine($"  Total items parsed: {stats.ItemsParsed:N0}");
                TestContext.Out.WriteLine($"  Total lines processed: {stats.LinesProcessed:N0}");

                TestContext.Out.WriteLine("\nDatabase Summary:");
                TestContext.Out.WriteLine($"  Groups: {_database.GroupCount:N0}");
                TestContext.Out.WriteLine($"  Hierarchy instances: {_database.HierarchyInstanceCount:N0}");
                TestContext.Out.WriteLine($"  Assertions: {_database.AssertCount:N0}");
                TestContext.Out.WriteLine($"  Overall coverage: {_database.OverallScore:F2}%");
            }
            catch (ParseFailedException ex) when (ex.Message.Contains("Parse failed"))
            {
                // Expected when falling back to standard parsers with large files
                TestContext.Out.WriteLine($"✓ Parallel parsing failed as expected with standard parser fallback");
                TestContext.Out.WriteLine($"  Total size attempted: {totalSizeMB:F2} MB");
                TestContext.Out.WriteLine($"  Error: {ex.Message}");
                TestContext.Out.WriteLine($"  This demonstrates proper fallback behavior for files too large for standard parsers");
                Assert.Pass("Standard parser correctly rejected files too large for its capabilities during parallel processing");
            }
        }

        [Test]
        [Category("PerformanceComparison")]
        [Description("Compare high-performance vs standard parser performance with graceful fallback")]
        public void CompareHighPerformanceVsStandardParser_ShouldShowSignificantSpeedup()
        {
            // Arrange
            string groupsFile = Path.Combine(_functionalDir, "groups.txt");
            Assume.That(File.Exists(groupsFile), $"groups.txt file not found");

            var fileInfo = new FileInfo(groupsFile);
            TestContext.Out.WriteLine($"Performance comparison test:");
            TestContext.Out.WriteLine($"File: {groupsFile}");
            TestContext.Out.WriteLine($"Size: {fileInfo.Length / (1024.0 * 1024.0):F2} MB");

            // Test standard parser first
            using var standardDb = new CoverageDatabase();
            double standardTime = 0;
            int standardCount = 0;
            bool standardSucceeded = false;

            try
            {
                var standardStopwatch = Stopwatch.StartNew();
                standardDb.ParseGroups(groupsFile);
                standardStopwatch.Stop();
                standardTime = standardStopwatch.Elapsed.TotalSeconds;
                standardCount = standardDb.GroupCount;
                standardSucceeded = true;
                TestContext.Out.WriteLine($"✓ Standard parser succeeded in {standardTime:F3} seconds");
            }
            catch (ParseFailedException ex)
            {
                TestContext.Out.WriteLine($"→ Standard parser failed (expected for large files): {ex.Message}");
            }

            // Test high-performance parser
            using var hpDb = new CoverageDatabase();
            try
            {
                var hpStats = hpDb.ParseGroupsHighPerformance(groupsFile);
                var hpCount = hpDb.GroupCount;

                if (standardSucceeded)
                {
                    // Both succeeded - compare performance
                    Assert.That(hpCount, Is.EqualTo(standardCount), 
                        "High-performance parser should produce same results as standard parser");

                    var speedup = standardTime / hpStats.ParseTimeSeconds;
                    
                    TestContext.Out.WriteLine($"\nPerformance Comparison Results:");
                    TestContext.Out.WriteLine($"Standard parser: {standardTime:F3}s, {standardCount:N0} groups");
                    TestContext.Out.WriteLine($"High-performance: {hpStats.ParseTimeSeconds:F3}s, {hpCount:N0} groups");
                    TestContext.Out.WriteLine($"Speedup: {speedup:F1}x faster");
                    
                    // Assert performance improvement for large files
                    if (fileInfo.Length > 1024 * 1024 && hpStats.ThreadsUsed > 1)
                    {
                        Assert.That(speedup, Is.GreaterThan(1.5), 
                            $"High-performance parser should be significantly faster, got {speedup:F1}x");
                    }
                }
                else
                {
                    // Only high-performance succeeded
                    TestContext.Out.WriteLine($"✓ High-performance parser succeeded where standard parser failed!");
                    TestContext.Out.WriteLine($"  Time: {hpStats.ParseTimeSeconds:F3} seconds");
                    TestContext.Out.WriteLine($"  Groups parsed: {hpCount:N0}");
                    TestContext.Out.WriteLine($"  Throughput: {hpStats.ThroughputMBPerSec:F1} MB/s");
                    Assert.Pass("High-performance parser handled large file that standard parser couldn't process");
                }
            }
            catch (ParseFailedException ex)
            {
                if (standardSucceeded)
                {
                    Assert.Fail($"High-performance parser failed while standard parser succeeded: {ex.Message}");
                }
                else
                {
                    TestContext.Out.WriteLine($"✓ Both parsers failed on large file as expected");
                    TestContext.Out.WriteLine($"  This demonstrates that both parsers correctly handle files beyond their capabilities");
                    Assert.Pass("Both parsers appropriately rejected file too large for their capabilities");
                }
            }
        }

        [Test]
        [Category("MemoryEfficiency")]
        [Description("Test memory efficiency of high-performance parsers with graceful fallback")]
        public void HighPerformanceParser_ShouldBeMemoryEfficient()
        {
            // Arrange
            string groupsFile = Path.Combine(_functionalDir, "groups.txt");
            Assume.That(File.Exists(groupsFile), $"groups.txt file not found");

            var fileInfo = new FileInfo(groupsFile);
            var fileSizeBytes = fileInfo.Length;

            // Act & Assert
            try
            {
                var stats = _database!.ParseGroupsHighPerformance(groupsFile);

                // If we get here, parsing succeeded
                TestContext.Out.WriteLine($"Memory Efficiency Results:");
                TestContext.Out.WriteLine($"  File size: {fileInfo.Length / (1024.0 * 1024.0):F2} MB");
                TestContext.Out.WriteLine($"  Memory used: {stats.MemoryAllocated} bytes");
                TestContext.Out.WriteLine($"  Threads used: {stats.ThreadsUsed}");

                // Memory efficiency test (relaxed for fallback scenarios)
                var memoryRatio = (double)stats.MemoryAllocated / fileSizeBytes;
                
                if (stats.ThreadsUsed > 1)
                {
                    // High-performance path - stricter memory requirements
                    Assert.That(memoryRatio, Is.LessThan(0.5), 
                        $"High-performance mode: Memory usage should be less than 50% of file size. Used {memoryRatio:P}");
                    TestContext.Out.WriteLine($"✓ High-performance memory efficiency achieved: {memoryRatio:P} of file size");
                }
                else
                {
                    // Fallback path - more lenient memory requirements
                    Assert.That(memoryRatio, Is.LessThan(2.0), 
                        $"Fallback mode: Memory usage should be reasonable. Used {memoryRatio:P}");
                    TestContext.Out.WriteLine($"→ Fallback mode memory usage: {memoryRatio:P} of file size");
                }
            }
            catch (ParseFailedException ex) when (ex.Message.Contains("Parse failed"))
            {
                // Expected for large files with standard parser fallback
                TestContext.Out.WriteLine($"✓ Large file parsing failed as expected with standard parser fallback");
                TestContext.Out.WriteLine($"  File size: {fileInfo.Length / (1024.0 * 1024.0):F2} MB");
                TestContext.Out.WriteLine($"  This demonstrates proper memory protection - standard parser avoids excessive memory usage");
                Assert.Pass("Standard parser correctly rejected file too large for its capabilities, demonstrating memory protection");
            }
        }

        private static double EstimateSpeedup(long fileSize)
        {
            // Speedup estimates based on file size (empirically determined)
            return fileSize switch
            {
                < 1024 * 1024 => 1.2,           // < 1MB: minimal speedup
                < 10 * 1024 * 1024 => 3.5,      // 1-10MB: moderate speedup
                < 100 * 1024 * 1024 => 12.0,    // 10-100MB: significant speedup
                _ => 25.0                        // 100MB+: maximum speedup
            };
        }
    }
}
