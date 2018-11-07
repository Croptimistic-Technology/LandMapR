/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/ioconductor.h"
//#include "strvect.h"

IOConductor::IOConductor()
{
}

IOConductor::~IOConductor()
{
}

double *IOConductor::extractDEM(string m_WorkingDirectory, string m_FileName, long m_Column, long m_Row)
{
	int m_iFileType;
	string sTmp, fnamext;
	double *pfDEMGrid;

	sTmp = m_WorkingDirectory + m_FileName;
	printf("Reading DEM file, %s ...\n", sTmp);

	fnamext = m_FileName.substr(m_FileName.find('.', 0) + 1, m_FileName.length());

	if (_stricmp(fnamext.c_str(), "ASC") == 0 || _stricmp(fnamext.c_str(), "TXT") == 0) m_iFileType = 0;
	else if (_stricmp(fnamext.c_str(), "FLT") == 0 || _stricmp(fnamext.c_str(), "HDR") == 0) m_iFileType = 1;
	else if (_stricmp(fnamext.c_str(), "CSV") == 0) m_iFileType = 2;
	else if (_stricmp(fnamext.c_str(), "DBF") == 0)  m_iFileType = 3;
	else {
		printf("Invalid file type, exiting ...");
		exit(1);
	}

	switch (m_iFileType)
	{
	case 0:			//ESRI *.asc or *.txt file
	{
		EsriAsc in(sTmp);

		if (m_Row != in.getNrows()) m_Row = in.getNrows();
		if (m_Column != in.getNcols()) m_Column = in.getNcols();

		/*
		if ((pfDEMGrid = new double[m_Column * m_Row]) == NULL)
		{
			printf("Unable to allocate memory for ascii elevation data, exiting ...");
			exit(1);
		}
		*/
		pfDEMGrid = in.getWholeData();
	}
	break;

	case 1:			//ESRI *.flt file
	{
		EsriFlt in(sTmp);

		if (m_Row != in.getNrows()) m_Row = in.getNrows();
		if (m_Column != in.getNcols()) m_Column = in.getNcols();

		/*
		if ((pfDEMGrid = new double[m_Column * m_Row]) == NULL)
		{
			printf("Unable to allocate memory for float elevation data, exiting ...");
			exit(1);
		}
		*/
		pfDEMGrid = in.getWholeData();
	}
	break;

	case 2:			//comma separated values, *.csv file
	{
		long j = 0;
		string tempstr;

		ifstream myFile;
		myFile.open(sTmp, ios_base::in);
		if (!myFile.is_open())
		{
			printf("Unable to open input CSV file, exiting ...");
			exit(1);
		}

		if ((pfDEMGrid = new double[m_Column * m_Row]) == NULL)
		{
			printf("Unable to allocate memory for elevation data, exiting ...");
			exit(1);
		}

		//read heading
		myFile.seekg(0, ios_base::beg);
		getline(myFile, tempstr);

		//read data
		while (!myFile.eof())
		{
			getline(myFile, tempstr);
			if (!tempstr.empty()) pfDEMGrid[j++] = stof(tempstr);
		}
		if (j != (m_Column * m_Row))
		{
			printf("Invalid number of entries in CSV file, exiting ...");
			exit(1);
		}

		myFile.close();
	}
	break;

	case 3:			// *.dbf file type
	{
		long totalpoints = m_Column * m_Row;
		ifstream myFile;
		myFile.open(sTmp, ios_base::in);
		if (!myFile.is_open())
		{
			printf("Unable to open input database file, exiting ...");
			exit(1);
		}
		int m_Col = m_Column;
		char szBuffer[255];

		myFile.seekg(4, ios_base::beg);
		myFile.read(szBuffer, 4);

		int* pShortInt = new int;
		*pShortInt = *(int*)&szBuffer;

		myFile.seekg(32, ios_base::beg);
		int* nElevLocation = new int;
		*nElevLocation = -1;

		int nFieldLength[35];
		int nLoop = 0;

		for (nLoop = 0; nLoop < 100; nLoop++)
		{
			myFile.read(szBuffer, 32);

			if (szBuffer[0] != 13)
			{
				if (_stricmp(szBuffer, "elev") == 0)
				{
					*nElevLocation = nLoop;
				}
				nFieldLength[nLoop] = szBuffer[16];
			}
			else
			{
				nLoop = 100;
			}
		}

		if (*nElevLocation == -1)
		{
			printf("Could not locate elev field, exiting ... ");
			exit(1);
		}

		myFile.seekg(8, ios_base::beg);
		myFile.read(szBuffer, 2);

		short int* piFirstRecordLoc = new short int;
		*piFirstRecordLoc = *(short int*)&szBuffer;

		myFile.seekg(10, ios_base::beg);
		myFile.read(szBuffer, 2);

		short int* piRecordLength = new short int;
		*piRecordLength = *(short int*)&szBuffer;

		myFile.seekg(*piFirstRecordLoc +
			(*pShortInt - 1) * (*piRecordLength), ios_base::beg);
		myFile.read(szBuffer, *piRecordLength);

		char szNumBuffer[8] = { '0','0','0','0','0','0','0','0' };
		int nBitesOfSet = 0;
		int nIndex = 0;

		double* pTemp = new double[totalpoints];

		nBitesOfSet = 0;

		for (nLoop = 0; nLoop < *nElevLocation; nLoop++)
		{
			nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
		}

		myFile.seekg(*piFirstRecordLoc, ios_base::beg);

		char* szElevBuffer = new char[255];

		for (nLoop = 0; nLoop < totalpoints; nLoop++)
		{
			myFile.read(szBuffer, *piRecordLength);
			for (nIndex = 0; nIndex < nFieldLength[*nElevLocation]; nIndex++)
			{
				szElevBuffer[nIndex] = szBuffer[nBitesOfSet + 1 + nIndex];
			}
			szElevBuffer[nIndex] = 0;
			pTemp[nLoop] = atof(szElevBuffer);
		}

		if ((pfDEMGrid = new double[totalpoints]) == NULL)
		{
			printf("Unable to allocate memory for elevation data, exiting ...");
			exit(1);
		}
		for (nLoop = 0; nLoop < totalpoints; nLoop++)
		{
			pfDEMGrid[nLoop] = pTemp[nLoop];
		}

		delete piRecordLength;
		delete pShortInt;
		delete piFirstRecordLoc;
		delete nElevLocation;
		delete[] szElevBuffer;
		delete[] pTemp;

		myFile.close();
	}
	break;

	default:
	{
		printf("Invalid input file type, exiting ...");
		exit(1);
	}
	break;
	}

	if (getDbfDemWriteFlag() && m_iFileType != 3)
	{
		//write pfDEMGrid to dbf file
		writedbfDEMfile(m_WorkingDirectory, m_FileName, pfDEMGrid, (m_Row * m_Column));
	}

	if (getCsvDemWriteFlag() && m_iFileType != 2)
	{
		//write pfDEMGrid to csv file
		writecsvDEMfile(m_WorkingDirectory, m_FileName, pfDEMGrid, (m_Row * m_Column));
	}

	return pfDEMGrid;
}

