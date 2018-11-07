/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// CodeFlowmapRTest.cpp : Defines the entry point for the console application.
//

#include "../Headers/stdafx.h"
#include "../Headers/flowmapR.h"
#include <string>
#include "../Headers/dem.h"
#include "../Headers/pond.h"
#include "../Headers/idem.h"
#include "../Headers/form.h"
#include "../Headers/relz.h"
#include "../Headers/RelzExtraForLen.h"
#include "../Headers/formprocess.h"
#include "../Headers/facetprocess.h"
#include <iostream>    
#include <stdio.h>
#include <direct.h>

char* landmapRProcess2(char* workingDirectory, char* demDataStr, double xllcorner, double yllcorner, char* outPrefix, int nrows, int ncolumns, double GridSize, double MissingDataValue, long PitArea, double PitDepth, bool pbInvertedElev, double dDSArea, double dUpslopeArea, char* aruleStr, char* cruleStr);

int main()
{

	printf("Starting landmapRProcess001");
	char* cwd = _getcwd(0, 0); // get the current working directory for C++ program
	std::string working_directory(cwd);
	std::free(cwd);
#pragma region inputParams 
	//std::string workingDirectory = "C:\\Users\\sliu\\Desktop\\testlandmapRculown"; //sample local directory for output
	std::string workingDirectory = working_directory.substr(0, working_directory.find_last_of("/\\"));
	workingDirectory = workingDirectory.substr(0, workingDirectory.find_last_of("/\\")) + "\\" + "Output";
	std::string outPrefix = "";
	int nrows = 165;
	int ncolumns = 235;
	double xllcorner = 231323.763;
	double yllcorner = 4356234.603;
	double GridSize = 5;
	printf("Starting landmapRProcess002");
	double MissingDataValue = -9999.0000;
	long PitArea = 10;
	double PitDepth = 0.15;
	double dDSArea = 300;
	double dUpSlopeArea = 300;
	bool pbInvertedElev = true;
	//string demFile = "C:\\Users\\sliu\\Desktop\\testlandmapRculown\\DA_elev.csv"; //target file for dem input
	string demFile = workingDirectory.substr(0, workingDirectory.find_last_of("/\\")) + "\\Input\\DA_elev.csv"; //target file for dem input
	printf("Starting landmapRProcess003");
	//Default Arule Json String
	char* aruleJsonStr = _strdup("[{\"SortOrder\":1,\"FileIn\":\"formfile\",\"AttrIn\":\"PROF\",\"ClassOut\":\"CONVEX_D\",\"ModelNo\":4,\"B\":5.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":2.5,\"B2\":0.0,\"D\":2.5},{\"SortOrder\":2,\"FileIn\":\"formfile\",\"AttrIn\":\"PROF\",\"ClassOut\":\"CONCAVE_D\",\"ModelNo\":5,\"B\":-5.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":-2.5,\"D\":2.5},{\"SortOrder\":3,\"FileIn\":\"formfile\",\"AttrIn\":\"PROF\",\"ClassOut\":\"PLANAR_D\",\"ModelNo\":1,\"B\":0.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":-2.5,\"B2\":2.5,\"D\":2.5},{\"SortOrder\":4,\"FileIn\":\"formfile\",\"AttrIn\":\"PLAN\",\"ClassOut\":\"CONVEX_A\",\"ModelNo\":4,\"B\":5.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":2.5,\"B2\":0.0,\"D\":2.5},{\"SortOrder\":5,\"FileIn\":\"formfile\",\"AttrIn\":\"PLAN\",\"ClassOut\":\"CONCAVE_A\",\"ModelNo\":5,\"B\":-5.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":-2.5,\"D\":2.5},{\"SortOrder\":6,\"FileIn\":\"formfile\",\"AttrIn\":\"PLAN\",\"ClassOut\":\"PLANAR_A\",\"ModelNo\":1,\"B\":0.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":-2.5,\"B2\":2.5,\"D\":2.5},{\"SortOrder\":7,\"FileIn\":\"formfile\",\"AttrIn\":\"QWETI\",\"ClassOut\":\"HIGH_WI\",\"ModelNo\":4,\"B\":7.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":3.5,\"B2\":0.0,\"D\":3.0},{\"SortOrder\":8,\"FileIn\":\"formfile\",\"AttrIn\":\"QWETI\",\"ClassOut\":\"LOW_WI\",\"ModelNo\":5,\"B\":0.5,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":3.5,\"D\":3.0},{\"SortOrder\":9,\"FileIn\":\"formfile\",\"AttrIn\":\"SLOPE\",\"ClassOut\":\"NEAR_LEVEL\",\"ModelNo\":5,\"B\":0.5,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":1.0,\"D\":0.5},{\"SortOrder\":10,\"FileIn\":\"formfile\",\"AttrIn\":\"SLOPE\",\"ClassOut\":\"REL_STEEP\",\"ModelNo\":4,\"B\":2.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":1.0,\"B2\":0.0,\"D\":1.0},{\"SortOrder\":11,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2ST\",\"ClassOut\":\"NEAR_DIV\",\"ModelNo\":4,\"B\":90.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":75.0,\"B2\":0.0,\"D\":15.0},{\"SortOrder\":12,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2ST\",\"ClassOut\":\"NEAR_HALF\",\"ModelNo\":1,\"B\":50.0,\"BLow\":50.0,\"BHi\":50.0,\"B1\":25.0,\"B2\":75.0,\"D\":25.0},{\"SortOrder\":13,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2ST\",\"ClassOut\":\"NEAR_CHAN\",\"ModelNo\":5,\"B\":10.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":25.0,\"D\":15.0},{\"SortOrder\":14,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2PIT\",\"ClassOut\":\"NEAR_PEAK\",\"ModelNo\":4,\"B\":90.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":75.0,\"B2\":0.0,\"D\":15.0},{\"SortOrder\":15,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2PIT\",\"ClassOut\":\"NEAR_MID\",\"ModelNo\":1,\"B\":50.0,\"BLow\":50.0,\"BHi\":50.0,\"B1\":25.0,\"B2\":75.0,\"D\":25.0},{\"SortOrder\":16,\"FileIn\":\"relzfile\",\"AttrIn\":\"PCTZ2PIT\",\"ClassOut\":\"NEAR_PIT\",\"ModelNo\":5,\"B\":5.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":0.0,\"B2\":10.0,\"D\":5.0},{\"SortOrder\":17,\"FileIn\":\"relzfile\",\"AttrIn\":\"Z2PIT\",\"ClassOut\":\"HI_ABOVE\",\"ModelNo\":4,\"B\":2.0,\"BLow\":0.0,\"BHi\":0.0,\"B1\":1.0,\"B2\":0.0,\"D\":1.0}]");
	//Default Crule Json String
	char* cruleJsonStr = _strdup("[{\"F_Name\":\"LCR\",\"FuzAttr\":\"NEAR_PEAK\",\"AttrWT\":30,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"NEAR_DIV\",\"AttrWT\":20,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":10,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":20,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":10,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":5,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"LCR\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":5,\"Facet_No\":11,\"F_Code\":1},{\"F_Name\":\"DSH\",\"FuzAttr\":\"NEAR_PEAK\",\"AttrWT\":30,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"DSH\",\"FuzAttr\":\"NEAR_DIV\",\"AttrWT\":20,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"DSH\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":10,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"DSH\",\"FuzAttr\":\"CONVEX_D\",\"AttrWT\":20,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"DSH\",\"FuzAttr\":\"CONVEX_A\",\"AttrWT\":10,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"DSH\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":10,\"Facet_No\":12,\"F_Code\":2},{\"F_Name\":\"UDE\",\"FuzAttr\":\"NEAR_PEAK\",\"AttrWT\":30,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"NEAR_DIV\",\"AttrWT\":20,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":10,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":10,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":10,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":10,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"UDE\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":10,\"Facet_No\":13,\"F_Code\":3},{\"F_Name\":\"BSL\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":20,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":15,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":25,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"BSL\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":5,\"Facet_No\":21,\"F_Code\":4},{\"F_Name\":\"DBS\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":20,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"CONVEX_A\",\"AttrWT\":20,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":15,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"DBS\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":10,\"Facet_No\":22,\"F_Code\":5},{\"F_Name\":\"CBS\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":20,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"CONCAVE_A\",\"AttrWT\":20,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":15,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"CBS\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":10,\"Facet_No\":23,\"F_Code\":6},{\"F_Name\":\"TER\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"TER\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"TER\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"TER\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":30,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"TER\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":15,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"TER\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":20,\"Facet_No\":24,\"F_Code\":7},{\"F_Name\":\"SAD\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"SAD\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"SAD\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"SAD\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":20,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"SAD\",\"FuzAttr\":\"CONCAVE_D\",\"AttrWT\":20,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"SAD\",\"FuzAttr\":\"CONVEX_A\",\"AttrWT\":20,\"Facet_No\":25,\"F_Code\":8},{\"F_Name\":\"MDE\",\"FuzAttr\":\"NEAR_HALF\",\"AttrWT\":20,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"NEAR_MID\",\"AttrWT\":10,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"HI_ABOVE\",\"AttrWT\":5,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":25,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"CONCAVE_D\",\"AttrWT\":10,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"CONCAVE_A\",\"AttrWT\":10,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"MDE\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":20,\"Facet_No\":26,\"F_Code\":9},{\"F_Name\":\"FSL\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":10,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":10,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"CONCAVE_D\",\"AttrWT\":20,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"CONCAVE_A\",\"AttrWT\":20,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":10,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"FSL\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":20,\"Facet_No\":31,\"F_Code\":10},{\"F_Name\":\"TSL\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"TSL\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":10,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"TSL\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":10,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"TSL\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":25,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"TSL\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":25,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"TSL\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":10,\"Facet_No\":32,\"F_Code\":11},{\"F_Name\":\"FAN\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"FAN\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":10,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"FAN\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":10,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"FAN\",\"FuzAttr\":\"CONVEX_A\",\"AttrWT\":25,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"FAN\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":25,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"FAN\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":10,\"Facet_No\":33,\"F_Code\":12},{\"F_Name\":\"LSM\",\"FuzAttr\":\"NEAR_DIV\",\"AttrWT\":10,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":10,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"NEAR_PEAK\",\"AttrWT\":10,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"REL_STEEP\",\"AttrWT\":10,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"CONVEX_D\",\"AttrWT\":15,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"CONVEX_A\",\"AttrWT\":15,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LSM\",\"FuzAttr\":\"LOW_WI\",\"AttrWT\":10,\"Facet_No\":41,\"F_Code\":13},{\"F_Name\":\"LLS\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"LLS\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":20,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"LLS\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":40,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"LLS\",\"FuzAttr\":\"PLANAR_D\",\"AttrWT\":5,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"LLS\",\"FuzAttr\":\"PLANAR_A\",\"AttrWT\":5,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"LLS\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":10,\"Facet_No\":42,\"F_Code\":14},{\"F_Name\":\"DEP\",\"FuzAttr\":\"NEAR_CHAN\",\"AttrWT\":20,\"Facet_No\":43,\"F_Code\":15},{\"F_Name\":\"DEP\",\"FuzAttr\":\"NEAR_PIT\",\"AttrWT\":30,\"Facet_No\":43,\"F_Code\":15},{\"F_Name\":\"DEP\",\"FuzAttr\":\"NEAR_LEVEL\",\"AttrWT\":20,\"Facet_No\":43,\"F_Code\":15},{\"F_Name\":\"DEP\",\"FuzAttr\":\"CONCAVE_A\",\"AttrWT\":10,\"Facet_No\":43,\"F_Code\":15},{\"F_Name\":\"DEP\",\"FuzAttr\":\"CONCAVE_D\",\"AttrWT\":10,\"Facet_No\":43,\"F_Code\":15},{\"F_Name\":\"DEP\",\"FuzAttr\":\"HIGH_WI\",\"AttrWT\":10,\"Facet_No\":43,\"F_Code\":15}]");
#pragma endregion
	printf("Starting landmapRProcess");

	char* workingDirchr = _strdup(workingDirectory.c_str());
	char* outPrefixchr = _strdup(outPrefix.c_str());
	//string fuzzyAttributeFilechr = _strdup((workingDirectory + "lm3arule.csv").c_str());
	//string fuzzyFacetsFilechr = _strdup((workingDirectory + "lm3crule.csv").c_str());

	// read the dem data from the user speficied file and separate it by comma and assign it to the demDatastr variable
	string demDataInputStr = "";
	string elevline;
	std::ifstream demfilestr;
	demfilestr.open(demFile, ios::in);
	if (!demfilestr.is_open())
	{
		printf("Parameter file not found, exiting ...");
		exit(1);
	}
	int totalpoints = 0;
	getline(demfilestr, elevline);
	while (getline(demfilestr, elevline))
	{
		demDataInputStr = demDataInputStr + elevline + ",";
		totalpoints = totalpoints + 1;
	}
	demDataInputStr = demDataInputStr.substr(0, demDataInputStr.length() - 1);
	demfilestr.close();
	char* demDatastr = _strdup(demDataInputStr.c_str());

	//start running the all-in-one landmapR process
	bool successful = landmapRProcess(workingDirchr, demDatastr, xllcorner, yllcorner, outPrefixchr, nrows, ncolumns, GridSize, MissingDataValue, PitArea, PitDepth, pbInvertedElev, dDSArea, dUpSlopeArea, aruleJsonStr, cruleJsonStr);


	return 0;
}




