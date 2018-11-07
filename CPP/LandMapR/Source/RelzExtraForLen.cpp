/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/RelzExtraForLen.h"

RelzExtraForLen::RelzExtraForLen()
{

}
RelzExtraForLen::RelzExtraForLen(int leng)
{
	setLength(leng);
	numCols = 9;

	if ((ST_ROW = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ST_ROW, exiting ...");
		exit(1);
	}
	if ((ST_COL = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for ST_COL, exiting ...");
		exit(1);
	}
	if ((CR_ROW = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for CR_ROW, exiting ...");
		exit(1);
	}
	if ((CR_COL = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for CR_COL, exiting ...");
		exit(1);
	}
	if ((PK_ROW = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PK_ROW, exiting ...");
		exit(1);
	}
	if ((PK_COL = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PK_COL, exiting ...");
		exit(1);
	}
	if ((PIT_ROW = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PIT_ROW, exiting ...");
		exit(1);
	}
	if ((PIT_COL = new short int[leng]) == NULL)
	{
		printf("Unable to allocate memory for PIT_COL, exiting ...");
		exit(1);
	}
}

int RelzExtraForLen::getLength()
{
	return length;
}

void RelzExtraForLen::setLength(int leng)
{
	length = leng;
}

RelzExtraForLen::~RelzExtraForLen() {

	delete[] ST_ROW;
	delete[] ST_COL;
	delete[] CR_ROW;
	delete[] CR_COL;
	delete[] PK_ROW;
	delete[] PK_COL;
	delete[] PIT_ROW;
	delete[] PIT_COL;

}
