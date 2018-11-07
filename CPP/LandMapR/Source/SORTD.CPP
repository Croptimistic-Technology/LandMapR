/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// SortD.cpp: implementation of the CSortD class.
//
//////////////////////////////////////////////////////////////////////

#include "../Headers/stdafx.h"
//#include "landmap.h"
#include "../Headers/SortD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

void CSortD::build_list(ARY_LIST input, int *piDEMSort)
{
	for (int i = 0; i < n; i++)
	{
		piDEMSort[i] = i;
		A[i] = input[i];
	}
}

//
//  debug_print ():
//    Print the array list A[n] for each iteration
//
//void CSortD::debug_print (int  iteration, int debug,
//                        char *hdr, int *piDEMSort)
//{
//	FILE *hDebug;
//
//	hDebug = fopen("debug.txt", "w");
//
//	if (debug == 0)
//		fprintf(hDebug, "\n %s \n", hdr);
//	else
//		fprintf(hDebug, " Pass #%d:", iteration + 1);
//	for (int i = 0; i < n; i++)
//		fprintf(hDebug, " %f", A[i]);
//	fprintf(hDebug, "\n");
//	for (int i = 0; i < n; i++)
//		fprintf(hDebug, " %d", piDEMSort[i]);
//	fprintf(hDebug, "\n");
//
//	fclose(hDebug);
//}

//
//  qsort(): Perform quick sort 
//
void CSortD::qsort(int First, int Last, int *piDEMSort)
{
	int			i;
	int			j;
	int			iTmp;
	DATA_TYPE_D	zTmp;

	for (i = First; i < Last; i++)
	{
		for (j = i + 1; j <= Last; j++)
		{
			if (A[i] >= A[j])
			{
				zTmp = A[i];
				A[i] = A[j];
				A[j] = zTmp;
				iTmp = piDEMSort[i];
				piDEMSort[i] = piDEMSort[j];
				piDEMSort[j] = iTmp;
			}
		}
	}
}

void CSortD::Quick_Sort(ARY_LIST input, int *piDEMSort)
{
	build_list(input, piDEMSort);  // Build array A
	//debug_print (0, 0,
	//  "List to be sorted in ascending order:", piDEMSort);
	if (n > 0)
		qsort(0, n - 1, piDEMSort);
}

