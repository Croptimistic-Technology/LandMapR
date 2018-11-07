/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/len.h"

Len::Len()
{

}

Len::Len(int leng)
{
	setLength(leng);
	numCols = 9;

	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
		exit(1);
	}
	if ((L2PIT = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for L2PIT, exiting ...");
		exit(1);
	}
	if ((L2PEAK = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for L2PEAK, exiting ...");
		exit(1);
	}
	if ((LPIT2PEAK = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for LPIT2PEAK, exiting ...");
		exit(1);
	}
	if ((PPIT2PEAKL = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PPIT2PEAKL, exiting ...");
		exit(1);
	}
	if ((L2STR = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for L2STR, exiting ...");
		exit(1);
	}
	if ((L2DIV = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for L2DIV, exiting ...");
		exit(1);
	}
	if ((LSTR2DIV = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for LSTR2DIV, exiting ...");
		exit(1);
	}
	if ((PSTR2DIVL = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PSTR2DIVL, exiting ...");
		exit(1);
	}
}

int Len::getLength()
{
	return length;
}

void Len::setLength(int leng)
{
	length = leng;
}

/** Writes Form object members to *.csv file * */
void Len::writeCSV(string fname) {

	int i, formlength = getLength();

	ofstream form;
	form.open(fname, ios::trunc);

	form << "SeqNo" << "," << "L2PIT" << "," << "L2PEAK" << "," << "LPIT2PEAK" << "," << "PPIT2PEAKL" << "," << "L2STR" << "," << "L2DIV" << "," << "LSTR2DIV" << "," << "PSTR2DIVL" << "\n";

	for (i = 0; i < formlength; i++) {
		form << SeqNo[i] << "," << L2PIT[i] << "," << L2PEAK[i] << "," << LPIT2PEAK[i] << "," << PPIT2PEAKL[i] << "," << L2STR[i] << "," << L2DIV[i] << "," << LSTR2DIV[i]
			<< "," << PSTR2DIVL[i] << "\n";
	}
	form.close();
}
bool Len::writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize)
{

	long i, lenlength = getLength();
	double easting, northing;

	ofstream len;
	len.open(fname, ios::trunc);

	len << "Easting" << "," << "Northing" << "," << "SeqNo" << "," << "L2PIT" << "," << "L2PEAK" << "," << "LPIT2PEAK" << "," << "PPIT2PEAKL" << "," << "L2STR" << "," << "L2DIV" << "," << "LSTR2DIV" << "," << "PSTR2DIVL" << "\n";

	for (i = 0; i < lenlength; i++) {
		if (missing[i] == false)
		{
			easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
			northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
			len.precision(12);
			len << easting << "," << northing << "," << SeqNo[i] << "," << L2PIT[i] << "," << L2PEAK[i] << "," << LPIT2PEAK[i] << "," << PPIT2PEAKL[i] << "," << L2STR[i] << "," << L2DIV[i] << "," << LSTR2DIV[i]
				<< "," << PSTR2DIVL[i] << "\n";
		}
	}
	len.close();
	return true;

}

Len::~Len() {

	delete[] SeqNo;
	delete[] L2PIT;
	delete[] L2PEAK;
	delete[] LPIT2PEAK;
	delete[] PPIT2PEAKL;
	delete[] L2STR;
	delete[] L2DIV;
	delete[] LSTR2DIV;
	delete[] PSTR2DIVL;

}


