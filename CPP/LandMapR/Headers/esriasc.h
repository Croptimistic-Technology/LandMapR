/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
* \brief Class definition for ESRI *.asc/*.txt file access class
*/
#ifndef _ESRIASC_H_
#define _ESRIASC_H_

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/** Class definition */
class EsriAsc
{
public:
	/// <summary>
	/// Default constructor 
	/// </summary>
	EsriAsc();	
	/// <summary>
	/// Cosntructor with a given file name
	/// </summary>
	EsriAsc(string fname);	
	/// <summary>
	/// Get number of columns from the file
	/// </summary>
	long getNcols();
	/// <summary>
	/// Get number of rows from file
	/// </summary>
	long getNrows();	
	/// <summary>
	/// get lower left corner's x-coordinate
	/// </summary>
	double getXllCorner();	
	/// <summary>
	/// gets lower left corner's y-coordinate
	/// </summary>
	double getYllCorner();	
	/// <summary>
	/// gets grid cell size
	/// </summary>
	double getCellSize();	
	/// <summary>
	/// get elevation value for cells outside the boundary of region of interest
	/// </summary>
	double getNoDataVal();	
	/// <summary>
	/// gets value at a given index
	/// </summary>
	double getData(long idx); 
	/// <summary>
	/// get the whold data into a 1x(nCols x nRows) array
	/// </summary>
	double *getWholeData(); 
	/// <summary>
	/// set number of columns
	/// </summary>
	bool setNcols(long c); 
	/// <summary>
	/// set number of rows
	/// </summary>
	bool setNrows(long r); 
	/// <summary>
	/// set x-coordinate of the lower left corner
	/// </summary>
	bool setXllCorner(double xllc);
	/// <summary>
	/// set x-coordinate of the lower left corner (simpler implementation)
	/// </summary>
	void setXllCorner2(double xllc);	
	/// <summary>
	/// set y-coordinate of the lower left corner
	/// </summary>
	bool setYllCorner(double yllc);	
	/// <summary>
	/// set grid size
	/// </summary>
	bool setCellSize(double cs);	
	/// <summary>
	///set 'elevation' data for cells outside the boundary of region of interest
	/// </summary>
	bool setNoDataVal(double nodat);	
	/// <summary>
	/// set elevation data at a given sequence number on a DEM grid array
	/// </summary>
	bool setDataVal(double val, long idx); 
	/// <summary>
	/// omit a given character coming from a file stream
	/// </summary>
	void gobblechars(ifstream &in, char c); 
	/// <summary>
	/// extract next string from a stream
	/// </summary>
	string nextString(ifstream &in); 
	bool initData(long leng);	
	/// <summary>
	/// Destructor
	/// </summary>
	~EsriAsc();	

//private:
	/// <summary>
	/// Number of columns in a grid
	/// </summary>
	long ncols;	
	/// <summary>
	/// Number of rows in a grid
	/// </summary>
	long nrows; 
	/// <summary>
	/// lower left corner's x-coordinates
	/// </summary>
	double xllcorner; 
	/// <summary>
	/// lower left corner's y-coordinates
	/// </summary>
	double yllcorner; 
	/// <summary>
	/// grid cell size
	/// </summary>
	double cellsize; 
	/// <summary>
	/// 'elevation' value of cells outside the boundary of the region of interest
	/// </summary>
	double noDataVal; 
	/// <summary>
	/// DEM grid array, holding the elevation of all grid cells
	/// </summary>
	double *data; 
};

#endif