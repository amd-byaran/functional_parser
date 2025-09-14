using System;
using System.Runtime.InteropServices;

class Program
{
    [DllImport("FunctionalCoverageParsers.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern IntPtr get_version_string();

    static void Main()
    {
        try
        {
            Console.WriteLine("Testing DLL function...");
            IntPtr result = get_version_string();
            if (result != IntPtr.Zero)
            {
                string version = Marshal.PtrToStringAnsi(result);
                Console.WriteLine($"SUCCESS: Version = {version}");
            }
            else
            {
                Console.WriteLine("FAILURE: Null pointer returned");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"ERROR: {ex.Message}");
        }
        
        Console.WriteLine("Press any key to exit...");
        Console.ReadKey();
    }
}