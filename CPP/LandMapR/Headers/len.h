/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//#pragma once

#ifndef _LENOBJECT_H_
#define _LENOBJECT_H_
#include <iostream>
#include <fstream>

using namespace std;

class Len {

public:
	/// <summary>
	/// Defalult destructor
	/// </summary>
	Len();	
	/// <summary>
	/// Constructor with known number of grid cells
	/// </summary>
	Len(int leng);	
	/// <summary>
	/// Returns the number of grid cells
	/// </summary>
	int getLength();	
	/// <summary>
	/// Sets the number of grid cells
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// write the results to a CSV file
	/// </summary>
	void writeCSV(string fname); 
	/// <summary>
	/// Writes Form data to *.csv map file
	/// </summary>
	bool writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize);		
	/// <summary>
	///  Default destructor
	/// </summary>
	~Len();	

	std::string lenHeadings[9] = { "SeqNo", "L2PIT", "L2PEAK", "LPIT2PEAK", "PPIT2PEAK", "L2STR", "L2DIV", "LSTR2DIV", "PSTR2DIVL" };

	int *SeqNo;
	double *L2PIT;
	double *L2PEAK;
	double *LPIT2PEAK;
	int *PPIT2PEAKL;
	double *L2STR;
	double *L2DIV;
	double *LSTR2DIV;
	int *PSTR2DIVL;

	int numCols;

private:
	int length;

};



#endif
