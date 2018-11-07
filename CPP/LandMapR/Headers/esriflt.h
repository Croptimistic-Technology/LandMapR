/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** \brief Class definition for the EsriFlt *.flt ( and *.hdr, joint) file access Class
*/
#ifndef _ESRIFLT_H_
#define _ESRIFLT_H

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
//#include "logger.h"

using namespace std;
/** 
EsriFlt Class definition 
*/
class EsriFlt 
{
public:
	/// <summary>
	/// Default constructor 
	/// </summary>
	EsriFlt(); 
	/// <summary>
	/// Constructor with a given file name for the floating point grid data file
	/// </summary>
	EsriFlt(string fname);	
	/// <summary>
	/// gets number of columns
	/// </summary>
	long getNcols(); 
	/// <summary>
	/// gets Number of rows
	/// </summary>
	long getNrows(); 
	/// <summary>
	/// gets the lower left corner's x-coordinate
	/// </summary>
	double getXllCorner();	
	/// <summary>
	/// gets the lower left corner's y-coordinate
	/// </summary>
	double getYllCorner();	
	/// <summary>
	/// gets grid cell size
	/// </summary>
	double getCellSize();	
	/// <summary>
	/// gets 'elevation' value for grid cells outside the boundary of region of interest
	/// </summary>
	double getNoDataVal();	
	/// <summary>
	/// gets elevation data at the 'idx' indes of the array 'data'
	/// </summary>
	double getData(long idx); 
	/// <summary>
	/// gets the whole array
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
	/// set lower left corner's x-coordinate
	/// </summary>
	bool setXllCorner(double xllc); 
	/// <summary>
	/// set lower left corner's 
	/// </summary>
	bool setYllCorner(double yllc);	
	/// <summary>
	/// set grid cell size
	/// </summary>
	bool setCellSize(double cs);	
	/// <summary>
	/// set 'elevation' value of grid cells outside the boundary of region of interest
	/// </summary>
	bool setNoDataVal(double nodat);	
	/// <summary>
	/// set value, in array 'data', at index 'idx' with a value of 'val'
	/// </summary>
	void setDataVal(double val, long idx);	
	/// <summary>
	/// initialize the array to hold grid elevation data
	/// </summary>
	bool initData(long leng); 
	/// <summary>
	/// extract next string from an input file stream
	/// </summary>
	string nextString(ifstream &in); 
	/// <summary>
	/// set the byteOrder value for the given input file
	/// </summary>
	void setByteOrder(string byteorder);
	/// <summary>
	/// get the byteOrder value 
	/// </summary>
	string getByteOrder();	
	/// <summary>
	/// convert binary to IEEE single precision floating point number
	/// </summary>
	double bin2SPfloat(bitset <32> in) const;	
	/// <summary>
	/// convert single precision floating point number to binary
	/// </summary>
	bitset<32> spfloat2bin(double flt);
	/// <summary>
	/// find the two's complement of a bitset number
	/// </summary>
	bitset <8> twosComplement(bitset<8> in);	
	/// <summary>
	/// convert integer values to binary
	/// </summary>
	bitset <32> int2bin(int in);	
	/// <summary>
	/// Destructor
	/// </summary>
	~EsriFlt();	
	/// <summary>
	/// number of columns in the input grid
	/// </summary>
	long ncols;	
	/// <summary>
	/// number of rows in the input grid
	/// </summary>
	long nrows; 
	/// <summary>
	/// x-coordinate of the lower left corner of the grid
	/// </summary>
	double xllcorner; 
	/// <summary>
	/// y-coordinate of the lower left corner of the grid
	/// </summary>
	double yllcorner; 
	/// <summary>
	/// grid cell size (m)
	/// </summary>
	double cellsize; 
	/// <summary>
	/// "elevation" value at grid cells outside the boundary of region of interest
	/// </summary>
	double noDataVal;	
	/// <summary>
	/// array to hold the DEM grid data
	/// </summary>
	double *data;	
	/// <summary>
	/// byteorder for writing grid data to the input binary file (MSBFIRST and LSBFIRST tested, but VMS_FFLOAT not)
	/// </summary>
	string byteOrder;	
};

#endif