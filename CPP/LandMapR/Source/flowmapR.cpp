/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../Headers/stdafx.h"
#include "../Headers/flowmapR.h"
//#include "dbffile.h"
#include "../Headers/demprocess.h"
#include "../Headers/ioconductor.h"
#include "assert.h"
#include <algorithm>

using namespace std;

FlowmapR::FlowmapR()
{

	//this->m_FileName = "";
	this->m_WorkingDirectory = "";
	this->m_LLX = 0.0;
	this->m_LLY = 0.0;
	this->m_MissDataValue = -9999;
	this->m_PitDepth = 0.0;
	this->m_PitArea = 0;
	//this->m_PitVolume = 0.0;
	this->m_Row = 1;
	this->m_Column = 1;
	this->m_InvertedElev = true;
	//this->m_iFileType = 0;
}
void FlowmapR::runProcess(string& resultJsonPath, string m_WorkingDirectory, double* pfDEMGrid, double xll, double yll, string m_NamePrefix, int m_Row, int m_Column, double m_GridSize, double m_MissDataValue, long m_PitArea, double m_PitDepth, bool pbInvertedElev, DEM* dem, Pond* pondobj, IDEM* idemobj)
{
	double * pElev;
	pElev = 0;

	//CDbfFile	*pDbfFile;
	//CDbfFile	*pOutDbfFile;
	DEMProcess	*pDEMProcess;
	double		*pfTmpDEMGrid;
	//double		*pfDEMGrid;
	double		*pfBottomUp;
	int			*piFlowGrid;
	int			*piShedGrid;
	int			*piShedWGrid;
	int			*piUpSlpGrid;
	int			*piDrecGrid;
	int			*piDEMSort;
	//string		sTmp;
	//string		sTmp1;
	int			i, ii;

	//3 Arrays used in calculation of volume to flood, mm to flood, and pArea
	double		*pfVol2FlGrid;
	double		*pfMm2FlGrid;
	int		*pfPArea;

	int totalpoints = m_Row * m_Column;

	/*if ((pfDEMGrid = new double[totalpoints]) == NULL)
	{
		printf("No elevation data to process, exiting ...\n");
		exit(1);
	}*/

	//sTmp = m_WorkingDirectory + m_NamePrefix + ".dbf";
	printf("Opening input file ...\n");

	//IOConductor io;
	//io.setDbfDemWriteFlag(stoi(params["DbfDemWrite"]) == 1 ? true : false);
	//io.setCsvDemWriteFlag(stoi(params["CsvDemWrite"]) == 1 ? true : false);
	//Get the DEM data
	//pfDEMGrid = io.extractDEM(m_WorkingDirectory, m_NamePrefix, m_Row, m_Column);



	if (pfDEMGrid == NULL)
	{
		printf("No elevation Data to Process..");
		exit(1);
	}

	pDEMProcess = new DEMProcess(m_Row, m_Column, m_GridSize);
	pDEMProcess->dMissingValue = m_MissDataValue;

	//-SNAP181010_091143
	//Taken out. We do not supply a prefix, nor do we need to check that there is one for our needs.
	//assert(m_NamePrefix != "");

	/*if (m_NamePrefix.find(".") > 0)
		m_NamePrefix.erase(m_NamePrefix.find("."), 4);*/

	if ((pfTmpDEMGrid = new double[totalpoints]) == NULL)
	{
		printf("Not enough memory for Tmp DEM grid!");
		exit(1);
	}
	if ((piDEMSort = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for sort grid!");
		exit(1);
	}

	//**********************************************************************
	//This part of the code creates an array of sorted DEM values
	//Positions with the missing value (typically -9999) are excluded
	//The result is a sorted array called piDEMSort
	int nMissingNum = 0, *pTempSort;
	int nIndex = 0;
	//int nIndex1 = 0;
	if (!(pTempSort = new int[totalpoints]))	//temp array to hold index of records with missing elevation field
	{
		printf("Not enough memory for pTempSort array, exiting ...");
		exit(1);
	}

	for (i = 0; i < totalpoints; i++)
	{
		if (!(pfDEMGrid[i] == m_MissDataValue))//if value is not missing
		{
			pfTmpDEMGrid[nIndex] = pfDEMGrid[i];
			piDEMSort[nIndex] = i;
			nIndex++;
		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values
		}
	}

	QuickSort(pfTmpDEMGrid, totalpoints - nMissingNum, piDEMSort); //sort only non missing values


	nIndex = 0;
	for (i = totalpoints - nMissingNum; i < totalpoints; i++) //make the rest of pointers which are
														  // pointing to the missing values, valid. 
	{
		piDEMSort[i] = pTempSort[nIndex];
		nIndex++;
	}
	delete[] pTempSort;
	delete[] pfTmpDEMGrid;

	//***********************************************************************************
	//Allocating various arrays to be used in later methods
	if ((pfVol2FlGrid = new double[totalpoints]) == NULL)
	{
		printf("Not enough memory for volume to flood grid!");
		exit(1);
	}

	if ((pfMm2FlGrid = new double[totalpoints]) == NULL)
	{
		printf("Not enough memory for mm to flood to flood grid!");
		exit(1);
	}

	if ((pfPArea = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for pArea grid!");
		exit(1);
	}
	//************************************************************************************

	if ((piFlowGrid = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for flow direction grid!");
		exit(1);
	}

	//************************************************************************************

	//************************************************************************************
	//FlowDir - Flow Direction
	//The basic idea is that a DEM represents an array of cells each of which holds a height
	//This routine goes through the DEM and creates an identically shaped array where each cell
	//holds coded information as to where water placed in each DEM cell would flow.
	//Calculating the flow direction (Uses the DEM and produces piFlowGrid)
	printf("Computing flow directions ...\n");
	pDEMProcess->FlowDir(pfDEMGrid, piFlowGrid);

	//************************************************************************************
	//FixFlat - Fixing Flat areas of the DEM which are difficult to assign flow directions to
	//This method uses a couple of tricks to fix things, but can result in an infinite loop.
	//Needs some reworking...
	printf("Running FixFlat() ...\n");
	pDEMProcess->FixFlat(pfDEMGrid, piFlowGrid);


	//************************************************************************************
	//Allocating space for the flow direction pointer grid/array
	if ((piDrecGrid = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for down slope record grid!");
		exit(1);
	}

	//************************************************************************************
	//CalcDrec - Representing the flow direction matrix values from above as pointers to other cells
	//Calculating a set of primitive pointers that aim from the current position into whatever cell
	//this cell happens to drain into.
	printf("Running CalcDrec() ...\n");
	pDEMProcess->CalcDrec(piFlowGrid, piDrecGrid);

	//************************************************************************************
	//CircleFlow - Checking for circular flow
	//
	printf("Running CircleFlow() ...\n");
	pDEMProcess->CircleFlow(piFlowGrid, piDrecGrid);

	if ((piShedGrid = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for watershed grid!");
		exit(1);
	}

	//start = chrono::high_resolution_clock::now();

	//***********************************************************************************
	//CalcWaterShed - goes into the DEM and various generated results and figures out which cells in the DEM
	//belong to which watershed.  
	printf("Computing watersheds ...\n");

	pDEMProcess->CalcWaterShed(pfDEMGrid, piFlowGrid, piDrecGrid, piShedGrid, piDEMSort);
	//***********************************************************************************
	//Allocating space for piUpSlopeGrid which is used in the next step
	if ((piUpSlpGrid = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for up slope grid!");
		exit(1);
	}

	//***********************************************************************************
	//CalcUpSlopeArea - 
	printf("Calculating upslope area...\n");
	pDEMProcess->CalcUpSlopeArea(pfDEMGrid, piFlowGrid, piDrecGrid, piUpSlpGrid, piDEMSort);



	//***********************************************************************************
	//Sorting and processing to get ready for Volume to Flood calculations...
	int		iPitNo = 0, *piSortedTemp;
	LSMSTATVAR	*pLsmStatVar;
	//LSMSTATVAR2	*pLsmStatVar2;

	if ((pfBottomUp = new double[totalpoints]) == NULL)
	{
		printf("Not enough memory for bottom up index grid!");
		exit(1);
	}

	if ((piShedWGrid = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for merge watershed grid!");
		exit(1);
	}

	if (!(piSortedTemp = new int[totalpoints]))	//temp array to hold index of records with missing elev field
	{
		printf("Not enough memory for pTempSort array, exiting ...");
		exit(1);
	}

	nIndex = 0;
	nMissingNum = 0;

	//Counting the number of pits ('5') and storing in iPitNo
	//Counting the number of missing values
	for (i = 0; i < totalpoints; i++)
	{
		piShedWGrid[i] = piShedGrid[i];

		if (piFlowGrid[i] == 5)
			iPitNo++;
		if (!(pfDEMGrid[i] == m_MissDataValue)) //make sure that value for elevation is not missing
		{
			piShedWGrid[i] = piShedGrid[i];
			pfBottomUp[nIndex] = (double)(((double)piShedWGrid[i] * pow(10.0, 18.0)) +
				((double)pfDEMGrid[i] * pow(10.0, 12.0)) +
				(1000000.0 - (double)piUpSlpGrid[i]));
			piDEMSort[nIndex] = i;
			nIndex++;
		}
		else
		{
			piSortedTemp[nMissingNum] = i;//keep track of missing value records 
			nMissingNum++;
		}
	}

	if ((pLsmStatVar = (LSMSTATVAR *)malloc(iPitNo * sizeof(LSMSTATVAR))) == NULL)
	{
		printf("Not enough memory for LSM statistic 1!");
		exit(1);
	}

	int pfpondCellElevSize = iPitNo * sizeof(LSMSTATVAR);

	QuickSort(pfBottomUp, totalpoints - nMissingNum, piDEMSort);//sort only records that have valid values
	nIndex = 0;
	for (i = totalpoints - nMissingNum; i < totalpoints; i++)//make the rest of sorted array point to 
														 //records that have missing values. 
	{
		piDEMSort[i] = piSortedTemp[nIndex];
		nIndex++;
	}
	delete[] piSortedTemp;

	int nPercentage = 10;
	printf("Computing fVaratio ...\n");

	iPitNo = 0;
	//Gerard-SNAP181011_133357
	//This is just a debug messagebuffer to see where in the looping this is crashing. Can be removed.
	//Gerard-SNAP181019_121941-DebugOOB char msgbuffer[1000];
	//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Total Points are: %d\n", totalpoints);
	//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);


	for (ii = 0; ii < totalpoints; ii++)
	{
		//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "ii is %d\n", ii);
		//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);


		if (ii == int(((totalpoints) / 100)*nPercentage))
		{
			nPercentage = nPercentage + 10;
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "nPercentage = %d\n", nPercentage);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);

		}
		/*else {
			sprintf(msgbuffer, "Completed 1\n");
			OutputDebugStringA((LPCSTR)msgbuffer);
		}*/
		if (piFlowGrid[piDEMSort[ii]] == 5 && (!(pfDEMGrid[piDEMSort[ii]] == m_MissDataValue)))
		{

			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Completed 2a\n");
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);

			pDEMProcess->ShedStat1(ii, pfpondCellElevSize, piDEMSort, pfDEMGrid, piFlowGrid, piShedGrid, piShedWGrid, &pLsmStatVar[iPitNo]);
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Completed 2b\n");
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);

			if (pLsmStatVar[iPitNo].iShedArea > 0) {
				//Gerard-SNAP181019_121941-DebugOOB 	sprintf(msgbuffer, "Completed 2c\n");
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				pLsmStatVar[iPitNo].fVaratio = (double)(pLsmStatVar[iPitNo].fPitVol / pLsmStatVar[iPitNo].iShedArea * 1000.0);
			}
			else {
				//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Completed 2d\n");
				//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
				pLsmStatVar[iPitNo].fVaratio = 0.0;
			}
			if (pLsmStatVar[iPitNo].fVaratio > 999999.99)
				pLsmStatVar[iPitNo].fVaratio = (double)999999.99;
			iPitNo++;
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA("iPitNo = " + iPitNo);
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "iPitNo = %d\n", iPitNo);
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
		}
		else {
			//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Completed 2\n");
			//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
		}
		//Gerard-SNAP181019_121941-DebugOOB sprintf(msgbuffer, "Looping\n");
		//Gerard-SNAP181019_121941-DebugOOB OutputDebugStringA((LPCSTR)msgbuffer);
	}

	int	*piShedOrd;
	double	*pfShed;
	if ((piShedOrd = new int[totalpoints]) == NULL)
	{
		printf("Not enough memory for watershed order index grid!");
		exit(1);
	}
	if ((pfShed = new double[totalpoints]) == NULL)
	{
		printf("Not enough memory for watershed !");
		exit(1);
	}
	/*if ((pLsmStatVar2 = new LSMSTATVAR2) == NULL)
	{
		printf("Not enough memory for LSM statistic 2!");
		exit(1);
	}*/

	for (i = 0; i < iPitNo; i++)
	{
		pfShed[i] = (double)(((20000.0 - (double)pLsmStatVar[i].fPitElev) * pow(10.0, 10.0)) + (int)pLsmStatVar[i].fVaratio);
		piShedOrd[i] = i;
	}

	//************************************************************
	//IGK
	//calculate volume to flood
	printf("Computing Volume2Flood ...\n");
	pDEMProcess->Volume2Flood(piDEMSort, pfDEMGrid, pLsmStatVar,
		pfVol2FlGrid, pfMm2FlGrid, pfPArea, piShedWGrid, piDEMSort, piDrecGrid, iPitNo, piFlowGrid);

	//*******************************************************************
	QuickSort(pfShed, iPitNo, piShedOrd);

	int	iPitRemoved = 0;

	//Currently variable iOutShed holds values for DrainsTo
	//because fox pro uses drainsto instead of OutShed, before going into the pit removal
	//function, we will copy values from iOutSHed into DrainsTo 
	int nTemp1 = 0;
	for (nTemp1 = 0; nTemp1 < iPitNo; nTemp1++)
	{
		pLsmStatVar[nTemp1].iDrainsTo = pLsmStatVar[nTemp1].iOutShed;
		pLsmStatVar[nTemp1].iNextPit = 0;
	}

	printf("Running Lsm_PitR...\n");
	pDEMProcess->Lsm_PitR(pfDEMGrid, piDEMSort, pLsmStatVar, piShedWGrid, piUpSlpGrid, iPitNo, piFlowGrid,
		piDrecGrid, m_PitArea, (double)m_PitDepth);

	int nLoop;
	int nLargest = 0;

	//IGK
	//int nLoop=0;
	// replace shed no with shednow
	for (nLoop = 0; nLoop < totalpoints; nLoop++)
	{
		piShedGrid[nLoop] = piShedWGrid[nLoop];
	}

	//remove ALL pits in a DEM from second lowest into lowest and 
	//then third lowest into the new lowest pit and so on and to compute pond & pit statistics for each pit, 
	//including higher order pits, as each is removed.
	printf("Running LowPitRemoval() ...\n");

	//*************************************************************
	//IGK
	//Declare variables to hold pond statistics data, represents pond table
	LSMPONDSTATS* pPondStatVar;
	if ((pPondStatVar = new LSMPONDSTATS[1000]) == NULL)
	{
		printf("Not enough memory for pond statistics!");
		exit(1);
	}
	int nPondFileSize = 0;

	LSMSTATVAR* pFillFile;
	int nFillFileSize = 0;

	if ((pFillFile = new LSMSTATVAR[1000]) == NULL)
	{
		printf("Not enough memory for fill file!");
		exit(1);
	}
	std::string sName;
	sName = "C++Vold.csv";
	VoldFile vold(sName);
	sName = "C++Mold.csv";
	VoldFile mold(sName);

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		pLsmStatVar[nLoop].fPreVol = 0.0;
	}
	for (nLoop = 0; nLoop < totalpoints; nLoop++)
	{
		piShedGrid[nLoop] = piShedWGrid[nLoop];
	}
	pDEMProcess->LowPitRemoval(pLsmStatVar, &pPondStatVar, m_GridSize,
		iPitNo, pfDEMGrid, piDEMSort, piShedOrd, pfShed, pfBottomUp, piShedWGrid, piUpSlpGrid,
		piDrecGrid, piFlowGrid, vold, nPondFileSize, pfVol2FlGrid, pfMm2FlGrid, pfPArea, piFlowGrid);

	delete[] pfShed;
	delete[] piShedOrd;

	//Fix Pit File
	printf("Running Fix_PitFile() ...\n");
	pDEMProcess->Fix_PitFile(pLsmStatVar, pfDEMGrid, piShedWGrid, iPitNo, piShedGrid);

	//Reset DEM
	printf("Running Reset_DEM() ...\n");
	pDEMProcess->Reset_Dem(vold, piFlowGrid, piDrecGrid, piUpSlpGrid, piShedWGrid, piShedGrid);

	//Running LSM_PitR8
	printf("Running LSM_PitR8() ...\n");
	pDEMProcess->LSM_PitR8(piShedGrid, pLsmStatVar, iPitNo, &pFillFile, nFillFileSize, pfDEMGrid, piShedWGrid, piUpSlpGrid, mold,
		piFlowGrid, piDrecGrid, pfVol2FlGrid, pfMm2FlGrid, pPondStatVar);

	int nStepNum = 0;
	int nCounter1 = 0;
	//char** pTempPointer = 0;
	//char* sCString = 0, 
	char *sCStringTemp;
	bool *pMissing;

	if (!(sCStringTemp = new char[255]))
	{
		printf("Not enough memory for pTempSort array, exiting ...");
		exit(1);
	}

	if (!(pMissing = new bool[totalpoints]))
	{
		printf("Not enough memory for pTempSort array, exiting ...");
		exit(1);
	}

	for (nLoop = 0; nLoop < totalpoints; nLoop++)
	{
		if (pfDEMGrid[nLoop] == m_MissDataValue)
		{
			pMissing[nLoop] = true;
		}
		else
		{
			pMissing[nLoop] = false;
		}
	}
	//std::string chrEdge = "T";
	//std::string chrFinal = "T";
	//std::string chrRemoved = "T";
	//std::string chrVisited = "T";
	std::string sTempName;
	sTempName = m_WorkingDirectory;
	//sTempName += m_NamePrefix.at(0);
	//sTempName += m_NamePrefix.at(1);
	//sTempName += m_NamePrefix.at(2);
	sTempName += m_NamePrefix;
	std::string sTemp;

	//Write pit data to csv file
	printf("Saving data to pit CSV file ...\n");

	/*char* pChar;

	if (!(pChar = new char[255]))
	{
		printf("Not enough memory for pChar array, exiting ...");
		exit(1);
	}*/

	double easting;
	double northing;
	int seqNo;

	sTemp = sTempName + "pit.csv";

	ofstream out;

	out.open(sTemp, ios::trunc);
	out.precision(12);
	out << "easting" << "," << "northing" << "," "ShedNo" << "," << "Edge" << "," << "Final" << "," << "EndPit" << "," << "ShedArea" << "," << "PitRow" << "," << "PitCol" << "," << "PitRec" << ","
		<< "PitElev" << "," << "PourElev" << "," << "PreVol" << "," << "PitVol" << ","
		<< "Varatio" << "," << "PitArea" << "," << "DrainsTo" << "," << "NextPit" << "," << "Becomes" << ","
		"Removed" << "," << "InRow" << "," << "InColumn" << "," << "InRec" << "," << "InElev" << "," << "OutRow" << "," << "OutCol" << "," << "OutRec" << "," << "OutElev" << ","
		<< "Stage" << "," << "Visited" << "\n";

	string finall;
	string removed;
	string visited;
	string edge;

	for (nLoop = 0; nLoop < iPitNo; nLoop++)
	{
		seqNo = (pLsmStatVar[nLoop].iPitRow - 1) * m_Column + pLsmStatVar[nLoop].iPitCol;
		easting = xll + ((seqNo - 1) % m_Column)*m_GridSize;
		northing = yll + (m_Row - (seqNo - 1) / m_Column)*m_GridSize;
		if (pLsmStatVar[nLoop].bFinal == true)
		{
			finall = "true";
		}
		else {
			finall = "false";
		}
		if (pLsmStatVar[nLoop].bRemoved == true)
		{
			removed = "true";
		}
		else {
			removed = "false";
		}
		if (pLsmStatVar[nLoop].bVisited == true)
		{
			visited = "true";
		}
		else {
			visited = "false";
		}
		out << easting << "," << northing << "," << pLsmStatVar[nLoop].iThisShed << "," << "false" << "," << finall << ","
			<< pLsmStatVar[nLoop].iEndPit << "," << pLsmStatVar[nLoop].iShedArea << "," << pLsmStatVar[nLoop].iPitRow << ","
			<< pLsmStatVar[nLoop].iPitCol << "," << pLsmStatVar[nLoop].iPitRec << "," << pLsmStatVar[nLoop].fPitElev << "," <<
			pLsmStatVar[nLoop].fPourElev << "," << pLsmStatVar[nLoop].fPreVol << "," << pLsmStatVar[nLoop].fPitVol
			<< "," << pLsmStatVar[nLoop].fVaratio << "," << pLsmStatVar[nLoop].iPitArea << ","
			<< pLsmStatVar[nLoop].iDrainsTo << "," << pLsmStatVar[nLoop].iNextPit << ","
			<< pLsmStatVar[nLoop].iBecomes << "," << removed << "," << pLsmStatVar[nLoop].iInRow << ","
			<< pLsmStatVar[nLoop].iInColumn << "," << pLsmStatVar[nLoop].iInRec << "," << pLsmStatVar[nLoop].fInElev << "," <<
			pLsmStatVar[nLoop].iOutRow << "," << pLsmStatVar[nLoop].iOutColumn << "," << pLsmStatVar[nLoop].iOutRec
			<< "," << pLsmStatVar[nLoop].fOutElev << "," << pLsmStatVar[nLoop].iStage << "," << visited << "\n";
	}
	out.close();
	free(pLsmStatVar);
	string filePathForReplace = "\"pitPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"pitPath\":\"" + sTemp + "\"");

	printf("Saving data to fill CSV file ...\n");
	sTemp = sTempName + "fill.csv";
	out.open(sTemp, ios::trunc);
	out.precision(12);
	out << "easting" << "," << "northing" << "," << "ShedNo" << "," << "Edge" << "," << "Final" << "," << "EndPit" << "," << "ShedArea" << "," << "PitRow" << "," << "PitCol" << "," << "PitRec" << ","
		<< "PitElev" << "," << "PourElev" << "," << "PreVol" << "," << "PitVol" << ","
		"Varatio" << "," << "PitArea" << "," << "DrainsTo" << "," << "NextPit" << "," << "Becomes" << ","
		"Removed" << "," << "InRow" << "," << "InColumn" << "," << "InRec" << "," << "InElev" << "," << "OutRow" << "," << "OutCol" << "," << "OutRec" << "," << "OutElev" << ","
		<< "Stage" << "," << "Visited" << "\n";

	for (nLoop = 0; nLoop < nFillFileSize; nLoop++)
	{
		seqNo = (pFillFile[nLoop].iPitRow - 1) * m_Column + pFillFile[nLoop].iPitCol;
		easting = xll + ((seqNo - 1) % m_Column)*m_GridSize;
		northing = yll + (m_Row - (seqNo - 1) / m_Column)*m_GridSize;
		if (pFillFile[nLoop].bEdge == true)
		{
			edge = "true";
		}
		else {
			edge = "false";
		}
		if (pFillFile[nLoop].bFinal == true)
		{
			finall = "true";
		}
		else {
			finall = "false";
		}
		if (pFillFile[nLoop].bRemoved == true)
		{
			removed = "true";
		}
		else {
			removed = "false";
		}
		if (pFillFile[nLoop].bVisited == true)
		{
			visited = "true";
		}
		else {
			visited = "false";
		}
		out << easting << "," << northing << "," << pFillFile[nLoop].iThisShed << "," << edge << "," << finall << ","
			<< pFillFile[nLoop].iEndPit << "," << pFillFile[nLoop].iShedArea << "," << pFillFile[nLoop].iPitRow << ","
			<< pFillFile[nLoop].iPitCol << "," << pFillFile[nLoop].iPitRec << "," << pFillFile[nLoop].fPitElev << "," <<
			pFillFile[nLoop].fPourElev << "," << pFillFile[nLoop].fPreVol << "," << pFillFile[nLoop].fPitVol
			<< "," << pFillFile[nLoop].fVaratio << "," << pFillFile[nLoop].iPitArea << ","
			<< pFillFile[nLoop].iDrainsTo << "," << pFillFile[nLoop].iNextPit << ","
			<< pFillFile[nLoop].iBecomes << "," << removed << "," << pFillFile[nLoop].iInRow << ","
			<< pFillFile[nLoop].iInColumn << "," << pFillFile[nLoop].iInRec << "," << pFillFile[nLoop].fInElev << "," <<
			pFillFile[nLoop].iOutRow << "," << pFillFile[nLoop].iOutColumn << "," << pFillFile[nLoop].iOutRec
			<< "," << pFillFile[nLoop].fOutElev << "," << pFillFile[nLoop].iStage << "," << visited << "\n";
	}

	out.close();
	delete[] pFillFile;

	filePathForReplace = "\"fillPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"fillPath\":\"" + sTemp + "\"");



	printf("Saving data to pond CSV file ...\n");
	sTemp = sTempName + "pond.csv";
	out.open(sTemp, ios::trunc);
	out.precision(12);
	out << "easting" << "," << "northing" << "," << "ShedNo" << "," << "Edge" << "," << "Final" << "," << "EndPit" << "," << "ShedArea" << "," << "PitRow" << "," << "PitCol" << "," << "PitRec" << ","
		<< "PitElev" << "," << "PourElev" << "," << "PreVol" << "," << "PitVol" << ","
		"Varatio" << "," << "PitArea" << "," << "DrainsTo" << "," << "NextPit" << "," << "Becomes" << ","
		"Removed" << "," << "InRow" << "," << "InColumn" << "," << "InRec" << "," << "InElev" << "," << "OutRow" << "," << "OutCol" << "," << "OutRec" << "," << "OutElev" << ","
		<< "Stage" << "," << "Visited" << "\n";

	pondobj->setLength(nPondFileSize);

	for (nLoop = 0; nLoop < nPondFileSize; nLoop++)
	{
		seqNo = (pPondStatVar[nLoop].iPitRow - 1) * m_Column + pPondStatVar[nLoop].iPitCol;
		easting = xll + ((seqNo - 1) % m_Column)*m_GridSize;
		northing = yll + (m_Row - (seqNo - 1) / m_Column)*m_GridSize;
		pondobj->ShedNo[nLoop] = pPondStatVar[nLoop].iThisShed;
		pondobj->Edge[nLoop] = pPondStatVar[nLoop].bEdge;
		pondobj->Final[nLoop] = pPondStatVar[nLoop].bFinal;
		pondobj->EndPit[nLoop] = pPondStatVar[nLoop].iEndPit;
		pondobj->ShedArea[nLoop] = pPondStatVar[nLoop].iShedArea;
		pondobj->PitRow[nLoop] = pPondStatVar[nLoop].iPitRow;
		pondobj->PitCol[nLoop] = pPondStatVar[nLoop].iPitCol;
		pondobj->PitRec[nLoop] = pPondStatVar[nLoop].iPitRec;
		pondobj->PitElev[nLoop] = pPondStatVar[nLoop].fPitElev;
		pondobj->PourElev[nLoop] = pPondStatVar[nLoop].fPourElev;
		pondobj->PreVol[nLoop] = pPondStatVar[nLoop].fPreVol;
		pondobj->PitVol[nLoop] = pPondStatVar[nLoop].fPitVol;
		pondobj->Varatio[nLoop] = pPondStatVar[nLoop].fVaratio;
		pondobj->PitArea[nLoop] = pPondStatVar[nLoop].iPitArea;
		pondobj->DrainsTo[nLoop] = pPondStatVar[nLoop].iDrainsTo;
		pondobj->NextPit[nLoop] = pPondStatVar[nLoop].iNextPit;
		pondobj->Becomes[nLoop] = pPondStatVar[nLoop].iBecomes;
		pondobj->Removed[nLoop] = pPondStatVar[nLoop].bRemoved;
		pondobj->InRow[nLoop] = pPondStatVar[nLoop].iInRow;
		pondobj->InCol[nLoop] = pPondStatVar[nLoop].iInColumn;
		pondobj->InRec[nLoop] = pPondStatVar[nLoop].iInRec;
		pondobj->InElev[nLoop] = pPondStatVar[nLoop].fInElev;
		pondobj->OutRow[nLoop] = pPondStatVar[nLoop].iOutRow;
		pondobj->OutCol[nLoop] = pPondStatVar[nLoop].iOutColumn;
		pondobj->OutRec[nLoop] = pPondStatVar[nLoop].iOutRec;
		pondobj->OutElev[nLoop] = pPondStatVar[nLoop].fOutElev;
		pondobj->Stage[nLoop] = pPondStatVar[nLoop].iStage;
		pondobj->Visited[nLoop] = pPondStatVar[nLoop].bVisited;

		if (pPondStatVar[nLoop].bEdge == true)
		{
			edge = "true";
		}
		else {
			edge = "false";
		}
		if (pPondStatVar[nLoop].bFinal == true)
		{
			finall = "true";
		}
		else {
			finall = "false";
		}
		if (pPondStatVar[nLoop].bRemoved == true)
		{
			removed = "true";
		}
		else {
			removed = "false";
		}
		if (pPondStatVar[nLoop].bVisited == true)
		{
			visited = "true";
		}
		else {
			visited = "false";
		}
		out << easting << "," << northing << "," << pPondStatVar[nLoop].iThisShed << "," << edge << "," << finall << ","
			<< pPondStatVar[nLoop].iEndPit << "," << pPondStatVar[nLoop].iShedArea << "," << pPondStatVar[nLoop].iPitRow << ","
			<< pPondStatVar[nLoop].iPitCol << "," << pPondStatVar[nLoop].iPitRec << "," << pPondStatVar[nLoop].fPitElev << "," <<
			pPondStatVar[nLoop].fPourElev << "," << pPondStatVar[nLoop].fPreVol << "," << pPondStatVar[nLoop].fPitVol
			<< "," << pPondStatVar[nLoop].fVaratio << "," << pPondStatVar[nLoop].iPitArea << ","
			<< pPondStatVar[nLoop].iDrainsTo << "," << pPondStatVar[nLoop].iNextPit << ","
			<< pPondStatVar[nLoop].iBecomes << "," << removed << "," << pPondStatVar[nLoop].iInRow << ","
			<< pPondStatVar[nLoop].iInColumn << "," << pPondStatVar[nLoop].iInRec << "," << pPondStatVar[nLoop].fInElev << "," <<
			pPondStatVar[nLoop].iOutRow << "," << pPondStatVar[nLoop].iOutColumn << "," << pPondStatVar[nLoop].iOutRec
			<< "," << pPondStatVar[nLoop].fOutElev << "," << pPondStatVar[nLoop].iStage << "," << visited << "\n";
	}

	out.close();
	delete[] pPondStatVar;

	filePathForReplace = "\"pondPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"pondPath\":\"" + sTemp + "\"");


	printf("Copying data to vold file ...\n");
	sTemp = sTempName + "vold.csv";
	out.open(sTemp, ios::trunc);
	out.precision(12);
	out << "easting" << "," << "northing" << "," << "SeqNo" << "," << "Ddir" << "," << "Drec" << "," << "Upslope" << "," << "ShedNow" << "," << "Stage" << "," << "Urec" << ","
		<< "Ds-Area" << "," << "Varatio" << "\n";

	int voldfilesize = vold.GetSize();

	for (nLoop = 0; nLoop < voldfilesize; nLoop++)
	{
		out << easting << "," << northing << "," << vold.GetSeqNo(nLoop) << "," << vold.GetDdir(nLoop) << "," << vold.GetDrec(nLoop) << "," << vold.GetUpSlop(nLoop) << ","
			<< vold.GetShedNow(nLoop) << "," << vold.GetStage(nLoop) << "," << vold.GetUrec(nLoop) << "," << vold.GetDs_Area(nLoop) << ","
			<< vold.GetVaration(nLoop) << "\n";
	}

	out.close();

	filePathForReplace = "\"voldPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"voldPath\":\"" + sTemp + "\"");

	printf("Saving data to mold file...\n");
	sTemp = sTempName + "mold.csv";
	out.open(sTemp, ios::trunc);
	out.precision(12);
	out << "easting" << "," << "northing" << "," << "SeqNo" << "," << "Ddir" << "," << "Drec" << "," << "Upslope" << "," << "ShedNow" << "," << "Stage" << "," << "Urec" << ","
		<< "Ds-Area" << "," << "Varatio" << "\n";

	int moldFileSize = mold.GetSize();

	for (nLoop = 0; nLoop < moldFileSize; nLoop++)
	{
		easting = xll + ((mold.GetSeqNo(nLoop) - 1) % m_Column)*m_GridSize;
		northing = yll + (m_Row - (mold.GetSeqNo(nLoop) - 1) / m_Column)*m_GridSize;
		out << easting << "," << northing << "," << mold.GetSeqNo(nLoop) << "," << mold.GetDdir(nLoop) << "," << mold.GetDrec(nLoop) << "," << mold.GetUpSlop(nLoop) << ","
			<< mold.GetShedNow(nLoop) << "," << mold.GetStage(nLoop) << "," << mold.GetUrec(nLoop) << "," << mold.GetDs_Area(nLoop) << ","
			<< mold.GetVaration(nLoop) << "\n";
	}
	out.close();

	filePathForReplace = "\"moldPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"moldPath\":\"" + sTemp + "\"");

	printf("Saving data to dem file...\n");
	nStepNum = (totalpoints) / 10;
	nCounter1 = nStepNum;
	int n = nStepNum;

	sTemp = sTempName + "dem.csv";
	const char* demfile = sTemp.c_str();
	/*out.open(sTemp, ios::trunc);
	out << "SeqNo" << "," << "Row" << "," << "Column" << "," << "Elev" << "," << "Ddir" << "," << "Drec" << "," << "Upslope" << ","
		<< "ShedNo" << "," << "ShedNow" << "," << "Missing" << "," << "Edge" << "," << "Vol2Fl" << "," << "Mm2fl" << "," << "PArea" << "\n";*/
	FILE* demfp;
	demfp = fopen(demfile, "w+");
	fprintf(demfp, "easting,northing,SeqNo,Row,Column,Elev,Ddir,Drec,Upslope,ShedNo,ShedNow,Missing,Edge,Vol2Fl,Mm2fl,PArea\n");
	//double dDoubleValue = 0;
	string missing;
	for (nLoop = 0; nLoop < totalpoints; nLoop++) {
		dem->SeqNo[nLoop] = nLoop + 1;
		dem->Row[nLoop] = nLoop / m_Column + 1;
		dem->Col[nLoop] = nLoop % m_Column + 1;
		dem->Elev[nLoop] = pfDEMGrid[nLoop];
		dem->Ddir[nLoop] = piFlowGrid[nLoop];
		dem->Drec[nLoop] = piDrecGrid[nLoop];
		dem->UpSlope[nLoop] = piUpSlpGrid[nLoop];
		dem->ShedNo[nLoop] = piShedGrid[nLoop];
		dem->ShedNow[nLoop] = piShedWGrid[nLoop];
		dem->Missing[nLoop] = pMissing[nLoop];
		dem->Edge[nLoop] = false;
		dem->Vol2Fl[nLoop] = pfVol2FlGrid[nLoop];
		dem->Mm2Fl[nLoop] = pfMm2FlGrid[nLoop];
		dem->PArea[nLoop] = pfPArea[nLoop];
		if (pMissing[nLoop] == true)
		{
			missing = "true";
		}
		else {
			missing = "false";
		}
		/*out << nLoop + 1 <<"," << nLoop / m_Column + 1 << "," << nLoop % m_Column + 1 <<","<< pfDEMGrid[nLoop] << "," <<
			piFlowGrid[nLoop] << "," << piDrecGrid[nLoop] << "," << piUpSlpGrid[nLoop] << "," <<
			piShedGrid[nLoop] << "," << piShedWGrid[nLoop] << "," << missing << ","<< "false" << ","
			<< pfVol2FlGrid[nLoop] << "," << pfMm2FlGrid[nLoop] << "," << pfPArea[nLoop] << "\n";*/
		fprintf(demfp, "%f,%f,%d,%d,%d,%f,%d,%d,%d,%d,%d,%s,%s,%f,%f,%d\n", easting, northing, nLoop + 1, dem->Row[nLoop], dem->Col[nLoop], pfDEMGrid[nLoop],
			piFlowGrid[nLoop], piDrecGrid[nLoop], piUpSlpGrid[nLoop], piShedGrid[nLoop], piShedWGrid[nLoop], missing.c_str(), "false", pfVol2FlGrid[nLoop],
			pfMm2FlGrid[nLoop], pfPArea[nLoop]);
	}
	fclose(demfp);
	//out.close();

	filePathForReplace = "\"demPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"demPath\":\"" + sTemp + "\"");

	delete[] piDrecGrid;
	delete[] piFlowGrid;
	delete[] piShedGrid;
	delete[] piShedWGrid;
	delete[] piUpSlpGrid;
	//delete pLsmStatVar2;

	if (pbInvertedElev == true)
	{
		double *pInvertedElev;
		if (!(pInvertedElev = new double[totalpoints]))
		{
			printf("Not enough memory for pInvertedElev array, exiting ...");
			exit(1);
		}

		double *fHighest = new double;
		*fHighest = 0.000;
		int *pInvertedFlow = new int[totalpoints];
		if (pInvertedFlow == NULL)
		{
			printf("Not enough memory to create inverted flow array.");
			exit(1);
		}

		int* pInvertedDrec = new int[totalpoints];
		if (pInvertedDrec == NULL)
		{
			printf("Not enough memory to create inverted flow record array.");
			exit(1);
		}

		int* pInvertedUpSlp = new int[totalpoints];
		if (pInvertedUpSlp == NULL)
		{
			printf("Not enough memory to create inverted UpSlope array.");
			exit(1);
		}

		int* pInvertedShedNo = new int[totalpoints];
		if (pInvertedShedNo == NULL)
		{
			printf("Not enough memory to create inverted ShedNo array.");
			exit(1);
		}

		int* pInvertedShedNow = new int[totalpoints];
		if (pInvertedShedNow == NULL)
		{
			printf("Not enough memory to create inverted ShedNow array.");
			exit(1);
		}

		float* pInvertedVol2Fld = new float[totalpoints];
		if (pInvertedVol2Fld == NULL)
		{
			printf("Not enough memory to create inverted volume to flood array.");
			exit(1);
		}

		double* pInvertedMm2Fld = new double[totalpoints];
		if (pInvertedMm2Fld == NULL)
		{
			printf("Not enough memory to create inverted Mm to flood array.");
			exit(1);
		}

		int* pInvertedPArea = new int[totalpoints];
		if (pInvertedPArea == NULL)
		{
			printf("Not enough memory to create inverted PArea array.");
			exit(1);
		}

		for (nLoop = 0; nLoop < totalpoints; nLoop++)
		{
			if (*fHighest < pfDEMGrid[nLoop])
			{
				*fHighest = pfDEMGrid[nLoop];
			}
		}

		for (nLoop = 0; nLoop < totalpoints; nLoop++)
		{
			if (!(pfDEMGrid[nLoop] == m_MissDataValue))
			{
				pInvertedElev[nLoop] = *fHighest - pfDEMGrid[nLoop];
			}
			else
			{
				pInvertedElev[nLoop] = m_MissDataValue;
			}
		}
		delete fHighest;

		printf("Running inverted flowdir..\n");
		pDEMProcess->FlowDir(pInvertedElev, pInvertedFlow);

		printf("Running inverted FixFlat..\n");
		pDEMProcess->FixFlat(pInvertedElev, pInvertedFlow);

		printf("Running inverted CalcDrec() ...\n");
		pDEMProcess->CalcDrec(pInvertedFlow, pInvertedDrec);

		printf("Running inverted CircleFlow() ...\n");
		pDEMProcess->CircleFlow(pInvertedFlow, pInvertedDrec);

		if ((pfTmpDEMGrid = new double[totalpoints]) == NULL)
		{
			printf("Not enough memory for TmpDEM grid!");
			exit(1);
		}

		if ((pTempSort = new int[totalpoints]) == NULL)
		{
			printf("Not enough memory for temp sort array for DEM grid!");
			exit(1);
		}

		nMissingNum = 0;
		nIndex = 0;

		for (i = 0; i < totalpoints; i++)
		{
			if (!(pInvertedElev[i] == m_MissDataValue))//if value is not missing
			{
				pfTmpDEMGrid[nIndex] = pInvertedElev[i];
				piDEMSort[nIndex] = i;
				nIndex++;
			}
			else
			{
				pTempSort[nMissingNum] = i;
				nMissingNum++; //keep count of missing values
			}
		}

		//original
		//QuickSort(pfTmpDEMGrid, totalpoints - nMissingNum, piDEMSort, true); //sort only non missing values

		//modified to conform to the new implementation of Quicksort
		QuickSort(pfTmpDEMGrid, totalpoints - nMissingNum, piDEMSort); //sort only non missing values

		delete[] pfTmpDEMGrid;
		nIndex = 0;
		for (i = totalpoints - nMissingNum; i < totalpoints; i++) //make the rest of pointers which are
															  // pointing to the missing values, valid. 
		{
			piDEMSort[i] = pTempSort[nIndex];
			nIndex++;
		}

		printf("Running inverted CalcWaterShed() ...\n");
		pDEMProcess->CalcWaterShed(pInvertedElev, pInvertedFlow, pInvertedDrec, pInvertedShedNo, piDEMSort);

		printf("Running inverted CalcUpSlopeArea() ...\n");
		pDEMProcess->CalcUpSlopeArea(pInvertedElev, pInvertedFlow, pInvertedDrec, pInvertedUpSlp, piDEMSort);

		int nInvertedPitNo = 0;
		piSortedTemp = new int[totalpoints];//temp array to hold index of records with missing elev field

		nIndex = 0;
		nMissingNum = 0;

		for (i = 0; i < totalpoints; i++)
		{
			pInvertedShedNow[i] = pInvertedShedNo[i];

			if (pInvertedFlow[i] == 5)
				nInvertedPitNo++;
			if (!(pInvertedElev[i] == m_MissDataValue)) //make sure that value for elevation is not missing
			{
				pInvertedShedNow[i] = pInvertedShedNo[i];
				pfBottomUp[nIndex] = (double)(((double)pInvertedShedNow[i] * pow(10.0, 18.0)) +
					((double)pInvertedElev[i] * pow(10.0, 12.0)) +
					(1000000.0 - (double)pInvertedUpSlp[i]));
				piDEMSort[nIndex] = i;
				nIndex++;
			}
			else
			{
				piSortedTemp[nMissingNum] = i;//keep track of missing value records 
				nMissingNum++;
			}
		}

		LSMSTATVAR* pInvertedLsmStatVar = 0;
		if ((pInvertedLsmStatVar = new LSMSTATVAR[nInvertedPitNo]) == NULL)
		{
			printf("Not enough memory for LSM Inverted statistic 1!");
			exit(1);
		}

		for (nLoop = 0; nLoop < nInvertedPitNo; nLoop++)
		{
			pInvertedLsmStatVar[nLoop].bEdge = false;
			pInvertedLsmStatVar[nLoop].bFinal = false;
			pInvertedLsmStatVar[nLoop].bRemoved = false;
			pInvertedLsmStatVar[nLoop].bVisited = false;
			pInvertedLsmStatVar[nLoop].iNextPit = 0;
			pInvertedLsmStatVar[nLoop].iBecomes = 0;
			pInvertedLsmStatVar[nLoop].iStage = 0;
			pInvertedLsmStatVar[nLoop].fPreVol = 0;
			pInvertedLsmStatVar[nLoop].iDrainsTo = 0;
			pInvertedLsmStatVar[nLoop].iEndPit = 0;
		}
		QuickSort(pfBottomUp, totalpoints - nMissingNum, piDEMSort);//sort only records that have valid values

		delete[] pfBottomUp;

		nIndex = 0;
		for (i = totalpoints - nMissingNum; i < totalpoints; i++)//make the rest of sorted array point to 
															 //records that have missing values. 
		{
			piDEMSort[i] = piSortedTemp[nIndex];
			nIndex++;
		}
		delete[] piSortedTemp;

		nPercentage = 10;
		nInvertedPitNo = 0;
		for (ii = 0; ii < totalpoints; ii++)
		{
			if (ii == int(((totalpoints) / 100)*nPercentage))
			{
				nPercentage = nPercentage + 10;
			}
			if (pInvertedFlow[piDEMSort[ii]] == 5 && (!(pInvertedElev[piDEMSort[ii]] == m_MissDataValue)))
			{
				pDEMProcess->ShedStat1(ii, pfpondCellElevSize, piDEMSort, pInvertedElev, pInvertedFlow,
					pInvertedShedNo, pInvertedShedNow, &pInvertedLsmStatVar[nInvertedPitNo]);

				if (pInvertedLsmStatVar[nInvertedPitNo].iShedArea > 0)
					pInvertedLsmStatVar[nInvertedPitNo].fVaratio =
					(double)(pInvertedLsmStatVar[nInvertedPitNo].fPitVol /
						pInvertedLsmStatVar[nInvertedPitNo].iShedArea * 1000.0);
				else
					pInvertedLsmStatVar[nInvertedPitNo].fVaratio = 0.0;
				if (pInvertedLsmStatVar[nInvertedPitNo].fVaratio > 999999.99)
					pInvertedLsmStatVar[nInvertedPitNo].fVaratio = (double)999999.99;

				nInvertedPitNo++;
			}
		}

		printf("Running inverted Lsm_PitR() ...\n");
		pDEMProcess->Lsm_PitR(pInvertedElev, piDEMSort, pInvertedLsmStatVar, pInvertedShedNow, pInvertedUpSlp, nInvertedPitNo, pInvertedFlow,
			pInvertedDrec, m_PitArea, (double)m_PitDepth);

		nStepNum = (totalpoints) / 10;
		nCounter1 = nStepNum;
		n = nStepNum;

		printf("Saving data to idem file ...\n");
		sTemp = sTempName + "idem.csv";
		const char* idemfile = sTemp.c_str();
		/*out.open(sTemp, ios::trunc);
		out << "SeqNo" << "," << "Row" << "," << "Column" << "," << "Elev" << "," << "Ddir" << "," << "Drec" << "," << "Upslope" << ","
			<< "ShedNo" << "," << "ShedNow" << "," << "Missing" << "," << "Edge" << "," << "Vol2Fl" << "," << "Mm2fl" << "," << "PArea" << "\n";*/
		FILE* idemfp;
		idemfp = fopen(idemfile, "w+");
		fprintf(idemfp, "easting,northing,SeqNo,Row,Column,Elev,Ddir,Drec,Upslope,ShedNo,ShedNow,Missing,Edge,Vol2Fl,Mm2fl,PArea\n");
		for (nLoop = 0; nLoop < totalpoints; nLoop++) {
			idemobj->SeqNo[nLoop] = nLoop + 1;
			easting = xll + ((idemobj->SeqNo[nLoop] - 1) % m_Column)*m_GridSize;
			northing = yll + (m_Row - (idemobj->SeqNo[nLoop] - 1) / m_Column)*m_GridSize;
			idemobj->Row[nLoop] = nLoop / m_Column + 1;
			idemobj->Col[nLoop] = nLoop % m_Column + 1;
			idemobj->Elev[nLoop] = pInvertedElev[nLoop];
			idemobj->Ddir[nLoop] = pInvertedFlow[nLoop];
			idemobj->Drec[nLoop] = pInvertedDrec[nLoop];
			idemobj->UpSlope[nLoop] = pInvertedUpSlp[nLoop];
			idemobj->ShedNo[nLoop] = pInvertedShedNo[nLoop];
			idemobj->ShedNow[nLoop] = pInvertedShedNow[nLoop];
			idemobj->Missing[nLoop] = pMissing[nLoop];
			idemobj->Edge[nLoop] = false;
			idemobj->Vol2Fl[nLoop] = pfVol2FlGrid[nLoop];
			idemobj->Mm2Fl[nLoop] = pfMm2FlGrid[nLoop];
			idemobj->PArea[nLoop] = pfPArea[nLoop];
			if (pMissing[nLoop] == true)
			{
				missing = "true";
			}
			else {
				missing = "false";
			}
			/*out << nLoop + 1 << "," << nLoop / m_Column + 1 << "," << nLoop % m_Column + 1 << "," << pInvertedElev[nLoop] << "," <<
				pInvertedFlow[nLoop] << "," << pInvertedDrec[nLoop] << "," << pInvertedUpSlp[nLoop] << "," <<
				pInvertedShedNo[nLoop] << "," << pInvertedShedNow[nLoop] << "," << missing << "," << "false" << ","
				<< pfVol2FlGrid[nLoop] << "," << pfMm2FlGrid[nLoop] << "," << pfPArea[nLoop] << "\n";*/
			fprintf(idemfp, "%f,%f,%d,%d,%d,%f,%d,%d,%d,%d,%d,%s,%s,%f,%f,%d\n", easting, northing, nLoop + 1, idemobj->Row[nLoop], idemobj->Col[nLoop], pInvertedElev[nLoop],
				pInvertedFlow[nLoop], pInvertedDrec[nLoop], pInvertedUpSlp[nLoop], pInvertedShedNo[nLoop], pInvertedShedNow[nLoop], missing.c_str(), "false", pfVol2FlGrid[nLoop],
				pfMm2FlGrid[nLoop], pfPArea[nLoop]);
		}
		fclose(idemfp);
		//out.close();
		filePathForReplace = "\"iDemPath\":null";
		while (sTemp.find("\\") != std::string::npos) {
			sTemp.replace(sTemp.find("\\"), 1, "slash");
		}
		while (sTemp.find("slash") != std::string::npos) {
			sTemp.replace(sTemp.find("slash"), 5, "\\\\");
		}
		resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"iDemPath\":\"" + sTemp + "\"");

		printf("Saving data to ipit file ...\n");
		sTemp = sTempName + "ipit.csv";
		out.open(sTemp, ios::trunc);
		out.precision(12);
		out << "easting" << "," << "northing" << "," << "ShedNo" << "," << "Edge" << "," << "Final" << "," << "EndPit" << "," << "ShedArea" << "," << "PitRow" << "," << "PitCol" << "," << "PitRec" << ","
			<< "PitElev" << "," << "PourElev" << "," << "PreVol" << "," << "PitVol" << ","
			"Varatio" << "," << "PitArea" << "," << "DrainsTo" << "," << "NextPit" << "," << "Becomes" << ","
			"Removed" << "," << "InRow" << "," << "InColumn" << "," << "InRec" << "," << "InElev" << "," << "OutRow" << "," << "OutCol" << "," << "OutRec" << "," << "OutElev" << ","
			<< "Stage" << "," << "Visited" << "\n";
		for (nLoop = 0; nLoop < nInvertedPitNo; nLoop++)
		{
			seqNo = (pInvertedLsmStatVar[nLoop].iPitRow - 1) * m_Column + pInvertedLsmStatVar[nLoop].iPitCol;
			easting = xll + ((seqNo - 1) % m_Column)*m_GridSize;
			northing = yll + (m_Row - (seqNo - 1) / m_Column)*m_GridSize;
			if (pInvertedLsmStatVar[nLoop].bFinal == true)
			{
				finall = "true";
			}
			else {
				finall = "false";
			}
			if (pInvertedLsmStatVar[nLoop].bRemoved == true)
			{
				removed = "true";
			}
			else {
				removed = "false";
			}
			if (pInvertedLsmStatVar[nLoop].bVisited == true)
			{
				visited = "true";
			}
			else {
				visited = "false";
			}

			out << easting << "," << northing << "," << pInvertedLsmStatVar[nLoop].iThisShed << "," << "false" << "," << finall << ","
				<< pInvertedLsmStatVar[nLoop].iEndPit << "," << pInvertedLsmStatVar[nLoop].iShedArea << "," << pInvertedLsmStatVar[nLoop].iPitRow << ","
				<< pInvertedLsmStatVar[nLoop].iPitCol << "," << pInvertedLsmStatVar[nLoop].iPitRec << "," << pInvertedLsmStatVar[nLoop].fPitElev << "," <<
				pInvertedLsmStatVar[nLoop].fPourElev << "," << pInvertedLsmStatVar[nLoop].fPreVol << "," << pInvertedLsmStatVar[nLoop].fPitVol
				<< "," << pInvertedLsmStatVar[nLoop].fVaratio << "," << pInvertedLsmStatVar[nLoop].iPitArea << ","
				<< pInvertedLsmStatVar[nLoop].iDrainsTo << "," << pInvertedLsmStatVar[nLoop].iNextPit << ","
				<< pInvertedLsmStatVar[nLoop].iBecomes << "," << removed << "," << pInvertedLsmStatVar[nLoop].iInRow << ","
				<< pInvertedLsmStatVar[nLoop].iInColumn << "," << pInvertedLsmStatVar[nLoop].iInRec << "," << pInvertedLsmStatVar[nLoop].fInElev << "," <<
				pInvertedLsmStatVar[nLoop].iOutRow << "," << pInvertedLsmStatVar[nLoop].iOutColumn << "," << pInvertedLsmStatVar[nLoop].iOutRec
				<< "," << pInvertedLsmStatVar[nLoop].fOutElev << "," << pInvertedLsmStatVar[nLoop].iStage << "," << visited << "\n";
		}
		out.close();

		filePathForReplace = "\"iPitPath\":null";
		while (sTemp.find("\\") != std::string::npos) {
			sTemp.replace(sTemp.find("\\"), 1, "slash");
		}
		while (sTemp.find("slash") != std::string::npos) {
			sTemp.replace(sTemp.find("slash"), 5, "\\\\");
		}
		resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"iPitPath\":\"" + sTemp + "\"");

		delete[] piDEMSort;
		delete pDEMProcess;

		delete[] sCStringTemp;
		delete[] pInvertedPArea;
		delete[] pInvertedElev;
		delete[] pInvertedFlow;
		delete[] pInvertedDrec;
		delete[] pInvertedShedNo;
		delete[] pInvertedShedNow;
		delete[] pInvertedVol2Fld;
		delete[] pInvertedMm2Fld;
		delete[] pInvertedUpSlp;

		delete[] pMissing;
		delete[] pInvertedLsmStatVar;


	}

	//delete[] pChar;
	delete[] pfVol2FlGrid;
	delete[] pfMm2FlGrid;
	delete[] pfPArea;

	printf("FlowMapR processing complete.\n");
}
FlowmapR::~FlowmapR()
{
}