/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// landmapDlg.h : header file
//

//#if !defined(AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_)
//#define AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef _FLOWMAPR_H_
#define _FLOWMAPR_H_

//#include<fstream>
//#include "ProgressBarDialog.h"
//#define WM_USER_RECALC_IN_PROGRESS (WM_USER + 1)


#include "VoldFile.h"
#include "dem.h"
#include "pond.h"
#include "idem.h"
/////////////////////////////////////////////////////////////////////////////
// CLandmapDlg dialog


//*********************************
//IK replaced by newer StatVar
/*typedef struct StatVar
{
	double	fInElev;
	double	fMinElev;
	double	fOutElev;
	double	fPitVol;
	double	fPitElev;
	double	fPourElev;
	double	fStartElev;
	double	fVaratio;
	int		iDrainsTo;
	int		iInRec;
	int		iInRow;
	int		iInColumn;
	int		iOutRec;
	int		iOutRow;
	int		iOutColumn;
	int		iOutShed;
	int		iPitRow;
	int		iPitCol;
	int		iPitArea;
	int		iPitRec;
	int		iPondCell;
	int		iShedArea;
	int		iThisShed;
} LSMSTATVAR;
*/


extern "C" __declspec(dllexport) char* landmapRProcess(char* workingDirectory, char*demDataStr, double xllcorner, double yllcorner, char* fileName, int nrows, int ncolumns, double GridSize, double MissingDataValue, long PitArea, double PitDepth, bool pbInvertedElev, double dDSArea, double dUpslopeArea, char* fuzzyAttributeFile, char* fuzzyFacetsFile);

typedef struct VoldFile1
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double  iVaration;
}LSMVOLDFILE;

typedef struct MoldFile
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double  iVaration;
}LSMMOLDFILE;


typedef struct StatVar
{

	int		iThisShed;
	bool	bEdge;
	bool    bFinal;
	int     iEndPit;
	int		iShedArea;
	int		iPitRow;
	int		iPitCol;
	int		iPitRec;
	double  fPitElev;
	double	fPourElev;
	double  fPreVol;
	double	fPitVol;
	double	fVaratio;
	int		iPitArea;
	int		iDrainsTo;
	int		iOutShed;
	int     iNextPit;
	int     iBecomes;
	bool    bRemoved;
	int		iInRow;
	int		iInColumn;
	int		iInRec;
	double	fInElev;
	int		iOutRow;
	int		iOutColumn;
	int		iOutRec;
	double	fOutElev;
	int     iStage;
	bool    bVisited;
	int		iPondCell;
	double  fStartElev;
	double  fMinElev;
	bool 	bDR_2_MV;

} LSMSTATVAR;

typedef struct PondStats
{
	int		iThisShed;
	bool	bEdge;
	bool    bFinal;
	int     iEndPit;
	int		iShedArea;
	int		iPitRow;
	int		iPitCol;
	int		iPitRec;
	double  fPitElev;
	double	fPourElev;
	double  fPreVol;
	double	fPitVol;
	double	fVaratio;
	int		iPitArea;
	int     iDrainsTo;
	int		iOutShed;
	int     iNextPit;
	int     iBecomes;
	bool    bRemoved;
	int		iInRow;
	int		iInColumn;
	int		iInRec;
	double	fInElev;
	int		iOutRow;
	int		iOutColumn;
	int		iOutRec;
	double	fOutElev;
	int     iStage;
	bool    bVisited;

} LSMPONDSTATS;

typedef struct StatVar2
{
	int		iThisShed;
	double	fMinElev;
	double	fPourElev;
	double	fStartElev;
	double	fOutElev;
	double	fPitVol;
	int		iPitRow;
	int		iPitCol;
	int		iPitArea;
	int		iPitRec;
	double	fPitElev;
	int		iInRec;
	double	fInElev;
	int		iCenRow;
	int		iCenColumn;
	int		iOutRec;
	int		iNeighRow;
	int		iNeighColumn;
	int		iOutShed;
	int		iShedArea;
	int		iPondCell;
	int		iInRow;
	int		iInColumn;
	int		iOutRow;
	int		iOutColumn;
	double	fVaratio;
} LSMSTATVAR2;

typedef struct ProcParm
{
	double	fMaxVol;
	int		iMaxArea;
	double	fMaxDepth;
	double	fMaxRunOff;
} LSMPROCPARM;

class FlowmapR 
{
	// Construction
public:
	bool bKillThread;
	std::string sFullCurrentPath;
	/// <summary>
	/// Default constructor
	/// </summary>
	FlowmapR();
	//std::string	m_FileName;
	/// <summary>
	/// Lower left x-coordinate of input elevation data
	/// </summary>
	double	m_LLX;
	/// <summary>
	/// Lower left y-coordinate of input elevation data
	/// </summary>
	double	m_LLY;
	/// <summary>
	/// Grid value for missing data
	/// </summary>
	double	m_MissDataValue;
	/// <summary>
	/// Maximum depth of pit to remove
	/// </summary>
	double	m_PitDepth;
	/// <summary>
	/// Maximum area of pit to remove
	/// </summary>
	long	m_PitArea;
	/// <summary>
	/// Number of Rows in DEM Matrix
	/// </summary>
	int		m_Row;
	/// <summary>
	/// Grid size
	/// </summary>
	double	m_GridSize;
	/// <summary>
	/// Number of columns in DEM Matrix
	/// </summary>
	int		m_Column;
	/// <summary>
	/// Working directory of output data
	/// </summary>
	std::string	m_WorkingDirectory;
	/// <summary>
	/// determine if u want to calculate inverted DEM
	/// </summary>
	bool	m_InvertedElev;
	/// <summary>
	/// FlowmapR destructor
	/// </summary>
	~FlowmapR();
	/// <summary>
	/// FlowmapR process
	/// </summary>
	void runProcess(std::string& resultJsonFilePath, std::string m_WorkingDirectory, double* pfDEMGrid, double xllcorner, double yllcorner, std::string m_FileName, int m_Row, int m_Column, double m_GridSize, double m_MissDataValue, long m_PitArea, double m_PitDepth, bool pbInvertedElev, DEM* dem, Pond* pond, IDEM* idem);
protected:
};


#endif // !defined(AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_)
