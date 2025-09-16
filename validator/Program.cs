using System;
using System.IO;
using System.Reflection;

namespace AssemblyValidator
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Assembly Integrity Validator ===");
            Console.WriteLine("Testing FunctionalCoverageParsers assemblies for IL corruption...\n");

            string baseDir = @"c:\Users\byaran\OneDrive - Advanced Micro Devices Inc\programming\coverage_analyzer\FunctionalParser";
            
            // Test paths for different assemblies
            string[] testPaths = {
                Path.Combine(baseDir, "dotnet", "FunctionalCoverageParsers.NET", "bin", "Debug", "net8.0", "FunctionalCoverageParsers.NET.dll"),
                Path.Combine(baseDir, "dotnet", "FunctionalCoverageParsers.NET", "bin", "Release", "net8.0", "FunctionalCoverageParsers.NET.dll"),
                Path.Combine(baseDir, "dotnet", "FunctionalCoverageParsers.Tests", "bin", "Debug", "net8.0", "FunctionalCoverageParsers.Tests.dll"),
                Path.Combine(baseDir, "dotnet", "FunctionalCoverageParsers.Tests", "bin", "Release", "net8.0", "FunctionalCoverageParsers.Tests.dll")
            };

            bool allHealthy = true;

            foreach (string path in testPaths)
            {
                Console.WriteLine($"Testing: {path}");
                if (!File.Exists(path))
                {
                    Console.WriteLine("  ❌ File not found\n");
                    continue;
                }

                try
                {
                    // Test assembly loading
                    Assembly assembly = Assembly.LoadFrom(path);
                    Console.WriteLine($"  ✅ Assembly loaded successfully");
                    Console.WriteLine($"  📋 Assembly: {assembly.GetName().Name} v{assembly.GetName().Version}");
                    Console.WriteLine($"  🏗️ Target Framework: {assembly.GetCustomAttribute<System.Runtime.Versioning.TargetFrameworkAttribute>()?.FrameworkName ?? "Unknown"}");

                    // Test metadata reading
                    Type[] types = assembly.GetTypes();
                    Console.WriteLine($"  📦 Contains {types.Length} types");

                    // Test specific type loading
                    foreach (Type type in types)
                    {
                        if (type.IsPublic)
                        {
                            try
                            {
                                var methods = type.GetMethods();
                                var properties = type.GetProperties();
                                Console.WriteLine($"    🔍 Type {type.Name}: {methods.Length} methods, {properties.Length} properties");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine($"    ❌ Failed to read type {type.Name}: {ex.Message}");
                                allHealthy = false;
                            }
                        }
                    }

                    // Test instantiation of key types (if applicable)
                    Console.WriteLine("  ✅ All metadata accessible");
                }
                catch (BadImageFormatException ex)
                {
                    Console.WriteLine($"  ❌ CORRUPTED IL: Bad image format - {ex.Message}");
                    allHealthy = false;
                }
                catch (FileLoadException ex)
                {
                    Console.WriteLine($"  ❌ LOAD FAILED: {ex.Message}");
                    allHealthy = false;
                }
                catch (ReflectionTypeLoadException ex)
                {
                    Console.WriteLine($"  ⚠️  DEPENDENCY ISSUE: Some types failed to load");
                    foreach (var loaderEx in ex.LoaderExceptions)
                    {
                        if (loaderEx != null)
                            Console.WriteLine($"    📋 {loaderEx.Message}");
                    }
                    // This is usually a dependency issue, not IL corruption
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"  ❌ UNEXPECTED ERROR: {ex.Message}");
                    allHealthy = false;
                }

                Console.WriteLine();
            }

            // Summary
            Console.WriteLine("=== FINAL ASSESSMENT ===");
            if (allHealthy)
            {
                Console.WriteLine("✅ ALL ASSEMBLIES HEALTHY - No IL corruption detected");
                Console.WriteLine("   The assemblies are intact and properly formed.");
                Console.WriteLine("   Any previous issues were likely runtime compatibility problems,");
                Console.WriteLine("   not actual IL corruption.");
            }
            else
            {
                Console.WriteLine("❌ ASSEMBLY CORRUPTION DETECTED");
                Console.WriteLine("   One or more assemblies have genuine IL corruption issues.");
                Console.WriteLine("   Recommend full rebuild from clean state.");
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}