/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// SortD.h: interface for the CSortD class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
//#if !defined(AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_)
//#define AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	double DATA_TYPE_D;

class CSortD
{
public:

	CSortD(int size) { iter = 0; A = new DATA_TYPE_D[n = size]; }
	~CSortD() { delete[]A; }
	void build_list(DATA_TYPE_D input[], int *iDEMSort);
	void debug_print(int iteration, int debug,
		char *hdr, int *iDEMSort);
	void qsort(int First, int Last, int *iDEMSort);
	void Quick_Sort(DATA_TYPE_D input[], int *iDEMSort);

private:
	//typedef    int        DATA_TYPE;
	typedef    DATA_TYPE_D  ARY_LIST[];
	DATA_TYPE_D  *A;  // Array list A[]
	int        n;   // size of A
	int        iter;
};

//#endif // !defined(AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_)
