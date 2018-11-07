/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/vold.h"

VOld::VOld()
{

}


VOld::VOld(int leng)
{

	setLength(leng);
	numCols = 9;
	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
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
	if ((ShedNow = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ShedNow, exiting ...");
		exit(1);
	}
	if ((Stage = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Stage, exiting ...");
		exit(1);
	}
	if ((Urec = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Urec, exiting ...");
		exit(1);
	}
	if ((Ds_Area = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Ds_Area, exiting ...");
		exit(1);
	}
	if ((Varatio = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Varatio, exiting ...");
		exit(1);
	}
}


int VOld::getLength()
{

	return length;
}

void VOld::setLength(int leng)
{

	length = leng;
}

void VOld::writeCSV(std::string fname)
{

	int i, leng = getLength();

	ofstream out;
	out.open(fname, ios::trunc);

	out << "SeqNo" << "," << "Ddir" << "," << "Drec" << "," << "UpSlope" << "," << "ShedNow" << "," << "Stage" << "," << "Urec" << "," << "Ds_Area" << "," << "Varatio" << "\n";

	for (i = 0; i < leng; i++) {
		out << SeqNo[i] << "," << Ddir[i] << "," << Drec[i] << "," << UpSlope[i] << "," << ShedNow[i] << "," << Stage[i] << "," << Urec[i] << "," << Ds_Area[i] << "," << Varatio[i] << "\n";
	}
	out.close();
}


VOld::~VOld()
{
	delete[] SeqNo;
	delete[] Ddir;
	delete[] Drec;
	delete[] UpSlope;
	delete[] ShedNow;
	delete[] Stage;
	delete[] Urec;
	delete[] Ds_Area;
	delete[] Varatio;
}
