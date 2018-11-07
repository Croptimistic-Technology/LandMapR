/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
FacetProcess.cpp: implementation of the facetprocess class.
*/
#include "../Headers/stdafx.h"
#include <iostream>
#include <algorithm>
#include "../Headers/Facetprocess.h"
#include <math.h>
#include "../Headers/Sort.h"


using namespace std;

FacetProcess::FacetProcess()
{

}

FacetProcess::~FacetProcess()
{

}

/** \brief This method computes the fuzzy classification of landforms based on the following agruments
*
* @param dem The DEM data object (separate from the DEM grid), matches with the DEM data file that is passed onto calling methods throughout the program
  @param form The Form data object that is populated by the terrain derivative computing module
  @param relz the relief data object that is populated by the terrain derivative computing module
  @param arule the Fuzzy attribute rules object
  @param crule the fuzzy classification rules object

  @author Modified based on original facetmapr, by Gashaw Ayalew
  @modified 2017-03-28
 */

Clsm *FacetProcess::calc_lsm(DEM *dem, Form *form, Relz *relz, Arule *arule, Crule *crule)
{

	printf("Retrieving rules data ...\n ");

	if (arule->getLength() == 0)
	{
		printf("Could not retrieve fuzzy attribute data, exiting ... ");
		exit(1);
	}

	printf("Retrieving facets data ...\n ");
	if (crule->getLength() == 0)
	{
		printf("Could not retrieve fuzzy facet data, exiting ...");
		exit(1);
	}

	printf("Retrieving DEM data ...\n ");

	if (dem->getLength() == 0)
	{
		printf("Could not retrieve DEM data, exiting ... ");
		exit(1);
	}

	int nTemp = 0, i = 0, j = 0;

	short int currzone = 0;
	bool bMissing = false;
	std::string strCRule;
	std::string strARule;

	int nNumOfRecords = dem->getLength();

	Clsm *lsm = new Clsm(dem->getLength());

	lsm->setLength(nNumOfRecords);

	//int dim = arule->getLength(), dimsq = dim*dim;
	int dim = 30, dimsq = dim * dim;
	int facet = 0;

	std::string max_class;
	double max_code = 0.0;
	double max_value = 0.0;

	printf("Retrieving form data ...\n");

	if (form->getLength() == 0)
	{
		printf("Could not retrieve form data, exiting ... ");
		exit(1);
	}
	printf("Retrieving relz data ...\n");
	if (relz->getLength() == 0)
	{
		printf("Could not retrieve relz data ... ");
		exit(1);
	}

	int nLoop = 0;

	for (nLoop = 0; nLoop < nNumOfRecords; nLoop++)
	{

		lsm->SeqNo[nLoop] = dem->SeqNo[nLoop];
		lsm->Missing[nLoop] = dem->Missing[nLoop];
		lsm->MaxClass[nLoop] = "";
		lsm->MaxCode[nLoop] = 0;
		lsm->MaxValue[nLoop] = 0;
	}

	int *fuzz_attrIndex;
	if ((fuzz_attrIndex = new int[dimsq]) == NULL)
	{
		printf("Unable to create fuzz_attrIndex array in calc_lsm(), exiting ...");
		exit(1);
	}

	bool bExit = false;
	int nLoop2 = 0;
	int nLoop3 = 0;
	std::string sVariableName;
	int nIndex = 0;

	printf("Running Calculations...");

	for (nLoop2 = 0; nLoop2 < arule->no_models; nLoop2++)
	{
		sVariableName = arule->psattr_out[nLoop2];

		for (nLoop3 = 0; nLoop3 < dimsq; nLoop3++)
		{
			if (_stricmp(sVariableName.c_str(), (crule->psfuzzattr[nLoop3]).c_str()) == 0)
			{
				fuzz_attrIndex[nLoop3] = nLoop2;
			}
		}
	}

	/*	//original longer loop, replaced by the following loop
	for (nLoop = 0;nLoop < nNumOfRecords;nLoop++)
		{
			bMissing = lsm->Missing[nLoop];

			if (bMissing == false)
			{
				bExit = false;
				while (bExit == false)
				{
					if (lsm->Missing[nLoop] == false)
					{
						nIndex = nLoop;
						calc_class3(arule->no_models, nIndex, arule->infield, arule->pimodel_no, arule->pdb, arule->pdb_low, arule->pdb_hi, arule->pdb1, arule->pdb2, arule->pdd, arule->psattr_out, max_code, max_class, max_value, crule->attr_wt, crule->psfuzzattr, crule->no_facets, crule->psf_name, crule->pif_code, fuzz_attrIndex, crule->no_attrib, dim);
						lsm->MaxClass[nLoop] = max_class;
						lsm->MaxCode[nLoop] = (int)(max_code);
						lsm->MaxValue[nLoop] = (int)(max_value + 0.5);
					}
					else
					{
						lsm->MaxClass[nLoop] = "NA";
						lsm->MaxCode[nLoop] = 0;
						lsm->MaxValue[nLoop] = 0;
					}
					if (nLoop < nNumOfRecords - 1)
					{
						nLoop++;
					}
					else
					{
						bExit = true;
					}
				}
			}
			else
			{
				bExit = true;
				if (lsm->Missing[nLoop] == true)
				{
					bExit = false;
				}
				while (bExit == false)
				{
					lsm->MaxClass[nLoop] = "NA";
					lsm->MaxCode[nLoop] = 0;
					lsm->MaxValue[nLoop] = 0;
					if (nLoop < nNumOfRecords - 1)
					{
						nLoop++;
						if (!(lsm->Missing[nLoop] == true))
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
		}
		*/

	for (nLoop = 0; nLoop < nNumOfRecords; nLoop++)
	{
		bMissing = lsm->Missing[nLoop];

		if (bMissing == false)
		{
			nIndex = nLoop;
			calc_class3(arule->no_models, nIndex, arule->infield, arule->pimodel_no, arule->pdb, arule->pdb_low, arule->pdb_hi, arule->pdb1, arule->pdb2, arule->pdd, arule->psattr_out, max_code, max_class, max_value, crule->attr_wt, crule->psfuzzattr, crule->no_facets, crule->psf_name, crule->pif_code, fuzz_attrIndex, crule->no_attrib, dim);
			lsm->MaxClass[nLoop] = max_class;
			lsm->MaxCode[nLoop] = (int)(max_code);
			lsm->MaxValue[nLoop] = (int)(max_value + 0.5);
		}
		else
		{
			lsm->MaxClass[nLoop] = "NA";
			lsm->MaxCode[nLoop] = 0;
			lsm->MaxValue[nLoop] = 0;
		}
	}


	//lsm->prefix = "test";
	delete[] fuzz_attrIndex;
	printf("Landform classification complete.");
	return lsm;
}

