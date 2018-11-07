/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/** /brief Implementation of the DEMProcess class
*
*/
#include "../Headers/stdafx.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include <chrono>
#include <malloc.h>
#include <time.h>
#include <chrono>
#include "../Headers/demprocess.h"
#include <vector>

using namespace std;
//#include "flowmapr.h"
//#include "stdafx.h"
//#include "logger.h"

/// <summary>
///  Constructor with known number of grid dimensions and grid cell size
/// </summary>
DEMProcess::DEMProcess(int iRow, int iColumn, double fGridSize)
{
	_iRow = iRow;
	_iColumn = iColumn;
	_fGridSize = fGridSize;

}


/// <summary>
/// Default destructor
/// </summary>
DEMProcess::~DEMProcess()
{
	free(_pFlatCell);
}


/// <summary>
/// Method to find the neighbor cell with the steepest downslope gradient away from the centre cell
/// the calculations are only applied to valid cells for a 3 by 3 window(e.g.cells within the data matrix
///	that are NOT missing values).
/// </summary>
int DEMProcess::FlowDir(double *pfDEMGrid, int *piDIRGrid)
{
	int     iRet = 0;
	double	dSlope[10];
	double	dHighestValue;
	int     i, j, k;
	int		iHighest;
	bool    bFlat;
	size_t	size;

	// Assign array to store flat cells
	_pFlatCell = (int *)malloc(10 * sizeof(int));
	size = _msize(_pFlatCell);
	_iNumFlatCell = 0;

	//      Process matrix by ROW/COL
	for (i = 0; i < _iRow; i++)
	{
		for (j = 0; j < _iColumn; j++)
		{
			// Initialize all the iDiff to -99999
			for (k = 0; k < 10; k++)
				dSlope[k] = -9999.0;
			if (!(pfDEMGrid[i*_iColumn + j] == dMissingValue))
			{
				//***********************************************************************
				//assign difference in elevation value to each valid cell in 3x3 metrix
				//*************************************************************************
				if ((i > 0) && (j > 0) && (*(pfDEMGrid + (((i - 1)*_iColumn) + (j - 1)))) >= 0)
					dSlope[7] = (*(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i - 1)*_iColumn) + (j - 1)))) / (double)sqrt2;

				if (i > 0 && (*(pfDEMGrid + (((i - 1)*_iColumn) + j))) >= 0)
					dSlope[8] = *(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i - 1)*_iColumn) + j));

				if ((i > 0) && (j < _iColumn - 1) && (*(pfDEMGrid + (((i - 1)*_iColumn) + (j + 1)))) >= 0)
					dSlope[9] = (*(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i - 1)*_iColumn) + (j + 1)))) / (double)sqrt2;

				if (j > 0 && (*(pfDEMGrid + ((i*_iColumn) + (j - 1)))) >= 0)
					dSlope[4] = *(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + ((i*_iColumn) + (j - 1)));

				if (j < _iColumn - 1 && (*(pfDEMGrid + ((i*_iColumn) + (j + 1)))) >= 0)
					dSlope[6] = *(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + ((i*_iColumn) + (j + 1)));

				if ((i < _iRow - 1) && (j > 0) && (*(pfDEMGrid + (((i + 1)*_iColumn) + (j - 1)))) >= 0)
					dSlope[1] = (*(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i + 1)*_iColumn) + (j - 1)))) / (double)sqrt2;

				if (i < _iRow - 1 && (*(pfDEMGrid + (((i + 1)*_iColumn) + j))) >= 0)
					dSlope[2] = *(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i + 1)*_iColumn) + j));

				if ((i < _iRow - 1) && (j < _iColumn - 1) && (*(pfDEMGrid + (((i + 1)*_iColumn) + (j + 1)))) >= 0)
					dSlope[3] = (*(pfDEMGrid + ((i*_iColumn) + j)) - *(pfDEMGrid + (((i + 1)*_iColumn) + (j + 1)))) / (double)sqrt2;

				bFlat = false;

				for (k = 1; k < 10; k++)
				{
					if (k != 5)
					{   //*****************************************************
						//check if at least one of the cells has valid elevation difference
						//******************************************************
						if (dSlope[k] <= 0)
							bFlat = true;
						else
						{
							bFlat = false;
							k = 10;
						}
					}
				}
				//*****************************************
				//check if there is a flow to be calculated
				//*****************************************
				if (!bFlat)
				{
					// Find the max
					//******************************************************
					// This cell will have the lowest elevation and
					// current cell will flow into it 
					//******************************************************
					iHighest = 1;
					dHighestValue = dSlope[0];
					for (k = 1; k < 10; k++)
					{
						if (k != 5)
						{
							if (dSlope[k] > dHighestValue)
							{
								iHighest = k;//direction of the flow
								dHighestValue = dSlope[k];
							}
						}
					}
				}
				//***********************************************************************************
				//if current cell is a flat cell, add it's location to the flatcell array, 
				// and set it's direction to 5
				//***********************************************************************************
				else
				{
					iHighest = 5;
					_pFlatCell[_iNumFlatCell] = i * _iColumn + j;
					_iNumFlatCell++;
					//******************************************************
					//if there is not enough space in the flat cell array
					// realocate memory for bigger array
					//******************************************************
					if (_iNumFlatCell % 10 == 0)
					{
						_pFlatCell = (int *)realloc(_pFlatCell, size + (10 * sizeof(int)));
						size = _msize(_pFlatCell);
					}
				}
			}
			else
			{
				iHighest = 0;
			}
			*(piDIRGrid + ((i*_iColumn) + j)) = iHighest;
		}
	}
	return iRet;
}


/*brief Method to assign a flow direction to flat cells  (cells with ddir of 5) if at all possible
The method operates in 2 steps.

Step 1 fixes flat cells which can drain out to a cell of lower elevation.  Such cells have as a neighbor
another cell of equal elevation which has aready been assigned a valid flow direction (i.e. drains to a
lower cell).  This method cycles through the list of flat cells until all flat cells with a neighbour at the
same elevation with a valid flow direction have been assigned flow directions pointing into this
neighbor cell.  Any cells that remain are flat cells within depressions that have no neighbors at the
same elevation that have been assigned valid flow directions.
Step 2 fixes flat cells which can only drain inward to a depression centre.  They have no possible
connection to any cells at a lower elevation (at least until pits are removed).  This  part of the
method cycles through all cells listed in the array of flat cells and checks all of the neighbors of
each flat cell to see if any have a valid flow direction pointing into the current flat cell.  If any neighbor
cell flows into the current flat cell, then all other neighbors are checked to see if any are at the same
elevation (i.e. are also flat).  If there are any neighbors at the same elevation and they do not already
point into the current flat cell, then the current flat cell is assigned a temporary flow direction pointing
into to its flat neighbor.  Temporary flow directions are reset to permanent flow directions (by subtracting
10 from the temporary flow direction value) after each cycle through the list of flat cells.  The method
checks and assigns drainage directions to flat cells until it completes a full cycle through the list of flat
cells without being able to assign a new flow direction to any cell.  At this point, any cells with a local
drainage direction of 5 should be true pit centres surronded by cells at equal or higher elevation which
have flow directions pointing into the central pit cell.

Before doing step 1 or step 2, we need to find neighbor cells that are valid(i.e. within matrix bounds)*/

int DEMProcess::FixFlat(double *pfDEMGrid, int *piFlowGrid)
{

	int		iRet = 0;
	int		i, j, k, l, m, n, ii;
	int		iStartRow;
	int		iStartCol;
	int		iEndRow;
	int		iEndCol;
	double	fCurElev;
	double	fNeigElev;
	int		iNeigFlow;
	int		iTempFlow;
	int		iLogicFlow;
	int		iCurFlow;
	bool	bNewDir;
	bool	bFlowIn;
	bool	bPitNeig;
	bool	bNewFlat;

	/*
	Flatout
	Method to go to each neighbor cell of a current flat cell and to determine if
	any of the neighbor cells are:
	(a) at the same elevation as the current flat cell  AND
	(b) have been assigned a valid local flow direction (1-9) other than 5 (or 0)
	If a neighbor cell of a flat cell meets these criteria then the current flat
	cell can flow into it (i.e. this is the logical outlet for the flat cell)
	A local flow direction (ldir) is then calculated and assigned to the current flat cell
	*/
	bNewFlat = true;
	while (bNewFlat)
	{
		bNewFlat = false;
		for (ii = 0; ii < _iNumFlatCell; ii++)
		{
			fCurElev = *(pfDEMGrid + _pFlatCell[ii]);
			iCurFlow = *(piFlowGrid + _pFlatCell[ii]);
			i = _pFlatCell[ii] / _iColumn;
			j = _pFlatCell[ii] % _iColumn;
			if (iCurFlow > 9)
			{
				iCurFlow = iCurFlow - 10;
				*(piFlowGrid + _pFlatCell[ii]) = iCurFlow;
			}
			if (iCurFlow == 5)// check if current cell is a flat cell
			{
				n = 0;
				//************************************************************************************
				//find valid neighbors for the current cell
				//i.e. if cell in the top left corner then there is no cells obove or on the left side
				//**************************************************************************************

				if ((i > 0) && (j > 0) && (i < _iRow - 1) && (j < _iColumn - 1))
				{
					iStartRow = iStartCol = 1;
					iEndRow = iEndCol = -1;
				}
				else if ((i == 0) && (j == 0))
				{
					iStartRow = iStartCol = 0;
					iEndRow = iEndCol = -1;
				}
				else if ((i == 0) && (j == _iColumn - 1))
				{
					iStartRow = 0;
					iStartCol = 1;
					iEndRow = -1;
					iEndCol = 0;
				}
				else if ((i == _iRow - 1) && (j == 0))
				{
					iStartRow = 1;
					iStartCol = 0;
					iEndRow = 0;
					iEndCol = -1;
				}
				else if ((i == _iRow - 1) && (j == _iColumn - 1))
				{
					iStartRow = iStartCol = 1;
					iEndRow = iEndCol = 0;
				}
				else if (i == 0)
				{
					iStartRow = 0;
					iStartCol = 1;
					iEndRow = iEndCol = -1;
				}
				else if (j == 0)
				{
					iStartRow = 1;
					iStartCol = 0;
					iEndRow = iEndCol = -1;
				}
				else if (i == _iRow - 1)
				{
					iStartRow = 1;
					iStartCol = 1;
					iEndRow = 0;
					iEndCol = -1;
				}
				else if (j == _iColumn - 1)
				{
					iStartRow = iStartCol = 1;
					iEndRow = -1;
					iEndCol = 0;
				}
				for (k = iStartRow; k >= iEndRow; k--)//go to each neighbor cell
				{
					m = 0;
					for (l = iStartCol; l >= iEndCol; l--)
					{
						if ((k != 0) || (l != 0))
						{

							iNeigFlow = *(piFlowGrid + (((i - k)*_iColumn) + (j - l)));//get neighbor flow
							fNeigElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));//get neighbor elevation
							//check if elevation is the same and flow of the neighbor is a valid flow 
							if ((fNeigElev == fCurElev) && (iNeigFlow < 10) && (iNeigFlow != 5))
							{
								//what do the next two lines do? // comment existed on the original FlowMapR
								//iTempFlow = (((3-(k+n))*3)+(l+m));
								if (k == -1)
								{
									iTempFlow = 1;
								}
								else
								{
									if (k == 0)
									{
										iTempFlow = 4;
									}
									else
									{
										if (k == 1)
										{
											iTempFlow = 7;
										}
									}
								}

								if (l == 0)
								{
									iTempFlow = iTempFlow + 1;
								}
								else
								{
									if (l == -1)
									{
										iTempFlow = iTempFlow + 2;
									}
								}

								if (iNeigFlow != (10 - iTempFlow))
								{
									iLogicFlow = iTempFlow + 10;
									*(piFlowGrid + _pFlatCell[ii]) = iLogicFlow;
									bNewFlat = true;
								}
							}
						}
						m = m + 2;
					}
					n = n + 2;
				}
			}
		}
	}

	/*
	//Flatin
	Method to assign a logical flow direction to flat cells within depressions.

	Such cells have no possible flow paths to an outlet cell which flows to a lower elevation.
	The method works by going to each neighbor cell of a current flat cell to determine if any of the
	neighbor cells:
	(a) are at the same elevation as the current flat cell (i.e. are also flat)
	(b) if yes to (a) then does the flat neighbor cell point back into the current flat cell?
	(c) if no to (b) then the current flat cell can be assigned a local flow direction pointing into the flat
	neighbor cell if (d) below is also true
	(d) are there any other neighbor cells that have a valid flow direction that points into the current flat cell
	This last check ensures that flow into depressions starts at the edge of the depression where cells with
	valid flow directions flow into the flat cells at the edge of the depression.  Flow directions are assigned
	progressively to other flat cells closer to the centre of a depression as outer flat cells are assigned
	valid flow directions.
	*/
	bNewFlat = true;
	while (bNewFlat)
	{
		bNewFlat = false;
		for (ii = 0; ii < _iNumFlatCell; ii++)
		{
			fCurElev = *(pfDEMGrid + _pFlatCell[ii]);
			iCurFlow = *(piFlowGrid + _pFlatCell[ii]);
			i = _pFlatCell[ii] / _iColumn;
			j = _pFlatCell[ii] % _iColumn;
			if (iCurFlow > 9)//if flow is more than 9 then it is a temp flow
			{
				iCurFlow = iCurFlow - 10;
				*(piFlowGrid + _pFlatCell[ii]) = iCurFlow;
			}
			if (iCurFlow == 5) //if current flow is 5 then get valid neighbor cells
			{
				bNewDir = false;
				bFlowIn = false;
				bPitNeig = false;
				n = 0;
				if ((i > 0) && (j > 0) && (i < _iRow - 1) && (j < _iColumn - 1))
				{
					iStartRow = iStartCol = 1;
					iEndRow = iEndCol = -1;
				}
				else if ((i == 0) && (j == 0))
				{
					iStartRow = iStartCol = 0;
					iEndRow = iEndCol = -1;
				}
				else if ((i == 0) && (j == _iColumn - 1))
				{
					iStartRow = 0;
					iStartCol = 1;
					iEndRow = -1;
					iEndCol = 0;
				}
				else if ((i == _iRow - 1) && (j == 0))
				{
					iStartRow = 1;
					iStartCol = 0;
					iEndRow = 0;
					iEndCol = -1;
				}
				else if ((i == _iRow - 1) && (j == _iColumn - 1))
				{
					iStartRow = iStartCol = 1;
					iEndRow = iEndCol = 0;
				}
				else if (i == 0)
				{
					iStartRow = 0;
					iStartCol = 1;
					iEndRow = iEndCol = -1;
				}
				else if (j == 0)
				{
					iStartRow = 1;
					iStartCol = 0;
					iEndRow = iEndCol = -1;
				}
				else if (i == _iRow - 1)
				{
					iStartRow = 1;
					iStartCol = 1;
					iEndRow = 0;
					iEndCol = -1;
				}
				else if (j == _iColumn - 1)
				{
					iStartRow = iStartCol = 1;
					iEndRow = -1;
					iEndCol = 0;
				}
				for (k = iStartRow; k >= iEndRow; k--)
				{
					m = 0;
					for (l = iStartCol; l >= iEndCol; l--)
					{
						if ((k != 0) || (l != 0))
						{
							iNeigFlow = *(piFlowGrid + (((i - k)*_iColumn) + (j - l)));
							if (iNeigFlow == 5)
								bPitNeig = true;
							fNeigElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));

							//iTempFlow = (((3-(k+n))*3)+(l+m));
							if (k == -1)
							{
								iTempFlow = 1;
							}
							else
							{
								if (k == 0)
								{
									iTempFlow = 4;
								}
								else
								{
									if (k == 1)
									{
										iTempFlow = 7;

									}
								}
							}

							if (l == 0)
							{
								iTempFlow = iTempFlow + 1;
							}
							else
							{
								if (l == -1)
								{
									iTempFlow = iTempFlow + 2;
								}
							}

							if (fCurElev == fNeigElev)//check if the same elevation
							{
								if (iNeigFlow != (10 - iTempFlow))
								{
									iLogicFlow = iTempFlow;
									bNewDir = true;
								}
								else
									bFlowIn = true;
							}
							else
							{
								if (iNeigFlow == (10 - iTempFlow))
									bFlowIn = true;
							}
						}
						m = m + 2;
					}
					n = n + 2;
				}
				if (bFlowIn && bNewDir && bPitNeig)
				{
					bNewFlat = true;
					*(piFlowGrid + _pFlatCell[ii]) = iLogicFlow;
				}
			}
		}
	}
	return iRet;
}


/**
*	\brief Purpose:	to compute vol-to-flood & mm-to-flood & parea for depressional cells.
*	@pre	Fields ELEV, DDIR, DREC, SHEDNOW must be initialized.
*
*	Note: Array are zero based, taking this into accoung whenever we try to retrieve record
*	we subtruct 1 to accomidate that. i.e. for *dbf inputs, record 1000 is at location 999 in the array.
*/
void DEMProcess::Volume2Flood(int *iDEMSort, double *DEMGrid, LSMSTATVAR *ShedStat,
	double *pfVol2FlGrid, double *pfMm2FlGrid, int *pfPArea, int *piShedWGrid,
	int *piDEMSort, int *piDrecGrid, int iPitNo, int* piFlowGrid)
{
	//Variable used in lsm_vol2fl procedure
	//********************************************
	int this_shed = 0;
	//int shed_area = 0;
	pit_rec = 0;
	//int pit_vol = 0;
	//int pit_area = 0;
	//double pour_elev = 1000000;
	int no_pits = 0;
	int num_recs = 0;
	//*********************************************

	//Variables used in calcvol2fl
	//*********************************************
	double curr_elev = 0.0;
	int total_cells = 0;
	double last_elev = 0;
	double elev_diff = 0;
	double curr_vol = 0.1;
	double curr_mm2fl = 0.0;
	int curr_parea = 0;
	int totalnum = _iRow * _iColumn;
	//*********************************************

	bool bLoop = true;
	bool bExit = false;
	int nIndex = 0;

	//initialize all 3 array
	for (int nLoop3 = 0; nLoop3 < totalnum; nLoop3++)
	{
		pfVol2FlGrid[nLoop3] = 0.0;
		pfMm2FlGrid[nLoop3] = 0.0;
		pfPArea[nLoop3] = 0;
	}

	double nNextElev = 0;
	for (int nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		bLoop = true;
		this_shed = ShedStat[nLoop].iThisShed;
		shed_area = ShedStat[nLoop].iShedArea;
		pit_rec = ShedStat[nLoop].iPitRec;
		pour_elev = ShedStat[nLoop].fPourElev;
		no_pits = no_pits + 1;

		if (ShedStat[nLoop].bEdge == true || ShedStat[nLoop].bDR_2_MV == true)
		{
			//printf("Edge cell");
		}
		else
		{
			nIndex = pit_rec - 1;//we subtruct 1 to access the record in the array
			last_elev = DEMGrid[nIndex];
			int nStartRecord = 0;
			//search for the location of the record in the indexed array
			//processing will be done from that record down.
			for (int nLoop4 = 0; nLoop4 < totalnum - 1; nLoop4++)
			{
				if (iDEMSort[nLoop4] == nIndex)
				{
					nStartRecord = nLoop4;
					nLoop4 = totalnum;
				}
			}
			curr_elev = 0.0;
			total_cells = 0;
			//last_elev =0;
			elev_diff = 0;
			curr_vol = (double) 0.1;
			curr_mm2fl = 0.0;

			while (bLoop == true)
			{
				if (nStartRecord < totalnum)
				{
					if (DEMGrid[piDEMSort[nStartRecord]] <= pour_elev &&
						piShedWGrid[piDEMSort[nStartRecord]] == this_shed)
					{
						//get elevation for the selected record
						curr_elev = DEMGrid[piDEMSort[nStartRecord]];

						int nNumOfCells = 0;
						bExit = false;
						while (bExit == false)
						{
							nNumOfCells++;
							total_cells = total_cells + 1;
							nStartRecord++;
							if (nStartRecord < totalnum)
							{
								if (!(curr_elev == DEMGrid[piDEMSort[nStartRecord]]))
								{
									bExit = true;
								}
							}
							else
							{
								bExit = true;
							}
						}

						if (nNumOfCells > 2)
						{
							nNumOfCells = 0;
						}
						nStartRecord--;

						elev_diff = (curr_elev - last_elev) * 1000;
						curr_vol = curr_vol + (elev_diff*(total_cells - 1));
						if (shed_area > 0)
						{
							curr_mm2fl = curr_vol / shed_area;
						}
						else
						{
							curr_mm2fl = curr_vol / 1;
						}
						pfVol2FlGrid[piDEMSort[nStartRecord]] = (double)curr_vol;
						pfMm2FlGrid[piDEMSort[nStartRecord]] = curr_mm2fl;
						pfPArea[piDEMSort[nStartRecord]] = total_cells;
						last_elev = DEMGrid[piDEMSort[nStartRecord]];
						nStartRecord++;;
					}
					else
					{
						bLoop = false;
					}
				}
				else
				{
					bLoop = false;
				}
			}
			for (int nLoop2 = 0; nLoop2 < total_cells; nLoop2++)
			{
				nStartRecord--;
				if (piFlowGrid[piDEMSort[nStartRecord]] == 5)
				{
					nLoop2 = total_cells;
				}
				else
				{
					if (pfPArea[piDEMSort[nStartRecord]] == 0)
					{
						pfVol2FlGrid[piDEMSort[nStartRecord]] = (double)curr_vol;
						pfMm2FlGrid[piDEMSort[nStartRecord]] = curr_mm2fl;
						pfPArea[piDEMSort[nStartRecord]] = curr_parea;
					}
					else
					{
						curr_vol = pfVol2FlGrid[piDEMSort[nStartRecord]];
						curr_mm2fl = pfMm2FlGrid[piDEMSort[nStartRecord]];
						curr_parea = pfPArea[piDEMSort[nStartRecord]];
					}
				}
			}
		}
	}
}

