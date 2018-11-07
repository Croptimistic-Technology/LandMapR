/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <fstream>

//Class represents Mold and Vold tables
//Use its functions to add and access values
//It will grow as needed automaticaly.

#ifndef _VOLDFILE_H_
#define _VOLDFILE_H_

struct  VoldFileStructure
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double   iVaration;
};





class VoldFile
{
public:
	void RemoveAll();
	VoldFile(std::string sFileName);
	~VoldFile();
	void AddNew(int iSeqNoNew, int iDdirNew, int iDrecNew, int iUpSlopeNew, int iShedNowNew,
		int iStageNew, int iUrecNew = -1, int iDs_AreaNew = -1, double iVarationNew = -1);
	int GetSeqNo(int nIndex);
	int GetDdir(int nIndex);
	int GetDrec(int nIndex);
	int GetUpSlop(int nIndex);
	int GetShedNow(int nIndex);
	int GetStage(int nIndex);
	int GetUrec(int nIndex);
	int GetDs_Area(int nIndex);
	double GetVaration(int nIndex);

	void RemoveAt(int nIndex);
	int GetSize();
private:
	VoldFileStructure * pFile;
	int nNumberOfRecords;
	int nArraySize;

protected:
	std::string sFileName;
};
#endif