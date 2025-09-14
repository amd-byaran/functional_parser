using System;

namespace FunctionalCoverageParsers
{
    /// <summary>
    /// Base exception for all FunctionalCoverageParsers errors
    /// </summary>
    public class CoverageParserException : Exception
    {
        /// <summary>
        /// Gets the native error code associated with this exception
        /// </summary>
        public int ErrorCode { get; protected set; }

        /// <summary>
        /// Initializes a new instance of the CoverageParserException class
        /// </summary>
        /// <param name="message">The error message</param>
        public CoverageParserException(string message) : base(message)
        {
            ErrorCode = 0;
        }

        /// <summary>
        /// Initializes a new instance of the CoverageParserException class
        /// </summary>
        /// <param name="message">The error message</param>
        /// <param name="innerException">The inner exception</param>
        public CoverageParserException(string message, Exception innerException) : base(message, innerException)
        {
            ErrorCode = 0;
        }

        /// <summary>
        /// Initializes a new instance of the CoverageParserException class
        /// </summary>
        /// <param name="errorCode">The native error code</param>
        /// <param name="message">The error message</param>
        public CoverageParserException(int errorCode, string message) : base(message)
        {
            ErrorCode = errorCode;
        }

        /// <summary>
        /// Initializes a new instance of the CoverageParserException class
        /// </summary>
        /// <param name="errorCode">The native error code</param>
        /// <param name="message">The error message</param>
        /// <param name="innerException">The inner exception</param>
        public CoverageParserException(int errorCode, string message, Exception innerException) : base(message, innerException)
        {
            ErrorCode = errorCode;
        }
    }

    /// <summary>
    /// Exception thrown when a coverage file is not found
    /// </summary>
    public class CoverageFileNotFoundException : CoverageParserException
    {
        /// <summary>
        /// Gets the filename that was not found
        /// </summary>
        public string FileName { get; }

        /// <summary>
        /// Initializes a new instance of the CoverageFileNotFoundException class
        /// </summary>
        /// <param name="fileName">The filename that was not found</param>
        public CoverageFileNotFoundException(string fileName) 
            : base($"Coverage file not found: {fileName}")
        {
            FileName = fileName;
            ErrorCode = 1; // ERROR_FILE_NOT_FOUND
        }

        /// <summary>
        /// Initializes a new instance of the CoverageFileNotFoundException class
        /// </summary>
        /// <param name="fileName">The filename that was not found</param>
        /// <param name="innerException">The inner exception</param>
        public CoverageFileNotFoundException(string fileName, Exception innerException) 
            : base($"Coverage file not found: {fileName}", innerException)
        {
            FileName = fileName;
            ErrorCode = 1; // ERROR_FILE_NOT_FOUND
        }
    }

    /// <summary>
    /// Exception thrown when a coverage file has an invalid format
    /// </summary>
    public class InvalidCoverageFormatException : CoverageParserException
    {
        /// <summary>
        /// Gets the filename with invalid format
        /// </summary>
        public string FileName { get; }

        /// <summary>
        /// Initializes a new instance of the InvalidCoverageFormatException class
        /// </summary>
        /// <param name="fileName">The filename with invalid format</param>
        public InvalidCoverageFormatException(string fileName) 
            : base($"Invalid coverage file format: {fileName}")
        {
            FileName = fileName;
            ErrorCode = 2; // ERROR_INVALID_FORMAT
        }

        /// <summary>
        /// Initializes a new instance of the InvalidCoverageFormatException class
        /// </summary>
        /// <param name="fileName">The filename with invalid format</param>
        /// <param name="message">Additional error details</param>
        public InvalidCoverageFormatException(string fileName, string message) 
            : base($"Invalid coverage file format: {fileName}. {message}")
        {
            FileName = fileName;
            ErrorCode = 2; // ERROR_INVALID_FORMAT
        }

