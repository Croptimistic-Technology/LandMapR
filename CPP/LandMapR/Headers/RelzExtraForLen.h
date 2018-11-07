/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//#pragma once

/*
This class is used for storing the additional data used for Len calculation
*/
#ifndef _RELZEXTRAFORLENOBJECT_H_
#define _RELZEXTRAFORLENOBJECT_H_
#include <iostream>
#include <fstream>


class RelzExtraForLen {

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	RelzExtraForLen();	
	/// <summary>
	/// Constructor with known number of grid cells
	/// </summary>
	RelzExtraForLen(int leng);	
	/// <summary>
	/// Returns the number of grid cells
	/// </summary>
	int getLength();	
	/// <summary>
	/// Sets the number of grid cells
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	///< Default destructor
	/// </summary>
	~RelzExtraForLen();	
	/// <summary>
	/// Headings of additional Relz output
	/// </summary>
	std::string relzExtraHeadings[8] = { "ST_ROW", "ST_COL", "CR_ROW", "CR_COL", "PK_ROW", "PK_COL", "PIT_ROW", "PIT_COL" };

	short int *ST_ROW;
	short int *ST_COL;
	short int *CR_ROW;
	short int *CR_COL;
	short int *PK_ROW;
	short int *PK_COL;
	short int *PIT_ROW;
	short int *PIT_COL;

	int numCols;

private:
	int length;

};
#endif