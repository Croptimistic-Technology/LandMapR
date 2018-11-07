/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
*\brief Implementation of the DEM class
* This class creates a data obect that is used to carry information from the flow topology computation module all the way to the WEPP module, and called on the way, as required.
*/
#include "../Headers/stdafx.h"
#include "../Headers/dem.h"


/**
Default constructor
*/
DEM::DEM()
{
}

/** \brief Constructor for a known number of grid cells. It allocates all dynamic arrays to hold data corresponding to each DEM grid cell
*
* @param leng: number of grid cells
*/
DEM::DEM(int leng)
{

	setLength(leng);
	numCols = 14;
	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
		exit(1);
	}
	if ((Row = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Row, exiting ...");
		exit(1);
	}
	if ((Col = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Col, exiting ...");
		exit(1);
	}
	if ((Elev = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Elev, exiting ...");
		exit(1);
	}
	if ((Ddir = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Ddir, exiting ...");
		exit(1);
	}
	if ((Drec = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Drec, exiting ...");
		exit(1);
	}
	if ((UpSlope = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for UpSlope, exiting ...");
		exit(1);
	}
	if ((ShedNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ShedNo, exiting ...");
		exit(1);
	}
	if ((ShedNow = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ShedNow, exiting ...");
		exit(1);
	}
	if ((Missing = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Missing, exiting ...");
		exit(1);
	}
	if ((Edge = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Edge, exiting ...");
		exit(1);
	}
	if ((Vol2Fl = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Vol2Fl, exiting ...");
		exit(1);
	}
	if ((Mm2Fl = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Mm2Fl, exiting ...");
		exit(1);
	}
	if ((PArea = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PArea, exiting ...");
		exit(1);
	}
}


int DEM::getLength() ///< Get number of grid cells
{

	return length;
}

/**
* \brief Set number of grid cells
* @param leng number of grid cells
*/
void DEM::setLength(int leng)
{

	length = leng;
}


void DEM::writeCSV(string fname)	///< Write DEM data to CSV file (note no coordinates are associated with the data in this case).
{

	int i, leng = getLength();

	ofstream out;
	out.open(fname, ios::trunc);
	out << "SeqNo" << "," << "Row" << "," << "Col" << "," << "Elev" << "," << "Ddir" << "," << "Drec" << "," << "UpSlope" << "," << "ShedNo" << "," << "ShedNow" << "," << "Missing" << "," << "Edge" << "," << "Vol2Fl" << "," << "Mm2Fl" << "," << "PArea" << "\n";

	for (i = 0; i < leng; i++) {

		out << SeqNo[i] << "," << Row[i] << "," << Col[i] << "," << Elev[i] << "," << Ddir[i] << "," << Drec[i] << "," << UpSlope[i] << "," << ShedNo[i] << "," << ShedNow[i] << "," << Missing[i] << "," << Edge[i] << "," << Vol2Fl[i] << "," << Mm2Fl[i] << "," << PArea[i] << "\n";
	}

	out.close();
}

/** \brief Write DEM data to a CSV data file, where a pair of corrdinates are associated with each grid cell, and no other points than those inside the boundary of the region of interest are written to the file */
bool DEM::writeCSVmap(string fname, long ncols, long nrows, double xll, double yll, double gridSize)
{
	long i, leng = getLength();
	double easting, northing;

	ofstream out;
	out.open(fname, ios::trunc);
	out << "Easting" << ',' << "Northing" << ',' << "SeqNo" << "," << "Row" << "," << "Col" << "," << "Elev" << "," << "Ddir" << "," << "Drec" << "," << "UpSlope" << "," << "ShedNo" << "," << "ShedNow" << "," << "Edge" << "," << "Vol2Fl" << "," << "Mm2Fl" << "," << "PArea" << "\n";

	for (i = 0; i < leng; i++) {
		if (((bool)Missing[i]) != true) {
			easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
			northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
			out.precision(12);
			out << easting << ',' << northing << ',' << SeqNo[i] << "," << Row[i] << "," << Col[i] << "," << Elev[i] << "," << Ddir[i] << "," << Drec[i] << "," << UpSlope[i] << "," << ShedNo[i] << "," << ShedNow[i] << "," << Edge[i] << "," << Vol2Fl[i] << "," << Mm2Fl[i] << "," << PArea[i] << "\n";
		}
	}
	out.close();
	return true;
}


DEM::~DEM()	///< Destructor
{
	delete[] SeqNo;
	delete[] Row;
	delete[] Col;
	delete[] Elev;
	delete[] Ddir;
	delete[] Drec;
	delete[] UpSlope;
	delete[] ShedNo;
	delete[] ShedNow;
	delete[] Missing;
	delete[] Edge;
	delete[] Vol2Fl;
	delete[] Mm2Fl;
	delete[] PArea;
}
