/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
\brief Fuzzy rule attributes class
*/
#include "../Headers/stdafx.h"
#include "../Headers/arule.h"
#include <algorithm>
#include <cstring>
#include <iterator>


using namespace std;

Arule::Arule()
{
}

Arule::Arule(int leng)
{
	numCols = 11;
	setLength(leng);
	pisortorder = new int[leng];
	psfile = new  std::string[leng];
	psattr_in = new std::string[leng];
	psattr_out = new std::string[leng];
	pimodel_no = new int[leng];
	pdb = new double[leng];
	pdb_low = new double[leng];
	pdb_hi = new double[leng];
	pdb1 = new double[leng];
	pdb2 = new double[leng];
	pdd = new double[leng];
}

void Arule::setLength(int leng)
{

	length = leng;
}


int Arule::getLength()
{

	return length;
}

Arule::Arule(char* aruleJsonStr, Form *form, Relz *relz)
{
	numCols = 11;
	string jsonStrArray[100];
	int count_Arules = 1;//use this for the loop of assigning the value of a string using a specific index
	char* pch;
	pch = strtok(aruleJsonStr, "}");
	string temp_str(pch);
	jsonStrArray[0] = temp_str;
	while (pch != NULL)
	{
		pch = strtok(NULL, "}");
		if (pch != NULL)
		{
			jsonStrArray[count_Arules] = temp_str.assign(pch);
			jsonStrArray[count_Arules] = jsonStrArray[count_Arules].substr(1);
			count_Arules++;
		}
	}
	count_Arules = count_Arules - 1;
	//items used for arules
	pisortorder = new int[count_Arules];
	psfile = new  std::string[count_Arules];
	psattr_in = new std::string[count_Arules];
	psattr_out = new std::string[count_Arules];
	pimodel_no = new int[count_Arules];
	pdb = new double[count_Arules];
	pdb_low = new double[count_Arules];
	pdb_hi = new double[count_Arules];
	pdb1 = new double[count_Arules];
	pdb2 = new double[count_Arules];
	pdd = new double[count_Arules];

	string* arulesitems = new string[11];
	int pos = 0;
	string val;
	for (int aruleRowNum = 0; aruleRowNum < count_Arules; aruleRowNum++)
	{
		stringstream arulestream(jsonStrArray[aruleRowNum]);
		int i = 0;
		while (getline(arulestream, arulesitems[i], ','))
		{
			pos = arulesitems[i].find(":") + 1;
			val = arulesitems[i].substr(pos);
			if (i == 1 || i == 2 || i == 3)
			{
				val = val.substr(1, val.length() - 2);
			}
			//cout << val << endl;
			switch (i)
			{
			case 0: pisortorder[aruleRowNum] = stoi(val); break;
			case 1:	psfile[aruleRowNum].append(val); break;
			case 2:	psattr_in[aruleRowNum].assign(val); break;
			case 3:	psattr_out[aruleRowNum].assign(val); break;
			case 4:	pimodel_no[aruleRowNum] = stoi(val); break;
			case 5:	pdb[aruleRowNum] = stof(val); break;
			case 6:	pdb_low[aruleRowNum] = stof(val); break;
			case 7:	pdb_hi[aruleRowNum] = stof(val); break;
			case 8:	pdb1[aruleRowNum] = stof(val); break;
			case 9:	pdb2[aruleRowNum] = stof(val); break;
			case 10: pdd[aruleRowNum] = stof(val); break;
			}
			i++;
		}
		cout << pisortorder[aruleRowNum] << "," << psfile[aruleRowNum] << "," << psattr_in[aruleRowNum] << "," << psattr_out[aruleRowNum] << ","
			<< pimodel_no[aruleRowNum] << "," << pdb[aruleRowNum] << "," << pdb_low[aruleRowNum] << "," << pdb_hi[aruleRowNum] << "," << pdb1[aruleRowNum]
			<< "," << pdb2[aruleRowNum] << "," << pdd[aruleRowNum] << "\n";
	}
	setLength(count_Arules);
	delete[] arulesitems;
	int nLoop = 0;
	no_models = 0;
	string sDataType, sString1, sString = " ", curtext = " ";
	infield = new double*[count_Arules];

	//for (int i = 0; i < leng; i++) {
	//	infield[i] = new double[dem->getLength()]; 
	//	//initialize the size of the double pointer
	//	// each i-th pointer is now pointing to dynamic array (size of the number of dem values)
	//}

	int nNumOfFiles = 0;

	for (nLoop = 0; nLoop < count_Arules; nLoop++)
	{
		sString = psfile[nLoop];
		sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
		std::transform(sString.begin(), sString.end(), sString.begin(), toupper);

		if (sString != curtext)
		{
			curtext = sString;
			nNumOfFiles++;
		}
		if (curtext == "FORMFILE")
		{
			no_models++;
			sString1 = psattr_in[nLoop];
			sString1.erase(remove(sString1.begin(), sString1.end(), ' '), sString1.end());
			std::transform(sString1.begin(), sString1.end(), sString1.begin(), toupper);
			infield[nLoop] = (double *)form->returnPointer(form->returnIndex(sString1));
			std::transform(psattr_out[nLoop].begin(), psattr_out[nLoop].end(), psattr_out[nLoop].begin(), toupper);
		}
		else if (curtext == "RELZFILE")
		{
			no_models++;
			sString1 = psattr_in[nLoop];
			sString1.erase(remove(sString1.begin(), sString1.end(), ' '), sString1.end());
			std::transform(sString1.begin(), sString1.end(), sString1.begin(), toupper);
			infield[nLoop] = (double *)relz->returnPointer(relz->returnIndex(sString1));
			std::transform(psattr_out[nLoop].begin(), psattr_out[nLoop].end(), psattr_out[nLoop].begin(), toupper);
		}
	}

}


