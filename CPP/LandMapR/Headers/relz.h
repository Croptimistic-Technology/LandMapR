/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief Definition of the relief data class, Relz
\attention Do not change the order of relzHeadings elements
*/
#ifndef _RELZOBJECT_H_
#define _RELZOBJECT_H_
#include <iostream>
#include <fstream>
//#include "logger.h"

using namespace std;

/** Definitin of the Relz Class */
class Relz
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Relz();	
	/// <summary>
	/// Constructor with known number of grid cells
	/// </summary>
	Relz(int leng);	
	/// <summary>
	/// Returns the number of grid cells
	/// </summary>
	int getLength();
	/// <summary>
	/// Set the number of grid cells
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// Writes Relz object elements to *.csv file
	/// </summary>
	void writeCSV(std::string fname);	
	/// <summary>
	/// Writes the contents of a Relz object to a *.csv map, omits cells outside the boundary of region of interest
	/// </summary>
	bool writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize);
	/// <summary>
	/// Returns void pointer to an array member of Relz object that corresponds with an idex of a relzHedings element of the same name
	/// </summary>
	void *returnPointer(int idx);	
	/// <summary>
	/// Returns used to generate the index used by Relz::returnPointer(int)
	/// </summary>
	int returnIndex(std::string heading);	
	/// <summary>
	/// Default destructor
	/// </summary>
	~Relz();	

	std::string relzHeadings[16] = { "SeqNo", "Z2ST", "Z2CR", "Z2PIT", "Z2PEAK", "Z2TOP", "ZCR2ST", "ZPIT2PEAK", "ZTOP2PIT", "PCTZ2ST", "PCTZ2PIT", "PCTZ2TOP", "PMIN2MAX", "N2ST","N2CR","N2PEAK" };		///< Headings of the *.csv files to be written

	int *SeqNo;	///< Sequence number of grid cells in DEM input
	double *Z2ST;	///< Array of values of elevation difference (relief) between a given cell and the first channel cell it is connected to
	double *Z2CR;	///< Array of values of elevation differnce between a given cell and the first divide cell it is connected by a defined flow path
	double *Z2PIT;	///< Array of values of elevation difference between a given cell and the local first order pit cell at which flow would terminate if the pit were not removed
	double *Z2PEAK;	///< Array of values of elevation difference between a given cell and a local peak cell at which up-slope flow would terminate
	double *Z2TOP;	///< Array of values of elevation difference between a given cell and a the highest point in the catchment a cell belongs to
	double *ZCR2ST;	///< Array of values of elevation difference between a divide to a channel of the flowpath that runs through a given cell
	double *ZPIT2PEAK;	///< Array of values of elevation differnce between a pit and a peak of a flow path that passes through a given grid cell
	double *ZTOP2PIT;	///< Array of values of elevation differnce between the highest to a lowest poit in a given catchment a given cell belongs to
	double *PCTZ2ST;		///< Array of values of percentage representation of Z2ST
	double *PCTZ2PIT;		///< Array of values of percentage representation of Z2PIT
	short int *PCTZ2TOP;		///< Array of values of relative relief with respect to the highest and lowest elevation  in the watershed in which the cell is located
	short int *PMIN2MAX;		///< Array of values of relative relief with respect to the highest and lowest elevation in the entire DEM data set
	short int *N2ST;
	short int *N2CR;
	short int *N2PEAK;

	int numCols;
private:
	int length;
};

#endif