char* landmapRProcess(char* workingDir, char* demDataStr, double xllcorner, double yllcorner, char* outPrefix, int nrows, int ncolumns, double GridSize, double MissingDataValue, long PitArea, double PitDepth, bool pbInvertedElev, double dDSArea, double dUpslopeArea, char* aruleStr, char* cruleStr)
{
	printf("Starting landmapRProcess20\n");
	bool success = false;
	string resultJsonFilePath = "{\"success\":false,\"errorMessage\":null,\"condensedLSMFuzzyClassesPath\":null,\"demPath\":null,\"fillPath\":null,\"formAllPath\":null,\"iDemPath\":null,\"iPitPath\":null,\"lenPath\":null,\"moldPath\":null,\"pitPath\":null,\"pondPath\":null,\"relzAllPath\":null,\"voldPath\":null}";
	printf("Starting landmapRProcess30\n");
	try {
		long size = nrows * ncolumns;
		double* pfDEMGrid;
		if ((pfDEMGrid = new double[size]) == NULL)
		{
			printf("No elevation data to process, exiting ...\n");
			exit(1);
		}
		printf("Starting landmapRProcess40\n");

		char* pch;
		pch = strtok(demDataStr, ",");
		pfDEMGrid[0] = atof(pch);
		int countPointNum = 1;
		while (pch != NULL)
		{
			pch = strtok(NULL, ",");
			if (pch != NULL)
			{
				pfDEMGrid[countPointNum] = atof(pch);
				countPointNum++;
			}
		}
		printf("Starting landmapRProcess50\n");
		OutputDebugStringA("Starting landmapRProcess50.");

		cout << countPointNum << "\n";
		printf("Starting landmapRProcess60\n");
		DEM dem = DEM(size);
		Pond pond = Pond(size);
		IDEM idem = IDEM(size);
		string workingDirectory(workingDir);
		workingDirectory = workingDirectory + "\\";
		string name_Prefix(outPrefix);
		OutputDebugStringA("Starting landmapRProcess60.\n");
		//FlowMapR Process
		OutputDebugStringA("Starting landmapRProcess61.\n");
		FlowmapR *flowmapR = new FlowmapR();
		OutputDebugStringA("Starting landmapRProcess62.\n");
		flowmapR->runProcess(resultJsonFilePath, workingDirectory, pfDEMGrid, xllcorner, yllcorner, outPrefix, nrows, ncolumns, GridSize, MissingDataValue, PitArea, PitDepth, pbInvertedElev, &dem, &pond, &idem);
		OutputDebugStringA("Starting landmapRProcess63.\n");
		delete[] pfDEMGrid;
		OutputDebugStringA("Starting landmapRProcess64.\n");
		delete flowmapR;
		OutputDebugStringA("Starting landmapRProcess70.\n");

		//FormMapR Process
		Form form = Form(size);
		Relz relz = Relz(size);
		//RelzExtraForLen relzExtra = RelzExtraForLen(size);
		//Len len = Len(size);
		OutputDebugStringA("Starting landmapRProcess80.");
		FormProcess* formProcess = new FormProcess();
		formProcess->dDSArea = dDSArea;
		formProcess->dGridSize = GridSize;  //We should use the same GridSize as flowMapR
		formProcess->dUpslopeArea = dUpslopeArea;
		OutputDebugStringA("Starting landmapRProcess90.");
		RelzExtraForLen relzExtra = RelzExtraForLen(size);
		Len len = Len(size);

		string workingDirectoryOutput = workingDirectory + name_Prefix;
		OutputDebugStringA("Starting landmapRProcess100.\n");
		formProcess->fprocess3(resultJsonFilePath, &dem, &pond, &relz, &idem, &form, &len, &relzExtra, workingDirectoryOutput, nrows, ncolumns, xllcorner, yllcorner);
		OutputDebugStringA("Starting landmapRProcess110.\n");
		//fuzzyAttributeFile = workingDirectory + fuzzyAttributeFile;
		//fuzzyFacetsFile = workingDirectory + fuzzyFacetsFile;
		//FacetMapR
		//Arule arule(fuzzyAttributeFile, &form, &relz);
		Arule arule(aruleStr, &form, &relz);
		//Crule crule(fuzzyFacetsFile, arule.getLength());
		//Crule crule(fuzzyFacetsFile, 30);
		OutputDebugStringA("Starting landmapRProcess115.\n");
		Crule crule(cruleStr, 30);
		OutputDebugStringA("Starting landmapRProcess120.\n");
		FacetProcess* facetprocess = new FacetProcess();
		printf("Calculating fuzzy condensed LSM fuzzy classes.\n");
		OutputDebugStringA("Calculating fuzzy condensed LSM fuzzy classes.\n");
		Clsm *lsm = facetprocess->calc_lsm(&dem, &form, &relz, &arule, &crule);
		OutputDebugStringA("Starting landmapRProcess140.\n");
		lsm->prefix = workingDirectoryOutput;
		//lsm->writeCSV();
		lsm->writeCSVMap(resultJsonFilePath, ncolumns, nrows, xllcorner, yllcorner, GridSize);


		delete facetprocess;
		printf("LandmapR process complete. \n");

		OutputDebugStringA("LandmapR process complete.\n");
		success = true;
	}
	catch (exception e)
	{
		success = false;
		string errorMessageForReplace = "\"errorMessage\":null";
		string _exception = e.what();
		resultJsonFilePath.replace(resultJsonFilePath.find(errorMessageForReplace), errorMessageForReplace.length(), "\"errorMessage\":\"" + _exception + "\"");
	}

	if (success == true)
	{
		string successMessageForReplace = "\"success\":false";
		string successful = "true";
		resultJsonFilePath.replace(resultJsonFilePath.find(successMessageForReplace), successMessageForReplace.length(), "\"success\":" + successful);
	}

	//return success;
	char* message_returned = _strdup(resultJsonFilePath.c_str());
	return message_returned;
}


