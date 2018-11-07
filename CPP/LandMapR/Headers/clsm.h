/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _CLSMOBJECT_H_
#define _CLSMOBJECT_H_
#include <iostream>

/** \brief A "condensed LSM" fuzzy land form class
*
* Defines an object of type fuzy class object according to the third choice (condensed LSM) in FacetMapR
*/
class Clsm
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Clsm(void);
	/// <summary>
	/// Constructor of a knowl length of grid cells
	/// </summary>
	Clsm(int leng);
	/// <summary>
	/// Set number of grid cells
	/// </summary>
	void setLength(int leng);
	/// <summary>
	/// Get number of grid cells
	/// </summary>
	int getLength();
	/// <summary>
	/// Writes object members to a *.csv file, it writes all points in the grid
	/// </summary>
	bool writeCSV();
	/// <summary>
	/// Writes clsm object members to *.csv map file
	/// </summary>
	bool writeCSVMap(std::string& resultJsonPath, long ncols, long nrows, double xll, double yll, double gridSize);
	/// <summary>
	/// Writes map data to a *.csv file, selects only grid cells inside the boundary of region of interest 	classtype: 4 for a 4-class landform map, 15 for a 15-class landform map, or 19 for both the 15- and 4-classes 
	/// </summary>
	bool writeCSVmap(long ncols, long nrows, double xll, double yll, double gridSize, int classtype);	
	/// <summary>
	/// Destructor
	/// </summary>
	~Clsm(void); 
	/// <summary>
	/// Headings of the csv file output
	/// </summary>
	std::string clsmHeadings[5] = {"SeqNo", "Missing", "MaxClass", "MaxCode", "MaxValue"};
	/// <summary>
	/// A dynamic array holding grid sequence numbers
	/// </summary>
	int *SeqNo; 
	/// <summary>
	/// A flag to identify if a grid cell is inside the boundary of region of interest
	/// </summary>
	bool *Missing; 
	/// <summary>
	/// A dynamic array of strings that holds the alphanumeric code of the landform class with the highest fuzzy membership for the given grid cell
	/// </summary>
	std::string *MaxClass; 
	/// <summary>
	/// the dynamic integer array that holds the integer value that identifies the most likely fuzzy out put class (MacMillan, 2003)
	/// </summary>
	int *MaxCode;	
	/// <summary>
	/// The numerical value of the highest fuzzy membership value corresponding to a grid cell
	/// </summary>
	int *MaxValue;	
	/// <summary>
	/// a prefix to the name of the output *.csv file 
	/// </summary>
	std::string prefix; 
private:
	/// <summary>
	/// Number of grid cells in the DEM file
	/// </summary>
	int length;
};

#endif