/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** Implementation of the Form class
*/
#include "../Headers/stdafx.h"
#include "../Headers/form.h"

/** Default constructor */
Form::Form()
{
}

/** Constructor with known number of grid cells */
Form::Form(int leng)
{
	setLength(leng);
	numCols = 9;
	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
		exit(1);
	}
	if ((Slope = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Slope, exiting ...");
		exit(1);
	}
	if ((Aspect = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Aspect, exiting ...");
		exit(1);
	}
	if ((Prof = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Prof, exiting ...");
		exit(1);
	}
	if ((Plan = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Plan, exiting ...");
		exit(1);
	}
	if ((QArea = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for QArea, exiting ...");
		exit(1);
	}
	if ((QWETI = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for QWETI, exiting ...");
		exit(1);
	}
	if ((LNQArea = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for LNQArea, exiting ...");
		exit(1);
	}
	if ((New_Asp = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for New_Asp, exiting ...");
		exit(1);
	}

}

/** Sets number of grid cells */
void Form::setLength(int leng)
{

	length = leng;
}

/** Writes Form object members to *.csv file */
void Form::writeCSV(string fname) {

	int i, formlength = getLength();

	ofstream form;
	form.open(fname, ios::trunc);

	form << "SeqNo" << "," << "Slope" << "," << "Aspect" << "," << "Prof" << "," << "Plan" << "," << "QArea" << "," << "QWETI" << "," << "LNQArea" << "," << "New_Asp" << "\n";

	for (i = 0; i < formlength; i++) {
		form << SeqNo[i] << "," << Slope[i] << "," << Aspect[i] << "," << Prof[i] << "," << Plan[i] << "," << QArea[i] << "," << QWETI[i] << "," << LNQArea[i]
			<< "," << New_Asp[i] << "\n";
	}
	form.close();
}

/** Writes contents of a Form object to a *.csv map file */
bool Form::writeCSVmap(std::string fname, bool *missing, long ncols, long nrows, double xll, double yll, double gridSize)
{

	long i, formlength = getLength();
	double easting, northing;

	ofstream form;
	form.open(fname, ios::trunc);

	form << "Easting" << "," << "Northing" << "," << "SeqNo" << "," << "Slope" << "," << "Aspect" << "," << "Prof" << "," << "Plan" << "," << "QArea" << "," << "QWETI" << ","
		<< "LNQArea" << "," << "New_Asp" << "\n";

	for (i = 0; i < formlength; i++) {

		if (missing[i] == false)
		{
			easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
			northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
			form.precision(12);

			form << easting << "," << northing << "," << SeqNo[i] << "," << Slope[i] << "," << Aspect[i] << "," << Prof[i] << "," << Plan[i] << "," << QArea[i] << "," << QWETI[i] << "," << LNQArea[i] << ","
				<< New_Asp[i] << "\n";
		}
	}
	form.close();

	return true;
}

/** Returns number of grid cells */
int Form::getLength()
{

	return length;
}

/** Returns a void pointer to a member whose index is given as idx */
void *Form::returnPointer(int idx)
{

	if (idx == 0)
		return (int *)SeqNo;
	else if (idx == 1)
		return (double *)Slope;
	else if (idx == 2)
		return (int *)Aspect;
	else if (idx == 3)
		return (double *)Prof;
	else if (idx == 4)
		return (double *)Plan;
	else if (idx == 5)
		return (double *)QArea;
	else if (idx == 6)
		return (double *)QWETI;
	else {
		printf("Error finding correct header in returnpointer() in form, exiting...");
		exit(1);
	}
}

/** Returns an index corresponding to a heading element */
int Form::returnIndex(std::string heading)
{

	bool found = false;
	int i;

	for (i = 0; i < numCols; i++)
	{
		if (_stricmp(heading.c_str(), formHeadings[i].c_str()) == 0)
		{
			return i;
		}
	}
	return -1;
}

/** Destructor */
Form::~Form()
{
	delete[] SeqNo;
	delete[] Slope;
	delete[] Aspect;
	delete[] Prof;
	delete[] Plan;
	delete[] QArea;
	delete[] QWETI;
	delete[] LNQArea;
	delete[] New_Asp;
}
