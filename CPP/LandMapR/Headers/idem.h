/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** \brief Inverted DEM class definition
*/

#ifndef _IDEMOBJECT_H_
#define _IDEMOBJECT_H_
#include <iostream>
#include <fstream>
//#include "logger.h"

/** IDEM class definition */
class IDEM
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	IDEM();		
	/// <summary>
	/// Constructor with a known number of grid cells
	/// </summary>
	IDEM(int leng);
	/// <summary>
	/// Get number of grid cells in an IDEM object
	/// </summary>
	int getLength();	
	/// <summary>
	/// Set number of grid cells in an IDEM object
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// Write IDEM object members to *.csv file
	/// </summary>
	void writeCSV(std::string fname);	
	/// <summary>
	/// Write members of IDEM object to a *.csv map file, omitting cells outside the boundary of region of interest.
	/// </summary>
	bool writeCSVmap(std::string fname, long ncols, long nrows, double xll, double yll, double gridSize);	
	/// <summary>
	/// Default constructor
	/// </summary>
	~IDEM();	
	/// <summary>
	/// Headings of a *.csv file
	/// </summary>
	std::string idemHeadings[14] = { "SeqNo", "Row", "Col", "Elev", "Ddir", "Drec", "UpSlope", "ShedNo", "ShedNow", "Missing", "Edge", "Vol2Fl", "Mm2Fl", "PArea" };	
	/// <summary>
	/// Array of sequence numbers of grid cells (cf. DEM::SeqNo)
	/// </summary>
	int *SeqNo;	
	/// <summary>
	/// Array of row number of grid cells
	/// </summary>
	int *Row; 
	/// <summary>
	/// Array of column number of grid cells
	/// </summary>
	int *Col; 
	/// <summary>
	/// Array of elevation of grid cells
	/// </summary>
	double *Elev; 
	/// <summary>
	/// Array of of direction of drainage for grid cells
	/// </summary>
	int *Ddir;
	/// <summary>
	/// Array of unique record numbers of cells to which current cells drain
	/// </summary>
	int *Drec; 
	/// <summary>
	/// Array of upslope area counts (number of cells) associated with current cells
	/// </summary>
	int *UpSlope; 
	/// <summary>
	/// Array of shed numbers of grid cells after first pit removal
	/// </summary>
	int *ShedNo;
	/// <summary>
	/// Array of global shed numbers associated with current grid cells
	/// </summary>
	int *ShedNow; 
	/// <summary>
	/// Array of a boolean quantity indicating if a current cell is located inside or outside of a region of interest (true: missing, false:valid elevation)
	/// </summary>
	bool *Missing;
	/// <summary>
	/// Array of a boolean quantity indicating whether current cell is located on the edge of the grid
	/// </summary>
	bool *Edge; 
	/// <summary>
	/// Array of the ratio of volume to flood (cf. DEM::Vol2Fl)
	/// </summary>
	double *Vol2Fl; 
	/// <summary>
	/// Array of mm to flood (cf. DEM::Mm2Fl)
	/// </summary>
	double *Mm2Fl; 
	/// <summary>
	/// Array of the surface area of a pond when it first innundates the particular grid cell
	/// </summary>
	int *PArea; 
	/// <summary>
	/// Number of elements in idemHeadings array, not used in current implementation
	/// </summary>
	int numCols;	

private:
	/// <summary>
	/// Number of grid cells in the IDEM object
	/// </summary>
	int length;	

};

#endif