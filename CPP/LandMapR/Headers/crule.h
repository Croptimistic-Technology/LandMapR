/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _CRULEOBJECT_H_
#define _CRULEOBJECT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "sort.h"

using namespace std;

/**
*\brief A class to read fuzzy classification rules file residing in the same directory as the executable (the working directory), and parse attribute weights as given in the classification rules.
*
* The classification rules file (i.e., *crule.csv) is organized in such a way that the first line holds the column headings, while the last line is a blank line (i.e., extra carriage return at the end of the data). The last entry on every line is
* ended with a carriage return, and no extra comma after the last entry on each row.
*
*/
class Crule
{
public:
	/// <summary>
	/// Names of class variables 
	/// </summary>
	std::string cruleHeadings[5] = {"F_Name", "Fuz_attr", "Attr_wt", "Facet_no", "F_code"};
	/// <summary>
	/// Default constructor
	/// </summary>
	Crule();
	/// <summary>
	/// Constructor with a known number of rules and attributes
	/// </summary>
	Crule(int leng, int dim);
	/// <summary>
	/// Constructor with a json string of rules and attributes
	/// </summary>
	Crule(char* cruleJsonStr, int dim);
	/// <summary>
	/// get the number of fuzzy rules
	/// </summary>
	int getLength();	
	/// <summary>
	/// set number of fuzzy rules in a Crule object
	/// </summary>
	void setLength(int leng); 
	/// <summary>
	/// Constructor when classification rule file is known, together with the number of distinct attributes in the rules file
	/// </summary>
	Crule(string name, int dim);	
	/// <summary>
	/// Default destructor
	/// </summary>
	~Crule();	
	/// <summary>
	/// Array to hold facet names (facet names such as FSL, TSL, LSM, etc)
	/// </summary>
	string *psf_name;	
	/// <summary>
	/// Array to hold facet name indices
	/// </summary>
	int *pifacet_no;
	/// <summary>
	/// Array to hold facet codes (integers)
	/// </summary>
	int *pif_code;
	/// <summary>
	/// Array to hold attribute weights, given in the classification rules file
	/// </summary>
	double *attr_wt;
	/// <summary>
	/// Array to hold attribute names such as hi_above, near_level, etc
	/// </summary>
	string *psfuzzattr;		
	/// <summary>
	/// Array to hold attribute numbers
	/// </summary>
	int *no_attrib;	
	/// <summary>
	/// number of columns columns in an input file, not used in this implementation
	/// </summary>
	int numCols;	
	/// <summary>
	/// equivalent to the number of entries in the classification rules file(cf.length), it is a legacy class attribute
	/// </summary>
	int no_facets;	
private:
	/// <summary>
	/// Number of non-header entries in the fuzzy classification rules file (crule.csv)
	/// </summary>
	int length;		
};

#endif