/** \brief  A fuzzy classification function for the condensed LSM mode of output (for each grid cell) */
void FacetProcess::calc_class3(int no_models, int nIndex,
	double** infield, int* model,
	double* B, double* b_low, double* b_hi, double* b1,
	double* b2, double *D, std::string* outattr, double &Max_Code,
	std::string &Max_Class, double &Max_Value, double* attr_wt,
	std::string* fuzz_attr, int no_facets, std::string* Facet_Name,
	int* Facet_Code, int* fuzz_attrIndex, int* no_attrib, int dim)
{

	int i = 0, j = 0;
	std::string sString;
	double x = 0.0, outattrval[70] = { 0.0 }, fazzval = 0.0, classval = 0.0, attrib_val = 0.0;
	int nNE_Aspect = -1, nSW_Aspect = -1, nSteep = -1, nSlopeLt20 = -1, nPlanarD = -1, nPlanarA = -1;

	for (i = 0; i < no_models; i++)
	{

		x = (double)infield[i][nIndex];

		switch (model[i])
		{
		case(1):
			fazzval = 1 / (1 + pow((x - B[i]) / D[i], 2));
			break;
		case(2):
			if (x > b_low[i] && x < b_hi[i])
			{
				fazzval = 1;
			}
			if (x <= b_low[i])
			{
				fazzval = 1 / (1 + pow((x - b_low[i]) / D[i], 2));
			}
			if (x >= b_hi[i])
			{
				fazzval = 1 / (1 + pow((x - b_hi[i]) / D[i], 2));
			}
			break;
		case(3):
			if (x > b1[i] && x < b2[i])
			{
				fazzval = 1;
			}
			if (x <= b1[i])
			{
				fazzval = 1 / (1 + pow((x - b1[i]) / D[i], 2));
			}
			if (x >= b2[i])
			{
				fazzval = 1 / (1 + pow((x - b2[i]) / D[i], 2));
			}
			break;
		case(4):
			if (x > B[i])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1 / (1 + pow((x - B[i]) / D[i], 2));
			}
			break;
		case(5):
			if (x < B[i])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1 / (1 + pow((x - B[i]) / D[i], 2));
			}
			break;
		}
		fazzval = fazzval * 100;
		outattrval[i] = fazzval;
	}

	for (i = 0; i < no_models; i++)
	{
		if (_stricmp(outattr[i].c_str(), "planar_d") == 0)
		{
			nPlanarD = i;
		}
		if (_stricmp(outattr[i].c_str(), "planar_a") == 0)
		{
			nPlanarA = i;
		}
	}
	assert(nPlanarD != -1);
	assert(nPlanarA != -1);

	Max_Code = 0;
	Max_Value = 0;
	Max_Class = "";

	for (i = 0; i < no_facets; i++)
	{
		classval = 0.0;
		for (j = 0; j < no_attrib[i]; j++)
		{
			attrib_val = outattrval[fuzz_attrIndex[i * dim + j]];
			classval = classval + attrib_val * attr_wt[i * dim + j];
		}
		if (classval >= Max_Value)
		{
			Max_Value = classval;
			Max_Class = Facet_Name[i];
			Max_Code = Facet_Code[i];
		}
	}
}
