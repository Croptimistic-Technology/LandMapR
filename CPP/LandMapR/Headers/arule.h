/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
* \brief Fuzzy attribute rules class definition
*/
#ifndef _ARULEOBJECT_H_
#define _ARULEOBJECT_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "form.h"
#include "relz.h"
//#include "dem.h"

using namespace std;

/** Fuzzy attribute rules class definition */
class Arule
{
public:
	/// <summary>
	/// Names of class variables 
	/// </summary>
	string aruleHeadings[11] = {"SortOrder", "File", "Attr_in", "Class_out", "Model_no", "B", "B_low", "B_hi", "B1", "B2", "D"}; 
	/// <summary>
	/// Default constructor 
	/// </summary>
	Arule(); 
	/// <summary>
	/// Constructor with known number of attributes 
	/// </summary>
	Arule(int leng);	
	/// <summary>
	/// Constructor when a file name, and attribute source files (form and relz in this case) are given
	/// </summary>
	Arule(string csvfname, Form *form, Relz *relz); 
	/// <summary>
	/// Constructor when a arule Json string which contains the content of the Arule table is given
	/// </summary>
	Arule(char* aruleJsonStr, Form *form, Relz *relz); 
	/// <summary>
	/// Get number of entries in a fuzzy attributes file
	/// </summary>
	int getLength();
	/// <summary>
	/// Set number of entries in a fuzzy attributes file
	/// </summary>
	void setLength(int leng);	
	/// <summary>
	/// Default destructor
	/// </summary>
	~Arule();	
	/// <summary>
	/// An array to hold indices of sorted DEM
	/// </summary>
	int *pisortorder;	
	/// <summary>
	/// A string array of input (attribute source) files that correspond (index to index) to an array of input attributes (as stored in psattr_in below)
	/// </summary>
	string *psfile; 
	/// <summary>
	/// A string array of input attributes
	/// </summary>
	string *psattr_in; 
	/// <summary>
	/// A string array of output attributes
	/// </summary>
	string *psattr_out;	
	/// <summary>
	/// Array of one of the five models given by MacMillan (2003)
	/// </summary>
	int *pimodel_no; 
	/// <summary>
	/// Array of parameter B
	/// </summary>
	double *pdb;	
	/// <summary>
	/// Array of parameter B_low
	/// </summary>
	double *pdb_low;
	/// <summary>
	/// Array of parameter B_high
	/// </summary>
	double *pdb_hi;		
	/// <summary>
	/// Array of parameter B1
	/// </summary>
	double *pdb1;	
	/// <summary>
	/// Array of parameter B2
	/// </summary>
	double *pdb2;	
	/// <summary>
	/// Array of the dispersion coefficient
	/// </summary>
	double *pdd;	
	/// <summary>
	/// Array holding pointers to the input field arrays
	/// </summary>
	double **infield;	
	/// <summary>
	/// Number of fuzzy models used in the fuzzy attributes file (it counts those models based on all attribute source files)
	/// </summary>
	int no_models;	
	/// <summary>
	/// number of headings, but not used in this implementation
	/// </summary>
	int numCols;	

private:
	/// <summary>
	/// Number of entries in the fuzzy rules attributes file
	/// </summary>
	int length;		
};

#endif