/**
* \brief Method to compute the record number in the data base for each cell according to the local drainage direction DDIR value assigned to that cell
*	This method is identical to the 'calcdrec' in fox pro module
*@author Author of Original FlowMapR, modified by Gashaw Ayalew
*/
int DEMProcess::CalcDrec(int *piFlowGrid, int *piDrecGrid)
{
	int iRet = 0;
	int i, j, k;
	int	iRow;
	int	iColumn;
	for (i = 0; i < _iRow; i++)
	{
		for (j = 0; j < _iColumn; j++)
		{
			//if (*(piFlowGrid+((i*_iColumn)+j)) >= 500)
			//	k = *(piFlowGrid+((i*_iColumn)+j)) - 500;
			//else
			k = *(piFlowGrid + ((i*_iColumn) + j));
			switch (k)
			{
			case 1:		iRow = i + 1;
				iColumn = j - 1;
				break;
			case 2:		iRow = i + 1;
				iColumn = j;
				break;
			case 3:		iRow = i + 1;
				iColumn = j + 1;
				break;
			case 4:		iRow = i;
				iColumn = j - 1;
				break;
			case 5:		iRow = i;
				iColumn = j;
				break;
			case 6:		iRow = i;
				iColumn = j + 1;
				break;
			case 7:		iRow = i - 1;
				iColumn = j - 1;
				break;
			case 8:		iRow = i - 1;
				iColumn = j;
				break;
			case 9:		iRow = i - 1;
				iColumn = j + 1;
				break;
			default:	iRow = i;
				iColumn = j;
				break;
			}
			*(piDrecGrid + ((i*_iColumn) + j)) = iRow * _iColumn + iColumn + 1;
		}
	}
	return iRet;
}


/**
\brief Method to check for circular flow in flat cells in depressions
*
*	\detailed If flow from a flat cell returns to any cell along the flow path before encountering a 5,
*	then flow is circular and must be stopped by giving the cell where flow returns a flow direction of 5
*	(i.e. defining it as a pit cell)
*	This method corresponds to 'circflow', 'pit_check', and 'neighbors' procedures in fox pro module
*@author Authors of Original LandMapR, modified by Gashaw Ayalew
*@modified 2016-08-01
*/
int DEMProcess::CircleFlow(int *piFlowGrid, int *piDrecGrid)
{
	int		iRet = 0;
	int		i, j, k, l, m, n, ii;
	int		iDrec;
	int		iStartRow;
	int		iStartCol;
	int		iEndRow;
	int		iEndCol;
	int		iNeigFlow;
	int		iTempFlow;
	int		iFlowLength;
	int		iPathCell[50000];
	bool	*pbEdgeGrid;
	int totalNums = _iRow * _iColumn;

	//pbEdgeGrid = (bool *) malloc ((_iRow * _iColumn) * sizeof (bool));
	if ((pbEdgeGrid = new bool[totalNums]) == NULL)
	{
		printf("Error creating edge bool array in CircleFlow(), exiting ...");
		exit(1);
	}
	for (i = 0; i < totalNums; i++)
		pbEdgeGrid[i] = false;

	for (ii = 0; ii < _iNumFlatCell; ii++)
	{
		iFlowLength = 0;
		iDrec = _pFlatCell[ii];
		while (true)
		{
			if (piFlowGrid[iDrec] == 5)
				break;
			if (pbEdgeGrid[iDrec])
			{
				piFlowGrid[iDrec] = 5;
				piDrecGrid[iDrec] = iDrec + 1;
				pbEdgeGrid[iDrec] = false;
				break;
			}
			else
			{
				iPathCell[iFlowLength] = iDrec;
				iFlowLength++;
				pbEdgeGrid[iDrec] = true;
				iDrec = piDrecGrid[iDrec] - 1;
			}
		}
		for (i = 0; i < iFlowLength; i++)
			pbEdgeGrid[iPathCell[i]] = false;
	}

	for (ii = 0; ii < _iNumFlatCell; ii++)
	{
		if (piFlowGrid[_pFlatCell[ii]] == 5)
		{
			i = _pFlatCell[ii] / _iColumn;
			j = _pFlatCell[ii] % _iColumn;
			n = 0;
			if ((i > 0) && (j > 0) && (i < _iRow - 1) && (j < _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == 0))
			{
				iStartRow = iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == _iColumn - 1))
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			else if ((i == _iRow - 1) && (j == 0))
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if ((i == _iRow - 1) && (j == _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = 0;
			}
			else if (i == 0)
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if (j == 0)
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if (i == _iRow - 1)
			{
				iStartRow = 1;
				iStartCol = 1;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if (j == _iColumn - 1)
			{
				iStartRow = iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			for (k = iStartRow; k >= iEndRow; k--)
			{
				m = 0;
				for (l = iStartCol; l >= iEndCol; l--)
				{
					if ((k != 0) || (l != 0))
					{
						iNeigFlow = *(piFlowGrid + (((i - k)*_iColumn) + (j - l)));
						if (iNeigFlow == 5)
						{
							//iTempFlow = (((3-(k+n))*3)+(l+m));
							if (k == -1)
							{
								iTempFlow = 1;
							}
							else
							{
								if (k == 0)
								{
									iTempFlow = 4;
								}
								else
								{
									if (k == 1)
									{
										iTempFlow = 7;
									}
								}
							}
							if (l == 0)
							{
								iTempFlow = iTempFlow + 1;
							}
							else
							{
								if (l == -1)
								{
									iTempFlow = iTempFlow + 2;
								}
							}
							*(piFlowGrid + _pFlatCell[ii]) = iTempFlow;
							*(piDrecGrid + _pFlatCell[ii]) = ((i - k) * _iColumn) + (j - l) + 1;
							l = iEndCol;
							k = iEndRow;
						}
					}
					m = m + 2;
				}
				n = n + 2;
			}
		}
	}
	delete[] pbEdgeGrid;
	return iRet;
}

/**
*	\brief Method to compute and assign an initial watershed number to every cell in a DEM data base
*
*	\detailed Method to go to each grid cell starting at the highest cell in the entire DEM (the top cell)
*	and to follow a flow path from each cell DOWNSLOPE from it to the final pit cell to which
*	each cell in the current watershed is connected by downstream flow (computed as flow from a grid
*	cell to its LOWEST downslope neighbor).  As the algorithm traverses the DEM matrix it
*	computes the watershed number for the initial (non-integrated ) watershed for each grid cell
*	in the DEM matrix
*	This Method corresponds to the calc_sheds procedure in fox pro module
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
int DEMProcess::CalcWaterShed(double *pfDEMGrid, int *piFlowGrid, int *piDrecGrid,
	int *piShedGrid, int *piDEMSort)
{
	int		iRet = 0;
	int		i, j;
	int		iShedStart;
	int		iShedNo;
	int		iMaxShed = 0;
	int		iNumDown;
	int		iThisShed;
	int		iDrec;
	bool	bGoDown;

	int totalNums = _iRow * _iColumn;
	iShedNo = 0;
	for (i = 0; i < totalNums; i++)
		piShedGrid[i] = 0;

	for (i = totalNums - 1; i >= 0; i--)
	{
		iShedStart = iDrec = piDEMSort[i];
		iNumDown = 0;
		bGoDown = true;
		if (!(pfDEMGrid[piDEMSort[i]] == dMissingValue))
		{
			while (bGoDown)
			{
				if ((piShedGrid[iDrec] == 0) && (piFlowGrid[iDrec] != 5))
				{
					iDrec = piDrecGrid[iDrec] - 1;
					iNumDown++;
				}
				else if (piShedGrid[iDrec] > 0)
				{
					iThisShed = piShedGrid[iDrec];
					bGoDown = false;
				}
				else if (piFlowGrid[iDrec] == 5)
				{
					iMaxShed++;
					iThisShed = iMaxShed;
					piShedGrid[iDrec] = iThisShed;
					bGoDown = false;
				}
			}
			if (iNumDown > 0)
				piShedGrid[iShedStart] = iThisShed;

			iDrec = piDrecGrid[iShedStart] - 1;
			for (j = 1; j < iNumDown; j++)
			{
				piShedGrid[iDrec] = iThisShed;
				iDrec = piDrecGrid[iDrec] - 1;
			}
		}
	}
	return iRet;
}

/**
	\brief Method to compute and assign an upslope area count to every cell in a DEM object
*
*	\detailed The medhod goes to each grid cell starting at the highest cell in the entire DEM (the top cell) and to  follow a flow path from each cell DOWNSLOPE from it to the final pit cell to which each cell in the current watershed is connected by downstream flow (computed as flow from a grid cell to its LOWEST downslope neighbor).  As the algorithm traverses the DEM matrix it computes the number of cells upslope of each cell in a watershed for each grid cell in the DEM matrix.
*This procedure performs functions of the LSM_UPS and CALCUPAREA procedures in fox pro module
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @ modified 2016-08-01
*/
void DEMProcess::CalcUpSlopeArea(double *pfDEMGrid, int *piFlowGrid, int *piDrecGrid,
	int *piUpSlpGrid, int *piDEMSort)
{
	int		i;
	int		iUpSlp;
	int		iDrec;
	bool	bGoDown;

	int totalNums = _iRow * _iColumn;

	for (i = 0; i < totalNums; i++)
		piUpSlpGrid[i] = 1;

	for (i = totalNums - 1; i >= 0; i--)
	{
		if (!(pfDEMGrid[piDEMSort[i]] == dMissingValue))
		{
			iDrec = piDEMSort[i];
			iUpSlp = 0;
			if (piUpSlpGrid[iDrec] == 1)
				bGoDown = true;
			else
				bGoDown = false;
			while ((piFlowGrid[iDrec] != 5) && (bGoDown))
			{
				iDrec = piDrecGrid[iDrec] - 1;
				iUpSlp++;
				if (piUpSlpGrid[iDrec] == 1)
					piUpSlpGrid[iDrec] = iUpSlp + 1;
				else
				{
					piUpSlpGrid[iDrec] = piUpSlpGrid[iDrec] + iUpSlp;
					bGoDown = false;
				}
			}

			while ((piFlowGrid[iDrec] != 5) && (iUpSlp > 0))
			{
				iDrec = piDrecGrid[iDrec] - 1;
				piUpSlpGrid[iDrec] = piUpSlpGrid[iDrec] + iUpSlp;
			}
		}
		else
		{
			piUpSlpGrid[piDEMSort[i]] = 0;
		}
	}

}

/**
*	\brief Method to determine all relevant watershed statistics for any watershed that originates at a pit centre cell coded as DDIR=5
*
*	\detailed First this method determins valid neighbors to the current center cell
*	Second it checks neighbor cells to determine if any nieghbor cell(s) belong to a different watershed than
*	the current centre 	cell.  If a neighbor belongs to a different watershed than the current centre cell
*	then it is a possible pour point for overspill from the current depression into the neighboring watershed
*
*	This method corresponds to procedures 'shedstat1', 'getneig1', 'chekneig1', 'CalcStat1'
*  @author Authors of Original LandMapR, modified by Gashaw Ayalew
*  @modified 2016-08-01
*/
void DEMProcess::ShedStat1(int ii, int pfPondCellElevSize, int *piDEMSort, double *pfDEMGrid, int *piFlowGrid, int *piShedGrid, int *piShedWGrid, LSMSTATVAR *pShedStat)
{
	//Gerard-SNAP181019_105243
	char msgbuffer[1000];
	if (ii == 237181) {
		//Gerard-SNAP181019_103428 this is just in here so I can breakpoint.
		int piiiughjghjg = 0000000;
	}
	mv_pour = 0;
	drains_2_mv = false;
	int		i, j, k, l;
	int		iStartRow;
	int		iStartCol;
	int		iEndRow;
	int		iEndCol;
	int		iPitCellNo;
	bool	bOverSpill;
	vector<double> *pfPondCellElev;
	//vArray	*pfPondCellElev = new vArray();
	pfPondCellElev = new vector<double>(pfPondCellElevSize);

	int totalnums = _iRow * _iColumn;
	bOverSpill = false;
	i = piDEMSort[ii] / _iColumn;
	j = piDEMSort[ii] % _iColumn;
	int nv;
	nv = pShedStat->iPitCol;
	pShedStat->iPitCol = 20;
	pShedStat->bEdge = false;
	pShedStat->iPitRow = pShedStat->iInRow = pShedStat->iOutRow = i + 1;
	pShedStat->iPitCol = pShedStat->iInColumn = pShedStat->iOutColumn = j + 1;
	pShedStat->iPitRec = pShedStat->iOutRec = piDEMSort[ii] + 1;
	pShedStat->iPondCell = 0;
	pShedStat->fPitElev = pShedStat->fInElev = pShedStat->fOutElev = pfDEMGrid[piDEMSort[ii]];
	pShedStat->fPourElev = 1000000.0;
	pShedStat->iShedArea = 0;
	pShedStat->fPitVol = 0.0;
	pShedStat->iThisShed = piShedWGrid[piDEMSort[ii]];

	while (ii < totalnums)
	{
		if (pShedStat->iThisShed == piShedWGrid[piDEMSort[ii]])
		{
			i = piDEMSort[ii] / _iColumn;
			j = piDEMSort[ii] % _iColumn;
			if ((i > 0) && (j > 0) && (i < _iRow - 1) && (j < _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == 0))
			{
				iStartRow = iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == _iColumn - 1))
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			else if ((i == _iRow - 1) && (j == 0))
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if ((i == _iRow - 1) && (j == _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = 0;
			}
			else if (i == 0)
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if (j == 0)
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if (i == _iRow - 1)
			{
				iStartRow = 1;
				iStartCol = 1;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if (j == _iColumn - 1)
			{
				iStartRow = iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			pShedStat->fStartElev = pfDEMGrid[piDEMSort[ii]];
			pShedStat->fMinElev = 1000000.0;


			for (k = iStartRow; k >= iEndRow; k--)
			{
				//Gerard-SNAP181019_121941-DebugOOB	sprintf(msgbuffer, "Beginning Loop k: %d\n", k);
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				for (l = iStartCol; l >= iEndCol; l--)
				{
					//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning Loop l: %d\n", k);
					//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
					if ((k != 0) || (l != 0))
					{
						//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning Loop k not zero and l not zero: %d\n", k);
						//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
						if (*(pfDEMGrid + (((i - k)*_iColumn) + (j - l))) != dMissingValue)	//if value is not missing
						{
							//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3a: %d\n", k);
							//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
							if (*(piShedWGrid + (((i - k)*_iColumn) + (j - l))) != pShedStat->iThisShed)
							{
								//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3b: %d\n", k);
								//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
								if (*(pfDEMGrid + (((i - k)*_iColumn) + (j - l))) < pShedStat->fMinElev) {
									//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3c: %d\n", k);
									//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
									pShedStat->fMinElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));
								}
								if ((*(pfDEMGrid + (((i - k)*_iColumn) + (j - l))) < pShedStat->fPourElev) &&
									(pShedStat->fStartElev <= pShedStat->fPourElev))
								{
									//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3d: %d\n", k);
									//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
									pShedStat->fOutElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));
									pShedStat->iOutRow = i - k + 1;
									pShedStat->iOutColumn = j - l + 1;
									pShedStat->iOutRec = ((i - k) * _iColumn) + pShedStat->iOutColumn;
									pShedStat->iOutShed = *(piShedWGrid + (((i - k)*_iColumn) + (j - l)));
									pShedStat->iPitArea = pShedStat->iPondCell + 1;
									pShedStat->iInRec = piDEMSort[ii] + 1;
									pShedStat->fInElev = pfDEMGrid[piDEMSort[ii]];
									pShedStat->iInRow = i + 1;
									pShedStat->iInColumn = j + 1;
									if (pShedStat->fOutElev > pShedStat->fInElev) {
										//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3e: %d\n", k);
										//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
										pShedStat->fPourElev = pShedStat->fOutElev;
									}
									else {
										//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Beginning 3f: %d\n", k);
										//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
										pShedStat->fPourElev = pShedStat->fInElev;
									}
								}
							}
						}
						else
						{
							drains_2_mv = true;
							mv_pour = pShedStat->fStartElev;
							//edge_pit = true;  //Bob makes a change on this part in 2005
						}
					}
				}

			}
			//prg 3 calculates and saves this variable in pit table
			//check if the cell not on the edge, ie all 9 neighbors are valid
			//****************************
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Prg3: %d,%d\n", i, j);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);

			if (!((iStartRow + iEndRow) == 0 && (iStartCol + iEndCol) == 0))
			{
				pShedStat->bEdge = true;
			}
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Drains: %d,%d\n", i, j);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
			if (drains_2_mv == true)
			{
				pShedStat->bEdge = true;
			}
			//************************************
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Overspill: %d,%d\n", i, j);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
			if ((pShedStat->fMinElev > pShedStat->fPourElev) && (pShedStat->fStartElev > pShedStat->fPourElev))
				bOverSpill = true;
			if (!bOverSpill)
			{
				//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "def not overspill1: %d,%d\n", i, j);
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				if ((pfPondCellElevSize - 1) > pShedStat->iPondCell) {
					pShedStat->iPondCell = pShedStat->iPondCell + 1;

					//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "def not overspill2: %d,%d\n", i, j);
					//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
					//pfPondCellElev->SetAtGrow(pShedStat->iPondCell, pfDEMGrid[piDEMSort[ii]]);
					//pfPondCellElev->SetAtGrow(pShedStat->iPondCell, pfDEMGrid[piDEMSort[ii]]);
					//Gerard-SNAP181019_121941-DebugOOB if (i == 726 && j == 344) {
					//Gerard-SNAP181019_121941-DebugOOB 	sprintf(msgbuffer, "def not overspill2a: %d,%d\n", i, j);
					//Gerard-SNAP181019_121941-DebugOOB 	OutputDebugStringA((LPCSTR)msgbuffer);
					//Gerard-SNAP181019_121941-DebugOOB }
					pfPondCellElev->at(pShedStat->iPondCell) = pfDEMGrid[piDEMSort[ii]];
					//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "def not overspill3: %d,%d\n", i, j);
					//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
					ii++;
					//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "def not overspill4: %d,%d\n", i, j);
					//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				}
				else {
					//Gerard-SNAP181019_111721
					//Added bounds checker on the iPondCell Increment.  pShedStat->iPondCell = pShedStat->iPondCell + 1;
					//Not doing anything else.
					ii++;
				}
			}
			else
			{
				//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "def overspill: %d,%d\n", i, j);
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				pShedStat->iShedArea = pShedStat->iPondCell;
				while (ii < totalnums)
				{
					//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "while loop: %d,%d\n", i, j);
					//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
					if (pShedStat->iThisShed == piShedWGrid[piDEMSort[ii]])
					{
						pShedStat->iShedArea = pShedStat->iShedArea + 1;
						ii++;
					}
					else
						ii = totalnums;
				}
				//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "end while loop: %d,%d\n", i, j);
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
			}
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "end overspill if: %d,%d\n", i, j);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
		}
		else {
			ii = totalnums;
		}

		//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "finish: %d,%d\n", i, j);
		//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
		//Gerard-SNAP181019_121941-DebugOOB if (i == 640 && j == 308) {
		//Gerard-SNAP181019_121941-DebugOOB 	sprintf(msgbuffer, "Just before second crash: %d,%d\n", i, j);
		//Gerard-SNAP181019_121941-DebugOOB 	OutputDebugStringA((LPCSTR)msgbuffer);
		//Gerard-SNAP181019_121941-DebugOOB }
	}
	if (pShedStat->iShedArea == 0)
	{
		pShedStat->iShedArea = pShedStat->iPondCell;
	}
	pShedStat->fPitVol = 0.0;
	for (iPitCellNo = 1; iPitCellNo <= pShedStat->iPitArea; iPitCellNo++) {
		//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "i,j,iPitCellNo: %d,%d,%d\n", i, j, iPitCellNo);
		//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
		//pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->GetAt(iPitCellNo));
	//pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->vary.at(iPitCellNo));
		//Gerard-SNAP181019_120420
		//Added bounds checker on the pfPondCellElevation so we don't go beyond the array:
		if ((pfPondCellElevSize - 1) > iPitCellNo) {
			pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->at(iPitCellNo));
		}
	}
	pShedStat->bDR_2_MV = drains_2_mv;
	delete (pfPondCellElev);
}