Arule::Arule(string csvfname, Form *form, Relz *relz)
{

	int line = 0, leng = 0, i = 0, j = 0, strleng = 0, _commas = 0;
	string read, temp = "", val = "";
	numCols = 11;
	ifstream in;
	in.open(csvfname, ios::in);

	if (!in) {
		printf("Attributes file could not be read, exiting ....");
		exit(1);
	}

	while (!in.ios::eof()) {
		getline(in, read, '\n');
		++line;
	}
	leng = line - 2; // first line is the heading, and the last line is blank

	in.close();
	setLength(leng);
	pisortorder = new int[leng];
	psfile = new  std::string[leng];
	psattr_in = new std::string[leng];
	psattr_out = new std::string[leng];
	pimodel_no = new int[leng];
	pdb = new double[leng];
	pdb_low = new double[leng];
	pdb_hi = new double[leng];
	pdb1 = new double[leng];
	pdb2 = new double[leng];
	pdd = new double[leng];

	in.open(csvfname, ios::in);
	getline(in, read, '\n');	//read the headings line and ignore

	for (std::string::iterator it = read.begin(); it != read.end();) {
		if ((*it) == ',') ++_commas;
		it++;
	}

	if (_commas != 10) {
		printf("Invalid number of columns in attributes file, exiting ...");
		exit(1);
	}

	while (!in.eof() && i < leng)
	{
		getline(in, read, '\n');

		std::stringstream lineStream(read);

		getline(lineStream, val, ',');
		pisortorder[i] = stoi(val);
		getline(lineStream, val, ',');
		psfile[i].append(val);
		getline(lineStream, val, ',');
		psattr_in[i].assign(val);
		getline(lineStream, val, ',');
		psattr_out[i].assign(val);
		getline(lineStream, val, ',');
		pimodel_no[i] = stoi(val);
		getline(lineStream, val, ',');
		pdb[i] = stof(val);
		getline(lineStream, val, ',');
		pdb_low[i] = stof(val);
		getline(lineStream, val, ',');
		pdb_hi[i] = stof(val);
		getline(lineStream, val, ',');
		pdb1[i] = stof(val);
		getline(lineStream, val, ',');
		pdb2[i] = stof(val);
		getline(lineStream, val, '\n');
		pdd[i] = stof(val);

		lineStream.clear();
		i++;
	}

	in.close();
	int nLoop = 0;
	no_models = 0;
	string sDataType, sString1, sString = " ", curtext = " ";
	infield = new double*[leng];

	//for (int i = 0; i < leng; i++) {
	//	infield[i] = new double[dem->getLength()]; 
	//	//initialize the size of the double pointer
	//	// each i-th pointer is now pointing to dynamic array (size of the number of dem values)
	//}

	int nNumOfFiles = 0;

	for (nLoop = 0; nLoop < leng; nLoop++)
	{
		sString = psfile[nLoop];
		sString.erase(remove(sString.begin(), sString.end(), ' '), sString.end());
		std::transform(sString.begin(), sString.end(), sString.begin(), toupper);

		if (sString != curtext)
		{
			curtext = sString;
			nNumOfFiles++;
		}
		if (curtext == "FORMFILE")
		{
			no_models++;
			sString1 = psattr_in[nLoop];
			sString1.erase(remove(sString1.begin(), sString1.end(), ' '), sString1.end());
			std::transform(sString1.begin(), sString1.end(), sString1.begin(), toupper);
			infield[nLoop] = (double *)form->returnPointer(form->returnIndex(sString1));
			std::transform(psattr_out[nLoop].begin(), psattr_out[nLoop].end(), psattr_out[nLoop].begin(), toupper);
		}
		else if (curtext == "RELZFILE")
		{
			no_models++;
			sString1 = psattr_in[nLoop];
			sString1.erase(remove(sString1.begin(), sString1.end(), ' '), sString1.end());
			std::transform(sString1.begin(), sString1.end(), sString1.begin(), toupper);
			infield[nLoop] = (double *)relz->returnPointer(relz->returnIndex(sString1));
			std::transform(psattr_out[nLoop].begin(), psattr_out[nLoop].end(), psattr_out[nLoop].begin(), toupper);
		}
	}
}

Arule::~Arule()
{
	//delete [] infield;	// it is taken care of the respective form and relz destructors, as its contents are those pointers from the two objects
	delete[] pisortorder;
	delete[] psfile;
	delete[] psattr_in;
	delete[] psattr_out;
	delete[] pimodel_no;
	delete[] pdb;
	delete[] pdb_low;
	delete[] pdb_hi;
	delete[] pdb1;
	delete[] pdb2;
	delete[] pdd;
}
