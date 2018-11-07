/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief Definition of the IOConductor class
\detailed This class defines the object that coordinates the input DEM files and the export of DEM data into *.dbf or *.csv files
*/

#ifndef _IOCONDUCTOR_H_
#define _IOCONDUCTOR_H_

#include <iostream>
#include <fstream>
//#include <string>
//#include "strvect.h"
#include "dbffile.h"
#include "esriasc.h"
#include "esriflt.h"

/** Definition of the IOConductor class */
class IOConductor
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	IOConductor();	
	/// <summary>
	/// Entry point to the DEM data extraction method
	/// </summary>
	double *extractDEM(std::string m_WorkingDirectory, std::string m_FileName, long m_Column, long m_Row);	
	/// <summary>
	/// Gets a std::string from an input file stream
	/// </summary>
	std::string getNextString(std::ifstream &in);
	/// <summary>
	/// Exports DEM data to a *.dbf file
	/// </summary>
	void writedbfDEMfile(std::string m_WorkingDirectory, std::string m_FileName, double *pfDEMGrid, long length); 
	/// <summary>
	/// Exports DEM data to a *.csv file
	/// </summary>
	void writecsvDEMfile(std::string m_WorkingDirectory, std::string m_FileName, double *pfDEMGrid, long length); 
	/// <summary>
	/// Sets a flag as to whether to export a DEM data to a *.dbf file
	/// </summary>
	void setDbfDemWriteFlag(bool flag);	
	/// <summary>
	/// Sets a flag as to whether to export a DEM data to a *.csv file
	/// </summary>
	void setCsvDemWriteFlag(bool flag);	
	/// <summary>
	/// Returns the value of the flag about *.dbf export (writedbfDEM)
	/// </summary>
	bool getDbfDemWriteFlag(); 
	/// <summary>
	/// Returns the value of the flag about *.csv export (writecsvDEM)
	/// </summary>
	bool getCsvDemWriteFlag(); 
	/// <summary>
	/// Defalult destructor
	/// </summary>
	~IOConductor();	

private:
	/// <summary>
	/// Name of the input DEM file
	/// </summary>
	std::string fname;	
	/// <summary>
	/// flag about export of *.dbf DEM
	/// </summary>
	bool writedbfDEM; 
	/// <summary>
	/// flag about export of *.csv DEM
	/// </summary>
	bool writecsvDEM;
};

#endif