/**
*	\brief Method to check if the current record in a ponddata file is for a watershed that is OK to remove (i.e. the *watershed's depression is less than the specified threshold values for pit_area, pit_volume, pit_depth and mm_2_flood *as estimated by varatio.
*	This method corresponds to the 'chekifok' procedure in fox pro module
*
* @author Authors of Original LandMapR, Modified by Gashaw Ayalew
* @modified 2016-08-01
*/
bool DEMProcess::CheckIfOk(LSMSTATVAR *pShedStat, LSMPROCPARM *pLsmProcParm, LSMSTATVAR2 *pShedStat2)
{
	bool bOk2Remove = false;

	if ((pShedStat->fPitVol <= pLsmProcParm->fMaxVol) &&
		(pShedStat->iPitArea <= pLsmProcParm->iMaxArea) &&
		(pShedStat->fPourElev - pShedStat->fPitElev <= pLsmProcParm->fMaxDepth) &&
		(pShedStat->fVaratio <= pLsmProcParm->fMaxRunOff))
	{
		bOk2Remove = true;
		pShedStat2->iPitRec = pShedStat->iPitRec;
		pShedStat2->iInRec = pShedStat->iInRec;
		pShedStat2->iOutRec = pShedStat->iOutRec;
		pShedStat2->iNeighRow = pShedStat->iOutRow;
		pShedStat2->iNeighColumn = pShedStat->iOutColumn;
		pShedStat2->iCenRow = pShedStat->iInRow;
		pShedStat2->iCenColumn = pShedStat->iInColumn;
	}
	return bOk2Remove;
}


/**
	\brief Method to remove the currently selected pit by locating the pour point and establishing a new drainage direction and pour rec pointing from the in_rec INTO the out_rec at the pour point location.

	\detailed The program then does the following things:
	1)  it follows the flow path from the inside pour point DOWN to the old pit centre along the original (old) local drainage directions
	2)  as it flows DOWN it reverses the original local flow direction (DDIR) to point back upslope into the upslope cell on the flow path that leads to the pour point
	3)  once it reaches the old pit centre (DDIR=5) it records the original value for upslope area recorded for that intitial pit centre
	4)   it then follows the revised flow path back to the pour point and adds the value of the old pit centre upslope area to the value stored for upslope area in each cell
	5)  once it reaches the out_cell at the pour point it simply adds the new value for upslope area to each cell along the flow path from the outflow point to the new depression centre
	6)  after this has been completed it then renumbers all cells that were part of the removed watershed with the number of the new, combined, watershed
	This method corresponds  to the 'removepit' procedure in fox pro module
* @author Authors of Old LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
int DEMProcess::RemovePit(int *piUpSlpGrid, int *piFlowGrid, int *piDrecGrid, int *piShedGrid, int *piShedWGrid,
	LSMSTATVAR2 *pShedStat2, int *piNewShed)
{

	int last_ups = 0;
	int	iNextDown;
	int	iRecordNo;
	int	iUpRec;
	int iNewDir;
	//int	iNewUp;
	int	iNewPitRec;
	int	iOldShed;
	int path_len = 0;
	int nLoop = 0;
	int iNewUps;

	int ups_diff = 0;
	iOldShed = piShedWGrid[pShedStat2->iInRec - 1];
	iNewDir = GetNewDir(pShedStat2->iNeighRow, pShedStat2->iNeighColumn, pShedStat2->iCenRow, pShedStat2->iCenColumn);
	//iRecordNo = pShedStat2->iInRec;
	//while (iRecordNo != pShedStat2->iPitRec)
	//{
		//iRecordNo = piDrecGrid[iRecordNo - 1];
	//}
	iRecordNo = pShedStat2->iInRec;
	iNextDown = piDrecGrid[iRecordNo - 1];
	last_ups = piUpSlpGrid[iRecordNo - 1];

	piDrecGrid[pShedStat2->iInRec - 1] = pShedStat2->iOutRec;
	piFlowGrid[pShedStat2->iInRec - 1] = iNewDir;
	while (iRecordNo != pShedStat2->iPitRec)
	{
		pShedStat2->iNeighRow = (iRecordNo - 1) / _iColumn;
		pShedStat2->iNeighColumn = (iRecordNo - 1) % _iColumn;
		iUpRec = iRecordNo;
		pShedStat2->iCenRow = (iNextDown - 1) / _iColumn;
		pShedStat2->iCenColumn = (iNextDown - 1) % _iColumn;
		iRecordNo = iNextDown;
		path_len++;

		//Added june 9 2003.
		ups_diff = piUpSlpGrid[iRecordNo - 1] - last_ups;
		last_ups = piUpSlpGrid[iRecordNo - 1];
		piUpSlpGrid[iRecordNo - 1] = ups_diff;

		iNextDown = piDrecGrid[iRecordNo - 1];
		iNewDir = GetNewDir(pShedStat2->iNeighRow, pShedStat2->iNeighColumn, pShedStat2->iCenRow, pShedStat2->iCenColumn);
		piDrecGrid[iRecordNo - 1] = iUpRec;
		piFlowGrid[iRecordNo - 1] = iNewDir;
	}

	last_ups = piUpSlpGrid[iRecordNo - 1];
	//iNewUp = piUpSlpGrid[iRecordNo - 1];

	for (nLoop = 0; nLoop < path_len; nLoop++)
	{
		iRecordNo = piDrecGrid[iRecordNo - 1];
		iNewUps = piUpSlpGrid[iRecordNo - 1] + last_ups;
		piUpSlpGrid[iRecordNo - 1] = iNewUps;
		last_ups = piUpSlpGrid[iRecordNo - 1];
	}

	while (piFlowGrid[iRecordNo - 1] != 5)
	{
		iRecordNo = piDrecGrid[iRecordNo - 1];
		piUpSlpGrid[iRecordNo - 1] = piUpSlpGrid[iRecordNo - 1] + last_ups;
	}

	*piNewShed = piShedWGrid[iRecordNo - 1];
	iNewPitRec = iRecordNo;

	/*	for( nLoop=0;nLoop<_iRow*_iColumn;nLoop++)
	{
		//if(piShedWGrid[nLoop] == iOldShed &&
	}
	*/
	nLoop = _iRow * _iColumn - 1;
	return iNewPitRec;
}


/**
	\brief Method to compute the new value for local drainage direction (DDIR) for each cell along a flow path from a pour point to a pit centre such that the new DDIR points upslope from a cell into the cell above it along a flow path that leads directly to the pour point location

	\detailed This method corresponds to the 'Getnewddir' procedure in fox pro module
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
int DEMProcess::GetNewDir(int iNeighRow, int iNeighColumn, int iCenRow, int iCenColumn)
{
	int	iRowDiff;
	int	iColDiff;
	int	iNewDDir;

	iRowDiff = iNeighRow - iCenRow;
	iColDiff = iNeighColumn - iCenColumn;

	switch (iColDiff)
	{
	case -1:	if (iRowDiff == -1)
		iNewDDir = 7;
		if (iRowDiff == 0)
			iNewDDir = 4;
		if (iRowDiff == 1)
			iNewDDir = 1;
		break;

	case 0:		if (iRowDiff == -1)
		iNewDDir = 8;
		if (iRowDiff == 0)
			iNewDDir = 5;
		if (iRowDiff == 1)
			iNewDDir = 2;
		break;

	case 1:		if (iRowDiff == -1)
		iNewDDir = 9;
		if (iRowDiff == 0)
			iNewDDir = 6;
		if (iRowDiff == 1)
			iNewDDir = 3;
		break;
	}
	return iNewDDir;
}


/**
*	\brief Method to determine all relevant watershed statistics for any watershed that originates at a pit centre cell coded as DDIR=5
*
*	\detailed This method corresponds to the 'shedstat2' procedure fox pro module
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::ShedStat2(int ii, int pfPondCellElevSize, int *piDEMSort, double *pfDEMGrid, int *piFlowGrid, int *piShedGrid, int *piShedWGrid, LSMSTATVAR2 *pShedStat)
{
	mv_pour = 0;
	drains_2_mv = false;
	int		i, j, k, l;
	int		iStartRow;
	int		iStartCol;
	int		iEndRow;
	int		iEndCol;
	int		iPitCellNo;
	bool	bOverSpill;
	vector<double>	*pfPondCellElev;
	//vArray	*pfPondCellElev = new vArray();
	pfPondCellElev = new vector<double>(pfPondCellElevSize);

	int totalNums = _iRow * _iColumn;

	bOverSpill = false;
	i = piDEMSort[ii] / _iColumn;
	j = piDEMSort[ii] % _iColumn;
	pShedStat->iPitRow = pShedStat->iInRow = pShedStat->iOutRow = i + 1;
	pShedStat->iPitCol = pShedStat->iInColumn = pShedStat->iOutColumn = j + 1;
	pShedStat->iPitRec = pShedStat->iOutRec = piDEMSort[ii] + 1;
	pShedStat->iPondCell = 0;
	pShedStat->fPitElev = pShedStat->fInElev = pShedStat->fOutElev = pfDEMGrid[piDEMSort[ii]];
	pShedStat->fPourElev = 1000000.0;
	pShedStat->iShedArea = 0;
	pShedStat->fPitVol = 0.0;
	pShedStat->iThisShed = piShedWGrid[piDEMSort[ii]];

	while (ii < totalNums)
	{
		if (pShedStat->iThisShed == piShedWGrid[piDEMSort[ii]])
		{
			i = piDEMSort[ii] / _iColumn;
			j = piDEMSort[ii] % _iColumn;
			if ((i > 0) && (j > 0) && (i < _iRow - 1) && (j < _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == 0))
			{
				iStartRow = iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if ((i == 0) && (j == _iColumn - 1))
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			else if ((i == _iRow - 1) && (j == 0))
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if ((i == _iRow - 1) && (j == _iColumn - 1))
			{
				iStartRow = iStartCol = 1;
				iEndRow = iEndCol = 0;
			}
			else if (i == 0)
			{
				iStartRow = 0;
				iStartCol = 1;
				iEndRow = iEndCol = -1;
			}
			else if (j == 0)
			{
				iStartRow = 1;
				iStartCol = 0;
				iEndRow = iEndCol = -1;
			}
			else if (i == _iRow - 1)
			{
				iStartRow = 1;
				iStartCol = 1;
				iEndRow = 0;
				iEndCol = -1;
			}
			else if (j == _iColumn - 1)
			{
				iStartRow = iStartCol = 1;
				iEndRow = -1;
				iEndCol = 0;
			}
			pShedStat->fStartElev = *(pfDEMGrid + ((i*_iColumn) + j));
			pShedStat->fMinElev = 1000000.0;
			for (k = iStartRow; k >= iEndRow; k--)
			{
				for (l = iStartCol; l >= iEndCol; l--)
				{
					if ((k != 0) || (l != 0))
					{
						if (*(piShedWGrid + (((i - k)*_iColumn) + (j - l))) != dMissingValue)
						{
							if (*(piShedWGrid + (((i - k)*_iColumn) + (j - l))) != pShedStat->iThisShed)
							{
								if (*(pfDEMGrid + (((i - k)*_iColumn) + (j - l))) < pShedStat->fMinElev)
									pShedStat->fMinElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));
								if ((*(pfDEMGrid + (((i - k)*_iColumn) + (j - l))) < pShedStat->fPourElev) &&
									(pShedStat->fStartElev <= pShedStat->fPourElev))
								{
									pShedStat->fOutElev = *(pfDEMGrid + (((i - k)*_iColumn) + (j - l)));
									pShedStat->iOutRow = i - k + 1;
									pShedStat->iOutColumn = j - l + 1;
									pShedStat->iOutRec = ((i - k) * _iColumn) + pShedStat->iOutColumn;
									pShedStat->iOutShed = *(piShedWGrid + (((i - k)*_iColumn) + (j - l)));
									pShedStat->iPitArea = pShedStat->iPondCell + 1;
									pShedStat->iInRec = piDEMSort[ii] + 1;
									pShedStat->fInElev = pfDEMGrid[piDEMSort[ii]];
									pShedStat->iInRow = i + 1;
									pShedStat->iInColumn = j + 1;
									if (pShedStat->fOutElev > pShedStat->fInElev)
										pShedStat->fPourElev = pShedStat->fOutElev;
									else
										pShedStat->fPourElev = pShedStat->fInElev;
								}
							}
						}
						else
						{
							drains_2_mv = true;
							mv_pour = pShedStat->fStartElev;
						}
					}
				}
			}
			if ((pShedStat->fMinElev > pShedStat->fPourElev) && (pShedStat->fStartElev > pShedStat->fPourElev))
				bOverSpill = true;
			if (!bOverSpill)
			{
				pShedStat->iPondCell = pShedStat->iPondCell + 1;
				//pfPondCellElev->SetAtGrow(pShedStat->iPondCell, pfDEMGrid[piDEMSort[ii]]);
				//pfPondCellElev->SetAtGrow(pShedStat->iPondCell, pfDEMGrid[piDEMSort[ii]]);
				pfPondCellElev->at(pShedStat->iPondCell) = pfDEMGrid[piDEMSort[ii]];
				ii++;
			}
			else
			{
				pShedStat->iShedArea = pShedStat->iPondCell;
				while (ii < totalNums)
				{
					if (pShedStat->iThisShed == piShedWGrid[piDEMSort[ii]])
					{
						pShedStat->iShedArea = pShedStat->iShedArea + 1;
						ii++;
					}
					else
						ii = totalNums;
				}
			}
		}
		else
			ii = totalNums;
	}
	pShedStat->fPitVol = 0.0;
	for (iPitCellNo = 1; iPitCellNo <= pShedStat->iPitArea; iPitCellNo++)
		//pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->GetAt(iPitCellNo));
		//pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->vary.at(iPitCellNo));
		pShedStat->fPitVol = pShedStat->fPitVol + (pShedStat->fPourElev - pfPondCellElev->at(iPitCellNo));
	delete pfPondCellElev;
}

/**
	* \brief Method to remove ALL pits in a DEM from second lowest into lowest and then third lowest into the new lowest pit and so on and to compute pond & pit statistics for each pit, including higher order pits, as each is removed
*/
void DEMProcess::LowPitRemoval(LSMSTATVAR* ShedStats, LSMPONDSTATS** ptstPondStats, double m_GridSize,
	int iPitNo,
	double* pfDEMGrid, int* piDEMSort, int* piShedOrd,
	double* pfShed, double* pfBottomUp, int* piShedWGrid, int* piUpSlpGrid,
	int* pDrecGrid, int* pFlowGrid,
	VoldFile& vold, int& nPondFileSize, double* pfVol2FlGrid, double* pfMm2FlGrid,
	int* pfPArea, int* piFlowGrid)
	//ptstPondStats is pointer to a pointer
	//when array grows, we can realocate it, and the pointer that was passed into this
	//function will point to the correct memory location after this function returns
{


	if (iPitNo > 1)//check if there is more than one pit
		//if not, there is nothing to remove, just exit.
	{
		int* pTempDEMSort = piDEMSort;
		//double* pondcell_elev = new double [_iRow*_iColumn];
		LSMPONDSTATS* pPondStats;
		//start = clock();
		int nShedPointer = 0;
		maxrows = 0;
		maxcols = 0;
		max_shed = 0;
		max_ponds = 0;
		overspill = false;
		pondcell_no = 0;
		this_shed = 0;

		alldone = false;

		shed_area = 0;
		pit_row = 0;
		pit_col = 0;
		pit_rec = 0;
		pit_elev = 0;
		pit_vol = 0;
		pit_area = 0;
		bool Ok_2_Remove = false;
		drains_2_mv = false;
		pit1_no = 0;
		pit1_z = 0;
		pit1_to = 0;
		pit1_rec = 0;
		pit1_vol = 0;
		pit2_no = 0;
		pit2_z = 0;
		pit2_rec = 0;
		pit2_vol = 0;
		pre_vol = 0;

		in_row = 0;
		in_col = 0;
		in_rec = 0;
		in_elev = 0;
		out_row = 0;
		out_col = 0;
		out_rec = 0;
		out_elev = 0;
		out_shed = 0;
		pour_elev = 1000000;
		neigh_col = 0;
		centre_col = 0;
		neigh_row = 0;
		centre_row = 0;
		new_shed = 0;
		old_shed = 0;
		bool ok_2_remove = false;
		int new_pitrec = 0;
		edge_pit = false;
		at_final = false;

		int pits_removed = 0;
		int nIndex = 0;

		cent_y = 0;
		cent_x = 0;
		cent_z = 0;
		centrec = 0;
		//int numneigh = 0;
		m_varatio = 0;

		pPondStats = *ptstPondStats;
		cell_area = m_GridSize * m_GridSize;

		m_seqno = 0;
		m_ddir = 5;
		m_drec = 0;
		m_upslope = 1;
		m_shednow = 0;
		int nLoop = 0;
		int i = 0;
		int j = 0;
		//std::string* sMessage = new std::basic_string<TCHAR>;
		//Loop to copy pit table into pond table
		//pond array should grow as needed
		nIndex = 0;
		for (nLoop = 0; nLoop < iPitNo; nLoop++)
		{
			if (nPondFileSize % 1000 == 0 && nPondFileSize != 0)
			{
				LSMPONDSTATS* pTemp = new LSMPONDSTATS[nPondFileSize + 1000];

				for (int nLoop2 = 0; nLoop2 < nPondFileSize; nLoop2++)
				{
					pTemp[nLoop2] = pPondStats[nLoop2];
				}
				delete pPondStats;
				pPondStats = pTemp;
				*ptstPondStats = pPondStats;
			}
			pPondStats[nLoop].iThisShed = ShedStats[nIndex].iThisShed;
			pPondStats[nLoop].bEdge = ShedStats[nIndex].bEdge;
			pPondStats[nLoop].bFinal = ShedStats[nIndex].bFinal;
			pPondStats[nLoop].iEndPit = 0;
			pPondStats[nLoop].iShedArea = ShedStats[nIndex].iShedArea;
			pPondStats[nLoop].iPitRow = ShedStats[nIndex].iPitRow;
			pPondStats[nLoop].iPitCol = ShedStats[nIndex].iPitCol;
			pPondStats[nLoop].iPitRec = ShedStats[nIndex].iPitRec;
			pPondStats[nLoop].fPitElev = ShedStats[nIndex].fPitElev;
			pPondStats[nLoop].fPourElev = ShedStats[nIndex].fPourElev;
			pPondStats[nLoop].fPreVol = ShedStats[nIndex].fPreVol;
			pPondStats[nLoop].fPitVol = ShedStats[nIndex].fPitVol;
			pPondStats[nLoop].fVaratio = ShedStats[nIndex].fVaratio;
			pPondStats[nLoop].iPitArea = ShedStats[nIndex].iPitArea;
			pPondStats[nLoop].iDrainsTo = ShedStats[nIndex].iDrainsTo;

			//using outshed also because it is used for writing drainto to the file
			pPondStats[nLoop].iOutShed = ShedStats[nIndex].iOutShed;

			pPondStats[nLoop].iNextPit = ShedStats[nIndex].iNextPit;
			pPondStats[nLoop].iBecomes = ShedStats[nIndex].iBecomes;
			pPondStats[nLoop].bRemoved = ShedStats[nIndex].bRemoved;
			pPondStats[nLoop].iInRow = ShedStats[nIndex].iInRow;
			pPondStats[nLoop].iInColumn = ShedStats[nIndex].iInColumn;
			pPondStats[nLoop].iInRec = ShedStats[nIndex].iInRec;
			pPondStats[nLoop].fInElev = ShedStats[nLoop].fInElev;
			pPondStats[nLoop].iOutRow = ShedStats[nIndex].iOutRow;
			pPondStats[nLoop].iOutColumn = ShedStats[nIndex].iOutColumn;
			pPondStats[nLoop].iOutRec = ShedStats[nIndex].iOutRec;
			pPondStats[nLoop].fOutElev = ShedStats[nIndex].fOutElev;
			pPondStats[nLoop].iStage = ShedStats[nIndex].iStage;
			pPondStats[nLoop].bVisited = ShedStats[nIndex].bVisited;
			nIndex++;
			nPondFileSize++;
		}

		//initialize some of the variables in pond table.

		for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
		{
			pPondStats[nLoop].bFinal = false;
			pPondStats[nLoop].iNextPit = pPondStats[nLoop].iDrainsTo;
			pPondStats[nLoop].bRemoved = false;
			pPondStats[nLoop].iBecomes = 0;
			pPondStats[nLoop].iStage = 1;
			if (pPondStats[nLoop].iThisShed > max_shed)
			{
				max_shed = pPondStats[nLoop].iThisShed;
			}
		}
		max_ponds = nPondFileSize;
		maxrows = _iRow;
		maxcols = _iColumn;

		//*********************************************
		//Sorting of the pit file
		for (i = 0; i < nPondFileSize; i++)
		{
			pfShed[i] = (double)((((double)pPondStats[i].fPitElev) * pow(10.0, 10.0)) +
				(int)pPondStats[i].fVaratio);
			piShedOrd[i] = i;
		}

		QuickSort(pfShed, nPondFileSize, piShedOrd);
		/*
		//Sorting of the dem file
		for (j = 0; j < _iRow * _iColumn; j++)
		{
			if(pfDEMGrid[nLoop]>=0)
			{
				pfBottomUp[j] = (double)(((double)piShedWGrid[j] * pow(10.0, 18.0)) +
									((double)pfDEMGrid[j] * pow(10.0, 12.0)) +
									(1000000.0 - (double)piUpSlpGrid[j]));
				piDEMSort[j] = j;
			}
		}
		QuickSort(pfBottomUp, _iRow * _iColumn, piDEMSort);
		*/

		bool bEOF = true;
		//piDEMSort is 0 based not 1
		cent_y = piDEMSort[0] / _iColumn;
		cent_x = piDEMSort[0] % _iColumn;
		cent_z = pfDEMGrid[piDEMSort[0]];
		centrec = piDEMSort[0];
		int nCounter = 1;
		int iNumPits = iPitNo;
		int*  piHeapTbl, *piTempSortedDEM;

		pits_removed = 0;
		if ((piHeapTbl = new int[_iRow*_iColumn]) == NULL)
		{
			printf("Unable to create piHeapTbl in LowPitRemoval(), exiting ...");
			exit(1);
		}

		if ((piTempSortedDEM = new int[_iRow*_iColumn]) == NULL)
		{
			printf("Unable to create piTempSortedDEM in LowPitRemoval(), exiting ...");
			exit(1);
		}
		int* pTempPoint = piTempSortedDEM;
		int* pTemp = 0;
		//double* pfBottomUp
		while (alldone == false)
		{
			bEOF = true;
			for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
			{
				if (pPondStats[piShedOrd[nLoop]].bRemoved == false && pPondStats[piShedOrd[nLoop]].bFinal == false)
				{

					nShedPointer = piShedOrd[nLoop];
					FindLast(pPondStats, nShedPointer, nPondFileSize, piShedOrd);

					nLoop = nPondFileSize;
					bEOF = false;
				}
			}
			edge_pit = false;
			if (bEOF == false)
			{
				if (pPondStats[nShedPointer].bRemoved == false)//if a pit is allready removed, don't remove again
				{
					pits_removed = pits_removed + 1;
					Remove2Pit(pPondStats, nShedPointer, piShedOrd, nPondFileSize, pDrecGrid, pFlowGrid, piUpSlpGrid, piShedWGrid, &piDEMSort,
						vold, pfBottomUp, pfDEMGrid, new_pitrec, Ok_2_Remove, piHeapTbl, &piTempSortedDEM);
					//pTemp = piTempSortedDEM;
					//piTempSortedDEM = piDEMSort;
					//piDEMSort = pTemp;

					if (Ok_2_Remove == true)
					{
						nCounter++;

						ShedStat3(new_pitrec, nShedPointer, piShedWGrid, piDEMSort, pfDEMGrid, &pPondStats, piUpSlpGrid, pfBottomUp,
							pfShed, piShedOrd, nPondFileSize, pfVol2FlGrid, pfMm2FlGrid, pfPArea, piFlowGrid);

						//Reset index to orderpits by pit elevation
						for (i = 0; i < nPondFileSize; i++)
						{
							pfShed[i] = (double)((((double)pPondStats[i].fPitElev) * pow(10.0, 10.0)) +
								(int)pPondStats[i].fVaratio);
							piShedOrd[i] = i;
						}
						QuickSort(pfShed, nPondFileSize, piShedOrd);
					}
					else
					{
						//skip
						nShedPointer++;
					}
				}
				else
				{
					if (bEOF == false)
					{
						//skip
						nShedPointer++;
					}
					else
					{
						alldone = true;
					}
				}
			}
			else
			{
				alldone = true;
			}
		}
		*ptstPondStats = pPondStats;//making sure that we return a valid pointer
		delete[] piHeapTbl;
		//delete piTempSortedDEM;
		//delete pondcell_elev;
		/*
		for(nLoop=0;nLoop<_iRow*_iColumn;nLoop++)
		{
			pTempDEMSort[nLoop] = piTempSortedDEM[nLoop];
		}
		*/
		delete pTempPoint;
		//delete piHeapTbl;
	}
}

