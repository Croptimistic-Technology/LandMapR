/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** Definition of the Form class
*/
#ifndef _FORMOBJECT_H_
#define _FORMOBJECT_H_
#include <fstream>
#include <iostream>
//#include "logger.h"


using namespace std;

/** Class definition */
class Form
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Form();		
	/// <summary>
	/// Constructor with known number of grid cells
	/// </summary>
	Form(int leng);
	/// <summary>
	/// Set the number of grid cells to process
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// Writes Form data to *.csv non-map file
	/// </summary>
	void writeCSV(std::string fname);	
	/// <summary>
	/// Writes Form data to *.csv map file
	/// </summary>
	bool writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize);	
	/// <summary>
	/// Gets number of grid cells
	/// </summary>
	int getLength();
	/// <summary>
	/// Returns a pointer to an array member of interest
	/// </summary>
	void *returnPointer(int idx);	
	/// <summary>
	/// Returns index of a heading string
	/// </summary>
	int returnIndex(std::string heading);	
	/// <summary>
	/// Default destructor
	/// </summary>
	~Form();		
	/// <summary>
	/// Headings of a Form table (also names of dynamic arrays). Used in returnIndex(heading).
	/// </summary>
	std::string formHeadings[9] = { "SeqNo", "Slope", "Aspect", "Prof", "Plan", "QArea", "QWETI", "LNQArea", "New_ASP" };	
	/// <summary>
	/// Sequence number of a grid cell in the input DEM data
	/// </summary>
	int *SeqNo;		
	/// <summary>
	/// Array of slopes at grid cells
	/// </summary>
	double *Slope;	
	/// <summary>
	/// Array of aspect at grid cells
	/// original Aspect was int, converted to double to make it consistent with all pointers collected in double **infield (in formprocess.h/ .cpp)
	/// </summary>
	int *Aspect;		
	/// <summary>
	/// Array of profile data at grid cells
	/// </summary>
	double *Prof;	
	/// <summary>
	/// Array of plan data at grid cells
	/// </summary>
	double *Plan;	
	/// <summary>
	/// Array of diffuse upslope area at grid cells
	/// </summary>
	double *QArea;	
	/// <summary>
	/// Array of Wettness index data at grid cells
	/// </summary>
	double *QWETI;	
	/// <summary>
	///  define a series of fuzzy attributes that express the likelihood of a cell being in a particular slope position
	/// </summary>
	double *LNQArea; 
	/// <summary>
	/// New aspect
	/// </summary>
	int *New_Asp; 
	/// <summary>
	/// Number of data arrays in the Form object
	/// </summary>
	int numCols;	

private:
	/// <summary>
	/// Number of grid cells in the DEM data
	/// </summary>
	int length;		
};

#endif