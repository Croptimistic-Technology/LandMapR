/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/** \brief A "condensed LSM" fuzzy land form class
*
* Defines an object of type fuzy land classes according to the third choice (condensed LSM) in FacetMapR
*
*/
#include "../Headers/stdafx.h"
#include "../Headers/clsm.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <map>

using namespace std;


Clsm::Clsm(void)	///< Default constructor
{
}


void Clsm::setLength(int leng)	///< Sets number of grid cells
{
	length = leng;
}


int Clsm::getLength()	///< Get number of grid cells
{
	return length;
}

/** Constructor with a known length of DEM grid cells */
Clsm::Clsm(int leng)
{

	setLength(leng);
	if ((SeqNo = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for SeqNo, exiting ...");
		exit(1);
	}
	if ((Missing = new bool[leng]) == NULL)
	{
		printf("Unable to allocate memory for Missing, exiting ...");
		exit(1);
	}
	if ((MaxClass = new std::string[leng]) == NULL)
	{
		printf("Unable to allocate memory for MaxClass, exiting ...");
		exit(1);
	}
	if ((MaxCode = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for MaxCode, exiting ...");
		exit(1);
	}
	if ((MaxValue = new int[leng]) == NULL)
	{
		printf("Unable to allocate memory for MaxValue, exiting ...");
		exit(1);
	}

	prefix = "";
}

/** Writes clsm object members to *.csv file */
bool Clsm::writeCSV()
{
	std::string fname = prefix + "_Condensed_LSM_fuzzy_classes.csv";
	int i, leng = getLength();
	std::ofstream out;
	out.open(fname, std::ofstream::trunc);
	out << "SeqNo, Missing, MaxClass, MaxCode, MaxValue\n";

	for (i = 0; i < leng; i++) {
		out << SeqNo[i] << "," << Missing[i] << "," << MaxClass[i] << "," << MaxCode[i] << "," << MaxValue[i] << "\n";
	}
	out.close();

	return true;
}

/** Writes clsm object members to *.csv map file */
bool Clsm::writeCSVMap(string& resultJsonPath, long ncols, long nrows, double xll, double yll, double gridSize)
{
	std::string fname = prefix + "_Condensed_LSM_fuzzy_classes.csv";
	int i, leng = getLength();
	double easting, northing;
	string missing;
	/*std::ofstream out;
	out.open(fname, std::ofstream::trunc);
	out << "easting, northing, SeqNo, Missing, MaxClass, MaxCode, MaxValue, ID No.\n";*/
	FILE* fpclsm;
	const char* fileclsm = fname.c_str();
	fpclsm = fopen(fileclsm, "w+");
	fprintf(fpclsm, "easting, northing, SeqNo, Missing, MaxClass, MaxCode, MaxValue, ID No., 4-landforms code\n");
	string ID_No = "";
	string landforms_4_code = "";
	for (i = 0; i < leng; i++) {
		easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
		northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
		//out.precision(12);
		if (MaxClass[i] == "NA")
		{
			ID_No = "INVALID";
			landforms_4_code = "INVALID";
		}
		if (MaxClass[i] == "LCR" || MaxClass[i] == "DSH" || MaxClass[i] == "UDE")
		{
			ID_No = "1";
			landforms_4_code = "Upper Slopes";
		}
		if (MaxClass[i] == "BSL" || MaxClass[i] == "DBS" || MaxClass[i] == "CBS" || MaxClass[i] == "TER" || MaxClass[i] == "SAD" || MaxClass[i] == "MDE")
		{
			ID_No = "2";
			landforms_4_code = "Mid Slopes";
		}
		if (MaxClass[i] == "FSL" || MaxClass[i] == "TSL" || MaxClass[i] == "FAN" || MaxClass[i] == "LSM")
		{
			ID_No = "3";
			landforms_4_code = "Lower Slopes";
		}
		if (MaxClass[i] == "LLS" || MaxClass[i] == "DEP")
		{
			ID_No = "4";
			landforms_4_code = "Depression";
		}

		if (Missing[i] == false)
		{
			missing = "false";
		}
		else {
			missing = "true";
		}
		//out << easting << "," << northing << "," << SeqNo[i] << "," << Missing[i] << "," << MaxClass[i] << "," << MaxCode[i] << "," << MaxValue[i] << "," << ID_No << "\n";
		fprintf(fpclsm, "%f,%f,%d,%s,%s,%d,%d,%s,%s\n", easting, northing, SeqNo[i], missing.c_str(), MaxClass[i].c_str(), MaxCode[i], MaxValue[i], ID_No.c_str(), landforms_4_code.c_str());
	}
	fclose(fpclsm);
	//out.close();

	string filePathForReplace = "\"condensedLSMFuzzyClassesPath\":null";
	while (fname.find("\\") != std::string::npos) {
		fname.replace(fname.find("\\"), 1, "slash");
	}
	while (fname.find("slash") != std::string::npos) {
		fname.replace(fname.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"condensedLSMFuzzyClassesPath\":\"" + fname + "\"");

	return true;
}


/** Writes map data to a *csv file, it omits grid cells outside the boundary of region of interest
* Depending on the value of cls4OR15, it writes 4-, 15-, or both 4- and 15-classes to map file
*/
bool Clsm::writeCSVmap(long ncols, long nrows, double xll, double yll, double gridSize, int cls4OR15)
{
	std::string fname = prefix + "_fuzzy_land_class_map_" + to_string(cls4OR15) + "_classes.csv", lmclass;
	int i, leng = getLength();

	map<string, int> maxClassNum = { {"LCR", 1}, {"DSH",2}, {"UDE",3}, {"BSL", 4}, {"DBS", 5}, {"CBS", 6}, {"TER", 7}, {"SAD", 8}, {"MDE", 9}, {"FSL", 10}, {"TSL", 11}, {"FAN", 12}, {"LSM",13}, {"LLS",14}, {"DEP",15} };
	map<string, int> fourClassNum = { {"UPPERSLOPE", 1}, {"MIDSLOPE", 2}, {"LOWERSLOPE", 3}, {"DEPRESSION", 4} };

	double tempx = 0.0, tempy = 0.0, easting = 0.0, northing = 0.0;
	std::ofstream out;
	out.open(fname, std::ofstream::trunc);

	switch (cls4OR15)
	{
	case 4:
	{
		out << "Easting" << ',' << "Northing" << ',' << "LandForm4" << ',' << "LandForm4-Index" << '\n';
		lmclass.clear();
		for (i = 0; i < leng; i++) {
			if (((bool)Missing[i]) != true) {
				easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
				northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
				if (MaxClass[i] == "LCR" || MaxClass[i] == "DSH" || MaxClass[i] == "UDE")
				{
					lmclass = "UPPERSLOPE";
				}
				else if (MaxClass[i] == "BSL" || MaxClass[i] == "DBS" || MaxClass[i] == "CBS" || MaxClass[i] == "TER" || MaxClass[i] == "SAD" || MaxClass[i] == "MDE")
				{
					lmclass = "MIDSLOPE";
				}
				else if (MaxClass[i] == "FSL" || MaxClass[i] == "TSL" || MaxClass[i] == "FAN" || MaxClass[i] == "LSM")
				{
					lmclass = "LOWERSLOPE";
				}
				else if (MaxClass[i] == "LLS" || MaxClass[i] == "DEP")
				{
					lmclass = "DEPRESSION";
				}
				else
				{
					lmclass = "INVALID";
				}
				out.precision(12);
				out << easting << "," << northing << "," << lmclass << "," << fourClassNum[lmclass] << "\n";
			}
		}
	}
	break;

	case 15:
	{
		out << "Easting" << ',' << "Northing" << ',' << "LandForm15" << ',' << "LandForm15-Index" << '\n';
		for (i = 0; i < leng; i++) {
			if (((bool)Missing[i]) != true) {
				easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
				northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
				out.precision(12);
				out << easting << "," << northing << "," << MaxClass[i] << ',' << maxClassNum[MaxClass[i]] << "\n";
			}
		}
	}
	break;

	case 19:
	{
		out << "Easting" << ',' << "Northing" << ',' << "LandForm15" << ',' << "LandForm15-Index" << ',' << "LandForm4" << ',' << "LandForm4-Index" << '\n';
		lmclass.clear();
		for (i = 0; i < leng; i++) {
			if (((bool)Missing[i]) != true) {
				easting = xll + ((SeqNo[i] - 1) % ncols)*gridSize;
				northing = yll + (nrows - (SeqNo[i] - 1) / ncols)*gridSize;
				if (MaxClass[i] == "LCR" || MaxClass[i] == "DSH" || MaxClass[i] == "UDE")
				{
					lmclass = "UPPERSLOPE";
				}
				else if (MaxClass[i] == "BSL" || MaxClass[i] == "DBS" || MaxClass[i] == "CBS" || MaxClass[i] == "TER" || MaxClass[i] == "SAD" || MaxClass[i] == "MDE")
				{
					lmclass = "MIDSLOPE";
				}
				else if (MaxClass[i] == "FSL" || MaxClass[i] == "TSL" || MaxClass[i] == "FAN" || MaxClass[i] == "LSM")
				{
					lmclass = "LOWERSLOPE";
				}

				else if (MaxClass[i] == "LLS" || MaxClass[i] == "DEP")
				{
					lmclass = "DEPRESSION";
				}
				else
				{
					lmclass = "INVALID";
				}
				out.precision(12);
				out << easting << "," << northing << "," << MaxClass[i] << "," << maxClassNum[MaxClass[i]] << "," << lmclass << "," << fourClassNum[lmclass] << "\n";
			}
		}
	}
	break;

	default:
	{
		printf("Invalid number of fuzzy land form classes, exiting ...");
		exit(1);
	}
	break;

	}
	out.close();
	return true;
}


Clsm::~Clsm(void) ///< Destructor
{
	delete[] SeqNo;
	delete[] Missing;
	delete[] MaxClass;
	delete[] MaxCode;
	delete[] MaxValue;
}
