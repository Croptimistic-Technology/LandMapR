/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief Definition of the first audit trail class (for the second pit removal), Vold

\detailed The term "Vold", taken from \latexonly\citet{macmillan03}\endlatexonly, stands for "old values associated with removal of pits by volume".
*/
#ifndef _VOLDOBJECT_H_
#define _VOLDOBJECT_H_
#include <iostream>
#include <fstream>
//#include "stdafx.h"
//#include "logger.h"

using namespace std;

/** Definition of the VOld Class */
class VOld
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	VOld();	
	/// <summary>
	/// Constructor with known (or estimated) number of grid cell entries to be held by a VOld object
	/// </summary>
	VOld(int leng);	
	/// <summary>
	/// Returns number of grid cell entries in the VOld object
	/// </summary>
	int getLength();	
	/// <summary>
	/// Sets the nubmer of grid cell entries in the VOld object
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	///< Writes contents of a VOld object to a *.csv file
	/// </summary>
	void writeCSV(std::string fname);	
	/// <summary>
	/// Default destructor
	/// </summary>
	~VOld();	

	std::string voldHeadings[9] = { "SeqNo", "Ddir", "Drec", "UpSlope", "ShedNow", "Stage", "Urec", "Ds_Area", "Varatio" };	///< Headings of *.csv file

	int *SeqNo;		///< Array holding sequence number (cell number) of modified grid cells
	int *Ddir;		///< Array of drainage direction values before change
	int *Drec;		///< Array of drainage record number values before change
	int *UpSlope;	///< Array of upslope area before change
	int *ShedNow; ///< Array of global shed number before change
	int *Stage;	///< Array of the stage at which the change in question happened
	int *Urec;	///< 
	int *Ds_Area;	///< Array of downslope flow upslope area
	double *Varatio;	///< Array of Varatio values. Varatio is the ratio of PitVol to ShedArea. It is meant to provide a relative measure of the mm rainfall required to fill a pit from empty.
	int numCols;	///< number of column headings, not used in this implementation

private:
	int length;		///< Number of grid cell entries in the object
};

#endif