/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** \brief Implementation o fthe EsriAsc class
*
* @author Gashaw Ayalew
*
*/

#include "../Headers/stdafx.h"
#include "../Headers/EsriAsc.h"
//#include "logger.h"

EsriAsc::EsriAsc()
{

}

EsriAsc::EsriAsc(string fname)
{
	int max = 50;
	size_t loc;
	long i, length = 0;
	string buf, buf1, buf2, buf3, buf4, temp, ext;

	printf("Opening ESRI ASCII file...");

	loc = fname.find('.', 0);
	ext = fname.substr(loc + 1, fname.length());

	if (ext != "txt" && ext != "asc")
	{
		printf("Invalid file name extension, exiting ...");
		exit(1);
	}

	ifstream ascin;
	ascin.open(fname, ios::in);

	if (ascin.is_open())
	{
		ascin.seekg(ios::beg);
		buf = nextString(ascin);
		if (_stricmp(buf.c_str(), "NCOLS") == 0)
			setNcols(stol(nextString(ascin)));
		else {
			printf("Could not read number of columns from input file, exiting...");
			exit(1);
		}
		buf = nextString(ascin);
		if (_stricmp(buf.c_str(), "NROWS") == 0)
			setNrows(stol(nextString(ascin)));
		else {
			printf("Could not read number of rows from input file, exiting...");
			exit(1);
		}
		buf1 = nextString(ascin);
		if (_stricmp(buf1.c_str(), "XLLCORNER") == 0 || _stricmp(buf1.c_str(), "XLLCENTER") == 0)
			setXllCorner(stof(nextString(ascin)));
		else {
			printf("Could not read lower-left x-coordinate from input file, exiting...");
			exit(1);
		}
		buf2 = nextString(ascin);
		if (_stricmp(buf2.c_str(), "YLLCORNER") == 0 || _stricmp(buf2.c_str(), "YLLCENTER") == 0)
			setYllCorner(stof(nextString(ascin)));
		else {
			printf("Could not read lower-left y-coordinate from input file, exiting...");
			exit(1);
		}
		buf = nextString(ascin);
		if (_stricmp(buf.c_str(), "CELLSIZE") == 0)
			setCellSize(stof(nextString(ascin)));
		else {
			printf("Could not read cell size from input file, exiting...");
			exit(1);
		}

		if (_stricmp(buf1.c_str(), "XLLCENTER") == 0)
			setXllCorner(getXllCorner() - (getCellSize() / 2.0));
		else
			;

		if (_stricmp(buf2.c_str(), "YLLCENTER") == 0)
			setYllCorner(getYllCorner() - (getCellSize() / 2.0));
		else
			;

		buf = nextString(ascin);
		if (_stricmp(buf.c_str(), "NODATA_value") == 0)
			setNoDataVal(stof(nextString(ascin)));
		else {
			printf("Could not read NODATA_value from input file, exiting...");
			exit(1);
		}

		length = getNcols() * getNrows();
		initData(length);
		i = 0;

		while (!ascin.eof())
		{
			buf = nextString(ascin);
			if (!buf.empty())setDataVal(stof(buf), i++);
		}

		if (i != length)
		{
			printf("Invalid number of data elements in the input file, exiting...");
			exit(1);
		}

		printf("ESRI ASCII file read, going on to exchange data...");
		ascin.close();
	}
	else
	{
		printf("Error reading input file, exiting ...");
		exit(1);
	}
}

long EsriAsc::getNcols()
{
	return ncols;
}

long EsriAsc::getNrows()
{
	return nrows;
}

double EsriAsc::getXllCorner()
{
	return xllcorner;
}

double EsriAsc::getYllCorner()
{
	return yllcorner;
}

double EsriAsc::getCellSize()
{
	return cellsize;
}

double EsriAsc::getNoDataVal()
{
	return noDataVal;
}

double EsriAsc::getData(long idx)
{
	return data[idx];
}

double *EsriAsc::getWholeData()
{
	return data;
}

bool EsriAsc::setNcols(long c)
{
	if (ncols = c) return true;
	else return false;
}

bool EsriAsc::setNrows(long r)
{
	if (nrows = r) return true;
	else return false;
}

bool EsriAsc::setXllCorner(double xllc)
{
	if (xllcorner = xllc) return true;
	else return false;
}

void EsriAsc::setXllCorner2(double xllc)
{
	xllcorner = xllc;
}

bool EsriAsc::setYllCorner(double yllc)
{
	if (yllcorner = yllc) return true;
	else return false;
}

bool EsriAsc::setCellSize(double cs)
{
	if (cellsize = cs)	return true;
	else return false;
}

bool EsriAsc::setNoDataVal(double nodat)
{
	if (noDataVal = nodat) return true;
	else return false;
}

bool EsriAsc::setDataVal(double val, long idx)
{
	if (data[idx] = val) return true;
	else return false;
}

bool  EsriAsc::initData(long leng)
{
	if ((data = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for data array, exiting...");
		exit(1);
	}
	return true;
}

EsriAsc::~EsriAsc()
{
	//delete[] data;
}

void EsriAsc::gobblechars(ifstream &in, char c)
{
	char cc;
	while (!in.eof())
	{
		cc = in.get();
		if (cc != c) break;
	}
	in.putback(cc);
}

string EsriAsc::nextString(ifstream &in)
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