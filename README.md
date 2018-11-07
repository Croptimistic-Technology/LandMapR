# LandMapR
Forked LandMapRLibrary from Niagara College Research &amp; Innovation. This project comes from http://gitlab.ncinnovation.ca/LandMapr/landmapr-library which you may download from or use the Nuget package 'LandMapRLibrary'. This is forked from the 1.2.1 release and fixes a few issues with the library (out of bounds checks, wrapper pass thrus and updates deprecated code for DLL injection). 

The original license is https://gitlab.ncinnovation.ca/LandMapr/landmapr-library/blob/master/license.txt so this one is the same.

Below is the original README.md, which is otherwise functionally identical to this one.

## Public NuGet

https://www.nuget.org/packages/LandMapRLibrary/

## C# LandMapRLibrary

- Must be built in x64 or the .dll won't load.
- To test, uncomment out the Main function in Program.cs, modify working directory and elevation file paths accordingly, and switch the Application build from Class Library to Console Application.
 
## Consumption

- After NuGet restore the packages, go into the Dll folder, select LandMapR.dll and set the Build Action to Content and set Copy to Output Directory to Always.

# Example Code for C# Application

```cs
// set your output directory
var outputDirectory = "<path to your output directory for the generated files>";

// elevation file as input
var inputFilePath = "<path to your elevation.csv file>";

// create the options object
var options = new LandMapROptions(outputDirectory, inputFilePath, 561769.763, 4745661.603, 165, 235, 5);

// create instance of landmapr object with options
var landMapR = new LandMapR(options);

// run land map r
var result = landMapR.Run();

// helper functions to retrieve points from generate files
var clsmPoints = LandMapRHelper.GetCondensedLSMFuzzyClassesPoints(result);
var demPoints = LandMapRHelper.GetDemPoints(result);
var fillPoints = LandMapRHelper.GetFillPoints(result);
var formPoints = LandMapRHelper.GetFormAllPoints(result);
var iDemPoints = LandMapRHelper.GetIDemPoints(result);
var iPitPoints = LandMapRHelper.GetIPitPoints(result);
var lenPoints = LandMapRHelper.GetLenPoints(result);
var moldPoints = LandMapRHelper.GetMoldPoints(result);
var pitPoints = LandMapRHelper.GetPitPoints(result);
var pondPoints = LandMapRHelper.GetPondPoints(result);
var relzAllPoints = LandMapRHelper.GetRelzAllPoints(result);
var voldPoints = LandMapRHelper.GetVoldPoints(result);
```

## Usage with ASP.NET 4.7.1 Web Application

Only works if IIS Express for Websites is configured to use the 64-bit version. To adjust this go to Tools -> Options -> Project and Solutions -> Web Projects in Visual Studio and enable 'Use the 64-bit version of IIS Express for web sites and projects'.

## C++ console application

- Recommended to modify the project properties to release mode in x64, to build the dll project, the target extension and configuration type must be set to .dll
- To test, switch the target extension and configuration type to .exe, then go to the main function in CodeLandmapRTest.cpp, in the inputParams region, modify working directory, 
  elevation file paths and other input parameters accordingly and run the program.  
    
## C++ Specification of input parameters and DEM dataset

- sample DEM dataset and values of input parameters are included in the Input folder
- description and range of input parameters
   - workingDirectory: the directory specified by the user for landmapR output files
   - xllcorner: lower left corner's x-coordinate
   - yllcorner: lower left corner's y-coordinate
   - nrows: number of rows in DEM matrix
   - ncolumns: number of columns in DEM matrix (Note: the multiplications of nrows and ncolumns must be equal to the count of elevation input values of the DEM dataset)
   - GridSize: grid size in meters
   - MissingDataValue: Grid values for missing data (must be set to -9999) 
   - outPrefix: prefix for output files
   - pbInvertedElev: Determine if user needs to calculate the inverted DEM (must be set to true because inverted DEM is one of the required inputs for FormMapR process)
   - PitArea: maximum area of pit to remove (suggest 10)
   - PitDepth: maximum depth of pit to remove (suggest 0.15)
   - dDSArea:  threshold value for upslope area	(suggest 300 since it has been found to work well with many kinds of terrain)
   - dUpSlopeArea: threshold value for ds_area area (suggest 300 since it has been found to work well with many kinds of terrain) 
   - LM3Arule: fuzzy attribute rule (see default values from LandMapRLibrary solution)
   - LM3Crule: fuzzy facet rule (see default values from LandMapRLibrary solution)