/**
	\brief Method to locate the last watershed in a list of watersheds. It should have the lowest pit centre in the sequence

	\detailed Follow pointers (DRAINSTO) until you return to a shed a second time. Then locate the lowest pit center on that particular cycle. If this watershed in not already removed, it is the one you want to remove. If it is an edge pit and drains to another edge pit, then don't remove it.

*  @author Authors of Original LandMapR, modified by Gashaw Ayalew
*  @modified 2016-08-01
*/
void DEMProcess::FindLast(LSMPONDSTATS* pPondStats, int& nPointer, int iPitNo, int* piShedOrd)
{
	bool at_bottom = false;
	at_final = false;

	int curr_pit = nPointer;
	int nLoop = 0;
	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		pPondStats[nLoop].bVisited = false;
	}
	double lowest_z = pPondStats[nPointer].fPitElev;
	lowest_pit = curr_pit;

	while (at_bottom == false)
	{
		if (pPondStats[nPointer].bVisited == true)
		{
			at_bottom = true;
			nPointer = lowest_pit;
		}
		else
		{
			pPondStats[nPointer].bVisited = true;
			pit1_no = pPondStats[nPointer].iThisShed;
			pit1_z = pPondStats[nPointer].fPitElev;
			pit1_to = pPondStats[nPointer].iNextPit;
			pit1_rec = nPointer;
			pit1_pour = pPondStats[nPointer].fPourElev;
			pit1_edge = pPondStats[nPointer].bEdge;

			if (pit1_z < lowest_z)
			{
				lowest_z = pit1_z;
				lowest_pit = pit1_rec;
			}

			for (nLoop = 0; nLoop < iPitNo; nLoop++)// find record with shedno = pit1_to
			{
				if (pPondStats[nLoop].iThisShed == pit1_to)
				{
					nPointer = nLoop;
					nLoop = iPitNo;
				}
			}

			if (pPondStats[nPointer].bFinal == true)
			{
				nPointer = pit1_rec;
				at_bottom = true;
				lowest_z = pit1_z;
				lowest_pit = pit1_rec;
				at_bottom = true;
			}
			else
			{
				if (pPondStats[nPointer].fPitElev < lowest_z)
				{
					lowest_z = pPondStats[nPointer].fPitElev;
					lowest_pit = nPointer;
				}

				if (pPondStats[nPointer].fPourElev < pit1_pour)
				{
					lowest_z = pPondStats[nPointer].fPitElev;
					lowest_pit = nPointer;
				}

				if (pPondStats[nPointer].bVisited == true)
				{
					nPointer = lowest_pit;
					at_bottom = true;
				}
				else
				{
					pit2_no = pPondStats[nPointer].iThisShed;
					pit2_z = pPondStats[nPointer].fPitElev;
					pit2_rec = nPointer;
					pit2_pour = pPondStats[nPointer].fPourElev;
					pit2_edge = pPondStats[nPointer].bEdge;

					if (pit2_pour < pit1_pour)
					{
						lowest_z = pPondStats[nPointer].fPitElev;
						lowest_pit = pit2_rec;
					}
					else
					{
						at_bottom = true;
						nPointer = lowest_pit;
						if (pPondStats[nPointer].bFinal == true)
						{
							at_final = true;
							nPointer = curr_pit;
							lowest_z = pPondStats[nPointer].fPitElev;
							lowest_pit = curr_pit;
						}
					}
				}
			}
		}
	}
}

/**
	\brief Method to remove the currently selected pit by locating the pour point and establishing a new drainage direction and pour rec pointing from the in_rec INTO the out_rec at the pour point location.
\detailed The program then does the following things:
	  1)  it follows the flow path from the inside pour point DOWN to the pit centre of the higher of 2 adjacent pits along the original (old) local drainage directions
	  2)  as it flows DOWN it reverses the original local flow direction (DDIR) to point back upslope into the upslope cell on the flow path that leads to the pour point
	  3)  as it flows down from the pour point to the pit center of the higher pit,it temporarily resets the upslope area count in each cell along the path to zero (0) as there is now no flow from upslope through these cells. It also adds 1 to a temporary variable called path_len to record the length of the flow path from the pour point to the current cell along the flow path.
	  4)  once it reaches the old pit centre (DDIR=5) it subtracts from the original value for upslope area the value computed for path length so that the new upslope area value at the old pit centre no longer includes a count of the number of cells along the reversed flow path
	  5)	The revised flow path is then re-traversed back to the pour point. Each cell along the path is assigned a new value for upslope area equal to the new value for upslope area at the old pit center plus the value for length of the reversed flow path from the old pit center to the current cell along the reversed flow path.
	  6)  once it reaches the out_cell at the pour point it then follows the original flow path from the pour point cell down slope until it reaches the pit center cell of the new, expanded pit.  As it flows downslope along this flow path, it adds to the original value for upslope area a new amount equal to the value for upslope from the old pit center MINUS the path length for the reversed flow path and PLUS the value for path length from the old pit cell to the pour point (in fact MINUS and PLUS cancel each other out and the value added is equal to the original value of UPSLOPE area at the old pit cell.
	  7)  after this has been completed it then renumbers all cells that were originally part of either of the 2 removed watersheds with a new watershed ID number for the new, combined, watershed
	First find the neighbor pit that this pit drains to.  Go to this pit and check that the pit elevation of the neighbor pit is less than the elevation of the current start pit.  Go to the pit with the higher pit elevation and remove the higher pit into the lower pit.
@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-09-01
	*/
void DEMProcess::Remove2Pit(LSMPONDSTATS* pPondStats, int& nPointer, int* piShedOrd, int iPitNo, int* pDirGrid,
	int* pFlowGrid, int* pUpSlpGrid, int* pShedNow, int** piDEMSort1,
	VoldFile& vold, double* pfBottomUp, double* pfDEMGrid,
	int& new_pitrec, bool& Ok_2_Remove,
	int*  piHeapTbl, int** piTempSortedDEM1)
{
	// a relic from *.dbf based data input/output
	//Note. Every time refer to the record number, column or row, have to take into accound that 
	//pit table is 1 based, and pPondStats array is 0 based.

	int* piTempSortedDEM = *piTempSortedDEM1;
	int* piDEMSort = *piDEMSort1;
	int* pTemp = 0;
	int nNewDir = 0;

	int ups_diff = 0;
	int last_ups = 0;
	int up_rec = 0;

	int nDEMPointer;

	long totalNums = _iRow * _iColumn;

	int nLoop = 0;
	int old_ups = 0;
	int new_ups = 0;
	pit1_no = pPondStats[nPointer].iThisShed;
	pit1_z = pPondStats[nPointer].fPitElev;
	pit1_to = pPondStats[nPointer].iNextPit;
	pit1_rec = nPointer;
	pit1_vol = pPondStats[nPointer].fPitVol;
	pit1_pour = pPondStats[nPointer].fPourElev;
	pit1_edge = pPondStats[nPointer].bEdge;
	m_varatio = pPondStats[nPointer].fVaratio;
	int pits_left = 0;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (pPondStats[nLoop].iThisShed == pit1_to)
		{
			nPointer = nLoop;
			nLoop = iPitNo;
		}
	}
	pit2_no = pPondStats[nPointer].iThisShed;
	pit2_z = pPondStats[nPointer].fPitElev;
	pit2_rec = nPointer;
	pit2_vol = pPondStats[nPointer].fPitVol;
	pit2_pour = pPondStats[nPointer].fPourElev;
	pit2_edge = pPondStats[nPointer].bEdge;

	if (pit1_pour == pit2_pour)
	{
		pre_vol = pit1_vol + pit2_vol;
	}
	else
	{
		if (pit1_pour < pit2_pour)
		{
			pre_vol = pit1_vol;
		}
		else
		{
			pre_vol = pit2_vol;
		}
	}

	nPointer = pit1_rec;
	if (pit1_z >= pit2_z)
	{
		pit_rec = pPondStats[nPointer].iPitRec - 1;
		in_rec = pPondStats[nPointer].iInRec - 1;
		out_rec = pPondStats[nPointer].iOutRec - 1;


		neigh_row = pPondStats[nPointer].iOutRow;
		neigh_col = pPondStats[nPointer].iOutColumn;
		centre_row = pPondStats[nPointer].iInRow;
		centre_col = pPondStats[nPointer].iInColumn;
	}
	else
	{
		in_rec = pPondStats[nPointer].iOutRec - 1;
		out_rec = pPondStats[nPointer].iInRec - 1;
		neigh_row = pPondStats[nPointer].iInRow;
		neigh_col = pPondStats[nPointer].iInColumn;
		centre_row = pPondStats[nPointer].iOutRow;
		centre_col = pPondStats[nPointer].iOutColumn;
		nPointer = pit2_rec;
		pit_rec = pPondStats[nPointer].iPitRec - 1;
	}
	if (pit1_edge == true && pit2_edge == true)
	{
		Ok_2_Remove = false;
		nPointer = pit1_rec;
		pPondStats[nPointer].bFinal = true;
		nPointer = pit2_rec;
		pPondStats[nPointer].bFinal = true;
	}
	else
	{
		Ok_2_Remove = true;
	}

	if (Ok_2_Remove == true)
	{
		for (nLoop = 0; nLoop < iPitNo; nLoop++)
		{
			if (pPondStats[nLoop].bRemoved == false)
			{
				if (pPondStats[nLoop].bFinal == false)
				{
					pits_left = pits_left + 1;
				}
			}
		}
		nPointer = nLoop - 1;
		if (pits_left == 0)
		{
			alldone = true;
		}

		if (alldone == false)
		{
			max_shed = max_shed + 1;
			max_ponds = max_ponds + 1;
			old_shed = 0;
			new_ups = 0;
			ups_diff = 0;
			last_ups = 0;

			nDEMPointer = in_rec;

			int Path_Len = 0;
			int next_down = 0;
			nNewDir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);
			next_down = pDirGrid[nDEMPointer] - 1;

			m_seqno = nDEMPointer;
			m_ddir = pFlowGrid[nDEMPointer];
			m_drec = pDirGrid[nDEMPointer] - 1;

			m_upslope = pUpSlpGrid[nDEMPointer];
			m_shednow = pShedNow[nDEMPointer];
			vold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, m_varatio);
			pDirGrid[nDEMPointer] = out_rec + 1;
			pFlowGrid[nDEMPointer] = nNewDir;

			last_ups = pUpSlpGrid[nDEMPointer];

			while (nDEMPointer != pit_rec)
			{
				neigh_col = nDEMPointer % _iColumn;
				neigh_row = nDEMPointer / _iColumn;

				up_rec = nDEMPointer;
				/*
				if (nDEMPointer !=pit_rec)
				{
					last_ups = pUpSlpGrid[nDEMPointer];
				}
				*/
				nDEMPointer = next_down;//go to the next cell
				Path_Len++;

				m_seqno = nDEMPointer;
				m_drec = pDirGrid[nDEMPointer] - 1;

				m_ddir = pFlowGrid[nDEMPointer];
				m_upslope = pUpSlpGrid[nDEMPointer];
				m_shednow = pShedNow[nDEMPointer];

				vold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, m_varatio);
				//if(nDEMPointer!=pit_rec)
				//{
				ups_diff = pUpSlpGrid[nDEMPointer] - last_ups;
				last_ups = pUpSlpGrid[nDEMPointer];
				pUpSlpGrid[nDEMPointer] = ups_diff;
				//}

				centre_row = nDEMPointer / _iColumn;
				centre_col = nDEMPointer % _iColumn;
				next_down = pDirGrid[nDEMPointer] - 1;

				nNewDir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);

				pDirGrid[nDEMPointer] = up_rec + 1;
				pFlowGrid[nDEMPointer] = nNewDir;
			}

			//old_ups = pUpSlpGrid[nDEMPointer];
			//new_ups = pUpSlpGrid[nDEMPointer] - last_ups;

			//pUpSlpGrid[nDEMPointer] = new_ups;

			//last_ups = new_ups;

			last_ups = pUpSlpGrid[nDEMPointer];
			for (nLoop = 0; nLoop < Path_Len; nLoop++)
			{
				nDEMPointer = pDirGrid[nDEMPointer] - 1;
				new_ups = pUpSlpGrid[nDEMPointer] + last_ups;
				pUpSlpGrid[nDEMPointer] = new_ups;
				last_ups = pUpSlpGrid[nDEMPointer];
			}

			while (pFlowGrid[nDEMPointer] != 5)//Loop until we reach pit
			{
				nDEMPointer = pDirGrid[nDEMPointer] - 1;

				Path_Len = Path_Len + 1;
				m_seqno = nDEMPointer;
				m_ddir = pFlowGrid[nDEMPointer];
				m_drec = pDirGrid[nDEMPointer] - 1;
				m_upslope = pUpSlpGrid[nDEMPointer];
				m_shednow = pShedNow[nDEMPointer];

				vold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, m_varatio);

				//pUpSlpGrid[nDEMPointer] = pUpSlpGrid[nDEMPointer] + old_ups;
				pUpSlpGrid[nDEMPointer] = pUpSlpGrid[nDEMPointer] + last_ups;
			}

			new_pitrec = nDEMPointer;
			nDEMPointer = 0;

			for (nLoop = 0; nLoop < totalNums; nLoop++)
			{
				if (pShedNow[nLoop] == pit1_no ||
					pShedNow[nLoop] == pit2_no)
				{
					pShedNow[nLoop] = max_shed;
				}
			}
			//int j =0;
			/*
			//Sorting of the dem file
			for (j = 0; j < _iRow * _iColumn; j++)
			{

				pfBottomUp[j] = (double)(((double)pShedNow[j] * pow(10.0, 18.0)) +
									((double)pfDEMGrid[j] * pow(10.0, 12.0)) +
									(1000000.0 - (double)pUpSlpGrid[j]));
				piDEMSort[j] = j;
			}

			QuickSort(pfBottomUp, _iRow * _iColumn, piDEMSort);
			//***************************************************
			*/

			SortBottomUp2(pShedNow, pfDEMGrid, pUpSlpGrid, &piDEMSort, max_shed, max_shed, max_shed,
				piHeapTbl, &piTempSortedDEM, pfBottomUp);

			//SortBottomUp2(pShedNow, pfDEMGrid,pUpSlpGrid,&piDEMSort,pit1_no,pit2_no, max_shed,
			//	piHeapTbl, &piTempSortedDEM,pfBottomUp);

			//pTemp = piTempSortedDEM;
			//piTempSortedDEM = piDEMSort;
			//piDEMSort = pTemp;
		}
	}
	*piDEMSort1 = piDEMSort;
	*piTempSortedDEM1 = piTempSortedDEM;
}

