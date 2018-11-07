/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
using Newtonsoft.Json;
using System.Collections.Generic;

namespace LandMapRLibrary
{
    /// <summary>
    /// LandMapR options object containing necessary paramaters required for C++ DLL.
    /// </summary>
    public class LandMapROptions
    {
        /// <summary>
        /// Output directory.
        /// </summary>
        public string OutputDirectory { get; set; }

        /// <summary>
        /// The file path to the input data csv.
        /// </summary>
        public string InputFilePath { get; set; }

        /// <summary>
        /// Lower left x coordinate.
        /// </summary>
        public double XLLCorner { get; set; }

        /// <summary>
        /// Lower left y coordinate.
        /// </summary>
        public double YLLCorner { get; set; }

        /// <summary>
        /// Prefix to be prepended to the name of the automatically generated files. Optional.
        /// </summary>
        public string OutputFilePrefix { get; set; }

        /// <summary>
        /// Number of rows in the grid.
        /// </summary>
        public int Rows { get; set; }

        /// <summary>
        /// Number of columns in the grid.
        /// </summary>
        public int Columns { get; set; }

        /// <summary>
        /// Resolution in meters.
        /// </summary>
        public double GridSize { get; set; }

        /// <summary>
        /// Value to determine which cells have missing data.
        /// </summary>
        public double MissingDataValue => -9999;

        /// <summary>
        /// Threshold values (in meters) for pit areas that are to be removed.
        /// </summary>
        public long PitArea { get; set; }

        /// <summary>
        /// Threshold values (in meters) for pit depths that are to be removed.
        /// </summary>
        public double PitDepth { get; set; }

        /// <summary>
        /// Whether elevation data is inverted.
        /// </summary>
        public bool InvertedElevation => true;

        /// <summary>
        /// Threshold value for downslope area.
        /// The 300 cell value seems to be particularly suitable when processing relatively high resolution 
        /// (5-10 m horizontal grid cells) DEM data for field and farm sized areas (800 m to 1600 m on a side). 
        /// For DEM data sets with a very small number of cells (e.g. less than 10,000) it may frequently be necessary
        /// to select lower threshold values (100-200 cells) as there are simply not enough cells in the DEM to produce
        /// large upslope area counts in the range of 300 or greater.  For very large DEM data sets containing 1 million 
        /// cells or more, it has often proven better to select threshold values closer to 1000 cells as lower thresholds
        /// can result in recognition of channel networks that are overly detailed and confusing. 
        /// </summary>
        public double DownslopeArea { get; set; }

        /// <summary>
        /// Threshold value for upslope area.
        /// The 300 cell value seems to be particularly suitable when processing relatively high resolution 
        /// (5-10 m horizontal grid cells) DEM data for field and farm sized areas (800 m to 1600 m on a side). 
        /// For DEM data sets with a very small number of cells (e.g. less than 10,000) it may frequently be necessary
        /// to select lower threshold values (100-200 cells) as there are simply not enough cells in the DEM to produce
        /// large upslope area counts in the range of 300 or greater.  For very large DEM data sets containing 1 million 
        /// cells or more, it has often proven better to select threshold values closer to 1000 cells as lower thresholds
        /// can result in recognition of channel networks that are overly detailed and confusing. 
        /// </summary>
        public double UpslopeArea { get; set; }

        /// <summary>
        /// Fuzzy attribute file.
        /// </summary>
        public string LM3ARules { get; set; }

        /// <summary>
        /// Fuzzy facet rules.
        /// </summary>
        public string LM3CRules { get; set; }