        /// <summary>
        /// Initializes a new instance of the InvalidCoverageFormatException class
        /// </summary>
        /// <param name="fileName">The filename with invalid format</param>
        /// <param name="innerException">The inner exception</param>
        public InvalidCoverageFormatException(string fileName, Exception innerException) 
            : base($"Invalid coverage file format: {fileName}", innerException)
        {
            FileName = fileName;
            ErrorCode = 2; // ERROR_INVALID_FORMAT
        }
    }

    /// <summary>
    /// Exception thrown when memory allocation fails
    /// </summary>
    public class CoverageMemoryException : CoverageParserException
    {
        /// <summary>
        /// Initializes a new instance of the CoverageMemoryException class
        /// </summary>
        public CoverageMemoryException() 
            : base("Memory allocation failed in coverage parser")
        {
            ErrorCode = 3; // ERROR_MEMORY_ALLOCATION
        }

        /// <summary>
        /// Initializes a new instance of the CoverageMemoryException class
        /// </summary>
        /// <param name="message">The error message</param>
        public CoverageMemoryException(string message) : base(message)
        {
            ErrorCode = 3; // ERROR_MEMORY_ALLOCATION
        }

        /// <summary>
        /// Initializes a new instance of the CoverageMemoryException class
        /// </summary>
        /// <param name="message">The error message</param>
        /// <param name="innerException">The inner exception</param>
        public CoverageMemoryException(string message, Exception innerException) : base(message, innerException)
        {
            ErrorCode = 3; // ERROR_MEMORY_ALLOCATION
        }
    }

    /// <summary>
    /// Exception thrown when invalid parameters are passed to the parser
    /// </summary>
    public class InvalidParameterException : CoverageParserException
    {
        /// <summary>
        /// Gets the name of the invalid parameter
        /// </summary>
        public string ParameterName { get; }

        /// <summary>
        /// Initializes a new instance of the InvalidParameterException class
        /// </summary>
        /// <param name="parameterName">The name of the invalid parameter</param>
        public InvalidParameterException(string parameterName) 
            : base($"Invalid parameter: {parameterName}")
        {
            ParameterName = parameterName;
            ErrorCode = 4; // ERROR_INVALID_PARAMETER
        }

        /// <summary>
        /// Initializes a new instance of the InvalidParameterException class
        /// </summary>
        /// <param name="parameterName">The name of the invalid parameter</param>
        /// <param name="message">Additional error details</param>
        public InvalidParameterException(string parameterName, string message) 
            : base($"Invalid parameter '{parameterName}': {message}")
        {
            ParameterName = parameterName;
            ErrorCode = 4; // ERROR_INVALID_PARAMETER
        }
    }

    /// <summary>
    /// Exception thrown when parsing fails
    /// </summary>
    public class ParseFailedException : CoverageParserException
    {
        /// <summary>
        /// Gets the filename that failed to parse
        /// </summary>
        public string? FileName { get; }

        /// <summary>
        /// Initializes a new instance of the ParseFailedException class
        /// </summary>
        /// <param name="message">The error message</param>
        public ParseFailedException(string message) : base(message)
        {
            ErrorCode = 5; // ERROR_PARSE_FAILED
        }

        /// <summary>
        /// Initializes a new instance of the ParseFailedException class
        /// </summary>
        /// <param name="fileName">The filename that failed to parse</param>
        /// <param name="message">The error message</param>
        public ParseFailedException(string fileName, string message) : base(message)
        {
            FileName = fileName;
            ErrorCode = 5; // ERROR_PARSE_FAILED
        }

        /// <summary>
        /// Initializes a new instance of the ParseFailedException class
        /// </summary>
        /// <param name="message">The error message</param>
        /// <param name="innerException">The inner exception</param>
        public ParseFailedException(string message, Exception innerException) : base(message, innerException)
        {
            ErrorCode = 5; // ERROR_PARSE_FAILED
        }
    }
}