/**
\brief Method to determine all relevant watershed statistics for any watershed that originates at a pit centre cell coded as DDIR=5
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::ShedStat3(int& new_pitrec, int& nPointer, int* piShedWGrid, int* piDEMSort,
	double* pfDEMGrid, LSMPONDSTATS** pTempPondStats, int* piUpSlpGrid,
	double* pondcell_elev, double* pfShed, int* piShedOrd, int& nPondFileSize,
	double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea,
	int* piFlowGrid)
{
	LSMPONDSTATS* pPondStats = *pTempPondStats;

	pondcell_no = 0;
	int nDEMPointer = 0;
	nDEMPointer = new_pitrec;
	int new_ups = piUpSlpGrid[nDEMPointer];
	overspill = false;
	drains_2_mv = false;
	mv_pour = 0;
	this_shed = piShedWGrid[nDEMPointer];
	shed_area = 0;
	pit_row = nDEMPointer / _iColumn;
	pit_col = nDEMPointer % _iColumn;
	pit_rec = nDEMPointer;
	pit_elev = pfDEMGrid[nDEMPointer];
	pit_vol = 0;
	pit_area = 0;
	in_row = nDEMPointer / _iColumn;
	in_col = nDEMPointer % _iColumn;
	in_rec = nDEMPointer;
	in_elev = pfDEMGrid[nDEMPointer];
	out_row = nDEMPointer / _iColumn;
	out_col = nDEMPointer % _iColumn;
	out_rec = nDEMPointer;
	out_elev = pfDEMGrid[nDEMPointer];
	pour_elev = 1000000;

	cent_y = nDEMPointer / _iColumn;
	cent_x = nDEMPointer % _iColumn;
	cent_z = pfDEMGrid[nDEMPointer];
	centrec = nDEMPointer;
	int numneigh = 0;
	int zrec[8];
	int nLoop = 0;
	shed_mm2fl = 0;
	new_shed_area = 0;
	int nNotSortedDEMPointer = 0;

	long totalNums = _iRow * _iColumn;

	nNotSortedDEMPointer = nDEMPointer;

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if (piDEMSort[nLoop] == nDEMPointer)
		{
			nDEMPointer = nLoop;
			nLoop = _iColumn * _iRow;
		}
	}
	bool bExist = false;

	bool bExit2 = false;
	while (bExit2 == false && nDEMPointer < totalNums)
	{
		if (this_shed == piShedWGrid[piDEMSort[nDEMPointer]])
		{
			nNotSortedDEMPointer = piDEMSort[nDEMPointer];
			cent_y = nNotSortedDEMPointer / _iColumn;
			cent_x = nNotSortedDEMPointer % _iColumn;
			cent_z = pfDEMGrid[nNotSortedDEMPointer];
			GetNeig1(zrec, cent_y, cent_x, cent_z, numneigh);
			CheckNeig3(nNotSortedDEMPointer, piDEMSort, pfDEMGrid, piShedWGrid, zrec);

			if (!overspill)
			{
				pondcell_no = pondcell_no + 1;
				pondcell_elev[pondcell_no - 1] = pfDEMGrid[piDEMSort[nDEMPointer]];
				nDEMPointer++;
			}
			else
			{
				shed_area = pondcell_no;
				bool bExit = false;
				while (bExit == false && nDEMPointer < totalNums)
				{
					if (piShedWGrid[piDEMSort[nDEMPointer]] == this_shed)
					{
						shed_area++;
						nDEMPointer++;
					}
					else
					{
						bExit = true;
					}
				}
			}
		}
		else
		{
			bExit2 = true;
		}
		if (nDEMPointer == totalNums - 1)
		{
			bExit2 = true;
		}
	}

	if (shed_area != new_ups)
	{
		shed_area = new_ups;
	}

	CalcStat1(pondcell_elev);
	if (!alldone)
	{
		AddStat3(&pPondStats, nPondFileSize, pfShed, piShedOrd, nPointer);
		if (!at_final)
		{
			New_FlodVol(piDEMSort, piShedWGrid, pfDEMGrid, piUpSlpGrid, pfVol2FlGrid, pfMm2FlGrid, pfPArea, piFlowGrid);
		}
	}

	*pTempPondStats = pPondStats;
}

void DEMProcess::GetNeig1(int *zrec, int cent_y, int cent_x, double cent_z, int& numneigh)
{
	int nLoop = 0;
	numneigh = 0;
	for (nLoop = 0; nLoop < 8; nLoop++)
	{
		zrec[nLoop] = -1;
	}

	//find all valid neighbors
	if (cent_x > 0 && cent_y > 0)//Check to see if cell NW of centre is there (#1)
	{
		numneigh++;
		zrec[0] = (cent_y - 1) * _iColumn + (cent_x - 1);
	}
	if (cent_y > 0)//Check to see if cell due N of centre is there (#2)
	{
		numneigh++;
		zrec[1] = (cent_y - 1)* _iColumn + cent_x;
	}

	if (cent_y > 0 && cent_x < _iColumn - 1)//Check to see if cell NE of centre is there (#3)
	{
		numneigh++;
		zrec[2] = (cent_y - 1)* _iColumn + cent_x + 1;

	}

	if (cent_x > 0) //Check to see if cell due W of centre is there (#4)
	{
		numneigh++;
		zrec[3] = (cent_y - 0)* _iColumn + cent_x - 1;
	}

	if (cent_x < _iColumn - 1)//Check to see if cell due E of centre is there (#5)
	{
		numneigh++;
		zrec[4] = (cent_y - 0)* _iColumn + cent_x + 1;
	}

	if (cent_y < _iRow - 1 && cent_x >0)//Check to see if cell SW of centre is there (#6)
	{
		numneigh++;
		zrec[5] = ((cent_y + 1) * _iColumn + cent_x - 1);
	}

	if (cent_y < _iRow - 1)//Check to see if cell due S of centre is there (#7)
	{
		numneigh++;
		zrec[6] = (cent_y + 1)* _iColumn + cent_x;
	}

	if (cent_y < _iRow - 1 && cent_x < _iColumn - 1)//Check to see if cell SE of centre is there (#8)
	{
		numneigh++;
		zrec[7] = (cent_y + 1) * _iColumn + cent_x + 1;
	}
}

/**
\brief Method to go to all possible neighbor cells to the current centre cell (usually 8 neighbors) and to determine if any nieghbor cell(s) belong to a different watershed than the current centre cell.

\details If a neighbor belongs to a different watershed than the current centre cell then it is a possible pour point for overspill from the current depression into the neighboring watershed
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::CheckNeig3(int nDEMPointer, int* piDEMSort, double* pfDEMGrid, int* pShedNow, int* zrec)
{
	int start_rec = nDEMPointer;
	double start_elev = pfDEMGrid[nDEMPointer];
	double min_elev = 1000000;
	int nLoop = 0;
	for (nLoop = 0; nLoop < 8; nLoop++)
	{
		if (zrec[nLoop] != -1)
		{
			nDEMPointer = zrec[nLoop];

			if ((!(pfDEMGrid[nDEMPointer] == dMissingValue)))
			{
				if (pShedNow[nDEMPointer] != this_shed)
				{
					if (pfDEMGrid[nDEMPointer] < min_elev)
					{
						min_elev = pfDEMGrid[nDEMPointer];
					}
					if (pfDEMGrid[nDEMPointer] < pour_elev && start_elev <= pour_elev)
					{
						out_elev = pfDEMGrid[nDEMPointer];
						out_row = nDEMPointer / _iColumn;
						out_col = nDEMPointer % _iColumn;
						out_rec = nDEMPointer;
						out_shed = pShedNow[nDEMPointer];
						pit_area = pondcell_no + 1;
						nDEMPointer = start_rec;
						in_rec = start_rec;
						in_elev = start_elev;
						in_row = nDEMPointer / _iColumn;
						in_col = nDEMPointer % _iColumn;
						if (out_elev > in_elev)
						{
							pour_elev = out_elev;
						}
						else
						{
							pour_elev = in_elev;
						}
					}
				}
			}
			else
			{
				drains_2_mv = true;
				mv_pour = start_elev;
			}
		}
		else
		{
			edge_pit = true;
		}
	}

	if (min_elev > pour_elev && start_elev > pour_elev)
	{
		overspill = true;
	}
	nDEMPointer = start_rec;
	if (drains_2_mv == true)
	{
		edge_pit = true;
	}
}

/**
\brief Method to compute the volume of the depression up to the outflow or pour point
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::CalcStat1(double* pondcell_elev)
{
	int pitcell_no = 0;
	pit_vol = 0.00;
	for (pitcell_no = 0; pitcell_no < pit_area; pitcell_no++)
	{
		pit_vol = pit_vol + (pour_elev - pondcell_elev[pitcell_no]);
	}

	if (pit_area == 0)
	{
		alldone = true;
	}
}

/*
\brief Method to write pond statistics for the current watershed out to the selected ponddata file
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::AddStat3(LSMPONDSTATS ** pTempPondStats, int& nPondFileSize, double* pfShed,
	int* piShedOrd, int& nPointer)
{
	int nLoop = 0;
	LSMPONDSTATS* pPondStats = *pTempPondStats;
	if (nPondFileSize % 1000 == 0 && nPondFileSize != 0)
	{
		LSMPONDSTATS* pTemp = new LSMPONDSTATS[nPondFileSize + 1000];

		for (int nLoop2 = 0; nLoop2 < nPondFileSize; nLoop2++)
		{
			pTemp[nLoop2] = pPondStats[nLoop2];
		}
		delete pPondStats;
		pPondStats = pTemp;
		*pTempPondStats = pPondStats;
	}
	pPondStats[nPondFileSize].bVisited = false;
	pPondStats[nPondFileSize].bFinal = false;
	pPondStats[nPondFileSize].iEndPit = 0;
	pPondStats[nPondFileSize].iBecomes = 0;
	pPondStats[nPondFileSize].iStage = 0;
	pPondStats[nPondFileSize].bRemoved = false;

	pPondStats[nPondFileSize].iThisShed = max_shed;
	pPondStats[nPondFileSize].iShedArea = shed_area;

	pPondStats[nPondFileSize].iPitRow = pit_row + 1;
	pPondStats[nPondFileSize].iPitCol = pit_col + 1;

	pPondStats[nPondFileSize].iPitRec = pit_rec + 1;
	pPondStats[nPondFileSize].fPitElev = pit_elev;
	pPondStats[nPondFileSize].fPitVol = pit_vol;
	pPondStats[nPondFileSize].iPitArea = pit_area;
	pPondStats[nPondFileSize].iDrainsTo = out_shed;
	pPondStats[nPondFileSize].iNextPit = out_shed;

	pPondStats[nPondFileSize].iInRow = in_row + 1;
	pPondStats[nPondFileSize].iInColumn = in_col + 1;

	pPondStats[nPondFileSize].iInRec = in_rec + 1;
	pPondStats[nPondFileSize].fInElev = in_elev;

	pPondStats[nPondFileSize].iOutRow = out_row + 1;
	pPondStats[nPondFileSize].iOutColumn = 1;

	pPondStats[nPondFileSize].iOutRow = out_row + 1;
	pPondStats[nPondFileSize].iOutColumn = out_col + 1;

	pPondStats[nPondFileSize].iOutRec = out_rec + 1;
	pPondStats[nPondFileSize].fOutElev = out_elev;
	pPondStats[nPondFileSize].fPourElev = pour_elev;
	pPondStats[nPondFileSize].bEdge = edge_pit;
	pPondStats[nPondFileSize].fPreVol = pre_vol;
	pPondStats[nPondFileSize].iOutShed = out_shed;

	if (shed_area > 0)
	{
		m_varatio = pit_vol / shed_area * 1000;
	}
	else
	{
		m_varatio = 0.0;
	}

	if (m_varatio > 999999.99)
	{
		m_varatio = (double)999999.99;
	}

	pPondStats[nPondFileSize].fVaratio = m_varatio;

	if (!at_final)
	{
		pPondStats[nPondFileSize].bFinal = false;
	}
	else
	{
		pPondStats[nPondFileSize].bFinal = true;
	}

	shed_mm2fl = m_varatio;
	new_shed_area = shed_area;

	nPondFileSize++;
	for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
	{
		if (pPondStats[nLoop].iThisShed == pit1_no)
		{
			pPondStats[nLoop].iBecomes = max_shed;
			pPondStats[nLoop].bRemoved = true;
			nLoop = nPondFileSize;
		}
	}

	for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
	{
		if (pPondStats[nLoop].iThisShed == pit2_no)
		{
			pPondStats[nLoop].iBecomes = max_shed;
			pPondStats[nLoop].bRemoved = true;
			nLoop = nPondFileSize;
		}
	}


	for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
	{
		if (((pPondStats[nLoop].bRemoved == false) && (pPondStats[nLoop].bFinal == false) &&
			(pPondStats[nLoop].iNextPit == pit1_no)) || (pPondStats[nLoop].iNextPit == pit2_no))
		{
			pPondStats[nLoop].iNextPit = max_shed;
		}
	}

	//*********************************************
	int i = 0;
	//Sorting of the pond file

	//Can pond file be larger than number of pits ?
	// if yes, have to realocate more space for pfShed and piSHedOrd
	for (i = 0; i < nPondFileSize; i++)
	{
		pfShed[i] = (double)((((double)pPondStats[i].fPitElev) * pow(10.0, 10.0)) +
			(int)pPondStats[i].fVaratio);
		piShedOrd[i] = i;
	}

	QuickSort(pfShed, nPondFileSize, piShedOrd);
	//***************************************************8
	nPointer = 0;
}


/**
\brief Method to compute vol-to-flood and parea for a single depressional watershed created by removing one pit into its lower downslope neighbor

\detailed Method to compute the variables parea and mm-2-flood for a closed depression that has just been processed to compute its volume and overspill locations
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::New_FlodVol(int* piDEMSort, int* piShedWGrid, double* pfDEMGrid, int* piUpSlpGrid
	, double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea, int* piFlowGrid)
{
	bool bLoop = true;
	bool bLoop2 = true;
	int curr_parea = 0;
	int j = 0;
	double* pfBottomUp;


	long totalNum = _iRow * _iColumn;

	if ((pfBottomUp = new double[totalNum]) == NULL)
	{
		printf("Unable to create pfBottomUp array in New_FloodVol(), exiting ...");
		exit(1);
	}
	//**********************************************
	//Sorting of the dem file
	/*
	for (j = 0; j < _iRow * _iColumn; j++)
	{

		pfBottomUp[j] = (double)(((double)piShedWGrid[j] * pow(10.0, 18.0)) +
							((double)pfDEMGrid[j] * pow(10.0, 12.0)) +
							(1000000.0 - (double)piUpSlpGrid[j]));
		piDEMSort[j] = j;
	}

	QuickSort(pfBottomUp, _iRow * _iColumn, piDEMSort);
	//***************************************************
	*/
	//SortBottomUp2(piShedWGrid, pfDEMGrid,piUpSlpGrid,piDEMSort,pit1_no,pit2_no, max_shed);
	int nDEMPointer = 0;
	int nLoop = 0;
	delete[] pfBottomUp;

	double curr_elev = 0.0;
	int total_cells = 0;
	double last_elev = 0.0;
	double elev_diff = 0.0;
	double curr_vol = double(0.1);
	double curr_mm2fl = 0.0;
	for (nLoop = 0; nLoop < totalNum; nLoop++)
	{
		if (piDEMSort[nLoop] == pit_rec)
		{
			nDEMPointer = nLoop;
			nLoop = totalNum;
		}
	}

	this_shed = piShedWGrid[piDEMSort[nDEMPointer]];
	last_elev = pfDEMGrid[piDEMSort[nDEMPointer]];

	while (bLoop)//loop until end of file riched or second condition is false.
	{
		if (nDEMPointer < totalNum)
		{
			if (pfDEMGrid[piDEMSort[nDEMPointer]] <= pour_elev && piShedWGrid[piDEMSort[nDEMPointer]] == this_shed)
			{
				curr_elev = pfDEMGrid[piDEMSort[nDEMPointer]];
				bLoop2 = true;
				while (bLoop2)//again loop until end of file or second condition is false
				{
					if (nDEMPointer < totalNum)
					{
						if (curr_elev == pfDEMGrid[piDEMSort[nDEMPointer]])
						{
							total_cells++;
							nDEMPointer++;
						}
						else
						{
							bLoop2 = false;
						}
					}
					else
					{
						bLoop2 = false;
					}
				}
				nDEMPointer--;
				elev_diff = (curr_elev - last_elev) * 1000;
				curr_vol = curr_vol + (elev_diff*(total_cells - 1));
				if (pfPArea[piDEMSort[nDEMPointer]] == 0)
				{
					pfVol2FlGrid[piDEMSort[nDEMPointer]] = (float)curr_vol;
					pfPArea[piDEMSort[nDEMPointer]] = total_cells;
				}
				last_elev = pfDEMGrid[piDEMSort[nDEMPointer]];
				nDEMPointer++;
			}
			else
			{
				bLoop = false;
			}
		}
		else
		{
			bLoop = false;
		}
	}

	for (nLoop = 0; nLoop < total_cells; nLoop++)
	{
		nDEMPointer--;
		if (piFlowGrid[piDEMSort[nDEMPointer]] == 5)
		{
			nLoop = total_cells;
		}
		if (pfPArea[piDEMSort[nDEMPointer]] == 0)
		{
			pfVol2FlGrid[piDEMSort[nDEMPointer]] = (float)curr_vol;
			pfPArea[piDEMSort[nDEMPointer]] = curr_parea;
		}
		else
		{
			curr_vol = pfVol2FlGrid[piDEMSort[nDEMPointer]];
			curr_parea = pfPArea[piDEMSort[nDEMPointer]];
		}
	}
}

/**
 \brief Method to add to the original pit file of first order pits a number that identifies the final order pit into which each first order pit will ultimately drain.  This is referred to as the END_PIT.  It is identified as part of the process of computing volume-to-flood and the topology of pit filling and overspill in the initial pit removing exercise (Remove_by_Z)
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
	*/
void DEMProcess::Fix_PitFile(LSMSTATVAR* ShedStat, double* pfDEMGrid, int* piShedNow,
	int iPitNo, int* piShedGrid)
{

	int nDEMPointer = 0;
	int curr_shed = 0;
	int curr_endpit = 0;
	long totalNum = _iColumn * _iRow;

#pragma omp parallel for
	for (int nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		curr_shed = ShedStat[nLoop].iThisShed;
		for (int nIndex = 0; nIndex < totalNum; nIndex++)
		{
			if (piShedGrid[nIndex] == curr_shed)
			{
				curr_endpit = piShedNow[nIndex];
				nIndex = totalNum;
			}
		}
		ShedStat[nLoop].iEndPit = curr_endpit;
	}
}

/**
	* \brief Method to take all changes made to the DEM file during the first pit removing exercise (removing from bottom to top) and to put the data back into the DEM file in reverse order to that in which it was changed.  In the end result of putting all the old data back in should be a DEM file identical to the DEM file at the point before the first pit removing exercise was carried out.

	\detailed This is not strictly true, as the DEM file now contains new data for vol2fl and parea for all cells that can inundate.  This was not in the original DEM file.
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
	*/
void DEMProcess::Reset_Dem(VoldFile& vold, int* piFlowGrid, int* piDrecGrid, int* piUpSlpGrid,
	int* piShedWGrid, int* piShedGrid)
{

	int curr_seqno = 0;
	int curr_ddir = 0;
	int curr_drec = 0;
	int curr_upslope = 0;
	int nDEMPointer = 0;
	int nSize = vold.GetSize();
	int nLoop = 0;

	long totalNum = _iRow * _iColumn;

	for (nLoop = nSize - 1; nLoop >= 0; nLoop--)
	{
		curr_seqno = vold.GetSeqNo(nLoop);
		curr_ddir = vold.GetDdir(nLoop);
		curr_drec = vold.GetDrec(nLoop);
		curr_upslope = vold.GetUpSlop(nLoop);
		nDEMPointer = curr_seqno - 1;
		piFlowGrid[nDEMPointer] = curr_ddir;
		piDrecGrid[nDEMPointer] = curr_drec;
		piUpSlpGrid[nDEMPointer] = curr_upslope;
	}
	for (nLoop = 0; nLoop < totalNum; nLoop++)
	{
		piShedWGrid[nLoop] = piShedGrid[nLoop];
	}
}

/**
	\brief Method to remove pits in order of mm-2-flood and to re-compute pond & pit statistics for all pits as they are removed or amalgamated
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
*@modified 2016-08-01
	*/
