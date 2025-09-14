using System;
using System.IO;
using NUnit.Framework;

namespace FunctionalCoverageParsers.Tests
{
    /// <summary>
    /// Tests for export functionality
    /// </summary>
    [TestFixture]
    public class ExportTests
    {
        private string _outputPath = string.Empty;

        [OneTimeSetUp]
        public void OneTimeSetUp()
        {
            _outputPath = Path.Combine(TestContext.CurrentContext.TestDirectory, "Output");
            Directory.CreateDirectory(_outputPath);
        }

        [OneTimeTearDown]
        public void OneTimeTearDown()
        {
            if (Directory.Exists(_outputPath))
            {
                try
                {
                    Directory.Delete(_outputPath, true);
                }
                catch
                {
                    // Ignore cleanup errors
                }
            }
        }

        [Test]
        public void ExportToXml_WithEmptyDatabase_ShouldSucceed()
        {
            // Arrange
            using var database = new CoverageDatabase();
            string xmlFile = Path.Combine(_outputPath, "empty_coverage.xml");

            // Act
            database.ExportToXml(xmlFile);

            // Assert
            Assert.That(File.Exists(xmlFile), Is.True, "XML file should be created");
            
            string content = File.ReadAllText(xmlFile);
            Assert.That(content, Is.Not.Empty, "XML file should not be empty");
            TestContext.Out.WriteLine($"XML file size: {new FileInfo(xmlFile).Length} bytes");
        }

        [Test]
        public void ExportToJson_WithEmptyDatabase_ShouldSucceed()
        {
            // Arrange
            using var database = new CoverageDatabase();
            string jsonFile = Path.Combine(_outputPath, "empty_coverage.json");

            // Act
            database.ExportToJson(jsonFile);

            // Assert
            Assert.That(File.Exists(jsonFile), Is.True, "JSON file should be created");
            
            string content = File.ReadAllText(jsonFile);
            Assert.That(content, Is.Not.Empty, "JSON file should not be empty");
            TestContext.Out.WriteLine($"JSON file size: {new FileInfo(jsonFile).Length} bytes");
        }

        [Test]
        public void ExportToXml_WithNullFilename_ShouldThrowArgumentNullException()
        {
            // Arrange
            using var database = new CoverageDatabase();

            // Act & Assert
            Assert.Throws<ArgumentNullException>(() => database.ExportToXml(null!));
        }

        [Test]
        public void ExportToJson_WithNullFilename_ShouldThrowArgumentNullException()
        {
            // Arrange
            using var database = new CoverageDatabase();

            // Act & Assert
            Assert.Throws<ArgumentNullException>(() => database.ExportToJson(null!));
        }

        [Test]
        public void ExportWithDisposedDatabase_ShouldThrowObjectDisposedException()
        {
            // Arrange
            var database = new CoverageDatabase();
            database.Dispose();
            string xmlFile = Path.Combine(_outputPath, "disposed_test.xml");
            string jsonFile = Path.Combine(_outputPath, "disposed_test.json");

            // Act & Assert
            Assert.Throws<ObjectDisposedException>(() => database.ExportToXml(xmlFile));
            Assert.Throws<ObjectDisposedException>(() => database.ExportToJson(jsonFile));
        }
    }
}
