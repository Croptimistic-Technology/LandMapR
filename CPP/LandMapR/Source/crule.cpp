/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
*\brief A class to read fuzzy classification rules file residing in the same directory as the executable (the working directory), and parse attribute weights as given in the classification rules.
*
* The classification rules file (i.e., *crule.csv) is organized in such a way that the first line holds the column headings, while the last line is a blank line (i.e., extra carriage return at the end of the data). The last entry on every line is
* ended with a carriage return, and no extra comma after the last entry on each row.
*/
#include "../Headers/stdafx.h"
#include "../Headers/crule.h"


/**
* The default constructor
*/
Crule::Crule()
{
}

Crule::Crule(char* cruleJsonStr, int dim)
{
	string jsonStrArray[100];
	int count_Crules = 1;//use this for the loop of assigning the value of a string using a specific index
	char* pch;
	pch = strtok(cruleJsonStr, "}");
	string temp_str(pch);
	jsonStrArray[0] = temp_str;
	while (pch != NULL)
	{
		pch = strtok(NULL, "}");
		if (pch != NULL)
		{
			jsonStrArray[count_Crules] = temp_str.assign(pch);
			jsonStrArray[count_Crules] = jsonStrArray[count_Crules].substr(1);
			count_Crules++;
		}
	}
	count_Crules = count_Crules - 1;

	psf_name = new std::string[dim]; //The count of non-null values for this array should be the number of types of facets
	pif_code = new int[dim];  //The count of non-null values for this array should be the number of types of facets	
	attr_wt = new double[dim * dim];	//The count of not-null values this array should be the number of facets multiply by the number of the fuzzy facet class

	setLength(count_Crules);
	psfuzzattr = new std::string[dim * dim];	//The count of not-null values this array should be the number of facets multiply by the number of the fuzzy facet class
	int *pcode = new int[count_Crules];
	std::string *pfname = new std::string[count_Crules];
	int *total_wt = new int[dim];
	pifacet_no = new int[count_Crules];
	int *pattrwt = new int[count_Crules];
	std::string *pfuzzattr = new std::string[count_Crules];

	string* crulesitems = new string[5];
	int pos = 0;
	string val;
	for (int cruleRowNum = 0; cruleRowNum < count_Crules; cruleRowNum++)
	{
		stringstream crulestream(jsonStrArray[cruleRowNum]);
		int i = 0;
		while (getline(crulestream, crulesitems[i], ','))
		{
			pos = crulesitems[i].find(":") + 1;
			val = crulesitems[i].substr(pos);
			if (i == 0 || i == 1)
			{
				val = val.substr(1, val.length() - 2);
			}
			switch (i)
			{
			case 0: pfname[cruleRowNum] = val; break;
			case 1:	pfuzzattr[cruleRowNum] = val; break;
			case 2:	pattrwt[cruleRowNum] = stoi(val); break;
			case 3:	pifacet_no[cruleRowNum] = stoi(val); break;
			case 4:	pcode[cruleRowNum] = stoi(val); break;
			}
			i++;
		}
		cout << pfname[cruleRowNum] << "," << pfuzzattr[cruleRowNum] << "," << pattrwt[cruleRowNum] << "," << pifacet_no[cruleRowNum] << "," << pcode[cruleRowNum] << "\n";
	}
	delete[] crulesitems;
	int nColLoc = 0;
	double *pSortValue = new double[count_Crules];
	int *pSortedIndex = new int[count_Crules];
	no_attrib = new int[dim];
	int nLoop = 0;

	for (nLoop = 0; nLoop < count_Crules; nLoop++)
	{
		pSortValue[nLoop] = pifacet_no[nLoop] * 100 + (99 - pattrwt[nLoop]);
		pSortedIndex[nLoop] = nLoop;
	}
	QuickSort(pSortValue, count_Crules, pSortedIndex);

	delete[] pSortValue;

	no_facets = 0;
	int facet = 0;

	int attribno = 0;
	std::string sString;
	std::string sFacetName;
	bool bExit = false;
	for (nLoop = 0; nLoop < count_Crules; nLoop++)
	{
		no_facets += 1;
		no_attrib[facet] = 0;
		sString = pfname[pSortedIndex[nLoop]];
		sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
		std::transform(sString.begin(), sString.end(), sString.begin(), toupper);
		psf_name[facet] = sString;
		pif_code[facet] = pcode[pSortedIndex[nLoop]];

		total_wt[facet] = 0;
		attribno = 0;
		sFacetName = sString;
		bExit = false;
		while (_stricmp(sFacetName.c_str(), psf_name[facet].c_str()) == 0 && bExit == false)
		{
			no_attrib[facet]++;
			sString = pfuzzattr[pSortedIndex[nLoop]];
			sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
			std::transform(sString.begin(), sString.end(), sString.begin(), toupper);

			psfuzzattr[facet * dim + attribno] = sString;
			attr_wt[facet * dim + attribno] = pattrwt[pSortedIndex[nLoop]];

			total_wt[facet] = total_wt[facet] + pattrwt[pSortedIndex[nLoop]];

			if (nLoop < count_Crules - 1)
			{
				nLoop++;
				sFacetName = pfname[pSortedIndex[nLoop]];
				sFacetName.erase(remove(sFacetName.begin(), sFacetName.end(), ' '), sFacetName.end());
			}
			else
			{
				bExit = true;
			}
			attribno++;
		}
		if (bExit == false)
		{
			nLoop--;
		}
		facet++;
	}

	int nLoop2 = 0;
	for (nLoop = 0; nLoop < no_facets; nLoop++)
	{
		for (nLoop2 = 0; nLoop2 < no_attrib[nLoop]; nLoop2++)
		{
			attr_wt[nLoop * dim + nLoop2] = attr_wt[nLoop * dim + nLoop2] / total_wt[nLoop];
		}
	}

	delete[] pSortedIndex;
	delete[] pfname;
	delete[] pcode;
	delete[] pfuzzattr;
}



