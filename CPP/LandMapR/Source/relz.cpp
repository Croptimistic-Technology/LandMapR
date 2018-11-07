/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/relz.h"


Relz::Relz()
{

}

Relz::Relz(int leng)
{

	setLength(leng);
	numCols = 15;
	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
		exit(1);
	}
	if ((Z2ST = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Z2ST, exiting ...");
		exit(1);
	}
	if ((Z2CR = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Z2CR, exiting ...");
		exit(1);
	}
	if ((Z2PIT = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Z2PIT, exiting ...");
		exit(1);
	}
	if ((Z2PEAK = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Z2PEAK, exiting ...");
		exit(1);
	}
	if ((Z2TOP = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Z2TOP, exiting ...");
		exit(1);
	}
	if ((ZCR2ST = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for ZCR2ST, exiting ...");
		exit(1);
	}
	if ((ZPIT2PEAK = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for ZPIT2PEAK, exiting ...");
		exit(1);
	}
	if ((ZTOP2PIT = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for ZTOP2PIT, exiting ...");
		exit(1);
	}
	if ((PCTZ2ST = new double[leng]) == NULL)		//modified to double * to conform to data types in facetprocess.h/.cpp
	{
		printf("Unable to allocate memory for PCTZ2ST, exiting ...");
		exit(1);
	}
	if ((PCTZ2PIT = new double[leng]) == NULL)	//modified to double * to conform to data types in facetprocess.h/.cpp
	{
		printf("Unable to allocate memory for PCTZ2PIT, exiting ...");
		exit(1);
	}
	if ((PCTZ2TOP = new short int[leng]) == NULL)	//modified to double * to conform to data types in facetprocess.h/.cpp
	{
		printf("Unable to allocate memory for PCTZ2TOP, exiting ...");
		exit(1);
	}
	if ((PMIN2MAX = new short int[leng]) == NULL)	//modified to double * to conform to data types in facetprocess.h/.cpp
	{
		printf("Unable to allocate memory for PMIN2MAX, exiting ...");
		exit(1);
	}
	if ((N2ST = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for N2ST, exiting ...");
		exit(1);
	}
	if ((N2CR = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for N2CR, exiting ...");
		exit(1);
	}
	if ((N2PEAK = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for N2CR, exiting ...");
		exit(1);
	}
}

int Relz::getLength()
{

	return length;
}

void Relz::setLength(int leng)
{

	length = leng;
}

void Relz::writeCSV(std::string fname)
{

	int i, relzlength = getLength();

	ofstream relz;
	relz.open(fname, ios::trunc);

	relz << "SeqNo" << "," << "Z2ST" << "," << "Z2CR" << "," << "Z2PIT" << "," << "Z2PEAK" << "," << "Z2TOP" << "," << "ZCR2ST" << "," << "ZPIT2PEAK" << "," << "ZTOP2PIT" << "," << "PCTZ2ST" << "," << "PCTZ2PIT" << "," << "PCTZ2TOP" << "," << "PMIN2MAX" << "," << "N2ST" << "," <<
		"N2CR" << "," << "N2PEAK" << "\n";

	for (i = 0; i < relzlength; i++) {
		relz << SeqNo[i] << "," << Z2ST[i] << "," << Z2CR[i] << "," << Z2PIT[i] << "," << Z2PEAK[i] << "," << Z2TOP[i] << "," << ZCR2ST[i] << "," << ZPIT2PEAK[i] << "," << ZTOP2PIT[i] << "," << PCTZ2ST[i] << "," << PCTZ2PIT[i] << "," << PCTZ2TOP[i] << "," << PMIN2MAX[i] << "," <<
			N2ST[i] << "," << N2CR[i] << "," << N2PEAK[i] << "\n";
	}

	relz.close();
}

bool Relz::writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize)
{

	long i, relzlength = getLength();
	double easting, northing;

	ofstream relz;
	relz.open(fname, ios::trunc);

	relz << "Easting" << "," << "Northing" << "," << "SeqNo" << "," << "Z2ST" << "," << "Z2CR" << "," << "Z2PIT" << "," << "Z2PEAK" << "," << "Z2TOP" << "," << "ZCR2ST" << "," << "ZPIT2PEAK" << "," << "ZTOP2PIT" << "," << "PCTZ2ST" << "," << "PCTZ2PIT" << "," << "PCTZ2TOP" << "," << "PMIN2MAX" << "," << "N2ST" << "," <<
		"N2CR" << "," << "N2PEAK" << "\n";

	for (i = 0; i < relzlength; i++) {
		if (missing[i] == false)
		{
			easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
			northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
			relz.precision(12);

			relz << easting << "," << northing << "," << SeqNo[i] << "," << Z2ST[i] << "," << Z2CR[i] << "," << Z2PIT[i] << "," << Z2PEAK[i] << "," << Z2TOP[i] << "," << ZCR2ST[i] << "," << ZPIT2PEAK[i] << "," << ZTOP2PIT[i] << "," << PCTZ2ST[i] << "," << PCTZ2PIT[i] << "," << PCTZ2TOP[i] << "," << PMIN2MAX[i] << "," <<
				N2ST[i] << "," << N2CR[i] << "," << N2PEAK[i] << "\n";
		}
	}

	relz.close();
	return true;
}

void *Relz::returnPointer(int idx)
{

	if (idx == 0)
		return (int*)SeqNo;
	else if (idx == 1)
		return (double *)Z2ST;
	else if (idx == 2)
		return (double *)Z2CR;
	else if (idx == 3)
		return (double *)Z2PIT;
	else if (idx == 4)
		return (double *)Z2PEAK;
	else if (idx == 5)
		return (double *)Z2TOP;
	else if (idx == 6)
		return (double *)ZCR2ST;
	else if (idx == 7)
		return (double *)ZPIT2PEAK;
	else if (idx == 8)
		return (double *)ZTOP2PIT;
	else if (idx == 9)
		return (double *)PCTZ2ST;		//modified to double * to conform to data types in facetprocess.h/.cpp
	else if (idx == 10)
		return (double *)PCTZ2PIT;	//modified to double * to conform to data types in facetprocess.h/.cpp
	else if (idx == 11)
		return (short int *)PCTZ2TOP;
	else if (idx == 12)
		return (short int *)PMIN2MAX;
	else {
		printf("Error finding correct header in returnpointer in relz object, exiting...");
		exit(1);
	}
}

int Relz::returnIndex(std::string heading)
{

	bool found = false;
	int i = -1;

	for (i = 0; i < numCols; i++) {

		if (_stricmp(heading.c_str(), relzHeadings[i].c_str()) == 0) return i;
	}
	return -1;
}

Relz::~Relz() {

	delete[] SeqNo;
	delete[] Z2ST;
	delete[] Z2CR;
	delete[] Z2PIT;
	delete[] Z2PEAK;
	delete[] Z2TOP;
	delete[] ZCR2ST;
	delete[] ZPIT2PEAK;
	delete[] ZTOP2PIT;
	delete[] PCTZ2ST;
	delete[] PCTZ2PIT;
	delete[] PCTZ2TOP;
	delete[] PMIN2MAX;
	delete[] N2PEAK;
}

