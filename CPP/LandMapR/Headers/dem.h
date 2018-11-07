/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _DEMOBJECT_H_
#define _DEMOBJECT_H_

#include <iostream>
#include <fstream>
#include <string>
//#include "logger.h"

using namespace std;

/**
The DEM Class definition
*/
class DEM
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	DEM();	
	/// <summary>
	/// constructor with the number of grid cells known
	/// </summary>
	DEM(int leng);	
	/// <summary>
	/// gets number of grid cells
	/// </summary>
	int getLength();	
	/// <summary>
	/// sets number of grid cells
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// writes data to CSV file
	/// </summary>
	void writeCSV(std::string fname);
	/// <summary>
	/// writes data to a CSV map file, without including points outside the boundary of region of interest
	/// </summary>
	bool writeCSVmap(std::string fname, long ncols, long nrows, double xll, double yll, double gridSize);	
	/// <summary>
	/// Destructor
	/// </summary>
	~DEM();
	/// <summary>
	/// Headings of dem data output
	/// </summary>
	std::string demHeadings[14] = { "SeqNo", "Row", "Col", "Elev", "Ddir", "Drec", "UpSlope", "ShedNo", "ShedNow", "Missing", "Edge", "Vol2Fl", "Mm2Fl", "PArea" };	
	/// <summary>
	/// An array of sequence numbers of grid cells
	/// </summary>
	int *SeqNo;	
	/// <summary>
	/// An array of row numbers of grid cells
	/// </summary>
	int *Row;	
	/// <summary>
	/// An array of column numbers of grid cells
	/// </summary>
	int *Col;	
	/// <summary>
	/// An array of elevations at grid cells
	/// </summary>
	double *Elev;	
	/// <summary>
	/// An array of outflow/drainage direction values from grid cells
	/// </summary>
	int *Ddir;	
	/// <summary>
	/// An array of unique record numbers in the file, associated with grid cells
	/// </summary>
	int *Drec;	
	/// <summary>
	/// An array of upslope area counts for grid cells
	/// </summary>
	int *UpSlope;	
	/// <summary>
	/// An array of local catchments to which grid cells are assigned
	/// </summary>
	int *ShedNo;	
	/// <summary>
	/// An array of global catchments to which grid cells are assigned
	/// </summary>
	int *ShedNow;	
	/// <summary>
	/// An array of boolean values to indicate whether cells are inside or outside a boundary of region of interest in a grid
	/// </summary>
	bool *Missing;	
	/// <summary>
	/// An array of boolean values to indicate whether cells are on the boundary of the grid 
	/// </summary>
	bool *Edge;	
	/// <summary>
	/// An array of values to record the volume (in cu-m) of water the required to innundate a depression at each of the grid cells
	/// </summary>
	double *Vol2Fl;	
	/// <summary>
	/// An array of approximate values to record the mm of rainfall required to innundate cells of the grid
	/// </summary>
	double *Mm2Fl;	
	/// <summary>
	/// An array of numbers to hold the surface area of ponds, whereby the pond (corresponding to a particular cell) first innundates
	/// </summary>
	int *PArea;	
	/// <summary>
	/// Not used in this implementation
	/// </summary>
	int numCols;	

private:
	/// <summary>
	/// Number of grid cells
	/// </summary>
	int length;	

};

#endif