void DEMProcess::LSM_PitR8(int* piShedGrid, LSMSTATVAR* PitFile, int& iPitNo, LSMSTATVAR** pFillFile,
	int& nFillFileSize,
	double* pfDEMGrid, int*  piShedWGrid, int* piUpSlpGrid,
	VoldFile& mold, int* piFlowGrid, int* piDrecGrid, double *pfVol2FlGrid,
	double* pfMm2FlGrid, LSMPONDSTATS * pPondStats)
{
	long totalNum = _iRow * _iColumn;

	if (iPitNo > 1)
	{
		int *piHeapTbl, *piTempSortedDEM;

		if ((piHeapTbl = new int[totalNum]) == NULL)
		{
			printf("Unable to create piHeapTbl in LSM_PitR8(), exiting ...");
			exit(1);
		}
		if ((piTempSortedDEM = new int[totalNum]) == NULL)
		{
			printf("Unable to create piTempSortedDEM in LSM_PitR8(), exiting ...");
			exit(1);
		}
		int nDEMPointer = 0;
		bool bEOF = false;
		int j = 0;
		maxrows = 0;
		maxcols = 0;
		max_shed = 0;
		max_ponds = 0;
		max_va = 0;
		alldone = false;
		no_pits = 0;

		overspill = false;
		pondcell_no = 0;
		this_shed = 0;
		shed_area = 0;
		pit_row = 0;
		pit_col = 0;
		pit_rec = 0;
		pit_elev = 0;
		pit_vol = 0;
		pit_area = 0;
		in_row = 0;
		in_col = 0;
		in_rec = 0;
		in_elev = 0;
		out_row = 0;
		out_col = 0;
		out_rec = 0;
		out_elev = 0;
		out_shed = 0;
		pour_elev = 1000000;
		neigh_col = 0;
		centre_col = 0;
		neigh_row = 0;
		centre_row = 0;
		new_shed = 0;
		old_shed = 0;
		bool ok_2_remove = false;

		drains_2_mv = false;
		mv_pour = 0;

		int new_ddir = 5;

		pit1_no = 0;
		pit1_rec = 0;
		pit2_no = 0;
		edge_pit = false;

		cent_y = 0;
		cent_x = 0;
		cent_z = 0;
		centrec = 0;
		int numneigh = 0;
		m_varatio = 0;

		int pit1_end = 0;
		pre_vol = 0;

		double* pfBottomUp;
		if ((pfBottomUp = new double[totalNum]) == NULL)
		{
			printf("Unable to create pfBottomUp array in LSM_PitR8(), exiting ...");
			exit(1);
		}
		int* piDEMSort;
		if ((piDEMSort = new int[totalNum]) == NULL)
		{
			printf("Unable to create piDEMSort array in LSM_PitR8(), exiting ...");
			exit(1);
		}
		cell_area = _fGridSize * _fGridSize;

		int nLoop = 0;
		for (nLoop = 0; nLoop < iPitNo; nLoop++)
		{
			if (PitFile[nLoop].iThisShed > max_shed)
			{
				max_shed = PitFile[nLoop].iThisShed;
			}
		}
		max_ponds = iPitNo;

		for (nLoop = 0; nLoop < iPitNo; nLoop++)
		{
			PitFile[nLoop].iNextPit = PitFile[nLoop].iDrainsTo;
			PitFile[nLoop].bRemoved = false;
			PitFile[nLoop].iBecomes = 0;
			PitFile[nLoop].iStage = 1;
			PitFile[nLoop].bVisited = false;
		}
		//**********************************************
		//Sorting of the dem file
		int nMissingNum = 0;
		int* pTempSorted;

		if ((pTempSorted = new int[totalNum]) == NULL)
		{
			printf("Unable to create pTempSorted array in LSM_PitR8(), exiting ...");
		}
		int nIndex = 0;
		int nIndex1 = 0;
		int i;
		for (i = 0; i < totalNum; i++)
		{
			if (!(pfDEMGrid[i] == dMissingValue))//make sure that field has valid value
			{
				pfBottomUp[nIndex] = (double)(((double)piShedWGrid[i] * pow(10.0, 18.0)) +
					((double)pfDEMGrid[i] * pow(10.0, 12.0)) +
					(1000000.0 - (double)piUpSlpGrid[i]));
				piDEMSort[nIndex] = i;
				nIndex++;
			}
			else
			{
				pTempSorted[nMissingNum] = i;
				nMissingNum++;//keep track of missig values
			}
		}

		//***************************************************
		QuickSort(pfBottomUp, totalNum - nMissingNum, piDEMSort);//	sort valid values

		nIndex = 0;
		for (i = totalNum - nMissingNum; i < totalNum; i++)//assing values to the rest of the sorted array
			//the rest is pointers to the records with missing values, and they not used, 
			//that is why we can make them point to 
		{
			piDEMSort[i] = pTempSorted[nIndex];
			nIndex++;
		}
		delete[] pTempSorted;
		maxrows = _iRow;
		maxcols = _iColumn;

		numneigh = 0;
		int pits_removed = 0;
		int nPitFilePointer = 0;
		bool all_done = false;
		int iNumPits = iPitNo;
		int* pTemp = 0;
		while (bEOF == false && all_done == false)
		{
			Find_LowVa(PitFile, iPitNo, ok_2_remove, nPitFilePointer);
			if (alldone == false)
			{
				Chek4Edge(PitFile, ok_2_remove, nPitFilePointer, iPitNo);
				if (ok_2_remove && pit1_rec + 1 <= no_pits)
				{
					curr_mm2flood = PitFile[nPitFilePointer].fVaratio;
					Find_Low_Pit(PitFile, nPitFilePointer, iPitNo);

					pits_removed = pits_removed + 1;
					RemovePit8(mold, piFlowGrid, piDrecGrid, pfDEMGrid, piUpSlpGrid, piShedWGrid, pfBottomUp, &piDEMSort,
						piHeapTbl, &piTempSortedDEM);

					ShedStat8(piShedWGrid, pfDEMGrid, piDEMSort, nDEMPointer, piUpSlpGrid, &*pFillFile, nFillFileSize, PitFile,
						iPitNo, pfVol2FlGrid, pfMm2FlGrid, pfBottomUp);
					nPitFilePointer = 0;
				}
				else
				{
					if (!(nPitFilePointer >= iPitNo - 1))
					{
						nPitFilePointer++;
					}
					else
					{
						bEOF = true;
					}
				}
			}
			else
			{
				bEOF = true;
			}
		}
		Final_Pits(PitFile, iPitNo, &*pFillFile, nFillFileSize, pPondStats);
		delete[] pfBottomUp;
		delete[] piTempSortedDEM;
		delete[] piDEMSort;
		delete[] piHeapTbl;
	}
}

/**
	\brief Method to check if the current record in a ponddata file is for a watershed that is ok to remove.  If the current watershed drains to a neighbor watershed that belongs to the same endpit sequence (e.g. both eventually flow to the same endpit), then it is OK to remove the current pit into the neighbor pit.  If the neighbor pit belongs to  different endpit sequence, then it is not OK to remove the current pit into its neighbor pit.
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::Find_LowVa(LSMSTATVAR* PitFile, int iPitNo, bool& ok_2_remove, int& nPitPointer)
{

	int curr_pit = 0;
	double lowest_va = 100000000;
	int no_change = true;
	int nIndex;
	no_pits = iPitNo;

	while (nPitPointer < iPitNo)
	{
		nIndex = nPitPointer;
		if (PitFile[nPitPointer].bVisited == false)
		{
			Chek4Edge(PitFile, ok_2_remove, nPitPointer, iPitNo);
			if (ok_2_remove == true)
			{
				if (PitFile[nPitPointer].fVaratio < lowest_va)
				{
					lowest_va = PitFile[nPitPointer].fVaratio;
					curr_pit = nPitPointer;
					no_change = false;
				}
			}
		}
		if (nPitPointer < iPitNo)
		{
			nPitPointer++;
		}
	}

	if (no_change)
	{
		alldone = true;
	}
	else
	{
		nPitPointer = curr_pit;
		if (max_va < lowest_va && ok_2_remove == true)
		{
			max_va = lowest_va;
		}
	}
}

/**
	\brief Method to check if the current record in a ponddata file is for a watershed that is ok to remove.  If the current watershed drains to a neighbor watershed that belongs to the same endpit sequence (e.g. both eventually flow to the same endpit), then it is OK to remove the current pit into the neighbor pit.  If the neighbor pit belongs to a different endpit sequence, then it is not OK to remove the current pit into its neighbor pit.
*
*@author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::Chek4Edge(LSMSTATVAR* PitFile, bool& ok_2_remove, int& nPitPointer, int iPitNo)
{
	int nLoop = 0;
	ok_2_remove = false;
	pit1_to = PitFile[nPitPointer].iNextPit;
	pit1_rec = nPitPointer;
	pit1_end = PitFile[nPitPointer].iEndPit;

	nPitPointer = iPitNo;
	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (PitFile[nLoop].iThisShed == pit1_to)
		{
			nPitPointer = nLoop;
			nLoop = iPitNo;
		}
	}

	pit2_rec = nPitPointer;
	pit2_end = 0;
	if (nPitPointer < iPitNo)
	{
		pit2_end = PitFile[nPitPointer].iEndPit;
	}

	if (pit1_end == pit2_end)
	{
		ok_2_remove = true;
	}
	else
	{
		ok_2_remove = false;
	}

	if (pit1_rec <= no_pits)
	{
		nPitPointer = pit1_rec;
	}

	if (ok_2_remove == false)
	{
		PitFile[pit1_rec].bVisited = true;
	}
}

/**
	\brief Method to check the elevation of the pit centre for the current pit to be removed and for the neighbot pit into which it is to be removed.
\detailed If the pit elevation of the neighbor pit is less than the pit elevation of the current pit, then remove the current pit into the neighbor pit. If the pit elevation of the neighbor pit is greater then the pit elevation of the current pit, then remove the neighbor pit into the current pit, rather than the other way round.  Always remove the higher pit into the lower pit.  Do this by identifying the IN_REC as the pour point of the pit that is to be removed.
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-10
*/
void DEMProcess::Find_Low_Pit(LSMSTATVAR *PitFile, int& nPitPointer, int iPitNo)
{

	int nLoop = 0;

	pit1_no = PitFile[nPitPointer].iThisShed;
	pit1_z = PitFile[nPitPointer].fPitElev;
	pit1_to = PitFile[nPitPointer].iNextPit;
	pit1_rec = nPitPointer; // record pit1_rec located at pit1_rec -1 location
	pit1_vol = PitFile[nPitPointer].fPitVol;
	pit1_pour = PitFile[nPitPointer].fPourElev;
	pit1_end = PitFile[nPitPointer].iEndPit;
	m_varatio = PitFile[nPitPointer].fVaratio;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (PitFile[nLoop].iThisShed == pit1_to)
		{
			nPitPointer = nLoop;
			nLoop = iPitNo;
		}
	}

	pit2_no = PitFile[nPitPointer].iThisShed;
	pit2_z = PitFile[nPitPointer].fPitElev;
	pit2_rec = nPitPointer;
	pit2_vol = PitFile[nPitPointer].fPitVol;
	pit2_pour = PitFile[nPitPointer].fPourElev;
	pit2_end = PitFile[nPitPointer].iEndPit;

	if (pit1_pour == pit2_pour)
	{
		pre_vol = pit1_vol + pit2_vol;
	}
	else
	{
		if (pit1_pour < pit2_pour)
		{
			pre_vol = pit1_vol;
		}
		else
		{
			pre_vol = pit2_vol;
		}
	}

	nPitPointer = pit1_rec;

	if (pit1_z >= pit2_z)
	{
		pit_rec = PitFile[nPitPointer].iPitRec - 1;
		in_rec = PitFile[nPitPointer].iInRec - 1;
		out_rec = PitFile[nPitPointer].iOutRec - 1;
		neigh_row = PitFile[nPitPointer].iOutRec / _iColumn;//PitFile[nPitPointer].iOutRow;
		neigh_col = PitFile[nPitPointer].iOutRec % _iColumn;//PitFile[nPitPointer].iOutColumn;
		centre_row = PitFile[nPitPointer].iInRec / _iColumn;//PitFile[nPitPointer].iInRow;
		centre_col = PitFile[nPitPointer].iInRec % _iColumn;//PitFile[nPitPointer].iInColumn;
	}
	else
	{
		in_rec = PitFile[nPitPointer].iOutRec - 1;
		out_rec = PitFile[nPitPointer].iInRec - 1;
		neigh_row = PitFile[nPitPointer].iInRec / _iColumn;//PitFile[nPitPointer].iInRow;
		neigh_col = PitFile[nPitPointer].iInRec %_iColumn;//PitFile[nPitPointer].iInColumn;
		centre_row = PitFile[nPitPointer].iOutRec / _iColumn;//PitFile[nPitPointer].iOutRow;
		centre_col = PitFile[nPitPointer].iOutRec % _iColumn;//PitFile[nPitPointer].iOutColumn;
		nPitPointer = pit2_rec;
		pit_rec = PitFile[nPitPointer].iPitRec - 1;
	}
}

/**
\brief Method to remove the currently selected pit by locating the pour point and establishing a new drainage direction and pour rec pointing from the in_rec INTO the out_rec at the pour point location.
*
	*  \detailed The program then does the following things:
	*  1)  it follows the flow path from the inside pour point DOWN to the old pit centre along the original (old) local drainage directions
	*  2)  as it flows DOWN it reverses the original local flow direction (DDIR) to point back upslope into the upslope cell on the flow path that leads to the pour point
	*  3)  once it reaches the old pit centre (DDIR=5) it records the original value for upslope area recorded for that intitial pit centre
	*  4)  it then follows the revised flow path back to the pour point and adds the value of the old pit centre upslope area to the value stored for upslope area in each cell
	*  5)  once it reaches the out_cell at the pour point it simply adds the new value for upslope area to each cell along the flow path from the outflow point to the new depression centre
	*  6)  after this has been completed it then renumbers all cells that were part of the removed watershed with the number of the new, combined, watershed
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
	*/
void DEMProcess::RemovePit8(VoldFile &mold, int *piFlowGrid, int *piDrecGrid, double *pfDEMGrid, int *piUpSlpGrid,
	int*  piShedWGrid, double* pfBottomUp, int** piDEMSort1,
	int* piHeapTbl, int** piTempSortedDEM1)
{
	int* piDEMSort = *piDEMSort1;
	int* piTempSortedDEM = *piTempSortedDEM1;
	int j = 0;
	int nLoop = 0;
	max_shed = max_shed + 1;
	max_ponds = max_ponds + 1;

	int old_ups = 0;

	int last_ups = 0;
	int ups_diff = 0;

	int up_rec;
	int new_dir = 0;
	int nDEMPointer = in_rec;
	int Path_Len = 0;
	new_dir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);
	int next_down = piDrecGrid[nDEMPointer] - 1;

	int totalNum = _iRow * _iColumn;

	m_seqno = nDEMPointer;
	m_ddir = piFlowGrid[nDEMPointer];
	m_drec = piDrecGrid[nDEMPointer] - 1;
	m_upslope = piUpSlpGrid[nDEMPointer];
	m_shednow = piShedWGrid[nDEMPointer];

	mold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, curr_mm2flood);

	piDrecGrid[nDEMPointer] = out_rec + 1;
	piFlowGrid[nDEMPointer] = new_dir;

	last_ups = piUpSlpGrid[nDEMPointer];
	while (nDEMPointer != pit_rec)
	{
		neigh_col = nDEMPointer % _iColumn;
		neigh_row = nDEMPointer / _iColumn;
		up_rec = nDEMPointer;
		/*
		if(nDEMPointer!=pit_rec)
		{
			last_ups = piUpSlpGrid[nDEMPointer];
		}
		*/
		nDEMPointer = next_down;
		Path_Len = Path_Len + 1;


		//if (nDEMPointer!=pit_rec)
		//{
		ups_diff = piUpSlpGrid[nDEMPointer] - last_ups;
		last_ups = piUpSlpGrid[nDEMPointer];
		piUpSlpGrid[nDEMPointer] = ups_diff;
		//}

		m_seqno = nDEMPointer;
		m_ddir = piFlowGrid[nDEMPointer];
		m_drec = piDrecGrid[nDEMPointer] - 1;
		m_upslope = piUpSlpGrid[nDEMPointer];
		m_shednow = piShedWGrid[nDEMPointer];

		mold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, curr_mm2flood);

		centre_row = nDEMPointer / _iColumn;
		centre_col = nDEMPointer % _iColumn;
		next_down = piDrecGrid[nDEMPointer] - 1;
		new_dir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);

		piDrecGrid[nDEMPointer] = up_rec + 1;
		piFlowGrid[nDEMPointer] = new_dir;
	}

	last_ups = piUpSlpGrid[nDEMPointer];
	//old_ups = piUpSlpGrid[nDEMPointer];
	//new_ups = piUpSlpGrid[nDEMPointer]-last_ups;

	//piUpSlpGrid[nDEMPointer] = new_ups;
	//last_ups = new_ups;

	for (nLoop = 0; nLoop < Path_Len; nLoop++)
	{
		nDEMPointer = piDrecGrid[nDEMPointer] - 1;
		new_ups = piUpSlpGrid[nDEMPointer] + last_ups;
		piUpSlpGrid[nDEMPointer] = new_ups;
		last_ups = piUpSlpGrid[nDEMPointer];
	}

	while (piFlowGrid[nDEMPointer] != 5)
	{
		nDEMPointer = piDrecGrid[nDEMPointer] - 1;
		Path_Len = Path_Len + 1;
		m_seqno = nDEMPointer;
		m_ddir = piFlowGrid[nDEMPointer];
		m_drec = piDrecGrid[nDEMPointer] - 1;
		m_upslope = piUpSlpGrid[nDEMPointer];
		m_shednow = piShedWGrid[nDEMPointer];
		mold.AddNew(m_seqno + 1, m_ddir, m_drec + 1, m_upslope, m_shednow, max_shed, 0, 0, curr_mm2flood);
		//piUpSlpGrid[nDEMPointer] = piUpSlpGrid[nDEMPointer]+ old_ups;
		piUpSlpGrid[nDEMPointer] = piUpSlpGrid[nDEMPointer] + last_ups;
	}

	/*
	* At this point, we have changed the original flow directions
	* to direct flow out of the pit we removed by flowing upslope
	* from the old pit center to the originally identified pour
	* point and from there into the adjacent neighbor watershed
	* and thence downslope from that pour point to the new pit
	* center (which must always be lower than the old pit center).

	* We now want to change the unique ID number assigned to all
	* cells in the new merged watershed from the 2 original ID
	* numbers to a new number that is 1 greater than the largest
	* number previously assigned to any watershed in the matrix.
	*/
	new_pitrec = nDEMPointer;

	for (nLoop = 0; nLoop < totalNum; nLoop++)
	{
		if (piShedWGrid[nLoop] == pit1_no || piShedWGrid[nLoop] == pit2_no)
		{
			piShedWGrid[nLoop] = max_shed;
		}
	}

	SortBottomUp2(piShedWGrid, pfDEMGrid, piUpSlpGrid, &piDEMSort, max_shed, max_shed, max_shed,
		piHeapTbl, &piTempSortedDEM, pfBottomUp);

	//SortBottomUp2(piShedWGrid,pfDEMGrid,piUpSlpGrid,&piDEMSort,pit1_no,pit2_no, max_shed,
	//	piHeapTbl, &piTempSortedDEM,pfBottomUp);
	*piTempSortedDEM1 = piTempSortedDEM;
	*piDEMSort1 = piDEMSort;
}