void IOConductor::writedbfDEMfile(string m_WorkingDirectory, string m_FileName, double *pfDEMGrid, long length)
{

	long i, j = 0;
	CDbfFile *dbfdemfile;
	string dbprefix, sTemp, shortname;
	char temp, *charAry;
	charAry = new char[30];
	double tempDbl = 0.0;

	shortname = m_FileName.substr(0, m_FileName.find_last_of('.', m_FileName.npos));

	i = 1;
	dbprefix.push_back(toupper(shortname.at(0)));

	while (i < shortname.length() && j < 2)
	{
		temp = shortname.at(i++);
		if (isupper(temp))
		{
			dbprefix.push_back(temp);
			j++;
		}
	}

	//if (dbprefix.length() == 2)
		//; // do nothing
	if (dbprefix.length() == 1) dbprefix.push_back(toupper(isalpha(shortname.at(1)) ? shortname.at(1) : (isalpha(shortname.at(2) ? shortname.at(2) : shortname.at(3)))));

	sTemp = m_WorkingDirectory + dbprefix + "2elev.dbf";
	if ((dbfdemfile = new CDbfFile(const_cast<char *>(sTemp.c_str()), OVERWRITE)) == NULL)
	{
		printf("Not enough memory for DEM dbf file, exiting ... ");
		exit(1);
	}

	dbfdemfile->SetDBFColumn("Elev", 10, 4);
	dbfdemfile->Initialize();

	for (j = 0; j < length; j++)
	{
		sprintf(charAry, "%.4f", pfDEMGrid[j]);
		tempDbl = atof(charAry);
		dbfdemfile->WriteRecord(j, 0, (void *)&tempDbl);
	}
	dbfdemfile->Close();
	//free(dbfdemfile); //original
	delete dbfdemfile;
}

void IOConductor::writecsvDEMfile(string m_WorkingDirectory, string m_FileName, double *pfDEMGrid, long length)
{

	long i, j = 0;
	string dbprefix, sTemp, shortname;
	double tempDbl = 0.0;
	char temp;
	ofstream csvout;

	shortname = m_FileName.substr(0, m_FileName.find_last_of('.', m_FileName.npos));

	i = 1;
	dbprefix.push_back(toupper(shortname.at(0)));

	while (i < shortname.length() && j < 2)
	{
		temp = shortname.at(i++);
		if (isupper(temp))
		{
			dbprefix.push_back(temp);
			j++;
		}
	}

	if (dbprefix.length() == 1) dbprefix.push_back(toupper(isalpha(shortname.at(1)) ? shortname.at(1) : (isalpha(shortname.at(2) ? shortname.at(2) : shortname.at(3)))));

	sTemp = m_WorkingDirectory + dbprefix + "_elev.csv";

	csvout.open(sTemp, ios::trunc);

	csvout << "Elev" << '\n';
	csvout.precision(10);

	for (j = 0; j < length; j++)
	{
		csvout << pfDEMGrid[j] << '\n';
	}
	csvout.close();
}

string IOConductor::getNextString(ifstream &in)
{
	string out;
	char c = '\0';
	//leading wspaces
	while (true)
	{
		c = in.get();
		if (c == EOF)
			return out;
		if (!isspace(c))
			break;
	}
	in.putback(c);

	do {
		c = in.get();
		if (c == EOF) break;
		if (!isspace(c))
			out.push_back(c);
	} while (!isspace(c));
	return out;
}

void IOConductor::setDbfDemWriteFlag(bool flag)
{
	writedbfDEM = flag;
}

void IOConductor::setCsvDemWriteFlag(bool flag)
{
	writecsvDEM = flag;
}

bool IOConductor::getDbfDemWriteFlag()
{
	return writedbfDEM;
}

bool IOConductor::getCsvDemWriteFlag()
{
	return writecsvDEM;
}