/**  \brief Constructor with known number of rows and columns of data
*
* @param leng the number of classification rules in the file (i.e., rows of classification rules)
* @param dim the number of distinct attributes in the rules file
*/

Crule::Crule(int leng, int dim)
{

	setLength(leng);
	numCols = 5;
	psf_name = new std::string[leng];
	pifacet_no = new int[leng];
	pif_code = new int[leng];
	attr_wt = new double[dim * dim];
	psfuzzattr = new std::string[dim * dim];
}

int Crule::getLength() //!get number of 
{

	return length;
}
void Crule::setLength(int leng)
{

	length = leng;
}

Crule::Crule(string csvname, int dim)
{

	int lines = 0, leng = 0, i = 0, j = 0, _commas = 0;
	std::string csvread, csvread2, val;
	ifstream incrule;
	incrule.open(csvname, ios::in);
	if (!incrule) {
		printf("Rules file could not be read, exiting ....");
		exit(1);
	}

	while (!incrule.ios::eof())
	{
		getline(incrule, csvread, '\n');
		++lines;
	}

	leng = lines - 2; //first line is the heading, and the the last carriage return is a blank line
	setLength(leng);
	incrule.close();
	psf_name = new std::string[dim]; //The count of non-null values for this array should be the number of types of facets
	pif_code = new int[dim];  //The count of non-null values for this array should be the number of types of facets	
	attr_wt = new double[dim * dim];	//The count of not-null values this array should be the number of facets multiply by the number of the fuzzy facet class

	psfuzzattr = new std::string[dim * dim];	//The count of not-null values this array should be the number of facets multiply by the number of the fuzzy facet class
	int *pcode = new int[leng];
	std::string *pfname = new std::string[leng];
	int *total_wt = new int[dim];
	pifacet_no = new int[leng];
	int *pattrwt = new int[leng];
	std::string *pfuzzattr = new std::string[leng];

	incrule.open(csvname, ios::in);
	getline(incrule, csvread, '\n');	//read the headings line and ignore

	for (std::string::iterator it = csvread.begin(); it != csvread.end();)
	{
		if ((*it) == ',') ++_commas;
		it++;
	}

	if (_commas != 4)
	{
		printf("Invalid number of columns in rules file, exiting ...");
		exit(1);
	}

	i = 0;
	while (!incrule.eof() && i < leng)
	{
		getline(incrule, csvread, '\n');
		std::stringstream lineStream(csvread);

		getline(lineStream, val, ',');
		pfname[i] = val;

		getline(lineStream, val, ',');
		pfuzzattr[i] = val;

		getline(lineStream, val, ',');
		pattrwt[i] = stoi(val);

		getline(lineStream, val, ',');
		pifacet_no[i] = stoi(val);

		getline(lineStream, val, '\n');
		pcode[i] = stoi(val);
		i++;

		lineStream.clear();
	}

	int nColLoc = 0;
	double *pSortValue = new double[leng];
	int *pSortedIndex = new int[leng];
	no_attrib = new int[dim];
	int nLoop = 0;

	for (nLoop = 0; nLoop < leng; nLoop++)
	{
		pSortValue[nLoop] = pifacet_no[nLoop] * 100 + (99 - pattrwt[nLoop]);
		pSortedIndex[nLoop] = nLoop;
	}
	QuickSort(pSortValue, leng, pSortedIndex);

	delete[] pSortValue;

	no_facets = 0;
	int facet = 0;

	int attribno = 0;
	std::string sString;
	std::string sFacetName;
	bool bExit = false;
	for (nLoop = 0; nLoop < leng; nLoop++)
	{
		no_facets += 1;
		no_attrib[facet] = 0;
		sString = pfname[pSortedIndex[nLoop]];
		sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
		std::transform(sString.begin(), sString.end(), sString.begin(), toupper);
		psf_name[facet] = sString;
		pif_code[facet] = pcode[pSortedIndex[nLoop]];

		total_wt[facet] = 0;
		attribno = 0;
		sFacetName = sString;
		bExit = false;
		while (_stricmp(sFacetName.c_str(), psf_name[facet].c_str()) == 0 && bExit == false)
		{
			no_attrib[facet]++;
			sString = pfuzzattr[pSortedIndex[nLoop]];
			sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
			std::transform(sString.begin(), sString.end(), sString.begin(), toupper);

			psfuzzattr[facet * dim + attribno] = sString;
			attr_wt[facet * dim + attribno] = pattrwt[pSortedIndex[nLoop]];

			total_wt[facet] = total_wt[facet] + pattrwt[pSortedIndex[nLoop]];

			if (nLoop < leng - 1)
			{
				nLoop++;
				sFacetName = pfname[pSortedIndex[nLoop]];
				sFacetName.erase(remove(sFacetName.begin(), sFacetName.end(), ' '), sFacetName.end());
			}
			else
			{
				bExit = true;
			}
			attribno++;
		}
		if (bExit == false)
		{
			nLoop--;
		}
		facet++;
	}

	int nLoop2 = 0;
	for (nLoop = 0; nLoop < no_facets; nLoop++)
	{
		for (nLoop2 = 0; nLoop2 < no_attrib[nLoop]; nLoop2++)
		{
			attr_wt[nLoop * dim + nLoop2] = attr_wt[nLoop * dim + nLoop2] / total_wt[nLoop];
		}
	}

	delete[] pSortedIndex;
	delete[] pfname;
	delete[] pcode;
	delete[] pfuzzattr;

}


Crule::~Crule()
{
	//int leng = getLength();
	delete[] psf_name;
	delete[] pifacet_no;
	delete[] pif_code;
	delete[] attr_wt;
	delete[] psfuzzattr;
}