/**
	\brief Method to determine all relevant watershed statistics for any watershed that originates at a pit centre cell coded as DDIR=5
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::ShedStat8(int *piShedWGrid, double *pfDEMGrid, int *piDEMSort, int& nDEMPointer,
	int* piUpSlpGrid, LSMSTATVAR** pFillFile, int& nFillFileSize,
	LSMSTATVAR *PitFile, int &iPitNo, double * pfVol2FlGrid, double* pfMm2FlGrid,
	double* pondcell_elev)
{
	bool bExit = false;
	bool bValid = true;
	int zrec[8];
	//double pondcell_elev[65000];
	int nLoop = 0;
	int nDEMSortedPointer = 0;
	nDEMPointer = new_pitrec;
	new_ups = piUpSlpGrid[nDEMPointer];
	overspill = false;
	pondcell_no = 0;
	this_shed = piShedWGrid[nDEMPointer];
	shed_area = 0;
	pit_row = nDEMPointer / _iColumn;
	pit_col = nDEMPointer % _iColumn;
	pit_rec = nDEMPointer;
	pit_elev = pfDEMGrid[nDEMPointer];
	in_row = nDEMPointer / _iColumn;
	in_col = nDEMPointer % _iColumn;
	in_rec = nDEMPointer;
	in_elev = pfDEMGrid[nDEMPointer];
	out_row = nDEMPointer / _iColumn;
	out_col = nDEMPointer % _iColumn;
	out_rec = nDEMPointer;
	out_elev = pfDEMGrid[nDEMPointer];
	pour_elev = 1000000;

	cent_y = nDEMPointer / _iColumn;
	cent_x = nDEMPointer % _iColumn;
	cent_z = pfDEMGrid[nDEMPointer];
	centrec = nDEMPointer;
	int numneigh = 0;
	edge_pit = false;
	drains_2_mv = false;
	mv_pour = 0;

	long totalNum = _iColumn * _iRow;

	for (nLoop = 0; nLoop < totalNum; nLoop++)
	{
		if (piDEMSort[nLoop] == nDEMPointer)
		{
			nDEMSortedPointer = nLoop;
			nLoop = totalNum;
		}
	}

	while (bValid == true)
	{
		if (this_shed == piShedWGrid[nDEMPointer])
		{
			cent_y = nDEMPointer / _iColumn;
			cent_x = nDEMPointer % _iColumn;
			cent_z = pfDEMGrid[nDEMPointer];
			GetNeig1(zrec, cent_y, cent_x, cent_z, numneigh);
			CheckNeig3(nDEMPointer, piDEMSort, pfDEMGrid, piShedWGrid, zrec);

			if (overspill == false)
			{
				pondcell_no = pondcell_no + 1;
				pondcell_elev[pondcell_no] = pfDEMGrid[nDEMPointer];
				nDEMSortedPointer++;
				nDEMPointer = piDEMSort[nDEMSortedPointer];
			}
			else
			{
				shed_area = pondcell_no;
				while (bExit == false)
				{
					if (this_shed == piShedWGrid[nDEMPointer])
					{
						shed_area++;
						nDEMSortedPointer++;

						if (nDEMSortedPointer >= totalNum)
						{
							bExit = true;
						}
						else
						{
							nDEMPointer = piDEMSort[nDEMSortedPointer];
						}
					}
					else
					{
						bExit = true;
					}
				}
			}

			if (nDEMSortedPointer >= totalNum)
			{
				bValid = false;
			}
		}
		else
		{
			bValid = false;
		}
	}
	if (shed_area != new_ups)
	{
		shed_area = new_ups;
	}

	//CalcStat8(pondcell_elev);
	pit_vol = 0.00;
	nLoop = 0;
	for (nLoop = 1; nLoop < pit_area + 1; nLoop++)
	{
		pit_vol = pit_vol + (pour_elev - pondcell_elev[nLoop]);
	}
	if (pit_area == 0)
	{
		alldone = true;
	}
	AddStat8(&*pFillFile, nFillFileSize, PitFile, iPitNo);
	New_mm2Flood(nDEMPointer, piDEMSort, pfDEMGrid, piShedWGrid, pfVol2FlGrid, pfMm2FlGrid);
}

/**
	\brief Method to compute the volume of the depression up to the outflow or pour point

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::CalcStat8(double* pondcell_elev)
{

	pit_vol = 0.00;
	int nLoop = 0;
	for (nLoop = 1; nLoop < pit_area + 1; nLoop++)
	{
		pit_vol = pit_vol + (pour_elev - pondcell_elev[nLoop]);
	}
	if (pit_area == 0)
	{
		alldone = true;
	}
}

/**
	\brief Method to write pond statistics for the current watershed out to the selected ponddata file
@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::AddStat8(LSMSTATVAR** pTempFillFile, int& nFillFileSize, LSMSTATVAR *PitFile, int &iPitNo)
{
	int nLoop = 0;
	int nLoop2 = 0;
	LSMSTATVAR* pFillFile = *pTempFillFile;
	bool bFound[] = { false,false };
	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (PitFile[nLoop].iThisShed == pit1_no)//check if equal
		{
			if (nFillFileSize % 1000 == 0 && nFillFileSize != 0)//check if we need a bigger array
			{
				LSMSTATVAR* pTemp = new LSMSTATVAR[nFillFileSize + 1000];//copy old pit stats into new pits stats
																	//  and make an old pointer point to the new array
				for (int nLoop3 = 0; nLoop3 < nFillFileSize; nLoop3++)
				{
					pTemp[nLoop3] = pFillFile[nLoop3];
				}
				delete pFillFile;
				pFillFile = pTemp;
				*pTempFillFile = pTemp;
			}

			pFillFile[nFillFileSize] = PitFile[nLoop];//copy data
			pFillFile[nFillFileSize].iBecomes = max_shed;
			pFillFile[nFillFileSize].bRemoved = true;
			nFillFileSize++;
			for (nLoop2 = nLoop; nLoop2 < iPitNo - 1; nLoop2++)//remove current pond data
				//by moving all records below current one up
			{
				PitFile[nLoop2] = PitFile[nLoop2 + 1];
			}
			iPitNo--;
			bFound[0] = true;
			if (pit1_no == pit2_no)//if first and second record we are looking for are the same
				//we want to exit after current if statment
			{
				bFound[1] = true;
			}
		}
		if (PitFile[nLoop].iThisShed == pit2_no && bFound[1] == false)
		{
			if (nFillFileSize % 1000 == 0 && nFillFileSize != 0)
			{
				LSMSTATVAR* pTemp = new LSMSTATVAR[nFillFileSize + 1000];
				for (int nLoop3 = 0; nLoop3 < nFillFileSize; nLoop3++)
				{
					pTemp[nLoop3] = pFillFile[nLoop3];
				}
				delete pFillFile;
				pFillFile = pTemp;
				*pTempFillFile = pTemp;
			}
			pFillFile[nFillFileSize] = PitFile[nLoop];
			pFillFile[nFillFileSize].iBecomes = max_shed;
			pFillFile[nFillFileSize].bRemoved = true;
			nFillFileSize++;
			for (nLoop2 = nLoop; nLoop2 < iPitNo - 1; nLoop2++)
			{
				PitFile[nLoop2] = PitFile[nLoop2 + 1];
			}
			iPitNo--;
			bFound[1] = true;
			nLoop--;//if found second record,  we move the rest of the array up by one. 
			//now we have to check the same array index if it is corresponds to the first 
			//variable we are searching for.
		}
		if (bFound[0] && bFound[1])//if both where found, exit the search loop
		{
			nLoop = iPitNo;
		}
	}
	PitFile[iPitNo].iThisShed = max_shed;
	PitFile[iPitNo].iShedArea = shed_area;
	PitFile[iPitNo].iPitRow = pit_row + 1;
	PitFile[iPitNo].iPitCol = pit_col + 1;
	PitFile[iPitNo].iPitRec = pit_rec + 1;
	PitFile[iPitNo].fPitElev = pit_elev;
	PitFile[iPitNo].fPitVol = pit_vol;
	PitFile[iPitNo].iPitArea = pit_area;
	PitFile[iPitNo].iDrainsTo = out_shed;
	PitFile[iPitNo].iNextPit = out_shed;
	PitFile[iPitNo].iInRow = in_row + 1;
	PitFile[iPitNo].iInColumn = in_col + 1;
	PitFile[iPitNo].iInRec = in_rec + 1;
	PitFile[iPitNo].fInElev = in_elev;
	PitFile[iPitNo].iOutRow = out_row + 1;
	PitFile[iPitNo].iOutColumn = out_col + 1;
	PitFile[iPitNo].iOutRec = out_rec + 1;
	PitFile[iPitNo].fOutElev = out_elev;
	PitFile[iPitNo].fPourElev = pour_elev;
	PitFile[iPitNo].bEdge = edge_pit;
	PitFile[iPitNo].iEndPit = pit1_end;
	PitFile[iPitNo].fPreVol = pre_vol;
	PitFile[iPitNo].iStage = 0;
	PitFile[iPitNo].bVisited = false;
	PitFile[iPitNo].bDR_2_MV = drains_2_mv;
	iPitNo++;

	if (shed_area > 0)
	{
		m_varatio = pit_vol / shed_area * 1000;
	}
	else
	{
		m_varatio = 0;
	}
	if (m_varatio > 999999.99)
	{
		m_varatio = 999999.99;
	}
	if (m_varatio < max_va)
	{
		PitFile[iPitNo - 1].fVaratio = (double)max_va + 0.001;
	}
	else
	{
		PitFile[iPitNo - 1].fVaratio = m_varatio;
	}
	shed_mm2fl = m_varatio;
	new_shed_area = shed_area;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (PitFile[nLoop].iNextPit == pit1_no ||
			PitFile[nLoop].iNextPit == pit2_no)
		{
			PitFile[nLoop].iNextPit = max_shed;
		}
	}

	*pTempFillFile = pFillFile;
}

/**
\brief Method to recompute the variable mm-2-flood (mm2fl) for any cell in a new combined watershed that has a previously computed value for mm2fl that is greater then the current value of mm-2-flood at which the current pit is being removed.

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::New_mm2Flood(int& nDEMPointer, int *piDEMSort, double *pfDEMGrid, int *piShedWGrid,
	double* pfVol2FlGrid, double* pfMm2FlGrid)
{
	bool bExit = false;

	int curr_area = shed_area;
	int nLoop = 0;
	int nSortedDEMPointer;

	long totalNum = _iColumn * _iRow;

	nDEMPointer = new_pitrec;
	for (nLoop = 0; nLoop < totalNum; nLoop++)
	{
		if (piDEMSort[nLoop] == nDEMPointer)
		{
			nSortedDEMPointer = nLoop;
			nLoop = _iColumn * _iRow;
		}
	}
	this_shed = piShedWGrid[nDEMPointer];
	while (bExit == false && nDEMPointer >= 0 && nDEMPointer < totalNum)
	{
		if (this_shed == piShedWGrid[nDEMPointer])
		{
			if (pfVol2FlGrid[nDEMPointer] > 0)
			{
				if (pfMm2FlGrid[nDEMPointer] < curr_mm2flood)
				{
					pfMm2FlGrid[nDEMPointer] = pfVol2FlGrid[nDEMPointer] / curr_area;
				}
			}
			nSortedDEMPointer++;
			nDEMPointer = piDEMSort[nSortedDEMPointer];
			if (nSortedDEMPointer >= totalNum - 1)
			{
				bExit = true;
			}
		}
		else
		{
			bExit = true;
		}
	}
}

/**
\brief Method to write pond statistics for the current watershed out to the selected ponddata file

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::Final_Pits(LSMSTATVAR* PitFile, int& iPitNo, LSMSTATVAR** pTempFillFile, int& iFillFileSize,
	LSMPONDSTATS * pPondStats)
{

	int nLoop = 0;
	LSMSTATVAR* pFillFile = *pTempFillFile;
	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		PitFile[nLoop].bFinal = true;
		PitFile[nLoop].bRemoved = false;
		PitFile[nLoop].iBecomes = PitFile[nLoop].iThisShed;
	}
	for (nLoop = 0; nLoop < iFillFileSize; nLoop++)
	{
		pFillFile[nLoop].bFinal = false;
	}

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (iFillFileSize % 1000 == 0 && iFillFileSize != 0)
		{
			LSMSTATVAR* pTemp = new LSMSTATVAR[iFillFileSize + 1000];
			for (int nLoop2 = 0; nLoop2 < iFillFileSize; nLoop2++)
			{
				pTemp[nLoop2] = pFillFile[nLoop2];
			}
			delete pFillFile;
			pFillFile = pTemp;
			*pTempFillFile = pFillFile;
		}
		pFillFile[iFillFileSize] = PitFile[nLoop];
		iFillFileSize++;
	}
	*pTempFillFile = pFillFile;
}

/**
	\brief Method to remove pits iteratively and selectively and to re-compute pond & pit statistics for those pits that are removed or amalgamated

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::Lsm_PitR(double *pfDEMGrid, int *piDEMSort, LSMSTATVAR *pPit, int* piShedWGrid,
	int* piUpSlpGrid, int &iPitNo, int *piFlowGrid, int *piDrecGrid,
	int max_area, double max_depth)
{
	int* pTempDEMSort = piDEMSort;
	long i = 0;
	double* pfShed = 0;

	int* piShedOrd = 0;
	int nSortedPitPointer = 0;
	long j = 0;
	maxrows = 0;
	maxcols = 0;
	overspill = false;
	pondcell_no = 0;
	this_shed = 0;
	shed_area = 0;
	pit_row = 0;
	pit_col = 0;
	pit_rec = 0;
	pit_elev = 0;
	pit_vol = 0;
	pit_area = 0;
	in_row = 0;
	in_col = 0;
	in_rec = 0;
	in_elev = 0;
	out_row = 0;
	out_col = 0;
	out_rec = 0;
	out_elev = 0;
	out_shed = 0;
	pour_elev = 1000000;
	neigh_col = 0;
	centre_col = 0;
	neigh_row = 0;
	centre_row = 0;
	new_shed = 0;
	old_shed = 0;
	bool ok_2_remove = false;
	drains_2_mv = false;
	mv_pour = 0;

	new_pitrec = 0;
	int new_ddir = 0;

	cent_y = 0;
	cent_x = 0;
	cent_z = 0;
	centrec = 0;
	m_varatio = 0;

	cell_area = _fGridSize * _fGridSize;

	max_ponds = iPitNo;
	maxrows = _iRow;
	maxcols = _iColumn;

	int nDEMPointer = 0;
	int nPitPointer = 0;
	int nSortedDEMPointer = 0;

	long totalNum = _iRow * _iColumn;

	int pits_removed = 0;
	bool bEOF = false;

	if ((pfShed = new double[iPitNo]) == NULL)
	{
		printf("Unable to create pfShed array in Lsm_PitR(), exiting ...");
		exit(1);
	}
	if ((piShedOrd = new int[iPitNo]) == NULL)
	{
		printf("Unable to create piShedOrd array in Lsm_PitR(), exiting ...");
		exit(1);
	}

	for (i = 0; i < iPitNo; i++)
	{
		pfShed[i] = (double)(((20000 - (double)pPit[i].fPitElev) * pow(10.0, 10.0)) +
			(int)pPit[i].fVaratio);
		piShedOrd[i] = i;
	}

	QuickSort(pfShed, iPitNo, piShedOrd);

	int iNumPits = iPitNo;
	int* nTemp = 0, *piHeapTbl, *piTempSortedDEM;
	double* pfBottomUp;

	if ((piHeapTbl = new int[totalNum]) == NULL)
	{
		printf("Unable to create piHeapTbl array in Lsm_PitR(), exiting ...");
		exit(1);
	}
	if ((piTempSortedDEM = new int[totalNum]) == NULL)
	{
		printf("Unable to create piTempSortedDEM array in Lsm_PitR(), exiting ...");
		exit(1);
	}

	int* pTempPoint = piTempSortedDEM;

	for (i = 0; i < totalNum; i++)
	{
		piTempSortedDEM[i] = pTempDEMSort[i];
	}

	if ((pfBottomUp = new double[totalNum]) == NULL)
	{
		printf("Unable to create piTempSortedDEM array in Lsm_PitR(), exiting ...");
		exit(1);
	}

	int nC = 1;
	int nLoc = 0;

	while (bEOF == false)
	{
		nPitPointer = piShedOrd[nSortedPitPointer];
		ok_2_remove = CheckIfOk1(pPit, nPitPointer, max_area, max_depth, iPitNo);
		if (ok_2_remove == true)
		{
			pits_removed = pits_removed + 1;
			RemovePit1(pfDEMGrid, piFlowGrid, piDrecGrid, piShedWGrid, pfDEMGrid, piUpSlpGrid, nDEMPointer);

			SortBottomUp(piShedWGrid, pfDEMGrid, piUpSlpGrid, piDEMSort, old_shed, new_shed,
				piHeapTbl, piTempSortedDEM, pfBottomUp);
			nTemp = piTempSortedDEM;
			piTempSortedDEM = piDEMSort;
			piDEMSort = nTemp;
			//***************************************************
			ShedStat21(pfDEMGrid, piShedWGrid, piUpSlpGrid, nDEMPointer, piDEMSort, pPit, iPitNo, pfBottomUp);
			//***************************************************
			for (i = 0; i < iPitNo; i++)
			{
				pfShed[i] = (double)(((20000 - (double)pPit[i].fPitElev) * pow(10.0, 10.0)) +
					(int)pPit[i].fVaratio);
				piShedOrd[i] = i;
			}
			QuickSort(pfShed, iPitNo, piShedOrd);
			//****************************************************
			nSortedPitPointer = 0;
		}
		else
		{
			nSortedPitPointer++;
		}

		if (nSortedPitPointer >= iPitNo || iPitNo == 1)
		{
			bEOF = true;
		}
	}
	delete[] piShedOrd;
	delete[] pfShed;
	delete[] pfBottomUp;

	for (int nLoop = 0; nLoop < totalNum; nLoop++)
	{
		pTempDEMSort[nLoop] = piTempSortedDEM[nLoop];
	}
	delete pTempPoint;
	delete[] piHeapTbl;
}


/**
	\brief Function to replace checkifok, for a better function to debug the original remove   pit function

\detailed Method to check if the current record in a ponddata file is for a watershed that is OK to remove (i.e. the watershed's depression is less than the specified threshold values for pit_area, pit_volume, pit_depth and mm_2_flood as estimated by varatio.

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
	*/
bool DEMProcess::CheckIfOk1(LSMSTATVAR *pPit, int& nPitPointer,
	int max_area, double max_depth, int iPitNo)
{
	bool bFound = false;
	int nLoop = 0;
	bool bOk2Remove = false;


	if (((pPit[nPitPointer].iPitArea <= max_area || (pPit[nPitPointer].fPourElev -
		pPit[nPitPointer].fPitElev) <= max_depth) && (pPit[nPitPointer].bDR_2_MV == false &&
			pPit[nPitPointer].bEdge == false)) && (pPit[nPitPointer].iThisShed !=
				pPit[nPitPointer].iDrainsTo))
	{
		bOk2Remove = true;

		pit1_no = pPit[nPitPointer].iThisShed;
		pit1_z = pPit[nPitPointer].fPitElev;
		pit1_to = pPit[nPitPointer].iNextPit;
		pit1_rec = nPitPointer;
		pit1_vol = pPit[nPitPointer].fPitVol;
		//pit1_pour = pPit[nPitPointer].fPourElev;
		pit1_edge = pPit[nPitPointer].bEdge;
		m_varatio = pPit[nPitPointer].fVaratio;

		for (nLoop = 0; nLoop < iPitNo; nLoop++)
		{
			if (pPit[nLoop].iThisShed == pit1_to)
			{
				nPitPointer = nLoop;
				nLoop = iPitNo;
				bFound = true;
			}
		}
		if (bFound == true)
		{
			pit2_no = pPit[nPitPointer].iThisShed;
			pit2_z = pPit[nPitPointer].fPitElev;
			pit2_rec = nPitPointer;
			pit2_vol = pPit[nPitPointer].fPitVol;
			//pit2_pour = pPit[nPitPointer].fPourElev;
			pit2_edge = pPit[nPitPointer].bEdge;
		}
		else
		{
			pit2_no = 0;
			pit2_z = 0;
			pit2_rec = 0;
			pit2_vol = 0;
			pit2_pour = 0;
			pit2_edge = 0;
		}

		/*
		if(pit1_pour<pit2_pour)
		{
			pour_elev = pit2_pour;
		}
		else
		{
			pour_elev = pit1_pour;
		}
		*/
		nPitPointer = pit1_rec;
		if ((pit1_z >= pit2_z))
		{
			pit_rec = pPit[nPitPointer].iPitRec - 1;
			in_rec = pPit[nPitPointer].iInRec - 1;
			out_rec = pPit[nPitPointer].iOutRec - 1;
			neigh_row = pPit[nPitPointer].iOutRow;
			neigh_col = pPit[nPitPointer].iOutColumn;
			centre_row = pPit[nPitPointer].iInRow;
			centre_col = pPit[nPitPointer].iInColumn;
			pour_elev = pPit[nPitPointer].fPourElev;
		}
		else
		{
			in_rec = pPit[nPitPointer].iOutRec - 1;
			out_rec = pPit[nPitPointer].iInRec - 1;
			neigh_row = pPit[nPitPointer].iInRow;
			neigh_col = pPit[nPitPointer].iInColumn;
			centre_row = pPit[nPitPointer].iOutRow;
			centre_col = pPit[nPitPointer].iOutColumn;
			nPitPointer = pit2_rec;
			pit_rec = pPit[nPitPointer].iPitRec - 1;
			pour_elev = pPit[nPitPointer].fPourElev;
		}
	}
	return bOk2Remove;
}

