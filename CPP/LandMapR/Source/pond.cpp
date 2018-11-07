/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/pond.h"


using namespace std;

Pond::Pond()
{
}

Pond::Pond(int leng)
{

	setLength(leng);
	numCols = 28;
	if ((ShedNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ShedNo, exiting ...");
		exit(1);
	}
	if ((Edge = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Edge, exiting ...");
		exit(1);
	}
	if ((Final = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Final, exiting ...");
		exit(1);
	}
	if ((EndPit = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for EndPit, exiting ...");
		exit(1);
	}
	if ((ShedArea = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ShedArea, exiting ...");
		exit(1);
	}
	if ((PitRow = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitRow, exiting ...");
		exit(1);
	}
	if ((PitCol = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitCol, exiting ...");
		exit(1);
	}
	if ((PitRec = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitRec, exiting ...");
		exit(1);
	}
	if ((PitElev = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitElev, exiting ...");
		exit(1);
	}
	if ((PourElev = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for PourElev, exiting ...");
		exit(1);
	}
	if ((PreVol = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for PreVol, exiting ...");
		exit(1);
	}
	if ((PitVol = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitVol, exiting ...");
		exit(1);
	}
	if ((Varatio = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for Varatio, exiting ...");
		exit(1);
	}
	if ((PitArea = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PitArea, exiting ...");
		exit(1);
	}
	if ((DrainsTo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for DrainsTo, exiting ...");
		exit(1);
	}
	if ((NextPit = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for NextPit, exiting ...");
		exit(1);
	}
	if ((Becomes = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Becomes, exiting ...");
		exit(1);
	}
	if ((Removed = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Removed, exiting ...");
		exit(1);
	}
	if ((InRow = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for InRow, exiting ...");
		exit(1);
	}
	if ((InCol = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for InCol, exiting ...");
		exit(1);
	}
	if ((InRec = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for InRec, exiting ...");
		exit(1);
	}
	if ((InElev = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for InElev, exiting ...");
		exit(1);
	}
	if ((OutRow = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for OutRow, exiting ...");
		exit(1);
	}
	if ((OutCol = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for OutCol, exiting ...");
		exit(1);
	}
	if ((OutRec = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for OutRec, exiting ...");
		exit(1);
	}
	if ((OutElev = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for OutElev, exiting ...");
		exit(1);
	}
	if ((Stage = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for Stage, exiting ...");
		exit(1);
	}
	if ((Visited = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Visited, exiting ...");
		exit(1);
	}
}

int Pond::getLength()
{

	return length;
}

void Pond::setLength(int leng)
{

	length = leng;
}

void Pond::writeCSV(string fname)
{

	int i, leng = getLength();

	ofstream out;
	out.open(fname, ios::trunc);

	out << "ShedNo" << "," << "Edge" << "," << "Final" << "," << "EndPit" << "," << "ShedArea" << "," << "PitRow" << "," << "PitCol" << "," << "PitRec" << "," << "PitElev" << "," << "PourElev" << "," << "PreVol" << "," << "PitVol" << "," << "Varatio" << "," << "PitArea" << "," << "DrainsTo" << "," << "NextPit" << "," << "Becomes" << "," << "Removed" << "," << "InRow" << "," << "InCol" << "," << "InRec" << "," << "InElev" << "," << "OutRow" << "," << "OutCol" << "," << "OutRec" << "," << "OutElev" << "," << "Stage" << "," << "Visited" << "\n";

	for (i = 0; i < leng; i++) {

		out << ShedNo[i] << "," << Edge[i] << "," << Final[i] << "," << EndPit[i] << "," << ShedArea[i] << "," << PitRow[i] << "," << PitCol[i] << "," << PitRec[i] << "," << PitElev[i] << "," << PourElev[i] << "," << PreVol[i] << "," << PitVol[i] << "," << Varatio[i] << "," << PitArea[i] << "," << DrainsTo[i] << "," << NextPit[i] << "," << Becomes[i] << "," << Removed[i] << "," << InRow[i] << "," << InCol[i] << "," << InRec[i] << "," << InElev[i] << "," << OutRow[i] << "," << OutCol[i] << "," << OutRec[i] << "," << OutElev[i] << "," << Stage[i] << "," << Visited[i] << "\n";
	}
	out.close();
}


Pond::~Pond()
{
	delete[] ShedNo;
	delete[] Edge;
	delete[] Final;
	delete[] EndPit;
	delete[] ShedArea;
	delete[] PitRow;
	delete[] PitCol;
	delete[] PitRec;
	delete[] PitElev;
	delete[] PourElev;
	delete[] PreVol;
	delete[] PitVol;
	delete[] Varatio;
	delete[] PitArea;
	delete[] DrainsTo;
	delete[] NextPit;
	delete[] Becomes;
	delete[] Removed;
	delete[] InRow;
	delete[] InCol;
	delete[] InRec;
	delete[] InElev;
	delete[] OutRow;
	delete[] OutCol;
	delete[] OutRec;
	delete[] OutElev;
	delete[] Stage;
	delete[] Visited;
}
