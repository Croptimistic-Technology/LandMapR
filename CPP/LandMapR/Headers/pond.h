/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief A data object that holds the second pit table as designed by \latexonly\citet{macmillan03}\endlatexonly
\detailed All variable definitions are taken from \latexonly\citet{macmillan03}\endlatexonly
*/
#ifndef _PONDOBJECT_H_
#define _PONDOBJECT_H_
#include <iostream>
#include <fstream>
//#include "logger.h"

/** Class Definition */
class Pond
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Pond();  
	/// <summary>
	/// Constructor with a known (or estimated) number of entries (pits)
	/// </summary>
	Pond(int leng); 
	/// <summary>
	/// Returns the number of pits
	/// </summary>
	int getLength();	
	/// <summary>
	/// Sets the number of entries
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// Writes pit data to a *.csv file
	/// </summary>
	void writeCSV(std::string fname);	
	/// <summary>
	/// Defalult destructor
	/// </summary>
	~Pond();	

	std::string pondHeadings[28] = { "ShedNo", "Edge", "Final", "EndPit", "ShedArea", "PitRow", "PitCol", "PitRec", "PitElev", "PourElev", "PreVol", "PitVol", "Varatio", "PitArea", "DrainsTo", "NextPit", "Becomes", "Removed", "InRow", "InCol", "InRec", "InElev", "OutRow", "OutCol", "OutRec", "OutElev", "Stage", "Visited" }; 	///< Header strings for each of the arrays

	int *ShedNo;	///< Shed number
	bool *Edge;		///< Is the cell on the edge of the grid?
	bool *Final;	///< Does a cell overspill to the outside world, and is part of a fully integrated catcment?
	int *EndPit;	///< An integer ID (unique) to which a given pit (assumed to be not an EndPit itself) overspills to when all intermediate pits take part in an integrated sufrace flow
	int *ShedArea;	///< Array of ShedArea values. ShedArea is total number of grid cells contained in an identified depressional catchment
	int *PitRow;	///< Array of grid cell row numbers of pits
	int *PitCol;	///< Array of grid cell column numbers of pits
	int *PitRec;	///< Array of PitRec values, which are the sequence number of the grid cell on the original DEM grid data
	double *PitElev;	///< Array of PitElev values, which are elevation of the central grid cells of pits
	double *PourElev;	///< Array of PourElev values. PourElev is the elevation of the pour point, the lowest grid cell in the pit, at which water stored in the pit can overspill and contribute flow into adjacent catchment. It is numerically equal to the higher of OutElev and InElev for a given grid cell.
	double *PreVol;		///< Array of PreVol values. A PreVol value is the total volume of water stored in any previously identified pit that may underlie a larger pit formed by coalescence of two or more underlying pits
	double *PitVol;		///< Array of PitVol values. A PitVol value is the total volume of a pit including those pits it subsumes
	double *Varatio;	///< Array of Varatio values. Varatio is the ratio of PitVol to ShedArea. It is meant to provide a relative measure of the mm rainfall required to fill a pit from empty.
	int *PitArea;		///< Arrray of PitArea values. PitArea is the number of grid cells covered with water when a given pit is full to overspill.
	int *DrainsTo;		///< Array of DrainsTo indices. DrainsTo is an ID number of an adjacent catchment where a given catchment is most likely to overspill.
	int *NextPit;		///< Array of NextPit indices. NextPit is an Id number of an alternate adjacent catchment that the current catchment can overspill into if "DrainsTo" catchment is already full and overspills back to the current catchment.
	int *Becomes;		///< Array of Becomes indices. Becomes is an ID of a new catchment formed when more than one smaller catchments coalesce to form a "higher order" catchment.
	bool *Removed;		///< Array of Removed flags. Removed is a boolean value indicating whether a pit has already been removed in during a pit removal exercise.
	int *InRow;			///< Array of InRow values. InRow is the row index of a grid cell at which overspill to an adjacent pit is most likely to occur.
	int *InCol;			///< Array of InCol values. InCol is the column index of a grid cell at which overspill to an adjacent pit is most likely to occur.
	int *InRec;			///< Array of InRec values. InRec is a sequence number of the grid cell in current catchment at which overspill occurs
	double *InElev;		///< Array of InElev. InElev is the higher elevation of the two cells in adjacent pits where one pit overspills into another
	int *OutRow;		///< Array of OutRow values. OutRow values are row numbers of grid cells where overspill occurs from an adjacent pit. 
	int *OutCol;		///< Array of OutCol values. OutCol values are column numbers of grid cells where overspill occurs from an adjacent pit.
	int *OutRec;		///< Array of OutRec values. OutRec values are sequence numbers of grid cells where overspill occurs from an adjacent pit.
	double *OutElev;	///< Array of OutElev values. OutElev is the lower elevation of a cell in an adjacent catchment, at which overspill can occur
	int *Stage;			///< Array of Stage values. Stage is set to 1 upon first pit removal processing
	bool *Visited;		///< Array of boolean value to indicate whether a pit has been visited. It is set to false at the start of pit removal.

	int numCols;		///< Number of data members that are written to file. It is not used in this implementation.
private:
	int length;		///< Number of pits in the Fill object (it may be an estimated value)
};

#endif
