/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

using Newtonsoft.Json;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace LandMapRLibrary
{
    /// <summary>
    /// The wrapper class used to run LandMapR.
    /// </summary>
    public class LandMapR
    {
        /// <summary>
        /// Native method from kernel32.dll to load the selected dll.
        /// </summary>
        /// <param name="dllToLoad"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string dllToLoad);

        /// <summary>
        /// Native method to get the function pointer from memory.
        /// </summary>
        /// <param name="hModule"></param>
        /// <param name="procedureName"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        /// <summary>
        /// Native method to free the library.
        /// </summary>
        /// <param name="hModule"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        private static extern bool FreeLibrary(IntPtr hModule);

        /// <summary>
        /// Declare the function delegate for C++ with matching parameters. Slightly modified in the C# wrapper to be have more developer friendly variable names in C#.
        /// </summary>
        /// <param name="workingDirectory"></param>
        /// <param name="demData"></param>
        /// <param name="xllcorner"></param>
        /// <param name="yllcorner"></param>
        /// <param name="fileName"></param>
        /// <param name="nrows"></param>
        /// <param name="ncolumns"></param>
        /// <param name="gridSize"></param>
        /// <param name="missingDataValue"></param>
        /// <param name="pitArea"></param>
        /// <param name="pitDepth"></param>
        /// <param name="pbInvertedElev"></param>
        /// <param name="dDSArea"></param>
        /// <param name="dUpslopeArea"></param>
        /// <param name="fuzzyAttributeFile"></param>
        /// <param name="fuzzyFacetsFile"></param>
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate IntPtr LandMapRProcess(string workingDirectory, string demData, double xllcorner, double yllcorner, string outPrefix, int nrows, int ncolumns, double gridSize, double missingDataValue, long pitArea, double pitDepth, bool pbInvertedElev, double dDSArea, double dUpslopeArea, string fuzzyAttributeFile, string fuzzyFacetsFile);

        /// <summary>
        /// The options for LandMapR.
        /// </summary>
        private LandMapROptions _options;

        /// <summary>
        /// The constructor with mandatory options parameter.
        /// </summary>
        /// <param name="options"></param>
        public LandMapR(LandMapROptions options)
        {
            _options = options;
        }

        private static string PtrToStringUtf8(IntPtr ptr) // aPtr is nul-terminated
        {
            if (ptr == IntPtr.Zero)
                return "";
            int len = 0;
            while (System.Runtime.InteropServices.Marshal.ReadByte(ptr, len) != 0)
                len++;
            if (len == 0)
                return "";
            byte[] array = new byte[len];
            System.Runtime.InteropServices.Marshal.Copy(ptr, array, 0, len);
            return System.Text.Encoding.UTF8.GetString(array);
        }

        /// <summary>
        /// Method to call the LandMapR C++ DLL through C#.
        /// </summary>
        public LandMapRResult Run()
        {
            try
            {

                Console.WriteLine("In RUN() of LandMapR C#.001.");

                // process the csv file
                var processedDEMData = ProcessCsvData();
                Console.WriteLine("In RUN() of LandMapR C#.002.");

                // check if the data has been process from the inputFile csv
                if (string.IsNullOrEmpty(processedDEMData))
                {
                    throw new ArgumentNullException("Elevation Data", "The elevation input data cannot be empty.");
                }
                Console.WriteLine("In RUN() of LandMapR C#.003.");
                // get the pointer for the landmapr dll
                string dllFile = Path.Combine(AppContext.BaseDirectory, "Dll", "LandMapR.dll");


                Console.WriteLine("BaseDirectory: " + AppContext.BaseDirectory.ToString());
                Console.WriteLine("FullPath: " + Path.Combine(AppContext.BaseDirectory, "Dll", "LandMapR.dll").ToString());
                if (File.Exists(dllFile))
                {
                    Console.WriteLine("DLL file at " + dllFile + " exists.");
                }
                else
                {
                    Console.WriteLine("File doesn't exist.");
                }

                Console.WriteLine("In RUN() of LandMapR C#.003.5.");
                //IntPtr dllPointer = LoadLibrary(dllFile);

                //Load and check the pointer out:
                IntPtr dllPointer = LoadLibrary(dllFile);
                Console.WriteLine("dllPointer: " + dllPointer.ToString());
                Console.WriteLine("dllPointer pointer size should be " + IntPtr.Size);

                if (dllPointer == IntPtr.Zero)
                {
                    int errorCode = Marshal.GetLastWin32Error();
                    throw new Exception(string.Format("Failed to load library (ErrorCode: {0})", errorCode));
                }
                Console.WriteLine("In RUN() of LandMapR C#.004.");

                // pointer to the c++ function
                IntPtr pAddressOfFunctionToCall = GetProcAddress(dllPointer, "landmapRProcess");
                Console.WriteLine("pAddressOfFunctionToCall: " + pAddressOfFunctionToCall.ToString());
                Console.WriteLine("In RUN() of LandMapR C#.005.");

                // the function for the object in c++
                //LandMapRProcess landMapRCPP = (LandMapRProcess)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(LandMapRProcess));
                //Gerard-SNAP181011_084623
                //This is the NEW method of calling this class. Updated to avoid obsolete warnings.
                LandMapRProcess landMapRCPP;

                landMapRCPP = (LandMapRProcess)Marshal.GetDelegateForFunctionPointer<LandMapRProcess>(pAddressOfFunctionToCall);
                Console.WriteLine("In RUN() of LandMapR C#.006.");

                // set the parameters for the c++ object using the wrapper from c# and run the program

                //Geard-SNAP181017_135318 Just putting the SSSS here to test things.
                //_options.OutputFilePrefix = "SSSSS";
                Console.WriteLine("Just ahead of the C++ DLL call.");

                var result = landMapRCPP(_options.OutputDirectory,
                    processedDEMData,
                    _options.XLLCorner,
                    _options.YLLCorner,
                    _options.OutputFilePrefix,
                    _options.Rows,
                    _options.Columns,
                    _options.GridSize,
                    _options.MissingDataValue,
                    _options.PitArea,
                    _options.PitDepth,
                    _options.InvertedElevation,
                    _options.DownslopeArea,
                    _options.UpslopeArea,
                    _options.LM3ARules,
                    _options.LM3CRules
                );
                Console.WriteLine("Just after the C++ DLL call.");
                string s = Marshal.PtrToStringAnsi(result);
                Console.WriteLine(s);

                // return serialized object
                var jsonResult = JsonConvert.DeserializeObject<LandMapRResult>(s);
                return jsonResult;



            }
            catch (Exception e)
            {
                Console.WriteLine("Exception " + e.ToString());
                var jsonResult = new LandMapRResult();
                return jsonResult;
            }

        }

        /// <summary>
        /// For reading elevation data from csv files.
        /// </summary>
        /// <param name="path"></param>
        private string ProcessCsvData()
        {
            // read from the input csv file
            using (var reader = new StreamReader(new FileStream(_options.InputFilePath, FileMode.Open, FileAccess.Read)))
            {
                // skip the header
                reader.ReadLine();

                // count the rows
                int count = 0;

                // create string builder for formatting elevation data
                var sb = new StringBuilder();

                // create comma separated string from the elevation file
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    sb.Append(line);
                    sb.Append(",");
                    count++;
                }

                // remove last comma
                if (sb.Length > 1)
                    sb.Remove(sb.Length - 1, 1);

                //check if the number of rows in csv match rows*columns 
                if (count != _options.Columns * _options.Rows)
                {
                    throw new InvalidDataException($"Number of rows in the file located at {_options.InputFilePath} does not match the number of columns ({_options.Columns}) multipled by the number of rows ({_options.Rows}) supplied in the constructor. The total row count in the file is {count} while the columns and rows set in the constructor is {_options.Columns * _options.Rows}.");
                }

                // set the data to be passed to landmapr from the elevation data
                return sb.ToString();
            }
        }
    }
}
