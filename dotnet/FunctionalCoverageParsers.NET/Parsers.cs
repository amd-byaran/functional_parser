using System;
using FunctionalCoverageParsers.Native;

namespace FunctionalCoverageParsers
{
    /// <summary>
    /// Interface for coverage parsers
    /// </summary>
    internal interface ICoverageParser
    {
        IntPtr Handle { get; }
    }

    /// <summary>
    /// Base class for all coverage file parsers
    /// </summary>
    public abstract class CoverageParser : IDisposable, ICoverageParser
    {
        protected IntPtr _handle;
        protected bool _disposed;

        /// <summary>
        /// Gets the native parser handle
        /// </summary>
        IntPtr ICoverageParser.Handle => _handle;

        /// <summary>
        /// Gets a value indicating whether this parser instance is valid
        /// </summary>
        public bool IsValid => _handle != IntPtr.Zero && !_disposed;

        protected CoverageParser(IntPtr handle)
        {
            _handle = handle;
            if (_handle == IntPtr.Zero)
            {
                throw new CoverageMemoryException("Failed to create parser");
            }
        }

        /// <summary>
        /// Parses a coverage file and populates the specified database
        /// </summary>
        /// <param name="filename">Path to the coverage file</param>
        /// <param name="database">Database to populate with parsed data</param>
        /// <exception cref="ArgumentNullException">Thrown when filename or database is null</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the parser has been disposed</exception>
        public void ParseFile(string filename, CoverageDatabase database)
        {
            if (filename == null)
                throw new ArgumentNullException(nameof(filename));
            if (database == null)
                throw new ArgumentNullException(nameof(database));

            ThrowIfDisposed();

            if (!database.IsValid)
                throw new ArgumentException("Database is not valid", nameof(database));

            // Use reflection to get the database handle
            var dbType = database.GetType();
            var handleField = dbType.GetField("_handle", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
            if (handleField?.GetValue(database) is not IntPtr dbHandle || dbHandle == IntPtr.Zero)
            {
                throw new ArgumentException("Database handle is invalid", nameof(database));
            }

            int result = NativeMethods.parse_coverage_file(_handle, filename, dbHandle);
            if (result != 0)
            {
                string error = GetErrorString(result);
                throw result switch
                {
                    1 => new CoverageFileNotFoundException(filename),
                    2 => new InvalidCoverageFormatException(filename, error),
                    3 => new CoverageMemoryException($"Memory allocation failed while parsing {filename}"),
                    4 => new InvalidParameterException("filename", error),
                    5 => new ParseFailedException(filename, error),
                    _ => new CoverageParserException(result, $"Parse failed for {filename}: {error}")
                };
            }
        }

        private static string GetErrorString(int errorCode)
        {
            var ptr = NativeMethods.get_error_string(errorCode);
            return NativeMethods.PtrToString(ptr) ?? $"Unknown error code: {errorCode}";
        }

        protected void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(GetType().Name);
        }

        #region IDisposable Implementation

        /// <summary>
        /// Releases all resources used by the parser
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed && _handle != IntPtr.Zero)
            {
                DestroyParser();
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }

        protected abstract void DestroyParser();

        /// <summary>
        /// Finalizer for the parser
        /// </summary>
        ~CoverageParser()
        {
            Dispose(false);
        }

        #endregion
    }

    /// <summary>
    /// Parser for dashboard coverage files (dashboard.txt)
    /// </summary>
    public sealed class DashboardParser : CoverageParser
    {
        /// <summary>
        /// Initializes a new instance of the DashboardParser class
        /// </summary>
        public DashboardParser() : base(NativeMethods.create_dashboard_parser())
        {
        }

        protected override void DestroyParser()
        {
            NativeMethods.destroy_parser(_handle);
        }
    }

    /// <summary>
    /// Parser for coverage groups files (groups.txt)
    /// </summary>
    public sealed class GroupsParser : CoverageParser
    {
        /// <summary>
        /// Initializes a new instance of the GroupsParser class
        /// </summary>
        public GroupsParser() : base(NativeMethods.create_groups_parser())
        {
        }

        protected override void DestroyParser()
        {
            NativeMethods.destroy_parser(_handle);
        }
    }

    /// <summary>
    /// Parser for hierarchy coverage files (hierarchy.txt)
    /// </summary>
    public sealed class HierarchyParser : CoverageParser
    {
        /// <summary>
        /// Initializes a new instance of the HierarchyParser class
        /// </summary>
        public HierarchyParser() : base(NativeMethods.create_hierarchy_parser())
        {
        }

        protected override void DestroyParser()
        {
            NativeMethods.destroy_parser(_handle);
        }
    }

    /// <summary>
    /// Parser for module list coverage files (modlist.txt)
    /// </summary>
    public sealed class ModuleListParser : CoverageParser
    {
        /// <summary>
        /// Initializes a new instance of the ModuleListParser class
        /// </summary>
        public ModuleListParser() : base(NativeMethods.create_modlist_parser())
        {
        }

        protected override void DestroyParser()
        {
            NativeMethods.destroy_parser(_handle);
        }
    }

    /// <summary>
    /// Parser for assertion coverage files (asserts.txt)
    /// </summary>
    public sealed class AssertParser : CoverageParser
    {
        /// <summary>
        /// Initializes a new instance of the AssertParser class
        /// </summary>
        public AssertParser() : base(NativeMethods.create_assert_parser())
        {
        }

        protected override void DestroyParser()
        {
            NativeMethods.destroy_parser(_handle);
        }
    }
}