/**
	\brief Method to remove the currently selected pit by locating the pour point and establishing a new drainage direction and pour rec pointing from the in_rec INTO the out_rec at the pour point location.

	\detailed  The program then does the following things:
	  1)  it follows the flow path from the inside pour point DOWN to the old pit centre along the original (old) local drainage directions
	  2)  as it flows DOWN it reverses the original local flow direction (DDIR) to point back upslope into the upslope cell on the flow path that leads to the pour point
	  3)  once it reaches the old pit centre (DDIR=5) it records the original value for upslope area recorded for that intitial pit centre
	  4)  it then follows the revised flow path back to the pour point and adds the value of the old pit centre upslope area to the value stored for upslope area in each cell
  5)  once it reaches the out_cell at the pour point it simply adds the new value for upslope area to each cell along the flow path from the outflow point to the new depression centre
	 6)  after this has been completed it then renumbers all cells that were part of the removed watershed with the number of the new, combined, watershed

@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::RemovePit1(double* pElev, int *pFlowGrid, int *pDrecGrid, int *pShedWGrid, double *pDEMGrid, int *pUpSlpGrid,
	int& nDEMPointer)
{
	int nLoop = 0;
	int next_down = 0;
	int new_ddir = 0;
	old_shed = 0;
	int path_len = 0;
	int old_ups = 0;
	new_ups = 0;
	int last_ups = 0;
	int ups_diff = 0;
	int up_rec = 0;

	long totalNums = _iRow * _iColumn;

	nDEMPointer = in_rec;
	old_shed = pShedWGrid[nDEMPointer];

	new_ddir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);
	next_down = pDrecGrid[nDEMPointer] - 1;
	pDrecGrid[nDEMPointer] = out_rec + 1;
	pFlowGrid[nDEMPointer] = new_ddir;
	last_ups = pUpSlpGrid[nDEMPointer];
	while (nDEMPointer != pit_rec)
	{
		neigh_col = nDEMPointer % _iColumn;
		neigh_row = nDEMPointer / _iColumn;
		up_rec = nDEMPointer;
		//if(nDEMPointer!=pit_rec)
		//{
		//	last_ups = pUpSlpGrid[nDEMPointer];
		//}
		nDEMPointer = next_down;
		path_len++;
		//if(nDEMPointer!=pit_rec)
		//{
		ups_diff = pUpSlpGrid[nDEMPointer] - last_ups;
		last_ups = pUpSlpGrid[nDEMPointer];
		pUpSlpGrid[nDEMPointer] = ups_diff;
		//}
		centre_row = nDEMPointer / _iColumn;
		centre_col = nDEMPointer % _iColumn;
		next_down = pDrecGrid[nDEMPointer] - 1;
		new_ddir = GetNewDir(neigh_row, neigh_col, centre_row, centre_col);
		pDrecGrid[nDEMPointer] = up_rec + 1;
		pFlowGrid[nDEMPointer] = new_ddir;
	}

	last_ups = pUpSlpGrid[nDEMPointer];
	//old_ups = pUpSlpGrid[nDEMPointer];
	//new_ups = pUpSlpGrid[nDEMPointer]-last_ups;

	//pUpSlpGrid[nDEMPointer] = new_ups;

	for (nLoop = 0; nLoop < path_len; nLoop++)
	{
		nDEMPointer = pDrecGrid[nDEMPointer] - 1;
		new_ups = pUpSlpGrid[nDEMPointer] + last_ups;

		pUpSlpGrid[nDEMPointer] = new_ups;
		last_ups = new_ups;
	}

	while (pFlowGrid[nDEMPointer] != 5)
	{
		nDEMPointer = pDrecGrid[nDEMPointer] - 1;
		pUpSlpGrid[nDEMPointer] = pUpSlpGrid[nDEMPointer] + last_ups;
	}

	new_shed = pShedWGrid[nDEMPointer];
	new_pitrec = nDEMPointer;

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if (pShedWGrid[nLoop] == old_shed && pElev[nLoop] < pour_elev)
		{
			pElev[nLoop] = pour_elev;
		}
	}

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if (pShedWGrid[nLoop] == old_shed)
		{
			pShedWGrid[nLoop] = new_shed;
		}
	}
}

/**
	\brief Method to determine all relevant watershed statistics for any watershed that originates at a pit centre cell coded as DDIR=5
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
	*/
void DEMProcess::ShedStat21(double *pfDEMGrid, int *piShedWGrid, int *piUpSlpGrid, int &nDEMPointer,
	int *piDEMSort, LSMSTATVAR* pPit, int& iPitNo, double* pondcell_elev)
{
	bool bLoop = true;
	//double* pondcell_elev;

	//pondcell_elev = new double [_iRow*_iColumn];

	int zrec[] = { 0,0,0,0,0,0,0,0 };
	int nNumNeigh = 0;
	int nLoop = 0;
	int nShedValue = 0;
	int nSortedDEMPointer = 0;

	long totalNums = _iRow * _iColumn;

	nDEMPointer = new_pitrec;
	overspill = false;
	pondcell_no = 0;
	this_shed = piShedWGrid[nDEMPointer];
	shed_area = 0;
	pit_row = nDEMPointer / _iColumn;
	pit_col = nDEMPointer % _iColumn;
	pit_rec = nDEMPointer;
	pit_elev = pfDEMGrid[nDEMPointer];
	pit_vol = 0;

	pit_area = 0;
	in_row = pit_row;
	in_col = pit_col;
	in_rec = pit_rec;
	in_elev = pit_elev;

	out_row = pit_row;
	out_col = pit_col;
	out_rec = pit_rec;
	out_elev = pit_elev;
	pour_elev = 100000000;
	new_ups = piUpSlpGrid[nDEMPointer];
	edge_pit = false;
	drains_2_mv = false;

	cent_y = pit_row;
	cent_x = pit_col;
	cent_z = pit_elev;
	centrec = nDEMPointer;

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if (piDEMSort[nLoop] == nDEMPointer)
		{
			nSortedDEMPointer = nLoop;
			//nLoop = _iRow*_iColumn;
			nLoop = totalNums;
		}
	}

	nShedValue = piShedWGrid[nDEMPointer];
	while (this_shed == nShedValue)
	{
		cent_y = nDEMPointer / _iColumn;
		cent_x = nDEMPointer % _iColumn;
		cent_z = pfDEMGrid[nDEMPointer];
		GetNeig1(zrec, cent_y, cent_x, cent_z, nNumNeigh);
		CheckNeig3(nDEMPointer, piDEMSort, pfDEMGrid, piShedWGrid, zrec);
		if (overspill == false)
		{
			pondcell_no = pondcell_no + 1;
			pondcell_elev[pondcell_no - 1] = pfDEMGrid[nDEMPointer];
			nSortedDEMPointer++;

			if (nSortedDEMPointer >= totalNums)
			{
				nShedValue = -1;//if end of file riched, we want to exit
			}
			else
			{
				nDEMPointer = piDEMSort[nSortedDEMPointer];
				nShedValue = piShedWGrid[nDEMPointer];
			}
		}
		else
		{
			shed_area = pondcell_no;

			nShedValue = piShedWGrid[nDEMPointer];
			while (this_shed == nShedValue)
			{
				shed_area = shed_area + 1;
				nSortedDEMPointer++;

				if (nSortedDEMPointer >= totalNums)
				{
					nShedValue = -1;//if the end of file riched, we exit
				}
				else
				{
					nDEMPointer = piDEMSort[nSortedDEMPointer];
					nShedValue = piShedWGrid[nDEMPointer];
				}
			}
		}
	}
	if (shed_area != new_ups)
	{
		shed_area = new_ups;
	}
	int pitcell_no = 0;
	pit_vol = 0.00;
	for (pitcell_no = 0; pitcell_no < pit_area; pitcell_no++)
	{
		pit_vol = pit_vol + (pour_elev - pondcell_elev[pitcell_no]);
	}

	if (pit_area == 0)
	{
		alldone = true;
	}
	//CalcStat1(pondcell_elev);
	//AddStat2(pPit,iPitNo);

		/*
	* Method to write pond statistics for the current watershed out to
	* the selected ponddata file
	*/
	//int nLoop = 0;
	bool bFound[] = { false,false };
	int nIndex = 0;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (pPit[nLoop].iThisShed == old_shed)
		{
			nIndex = nLoop;

			for (nIndex; nIndex < iPitNo - 1; nIndex++)
			{
				pPit[nIndex] = pPit[nIndex + 1];
			}
			iPitNo--;
			bFound[0] = true;
			if (old_shed == new_shed)
			{
				bFound[1] = true;
			}
			if (bFound[1] == true)
			{
				nLoop = iPitNo - 1;
			}
		}

		if (pPit[nLoop].iThisShed == new_shed && bFound[1] == false)
		{
			if (iPitNo > 0)
			{
				nIndex = nLoop;

				for (nIndex; nIndex < iPitNo - 1; nIndex++)
				{
					pPit[nIndex] = pPit[nIndex + 1];
				}
				iPitNo--;
				bFound[1] = true;
				if (bFound[0] == false)
				{
					nLoop--; //because we move all of the pits up by one,
						 //if the two records are side by side we will miss one lets say we want to remove 3
						// if we don't do this. ie 4 becomes 3 but then we will move to 4 which was 5
						// so we never checked for record 4.
				}
				else
				{
					nLoop = iPitNo - 1;
				}
			}
		}
	}
	pPit[iPitNo].iThisShed = this_shed;
	pPit[iPitNo].iShedArea = shed_area;
	pPit[iPitNo].iPitRow = pit_row + 1;
	pPit[iPitNo].iPitCol = pit_col + 1;
	pPit[iPitNo].iPitRec = pit_rec + 1;
	pPit[iPitNo].fPitElev = pit_elev;
	pPit[iPitNo].fPreVol = 0;
	pPit[iPitNo].fPitVol = pit_vol;
	pPit[iPitNo].iPitArea = pit_area;
	pPit[iPitNo].iDrainsTo = out_shed;
	pPit[iPitNo].iInRow = in_row + 1;
	pPit[iPitNo].iInColumn = in_col + 1;
	pPit[iPitNo].iInRec = in_rec + 1;
	pPit[iPitNo].fInElev = in_elev;
	pPit[iPitNo].iOutRow = out_row + 1;
	pPit[iPitNo].iOutColumn = out_col + 1;
	pPit[iPitNo].iOutRec = out_rec + 1;
	pPit[iPitNo].fOutElev = out_elev;
	pPit[iPitNo].fPourElev = pour_elev;
	pPit[iPitNo].bEdge = edge_pit;
	pPit[iPitNo].bVisited = false;

	pPit[iPitNo].bDR_2_MV = drains_2_mv;

	if (shed_area > 0)
	{
		m_varatio = (pit_vol / shed_area) * 1000;
	}
	else
	{
		m_varatio = 0.0;
	}
	if (m_varatio > 999999.99)
	{
		m_varatio = (double)999999.99;
	}

	pPit[iPitNo].fVaratio = m_varatio;
	iPitNo++;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (pPit[nLoop].iDrainsTo == old_shed)
		{
			pPit[nLoop].iDrainsTo = new_shed;
		}
	}
	//delete pondcell_elev;
}

/**
	\brief Method to write pond statistics for the current watershed out to the selected ponddata file
@author Authors of Original LandMapR, modified by Gashaw Ayalew
@modified 2016-08-01
*/
void DEMProcess::AddStat2(LSMSTATVAR *pPit, int& iPitNo)
{

	int nLoop = 0;
	bool bFound[] = { false,false };
	int nIndex = 0;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (pPit[nLoop].iThisShed == old_shed)
		{
			nIndex = nLoop;

			for (nIndex; nIndex < iPitNo - 1; nIndex++)
			{
				pPit[nIndex] = pPit[nIndex + 1];
			}
			iPitNo--;
			bFound[0] = true;
			if (old_shed == new_shed)
			{
				bFound[1] = true;
			}
			if (bFound[1] == true)
			{
				nLoop = iPitNo - 1;
			}
		}

		if (pPit[nLoop].iThisShed == new_shed && bFound[1] == false)
		{
			if (iPitNo > 0)
			{
				nIndex = nLoop;

				for (nIndex; nIndex < iPitNo - 1; nIndex++)
				{
					pPit[nIndex] = pPit[nIndex + 1];
				}
				iPitNo--;
				bFound[1] = true;
				if (bFound[0] == false)
				{
					nLoop--; //because we move all of the pits up by one,
						 //if the two records are side by side we will miss one lets say we want to remove 3
						// if we don't do this. ie 4 becomes 3 but then we will move to 4 which was 5
						// so we never checked for record 4.
				}
				else
				{
					nLoop = iPitNo - 1;
				}
			}
		}
	}

	pPit[iPitNo].iThisShed = this_shed;
	pPit[iPitNo].iShedArea = shed_area;
	pPit[iPitNo].iPitRow = pit_row + 1;
	pPit[iPitNo].iPitCol = pit_col + 1;
	pPit[iPitNo].iPitRec = pit_rec + 1;
	pPit[iPitNo].fPitElev = pit_elev;
	pPit[iPitNo].fPreVol = 0;
	pPit[iPitNo].fPitVol = pit_vol;
	pPit[iPitNo].iPitArea = pit_area;
	pPit[iPitNo].iDrainsTo = out_shed;
	pPit[iPitNo].iInRow = in_row + 1;
	pPit[iPitNo].iInColumn = in_col + 1;
	pPit[iPitNo].iInRec = in_rec + 1;
	pPit[iPitNo].fInElev = in_elev;
	pPit[iPitNo].iOutRow = out_row + 1;
	pPit[iPitNo].iOutColumn = out_col + 1;
	pPit[iPitNo].iOutRec = out_rec + 1;
	pPit[iPitNo].fOutElev = out_elev;
	pPit[iPitNo].fPourElev = pour_elev;
	pPit[iPitNo].bEdge = edge_pit;
	pPit[iPitNo].bVisited = false;
	pPit[iPitNo].bDR_2_MV = drains_2_mv;

	if (shed_area > 0)
	{
		m_varatio = (pit_vol / shed_area) * 1000;
	}
	else
	{
		m_varatio = 0.0;
	}
	if (m_varatio > 999999.99)
	{
		m_varatio = (double)999999.99;
	}
	pPit[iPitNo].fVaratio = m_varatio;
	iPitNo++;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		if (pPit[nLoop].iDrainsTo == old_shed)
		{
			pPit[nLoop].iDrainsTo = new_shed;
		}
	}
}

/**
\brief Use this function when one pit removed into another, i.e. old_pit and new_pit used
*
* @author Authors of Original LandMapR, modified by Gashaw Ayalew
* @modified 2016-08-01
*/
void DEMProcess::SortBottomUp(int *piShedWGrid, double *pfDEMGrid, int *piUpSlpGrid, int* piSortedDEM, int nOldShed,
	int nNewShed, int* piHeapTbl, int* piTempSortedDEM, double* pfBottomUp)
{
	//int* piHeapTbl = new int [_iRow*_iColumn];
	//int* piTempSortedDEM = new int[_iRow*_iColumn];
	int nInsertLocation = 0;
	//double* pfBottomUp = new double [_iRow*_iColumn];
	int nPartialArrayIndex = 0;
	int nLoop = 0;
	bool bFound[2];

	long totalNums = _iRow * _iColumn;

	bFound[1] = false;
	bFound[0] = false;

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if ((piShedWGrid[nLoop] == nOldShed) ||
			(piShedWGrid[nLoop] == nNewShed))
		{
			if (piShedWGrid[nLoop] == nOldShed)
			{
				piShedWGrid[nLoop] = nNewShed;
			}
			pfBottomUp[nPartialArrayIndex] = (double)(((double)piShedWGrid[nLoop] * pow(10.0, 18.0)) +
				((double)pfDEMGrid[nLoop] * pow(10.0, 12.0)) +
				(1000000.0 - (double)piUpSlpGrid[nLoop]));
			piHeapTbl[nPartialArrayIndex] = nLoop;
			nPartialArrayIndex++;

		}
	}

	QuickSort(pfBottomUp, nPartialArrayIndex, piHeapTbl);

	bool bDoneIns = false;

	bool bFirst = false;
	int nPrevShed = -1;
	if (piShedWGrid[piSortedDEM[0]] == nNewShed)//check if it is the first watershed
	{
		bFirst = true;
		bDoneIns = true;
		for (nLoop = 0; nLoop < nPartialArrayIndex; nLoop++)
		{
			piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop];
			nInsertLocation++;
		}
	}

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if ((nPrevShed > nNewShed) && (!bDoneIns))
		{
			nLoop--;
			for (int nLoop2 = 0; nLoop2 < nPartialArrayIndex; nLoop2++)
			{
				piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop2];
				nInsertLocation++;
			}
			bDoneIns = true;
		}
		else
		{
			if (piShedWGrid[piSortedDEM[nLoop]] != nNewShed)
			{
				piTempSortedDEM[nInsertLocation] = piSortedDEM[nLoop];
				nInsertLocation++;
				if (!bDoneIns)
				{
					nPrevShed = piShedWGrid[piSortedDEM[nLoop]];
				}
			}
		}
	}

	if (!bDoneIns)//that means that new watershed is the last one
		//go back and insert it
	{
		for (nLoop = 0; nLoop < nPartialArrayIndex; nLoop++)
		{
			piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop];
			nInsertLocation++;
		}
	}
}

/** Method used when pit1_no, pit2_no and max_shed are also used. */
void DEMProcess::SortBottomUp2(int *piShedWGrid, double *pfDEMGrid, int *piUpSlpGrid, int** piSortedDEM1, int nOldShed1, int nOldShed2,
	int nNewShed, int*  piHeapTbl, int** piTempSortedDEM1, double* pfBottomUp)
{

	int* piSortedDEM = *piSortedDEM1;
	int* piTempSortedDEM = *piTempSortedDEM1;

	int nInsertLocation = 0;

	int nPartialArrayIndex = 0;
	int nLoop = 0;
	long totalNums = _iColumn * _iRow;

	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if ((piShedWGrid[nLoop] == nOldShed1) || (piShedWGrid[nLoop] == nOldShed2))
		{
			piShedWGrid[nLoop] = nNewShed;

			pfBottomUp[nPartialArrayIndex] = (double)(((double)piShedWGrid[nLoop] * pow(10.0, 18.0)) +
				((double)pfDEMGrid[nLoop] * pow(10.0, 12.0)) +
				(1000000.0 - (double)piUpSlpGrid[nLoop]));
			piHeapTbl[nPartialArrayIndex] = nLoop;
			nPartialArrayIndex++;
		}
	}
	QuickSort(pfBottomUp, nPartialArrayIndex, piHeapTbl);

	bool bDoneIns = false;
	bool bFirst = false;
	int nPrevShed = -1;
	if (piShedWGrid[piSortedDEM[0]] == nNewShed)//check if it is the first watershed
	{
		bFirst = true;
		bDoneIns = true;
		for (nLoop = 0; nLoop < nPartialArrayIndex; nLoop++)
		{
			piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop];
			nInsertLocation++;
		}
	}
	for (nLoop = 0; nLoop < totalNums; nLoop++)
	{
		if ((nPrevShed > nNewShed) && (!bDoneIns))
		{
			nLoop--;
			for (int nLoop2 = 0; nLoop2 < nPartialArrayIndex; nLoop2++)
			{
				piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop2];
				nInsertLocation++;
			}
			bDoneIns = true;
		}
		else
		{
			if (piShedWGrid[piSortedDEM[nLoop]] != nNewShed)
			{
				piTempSortedDEM[nInsertLocation] = piSortedDEM[nLoop];
				nInsertLocation++;
				if (!bDoneIns)
				{
					nPrevShed = piShedWGrid[piSortedDEM[nLoop]];
				}
			}
		}
	}

	if (!bDoneIns)//that means that new watershed is the last one
		//go back and insert it
	{
		for (nLoop = 0; nLoop < nPartialArrayIndex; nLoop++)
		{
			piTempSortedDEM[nInsertLocation] = piHeapTbl[nLoop];
			nInsertLocation++;
		}
	}

	*piTempSortedDEM1 = piSortedDEM;
	*piSortedDEM1 = piTempSortedDEM;
}
