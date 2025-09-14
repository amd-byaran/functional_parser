using System;
using System.IO;
using NUnit.Framework;

namespace FunctionalCoverageParsers.Tests
{
    /// <summary>
    /// Tests for the basic functionality of the .NET wrapper
    /// </summary>
    [TestFixture]
    public class BasicFunctionalityTests
    {
        [Test]
        public void LibraryInfo_ShouldReturnValidString()
        {
            // Arrange & Act
            string version = CoverageDatabase.Version;
            string libraryInfo = CoverageDatabase.LibraryInfo;

            // Assert
            Assert.That(version, Is.Not.Null.And.Not.Empty, "Version should not be null or empty");
            Assert.That(libraryInfo, Is.Not.Null.And.Not.Empty, "Library info should not be null or empty");
            
            TestContext.Out.WriteLine($"Version: {version}");
            TestContext.Out.WriteLine($"Library Info: {libraryInfo}");
        }

        [Test]
        public void CreateCoverageDatabase_ShouldCreateValidInstance()
        {
            // Arrange & Act
            using var database = new CoverageDatabase();

            // Assert
            Assert.That(database.IsValid, Is.True, "Database should be valid after creation");
            Assert.That(database.GroupCount, Is.EqualTo(0), "New database should have zero groups");
            Assert.That(database.ModuleCount, Is.EqualTo(0), "New database should have zero modules");
            Assert.That(database.HierarchyInstanceCount, Is.EqualTo(0), "New database should have zero hierarchy instances");
            Assert.That(database.AssertCount, Is.EqualTo(0), "New database should have zero assertions");
        }

        [Test]
        public void ValidateDatabase_OnEmptyDatabase_ShouldReturnTrue()
        {
            // Arrange
            using var database = new CoverageDatabase();

            // Act
            bool isValid = database.Validate();

            // Assert
            Assert.That(isValid, Is.True, "Empty database should be valid");
        }

        [Test]
        public void OverallScore_OnEmptyDatabase_ShouldReturnZero()
        {
            // Arrange
            using var database = new CoverageDatabase();

            // Act
            double score = database.OverallScore;

            // Assert
            Assert.That(score, Is.EqualTo(0.0), "Empty database should have zero overall score");
        }

        [Test]
        public void Dispose_ShouldMakeDatabaseInvalid()
        {
            // Arrange
            var database = new CoverageDatabase();
            Assert.That(database.IsValid, Is.True, "Database should be valid before disposal");

            // Act
            database.Dispose();

            // Assert
            Assert.That(database.IsValid, Is.False, "Database should be invalid after disposal");
        }

        [Test]
        public void AccessAfterDispose_ShouldThrowObjectDisposedException()
        {
            // Arrange
            var database = new CoverageDatabase();
            database.Dispose();

            // Act & Assert
            Assert.Throws<ObjectDisposedException>(() => _ = database.GroupCount);
            Assert.Throws<ObjectDisposedException>(() => _ = database.ModuleCount);
            Assert.Throws<ObjectDisposedException>(() => database.Validate());
            Assert.Throws<ObjectDisposedException>(() => _ = database.OverallScore);
        }

        [Test]
        public void GetMemoryUsage_ShouldReturnValidData()
        {
            // Act
            var (totalBytes, numAllocations) = CoverageDatabase.GetMemoryUsage();

            // Assert
            Assert.That(totalBytes, Is.GreaterThanOrEqualTo(0), "Total bytes should be non-negative");
            Assert.That(numAllocations, Is.GreaterThanOrEqualTo(0), "Number of allocations should be non-negative");
            
            TestContext.Out.WriteLine($"Memory Usage - Total Bytes: {totalBytes}, Allocations: {numAllocations}");
        }
    }
}