        /// <summary>
        /// Constructor with default fuzzy classification rules.
        /// </summary>
        /// <param name="outputDirectory">The directory in which the files processed from the C++ DLL will be outputted to.</param>
        /// <param name="inputFilePath">The interpolated input elevation file with just the elevation values.</param>
        /// <param name="xLLCorner">The lower left x coordinate of the interpolated data set you are inputting. Must be in meters. Lat/Lng not accepted.</param>
        /// <param name="yLLCorner">The lower left y coordinate of the interpolated data set you are inputting. Must be in meters. Lat/Lng not accepted.</param>
        /// <param name="rows">The number of rows in the interpolated input file.</param>
        /// <param name="columns">The number of columns in the interpolated input file.</param>
        /// <param name="gridSize">The resoultion in meters.</param>
        /// <param name="pitArea">Maximum area of pit to remove. Suggested default value of 10.</param>
        /// <param name="pitDepth">Maximum depth of pit to remove. Suggested default value of 0.15.</param>
        /// <param name="downslopeArea">Threshold value for downslope area. Suggested default of 300 since it has been found to work well with many kinds of terrain.</param>
        /// <param name="upslopeArea">Threshold value for upslope area. Suggested default of 300 since it has been found to work well with many kinds of terrain.</param>
        /// <param name="outputFilePrefix">The name to prefix your output files with.</param>
        public LandMapROptions(string outputDirectory, string inputFilePath, double xLLCorner, double yLLCorner, int rows, int columns, double gridSize, long pitArea = 10, double pitDepth = 0.15, double downslopeArea = 300, double upslopeArea = 300, string outputFilePrefix = "")
        {
            OutputDirectory = outputDirectory;
            InputFilePath = inputFilePath;
            XLLCorner = xLLCorner;
            YLLCorner = yLLCorner;
            Rows = rows;
            Columns = columns;
            GridSize = gridSize;
            PitArea = pitArea;
            PitDepth = pitDepth;
            DownslopeArea = downslopeArea;
            UpslopeArea = upslopeArea;
            LM3ARules = JsonConvert.SerializeObject(LM3ARULE.Defaults);
            LM3CRules = JsonConvert.SerializeObject(LM3CRULE.Defaults);
            OutputFilePrefix = outputFilePrefix;
        }

        /// <summary>
        /// Constructor that allows customized fuzzy classification rules.
        /// </summary>
        /// <param name="outputDirectory">The directory in which the files processed from the C++ DLL will be outputted to.</param>
        /// <param name="inputFilePath">The interpolated input elevation file with just the elevation values.</param>
        /// <param name="xllCorner">The lower left x coordinate of the interpolated data set you are inputting. Must be in meters. Lat/Lng not accepted.</param>
        /// <param name="yllCorner">The lower left y coordinate of the interpolated data set you are inputting. Must be in meters. Lat/Lng not accepted.</param>
        /// <param name="rows">The number of rows in the interpolated input file.</param>
        /// <param name="columns">The number of columns in the interpolated input file.</param>
        /// <param name="gridSize">The resoultion in meters.</param>
        /// <param name="lm3aRules">Custom LM3ARules. If you specify your own we have no idea what will happen. Use at your own discretion.</param>
        /// <param name="lm3cRules">Custom LM3CRules. If you specify your own we have no idea what will happen. Use at your own discretion.</param>
        /// <param name="pitArea">Maximum area of pit to remove. Suggested default value of 10.</param>
        /// <param name="pitDepth">Maximum depth of pit to remove. Suggested default value of 0.15.</param>
        /// <param name="downslopeArea">Threshold value for downslope area. Suggested default of 300 since it has been found to work well with many kinds of terrain. See property description for more detailed information.</param>
        /// <param name="upslopeArea">Threshold value for upslope area. Suggested default of 300 since it has been found to work well with many kinds of terrain. See property description for more detailed information.</param>
        /// <param name="outputFilePrefix">The name to prefix your output files with.</param>
        public LandMapROptions(string outputDirectory, string inputFilePath, double xLLCorner, double yLLCorner, int rows, int columns, double gridSize, List<LM3ARULE> lm3aRules, List<LM3CRULE> lm3cRules, long pitArea = 10, double pitDepth = 0.15, double downslopeArea = 300, double upslopeArea = 300, string outputFilePrefix = "")
        {
            OutputFilePrefix = outputFilePrefix;
            OutputDirectory = outputDirectory;
            InputFilePath = inputFilePath;
            XLLCorner = xLLCorner;
            YLLCorner = yLLCorner;
            Rows = rows;
            Columns = columns;
            GridSize = gridSize;
            PitArea = pitArea;
            PitDepth = pitDepth;
            DownslopeArea = downslopeArea;
            UpslopeArea = upslopeArea;
            LM3ARules = JsonConvert.SerializeObject(lm3aRules);
            LM3CRules = JsonConvert.SerializeObject(lm3cRules);
        }
    }
}
