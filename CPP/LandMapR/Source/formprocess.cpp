/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief Implementation of the FormProcess class.
*/
#include "../Headers/stdafx.h"
#include <math.h>
#include <assert.h>
#include "../Headers/formprocess.h"
#include "../Headers/sort.h"


/** Constructor with give parameters */
FormProcess::FormProcess()
{
	missing_value = -9999.0000;
	//max_vol = 10.0;
	//max_area = 300.0;
	//max_depth = 0.15;
	//max_runoff = 300.0;
	//dDSArea = 300;
	//dUpSlopeArea = 300;
	//dGridSize = 500; 
}

/** Default destructor */
FormProcess::~FormProcess()
{

}


//void FormProcess::runProcess(DEM *dem, Pond *pond, Relz *relz, IDEM *idem, Form *form, RelzExtraForLen *relzExtra, Len* len, string m_WorkingDirectory, string option)
//{
//	if (option == "A")
//	{
//		fprocess1(dem, pond, relz, idem, form, m_WorkingDirectory);
//	}
//	if (option == "B")
//	{
//		fprocess2(dem, relz, form, m_WorkingDirectory);
//	}
//	if (option == "C")
//	{
//		fprocess3(dem, pond, relz, idem, form, len, relzExtra, m_WorkingDirectory);
//	}
//
//}

/** Processes */
bool FormProcess::fprocess1(DEM *dem, Pond *pond, Relz *relz, IDEM *idem, Form *form, string m_WorkingDirectory)
{

	int nLoop, nSize = dem->getLength();
	bool bReturnValue = false;

	for (nLoop = 0; nLoop < nSize; nLoop++)
	{
		form->SeqNo[nLoop] = dem->SeqNo[nLoop];
		form->Slope[nLoop] = 0;
		form->Aspect[nLoop] = 0;
		form->Prof[nLoop] = 0;
		form->Plan[nLoop] = 0;
		form->QArea[nLoop] = 0;
		form->QWETI[nLoop] = 0;
		form->LNQArea[nLoop] = 0;
		form->New_Asp[nLoop] = 0;
	}

	printf("Running fcalcForm() ...\n");
	fcalcForm(dem, form);

	printf("Running fcalcweti() ...\n");
	fcalcweti(dem, form, 1);

	form->writeCSV(m_WorkingDirectory + "form_lsm.csv");

	for (nLoop = 0; nLoop < nSize; nLoop++)
	{
		relz->SeqNo[nLoop] = nLoop + 1;
		relz->Z2ST[nLoop] = 0.0;
		relz->Z2CR[nLoop] = 0.0;
		relz->Z2PIT[nLoop] = 0.0;
		relz->Z2PEAK[nLoop] = 0.0;
		relz->Z2TOP[nLoop] = 0.0;
		relz->ZCR2ST[nLoop] = 0.0;
		relz->ZPIT2PEAK[nLoop] = 0.0;
		relz->ZTOP2PIT[nLoop] = 0.0;
		relz->PCTZ2ST[nLoop] = 0;
		relz->PCTZ2PIT[nLoop] = 0;
		relz->PCTZ2TOP[nLoop] = 0;
		relz->PMIN2MAX[nLoop] = 0;
		relz->N2ST[nLoop] = 0;
		relz->N2CR[nLoop] = 0;
		relz->N2PEAK[nLoop] = 0;
	}

	fcalcRelz1(dem, pond, idem, relz);

	relz->writeCSV(m_WorkingDirectory + "Relz_lsm.csv");

	printf("Form processing complete.\n");

	return bReturnValue;
}

bool FormProcess::fprocess2(DEM *dem, Relz *relz, Form *form, string m_WorkingDirectory)
{
	int nSize = dem->getLength();

	for (int nLoop = 0; nLoop < nSize; nLoop++)
	{
		form->SeqNo[nLoop] = dem->SeqNo[nLoop];
		form->Slope[nLoop] = 0;
		form->Aspect[nLoop] = 0;
		form->Prof[nLoop] = 0;
		form->Plan[nLoop] = 0;
		form->QArea[nLoop] = 0;
		form->QWETI[nLoop] = 0;
		form->LNQArea[nLoop] = 0;
		form->New_Asp[nLoop] = 0;
	}

	printf("Running fcalcForm() ...\n");
	fcalcForm(dem, form);

	printf("Running fcalcweti() ...\n");
	fcalcweti(dem, form, 2);

	form->writeCSV(m_WorkingDirectory + "form_dss.csv");

	for (int nLoop = 0; nLoop < nSize; nLoop++)
	{
		relz->SeqNo[nLoop] = nLoop + 1;
		relz->Z2ST[nLoop] = 0.0;
		relz->Z2CR[nLoop] = 0.0;
		relz->Z2PIT[nLoop] = 0.0;
		relz->Z2PEAK[nLoop] = 0.0;
		relz->Z2TOP[nLoop] = 0.0;
		relz->ZCR2ST[nLoop] = 0.0;
		relz->ZPIT2PEAK[nLoop] = 0.0;
		relz->ZTOP2PIT[nLoop] = 0.0;
		relz->PCTZ2ST[nLoop] = 0;
		relz->PCTZ2PIT[nLoop] = 0;
		relz->PCTZ2TOP[nLoop] = 0;
		relz->PMIN2MAX[nLoop] = 0;
		relz->N2ST[nLoop] = 0;
		relz->N2CR[nLoop] = 0;
		relz->N2PEAK[nLoop] = 0;
	}
	printf("Running Calc_Relz2 function.. \n");
	fcalcRelz2(dem, relz);

	relz->writeCSV(m_WorkingDirectory + "Relz_BCPEM.csv");

	printf("Form processing complete.");

	return true;
}

bool FormProcess::fprocess3(string& resultJsonPath, DEM *dem, Pond *pond, Relz *relz, IDEM *idem, Form *form, Len *len, RelzExtraForLen *relzExtra, string m_WorkingDirectory, int numRows, int numCols, double xll, double yll)
{
	int nSize = dem->getLength();

	for (int nLoop = 0; nLoop < nSize; nLoop++)
	{
		form->SeqNo[nLoop] = dem->SeqNo[nLoop];
		form->Slope[nLoop] = 0;
		form->Aspect[nLoop] = 0;
		form->Prof[nLoop] = 0;
		form->Plan[nLoop] = 0;
		form->QArea[nLoop] = 0;
		form->QWETI[nLoop] = 0;
		form->LNQArea[nLoop] = 0;
		form->New_Asp[nLoop] = 0;
	}

	printf("Running Calc_Form function.. \n");
	fcalcForm(dem, form);

	printf("Running fcalcweti()...\n");
	fcalcweti(dem, form, 3);

	//form->writeCSV(m_WorkingDirectory + "form_all.csv");
	form->writeCSVmap(m_WorkingDirectory + "form_all.csv", dem->Missing, numCols, numRows, xll, yll, dGridSize);

	string sTemp = m_WorkingDirectory + "form_all.csv";
	string filePathForReplace = "\"formAllPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"formAllPath\":\"" + sTemp + "\"");

	for (int nLoop = 0; nLoop < nSize; nLoop++)
	{
		relz->SeqNo[nLoop] = nLoop + 1;
		relz->Z2ST[nLoop] = 0.0;
		relz->Z2CR[nLoop] = 0.0;
		relz->Z2PIT[nLoop] = 0.0;
		relz->Z2PEAK[nLoop] = 0.0;
		relz->Z2TOP[nLoop] = 0.0;
		relz->ZCR2ST[nLoop] = 0.0;
		relz->ZPIT2PEAK[nLoop] = 0.0;
		relz->ZTOP2PIT[nLoop] = 0.0;
		relz->PCTZ2ST[nLoop] = 0;
		relz->PCTZ2PIT[nLoop] = 0;
		relz->PCTZ2TOP[nLoop] = 0;
		relz->PMIN2MAX[nLoop] = 0;
		relz->N2ST[nLoop] = 0;
		relz->N2CR[nLoop] = 0;
		relz->N2PEAK[nLoop] = 0;
		relzExtra->ST_COL[nLoop] = 0;
		relzExtra->ST_ROW[nLoop] = 0;
		relzExtra->CR_COL[nLoop] = 0;
		relzExtra->CR_ROW[nLoop] = 0;
		relzExtra->PIT_ROW[nLoop] = 0;
		relzExtra->PIT_COL[nLoop] = 0;
		relzExtra->PK_ROW[nLoop] = 0;
		relzExtra->PK_COL[nLoop] = 0;
	}
	printf("Running Calc_Relz3 function.. \n");
	fcalcRelz3(dem, pond, relz, len, idem, relzExtra);

	//relz->writeCSV(m_WorkingDirectory + "Relz_all.csv");
	relz->writeCSVmap(m_WorkingDirectory + "Relz_all.csv", dem->Missing, numCols, numRows, xll, yll, dGridSize);

	sTemp = m_WorkingDirectory + "Relz_all.csv";
	filePathForReplace = "\"relzAllPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"relzAllPath\":\"" + sTemp + "\"");

	printf("Running calc_len function.. \n");
	calc_len3(len, relz, dem, relzExtra);

	//len->writeCSV(m_WorkingDirectory + "Len.csv");
	len->writeCSVmap(m_WorkingDirectory + "Len.csv", dem->Missing, numCols, numRows, xll, yll, dGridSize);

	sTemp = m_WorkingDirectory + "Len.csv";
	filePathForReplace = "\"lenPath\":null";
	while (sTemp.find("\\") != std::string::npos) {
		sTemp.replace(sTemp.find("\\"), 1, "slash");
	}
	while (sTemp.find("slash") != std::string::npos) {
		sTemp.replace(sTemp.find("slash"), 5, "\\\\");
	}
	resultJsonPath.replace(resultJsonPath.find(filePathForReplace), filePathForReplace.length(), "\"lenPath\":\"" + sTemp + "\"");

	printf("Form processing complete.");

	return true;
}



/** Eyton form processing wrapper method */
void FormProcess::fcalcForm(DEM *dem, Form *form)
{
	int no_rows = 0;
	int no_cols = 0;

	int rowno = 1;
	int *pRow, *pCol, nNumRec;
	double *pElev;
	bool *pMissing;

	nNumRec = dem->getLength();

	pRow = dem->Row;
	pCol = dem->Col;
	pElev = dem->Elev;
	pMissing = dem->Missing;

	no_rows = pRow[nNumRec - 1];
	no_cols = pCol[nNumRec - 1];

	double* row1z, *row2z, *row3z;
	if (!(row1z = new double[no_cols]))
	{
		printf("Not enough memory for row1z array, exiting ...");
		exit(1);
	}
	if (!(row2z = new double[no_cols]))
	{
		printf("Not enough memory for row2z array, exiting ...");
		exit(1);
	}
	if (!(row3z = new double[no_cols]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}

	int start1rec = 0;
	int start2rec = 0;
	int start3rec = 0;
	int start4rec = 0;

	int nLoop = 0;
	int nLoop2 = 0;
	int nLoop3 = 0;

	for (nLoop = 0; nLoop < no_cols; nLoop++)
	{
		row1z[nLoop] = 0;
	}

	for (nLoop = 0; nLoop < no_cols; nLoop++)
	{
		row2z[nLoop] = pElev[nLoop];
	}

	start3rec = nLoop;
	nLoop2 = nLoop + no_cols;

	for (nLoop; nLoop < nLoop2; nLoop++)
	{
		row3z[nLoop3] = pElev[nLoop];
		nLoop3++;
	}
	start4rec = nLoop;

	bool bExit = false;
	int this_row = 0;
	int this_col = 0;
	double  *pSlope, *pProf, *pPlan, *pQArea, *pQweti;
	int *pAspect;

	pSlope = form->Slope;
	pAspect = form->Aspect;
	pProf = form->Prof;
	pPlan = form->Plan;
	pQArea = form->QArea;
	pQweti = form->QWETI;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{

		this_row = pRow[nLoop];
		this_col = pCol[nLoop];
		if (pMissing[nLoop] == true)
		{
			pSlope[nLoop] = 0;
			pAspect[nLoop] = -1;
			pProf[nLoop] = 0;
			pPlan[nLoop] = 0;
			pQArea[nLoop] = 0;
			pQweti[nLoop] = 0;
		}
		else
		{
			if ((this_col == 1) || (this_col == no_cols))
			{
				pSlope[nLoop] = 0;
				pAspect[nLoop] = 0;
				pProf[nLoop] = 0;
				pPlan[nLoop] = 0;
			}
			else
			{
				if (this_row == 1 || this_row == no_rows)
				{
					pSlope[nLoop] = 0;
					pAspect[nLoop] = 0;
					pProf[nLoop] = 0;
					pPlan[nLoop] = 0;
				}
				else
				{
					eyton_form(row1z, row2z, row3z, this_col, pSlope, pAspect, pProf, pPlan, nLoop);
				}
			}
		}
		if (this_col == no_cols)
		{
			if (this_row == no_rows)
			{

			}
			else
			{
				rowno = rowno + 1;


				for (nLoop2 = 0; nLoop2 < no_cols; nLoop2++)
				{
					row1z[nLoop2] = row2z[nLoop2];
					row2z[nLoop2] = row3z[nLoop2];
				}

				if (rowno < no_rows)
				{
					nLoop = start4rec;
					for (nLoop2 = 0; nLoop2 < no_cols; nLoop2++)
					{
						row3z[nLoop2] = pElev[nLoop + nLoop2];
					}
				}
				start2rec = start3rec;
				start3rec = start4rec;
				start4rec = nLoop + nLoop2;

				nLoop = start2rec - 1;
			}
		}
		else
		{
		}
	}
	delete[] row1z;
	delete[] row2z;
	delete[] row3z;

	pad_edges(pSlope, pAspect, pProf, pPlan, no_cols, no_rows);
}

/** Implementation of algorithm of Eyton (1991) to compute gradient, aspect, profile, and plan curvature */
void FormProcess::eyton_form(double* row1z, double* row2z, double* row3z, int this_col,
	double* pSlope, int* pAspect, double* pProf, double* pPlan, int nIndex)
{
	double	slope_pct = 0.0;
	double 	slope_deg = 0.0;
	double 	azimuth = 0.0;
	double 	this_prof = 0.0;
	double 	this_plan = 0.0;
	double local_angle = 0.0;

	double twod = dGridSize * 2;
	double z11 = 0.0;
	double z12 = 0.0;
	double z13 = 0.0;

	bool one_missing = false;

	if (row1z[this_col - 2] != missing_value)
	{
		z11 = row1z[this_col - 2] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row1z[this_col - 1] != missing_value)
	{
		z12 = row1z[this_col - 1] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row1z[this_col] != missing_value)
	{
		z13 = row1z[this_col] * 100;
	}
	else
	{
		one_missing = true;
	}

	double z21 = 0.0;
	double z22 = 0.0;
	double z23 = 0.0;

	if (row2z[this_col - 2] != missing_value)
	{
		z21 = row2z[this_col - 2] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row2z[this_col - 1] != missing_value)
	{
		z22 = row2z[this_col - 1] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row2z[this_col] != missing_value)
	{
		z23 = row2z[this_col] * 100;
	}
	else
	{
		one_missing = true;
	}

	double z31 = 0.0;
	double z32 = 0.0;
	double z33 = 0.0;

	if (row3z[this_col - 2] != missing_value)
	{
		z31 = row3z[this_col - 2] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row3z[this_col - 1] != missing_value)
	{
		z32 = row3z[this_col - 1] * 100;
	}
	else
	{
		one_missing = true;
	}

	if (row3z[this_col] != missing_value)
	{
		z33 = row3z[this_col] * 100;
	}
	else
	{
		one_missing = true;
	}


	if (one_missing == false)
	{
		double slope_in_x = (z23 - z21) / twod;
		double slope_in_y = (z32 - z12) / twod;
		double this_slope = sqrt(slope_in_x * slope_in_x + slope_in_y * slope_in_y);
		slope_pct = this_slope;
		double slope_rad = atan(this_slope / 100);
		double slope_deg = (slope_rad * 180) / 3.141592;
		if (this_slope > 0)
		{
			azimuth = 0.0;
			local_angle = (acos(fabs(slope_in_x) / this_slope) * 180) / 3.141592;
			if (slope_in_x > 0 && slope_in_y > 0)
			{
				azimuth = 270 + local_angle;
			}
			else
			{
				if (slope_in_x > 0 && slope_in_y < 0)
				{
					azimuth = 270.0 - local_angle;
				}
				else
				{
					if (slope_in_x < 0 && slope_in_y >0)
					{
						azimuth = 90.0 - local_angle;
					}
					else
					{
						if (slope_in_x < 0 && slope_in_y < 0)
						{
							azimuth = 90.0 + local_angle;
						}
						else
						{
							if (slope_in_y == 0 && slope_in_x < 0)
							{
								azimuth = 90.0;
							}
							else
							{
								if (slope_in_y == 0 && slope_in_x > 0)
								{
									azimuth = 270.0;
								}
								else
								{
									if (slope_in_x == 0 && slope_in_y < 0)
									{
										azimuth = 180.0;
									}
									else
									{
										if (slope_in_x == 0 && slope_in_y > 0)
										{
											azimuth = 360.0;
										}
										else
										{
											azimuth = 361.0;
										}
									}
								}
							}
						}
					}
				}
			}

			double prof_azimuth = azimuth;
			if (prof_azimuth > 180)
			{
				prof_azimuth = prof_azimuth - 180.0;
			}

			double plan_azimuth = prof_azimuth + 90.0;
			if (plan_azimuth > 180.0)
			{
				plan_azimuth = plan_azimuth - 180.0;
			}

			double prof_xa = 2.0 + sin((prof_azimuth *3.141592) / 180.0);
			double prof_ya = 2.0 - cos((prof_azimuth *3.141592) / 180.0);
			double prof_xb = 2.0 - sin((prof_azimuth *3.141592) / 180.0);
			double prof_yb = 2.0 + cos((prof_azimuth *3.141592) / 180.0);

			double plan_xa = 2.0 + sin((plan_azimuth *3.141592) / 180.0);
			double plan_ya = 2.0 - cos((plan_azimuth *3.141592) / 180.0);
			double plan_xb = 2.0 - sin((plan_azimuth *3.141592) / 180.0);
			double plan_yb = 2.0 + cos((plan_azimuth *3.141592) / 180.0);

			double prof_za = 0.0;
			double prof_zb = 0.0;
			double plan_za = 0.0;
			double plan_zb = 0.0;
			if (prof_azimuth <= 90.0)
			{
				prof_za = (2.0 - prof_ya)*((z13*(prof_xa - 2.0)) + (z12*(3.0 - prof_xa)));
				prof_za = prof_za + ((prof_ya - 1.0)*((z23*(prof_xa - 2.0)) + (z22*(3.0 - prof_xa))));

				prof_zb = (3.0 - prof_yb)*((z22*(prof_xb - 1.0)) + (z21*(2.0 - prof_xb)));
				prof_zb = prof_zb + ((prof_yb - 2.0)*((z32*(prof_xb - 1.0)) + (z31*(2.0 - prof_xb))));
			}
			else
			{
				prof_za = (3.0 - prof_ya)*((z23*(prof_xa - 2.0)) + (z22*(3.0 - prof_xa)));
				prof_za = prof_za + ((prof_ya - 2.0)*((z33*(prof_xa - 2.0)) + (z32*(3.0 - prof_xa))));

				prof_zb = (2.0 - prof_yb)*((z12*(prof_xb - 1.0)) + (z11*(2.0 - prof_xb)));
				prof_zb = prof_zb + ((prof_yb - 1.0)*((z22*(prof_xb - 1.0)) + (z21*(2.0 - prof_xb))));
			}

			if (plan_azimuth <= 90.0)
			{
				plan_za = (2.0 - plan_ya)*((z13*(plan_xa - 2.0)) + (z12*(3.0 - plan_xa)));
				plan_za = plan_za + ((plan_ya - 1.0)*((z23*(plan_xa - 2.0)) + (z22*(3.0 - plan_xa))));

				plan_zb = (3.0 - plan_yb)*((z22*(plan_xb - 1.0)) + (z21*(2.0 - plan_xb)));
				plan_zb = plan_zb + ((plan_yb - 2.0)*((z32*(plan_xb - 1.0)) + (z31*(2.0 - plan_xb))));
			}

			else
			{
				plan_za = (3.0 - plan_ya)*((z23*(plan_xa - 2.0)) + (z22*(3.0 - plan_xa)));
				plan_za = plan_za + ((plan_ya - 2.0)*((z33*(plan_xa - 2.0)) + (z32*(3.0 - plan_xa))));

				plan_zb = (2.0 - plan_yb)*((z12*(plan_xb - 1.0)) + (z11*(2.0 - plan_xb)));
				plan_zb = plan_zb + ((plan_yb - 1.0)*((z22*(plan_xb - 1.0)) + (z21*(2.0 - plan_xb))));
			}

			this_prof = ((atan((((2 * z22) - prof_za - prof_zb) / (dGridSize*dGridSize)))) * 180) / 3.141592;
			this_plan = ((atan((((2 * z22) - plan_za - plan_zb) / (dGridSize*dGridSize)))) * 180) / 3.141592;
		}
		else
		{
			this_slope = 0.0;
			slope_pct = 0.0;
			slope_deg = 0.0;
			local_angle = 360.0;
			azimuth = 360.0;
			this_plan = 0.0;
			this_prof = 0.0;
		}
	}
	else
	{
		slope_pct = 0.0;
		slope_deg = 0.0;
		azimuth = 360.0;
		this_prof = 0.0;
		this_plan = 0.0;
	}

	pSlope[nIndex] = slope_pct;
	pAspect[nIndex] = (int)(azimuth + 0.5);
	pProf[nIndex] = this_prof;
	pPlan[nIndex] = this_plan;
}

void FormProcess::pad_edges(double *pSlope, int *pAspect, double *pProf, double *pPlan, int no_cols, int no_rows)
{
	int cells = 0;
	int first_col = no_cols;
	int next_col = first_col + 1;
	int nLoop = 0;

	double next_slope = 0.0;
	//int next_aspect = 0;	//original
	double next_aspect = 0.0;
	double next_prof = 0.0;
	double next_plan = 0.0;

	for (nLoop = 0; nLoop < no_rows - 2; nLoop++)
	{
		next_slope = pSlope[next_col];
		next_aspect = pAspect[next_col];
		next_prof = pProf[next_col];
		next_plan = pPlan[next_col];

		pSlope[first_col] = next_slope;
		pAspect[first_col] = next_aspect;
		pProf[first_col] = next_prof;
		pPlan[first_col] = next_plan;

		first_col = first_col + no_cols;
		next_col = first_col + 1;
	}

	int last_col = no_cols * 2 - 1;
	int prev_col = last_col - 1;

	for (nLoop = 0; nLoop < no_rows - 2; nLoop++)
	{

		next_slope = pSlope[prev_col];
		next_aspect = pAspect[prev_col];
		next_prof = pProf[prev_col];
		next_plan = pPlan[prev_col];

		pSlope[last_col] = next_slope;
		pAspect[last_col] = next_aspect;
		pProf[last_col] = next_prof;
		pPlan[last_col] = next_plan;
		last_col = last_col + no_cols;
		prev_col = last_col - 1;
	}

	int top_col = -1;
	int col_below = top_col + no_cols;

	for (nLoop = 0; nLoop < no_cols; nLoop++)
	{

		top_col = top_col + 1;
		col_below = top_col + no_cols;
		next_slope = pSlope[col_below];
		next_aspect = pAspect[col_below];
		next_prof = pProf[col_below];
		next_plan = pPlan[col_below];

		pSlope[top_col] = next_slope;
		pAspect[top_col] = next_aspect;
		pProf[top_col] = next_prof;
		pPlan[top_col] = next_plan;
	}

	int col_above = (no_rows * no_cols) - (2 * no_cols) - 1;
	int bot_col = col_above + no_cols;

	for (nLoop = 0; nLoop < no_cols; nLoop++)
	{
		col_above = col_above + 1;
		bot_col = col_above + no_cols;

		next_slope = pSlope[col_above];
		next_aspect = pAspect[col_above];
		next_prof = pProf[col_above];
		next_plan = pPlan[col_above];

		pSlope[bot_col] = next_slope;
		pAspect[bot_col] = next_aspect;
		pProf[bot_col] = next_prof;
		pPlan[bot_col] = next_plan;
	}
}

/** Compute LSM wetting index using the method of \latexonly\citet{quinn91}\endlatexonly */
void FormProcess::fcalcweti(DEM *dem, Form *form, int nNum)
{
	double l1 = dGridSize * 0.5;
	double l2 = dGridSize * 0.354;
	double l_sqr = dGridSize * dGridSize;

	double orthogonal = dGridSize;
	double diagonal = dGridSize * sqrt(2);

	int nColumn = 0, *pRow, *pCol, *pSeqNo, *pUpslope, nNumRec;

	double *pQArea, *pQweti, *pElev;

	nNumRec = dem->getLength();

	pRow = dem->Row;
	pCol = dem->Col;
	pSeqNo = dem->SeqNo;
	pQArea = form->QArea;
	pQweti = form->QWETI;


	int maxrows = pRow[nNumRec - 1];
	int maxcols = pCol[nNumRec - 1];

	int lastrec = nNumRec;
	int a_seqno = pSeqNo[nNumRec - 1];
	int nLoop = 0;

	if (nNum == 1)
	{
		for (nLoop = 0; nLoop < nNumRec; nLoop++)
		{
			pQArea[nLoop] = 1;
		}
	}
	else
	{
		for (nLoop = 0; nLoop < nNumRec; nLoop++)
		{
			pQArea[nLoop] = l_sqr;
		}
	}

	pUpslope = dem->UpSlope;
	pElev = dem->Elev;

	double* pSortValue;
	int* pSortedIndex;

	if (!(pSortValue = new double[nNumRec]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}
	if (!(pSortedIndex = new int[nNumRec]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}

	//original code; but did crash on debug mode, for homewest plot
//for(nLoop=0;nLoop<nNumRec;nLoop++)
//{
//	pSortValue[nLoop]  = ((1000000 - pElev[nLoop]) * 10000000000) + pUpslope[nLoop];
//	//pSortValue[nLoop] = pQAreaA[nLoop];
//	pSortedIndex[nLoop] = nLoop;
//}

//QuickSort(pSortValue, nNumRec,pSortedIndex);


/*Substitution of code begins here*/
//purpose: to replace the above routine that does sort all cells from the beginning to the end without regard to 'missing' cells
	int nindex = 0, i = 0, nMissingNum = 0, *pTempSort;
	pTempSort = new int[nNumRec];
	for (i = 0; i < nNumRec; i++)
	{
		if (!(pElev[i] == FormProcess::missing_value))//if value is not missing
		{
			pSortValue[nindex] = ((1000000 - pElev[i]) * 10000000000) + pUpslope[i];
			pSortedIndex[nindex] = i;
			nindex++;
		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values
		}
	}
	QuickSort(pSortValue, nNumRec - nMissingNum, pSortedIndex); //sort only non missing values
	nindex = 0;
	for (i = nNumRec - nMissingNum; i < nNumRec; i++) //make the rest of pointers which are
													  // pointing to the missing values, valid. 
	{
		pSortedIndex[i] = pTempSort[nindex];
		nindex++;
	}
	delete[] pTempSort;
	/*Substitution of code ends here*/


	int cent_x = 0;
	int cent_y = 0;
	double cent_z = 0;
	double cent_qa = 0;
	int centrec = 0, *pDDir, *pDrec;

	centrec = pSortedIndex[0];
	cent_z = pElev[pSortedIndex[0]];

	int numneigh = 0, *zrec;
	double *deltax, *l;

	if (!(zrec = new int[9]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}
	if (!(deltax = new double[9]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}
	if (!(l = new double[9]))
	{
		printf("Not enough memory for row3z array, exiting ...");
		exit(1);
	}

	pDDir = dem->Ddir;
	pDrec = dem->Drec;

	int nIndex = 0;

	double qc = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];

		cent_y = pRow[nIndex];
		cent_x = pCol[nIndex];
		cent_z = pElev[nIndex];
		centrec = nIndex;

		cent_qa = pQArea[nIndex];
		if (cent_z != missing_value)
		{
			qfindneig(cent_x, cent_y, cent_z, cent_qa, numneigh, zrec, l, deltax, orthogonal, diagonal, maxcols, maxrows, l1, l2);
			calc_quinn(numneigh, cent_z, cent_qa, zrec, deltax, pElev, (int *)pDDir, pDrec, centrec, l, pQArea, pQweti);
		}
		else
		{
			qc = cent_qa / 0.0001;
			pQweti[nIndex] = log(1 + qc);
		}
	}

	if (nNum > 1)		//for FormMapR with options other than the LSM (first option in the 2003 version), Gashaw.
	{
		double *pLnqArea = form->LNQArea;
		int *pNewAsp = form->New_Asp;
		int *pAspect = form->Aspect;

		for (nLoop = 0; nLoop < nNumRec; nLoop++)
		{
			if (pAspect[nLoop] > -1)
			{
				pLnqArea[nLoop] = log(pQArea[nLoop]);
				pNewAsp[nLoop] = pAspect[nLoop] + 45;

				if (pNewAsp[nLoop] > 360)
				{
					pNewAsp[nLoop] = pNewAsp[nLoop] - 360;
				}
			}

			else
			{
				pAspect[nLoop] = 0;
				pLnqArea[nLoop] = 0;
				pNewAsp[nLoop] = 0;
			}

		}

		//printf("Invalid fcalcweti() option specified!");

	}

	delete[] pSortValue;
	delete[] pSortedIndex;
	delete[] l;
	delete[] deltax;
	delete[] zrec;
}

/** Compute relative elevations of neighbouring cells for \latexonly\citet{quinn91}\endlatexonly algorithm for Qarea and Wetting index */
void FormProcess::qfindneig(int cent_x, int cent_y, double cent_z, double cent_qa, int &numneigh, int *zrec,
	double *l, double *deltax, double orthogonal, double diagonal, int maxcols, int maxrows,
	double l1, double l2)
{
	numneigh = 0;
	if (cent_x > 1 && cent_y > 1)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = (((cent_y - 2) * maxcols) + (cent_x - 1));

		l[numneigh - 1] = l2;
		deltax[numneigh - 1] = diagonal;
	}

	if (cent_y > 1)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = (((cent_y - 2) * maxcols) + cent_x);
		l[numneigh - 1] = l1;
		deltax[numneigh - 1] = orthogonal;
	}

	if (cent_y > 1 && cent_x < maxcols)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = (((cent_y - 2) * maxcols) + (cent_x + 1));
		l[numneigh - 1] = l2;
		deltax[numneigh - 1] = diagonal;
	}
	if (cent_x > 1)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = (((cent_y - 1) * maxcols) + (cent_x - 1));
		l[numneigh - 1] = l1;
		deltax[numneigh - 1] = orthogonal;
	}

	if (cent_x < maxcols)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = (((cent_y - 1) * maxcols) + (cent_x + 1));
		l[numneigh - 1] = l1;
		deltax[numneigh - 1] = orthogonal;
	}

	if (cent_x > 1 && cent_y < maxrows)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = ((cent_y * maxcols) + (cent_x - 1));
		l[numneigh - 1] = l2;
		deltax[numneigh - 1] = diagonal;
	}

	if (cent_y < maxrows)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = ((cent_y * maxcols) + cent_x);
		l[numneigh - 1] = l1;
		deltax[numneigh - 1] = orthogonal;
	}

	if (cent_x < maxcols && cent_y < maxrows)
	{
		numneigh = numneigh + 1;
		zrec[numneigh - 1] = ((cent_y * maxcols) + (cent_x + 1));
		l[numneigh - 1] = l2;
		deltax[numneigh - 1] = diagonal;
	}

}


/** Calculate Qarea and Wetting indices according to \latexonly\citet{quinn91}\endlatexonly . */
void FormProcess::calc_quinn(int numneigh, double cent_z, double cent_qa, int *zrec, double *deltax,
	double *pElev, int *pDDir, int *pDrec, int centrec, double* l,
	double* pQArea, double* pQweti)
{
	int j = 0;
	int numlower = 0;
	int nLoop = 0;
	int nIndex = 0;
	double ql[9];
	double sumtanbl = 0;

	double qc = 0;

	int down[9];

	double tanb[9];
	for (nLoop = 0; nLoop < numneigh; nLoop++)
	{
		nIndex = zrec[nLoop];

		if (pElev[nIndex - 1] < cent_z && pElev[nIndex - 1] != missing_value)
		{
			j = j + 1;
			tanb[j - 1] = (cent_z - pElev[nIndex - 1]) / deltax[nLoop];
			down[j - 1] = zrec[nLoop];
			ql[j - 1] = l[nLoop];
			sumtanbl = sumtanbl + (tanb[j - 1] * ql[j - 1]);
			numlower = numlower + 1;
		}

	}

	double new_qa[9];
	if (numlower > 0)
	{
		qc = cent_qa / sumtanbl;
		for (j = 0; j < numlower; j++)
		{
			new_qa[j] = qc * (tanb[j] * ql[j]);
			nIndex = down[j];
			pQArea[nIndex - 1] = pQArea[nIndex - 1] + new_qa[j];

		}
	}
	else
	{
		nIndex = centrec;
		qc = cent_qa / 0.0001;
		if (pDDir[nIndex] != 5)
		{
			nIndex = pDrec[nIndex];
			pQArea[nIndex - 1] = pQArea[nIndex - 1] + cent_qa;
		}
	}

	nIndex = centrec;
	if (qc > 1)
	{
		pQweti[nIndex] = log(qc);
	}
	else
	{
		pQweti[nIndex] = log(1 + qc);
	}

}
/** Compute LSM relief derivatives */
void FormProcess::fcalcRelz1(DEM *dem, Pond *pond, IDEM *idem, Relz *relz)
{
	int tempz2cr = 0;
	double d = dGridSize;
	int numup = 0;
	int numdn = 0;
	int j = 0;


	printf("Running go_stream1() ...\n");
	go_stream1(dem, relz, numup, numdn);

	printf("Running go_pit1() ...\n");
	go_pit1(dem, pond, relz);


	printf("Running go_ridge1() ...\n");
	go_ridge1(idem, relz);


	printf("Running up2peak1() ...\n");
	up2peak1(idem, relz);

	printf("Running calcrelief1() ...\n");
	calcrelief1(dem, relz);
}
/** Compute BC-PEM Site Series relief derivatives */
void FormProcess::fcalcRelz2(DEM *dem, Relz *relz)
{
	printf("Running go_stream2()");
	go_stream2(dem, relz);
}

/** Compute full-set relief derivatives */
void FormProcess::fcalcRelz3(DEM *dem, Pond *pond, Relz *relz, Len *len, IDEM *idem, RelzExtraForLen *relzExtra)
{
	//int* pSortedIndex = 0;
	//int* pSortedIndex2 = 0;


	printf("Running go_stream3()...\n");
	go_stream3(dem, relz, relzExtra);

	printf("Running go_pit3()...\n");
	go_pit3(dem, pond, relz, relzExtra);

	printf("Running go_ridge3()...\n");
	go_ridge3(idem, relz, relzExtra);

	printf("Running go_peak3()...\n");
	go_peak3(relz, idem, relzExtra);

	printf("Running calcrelief3...\n");
	calcrelief3(dem, relz);


	//delete pSortedIndex;
	//delete pSortedIndex2;
}


/** Compute stream/channel related LSM relief quantities */
void FormProcess::go_stream1(DEM *dem, Relz *relz, int &numup, int &numdn)
{
	double str_elev = 0;
	double extra_z = 0;
	double current_n = 0;

	int nNumRec, nLoop = 0;

	int nColumn = 0;
	nNumRec = dem->getLength();

	//pDEMSeqNo = dem->SeqNo;
	//pDEMShednow = dem->ShedNow;
	//pUpslope = dem->UpSlope;
	//pDDir = dem->Ddir;
	//pDrec = dem->Drec;
	//pElev = dem->Elev;

	//pRelzSeqNo = relz->SeqNo;
	//pzpit2peak = relz->ZPIT2PEAK;
	//pz2pit = relz->Z2PIT;
	//pz2peak = relz->Z2PEAK;
	//relz->Z2ST = relz->Z2ST;
	//pz2cr = relz->Z2CR;
	//pz2top = relz->Z2TOP;
	//pzcr2st = relz->ZCR2ST;
	//pztop2pit = relz->ZTOP2PIT;

	//Initialize file object.
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		relz->SeqNo[nLoop] = dem->SeqNo[nLoop];
		relz->Z2ST[nLoop] = 0;
		relz->Z2CR[nLoop] = 0;
		relz->Z2PEAK[nLoop] = 0;
		relz->ZTOP2PIT[nLoop] = 0;
		relz->ZPIT2PEAK[nLoop] = 0;
		relz->ZCR2ST[nLoop] = 0;
		relz->Z2TOP[nLoop] = 0;
		//relz->Z2PIT[nLoop] = 0;
	}

	double *pSortValue;
	int *pSortedIndex;

	if (!(pSortValue = new double[nNumRec]))
	{
		printf("Not enough memory for pSortValue array, exiting ...");
		exit(1);
	}
	if (!(pSortedIndex = new int[nNumRec]))
	{
		printf("Not enough memory for pSortedIndex array, exiting ...");
		exit(1);
	}

	int nCounter = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			//pSortValue[nCounter] = dem->ShedNow[nLoop] * 10000000000000000 + ((100000 - dem->Elev[nLoop]) * 100000000000) + dem->UpSlope[nLoop];	//original
			pSortValue[nCounter] = dem->ShedNow[nLoop] * 100000000000000000 + ((100000 - dem->Elev[nLoop]) * 100000000000) + dem->UpSlope[nLoop];	//the factor for ShedNow is increased by a factor or 10, that solved a bug for some DEM
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int nIndex = 0;
	int start_rec = 0;

	bool bgo_dn = 0;
	double extraz = 0;
	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];

		if (dem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if (relz->Z2ST[nIndex] > 0 || dem->UpSlope[nIndex] >= dDSArea)
			{
				if (dem->UpSlope[nIndex] >= dDSArea)
				{
					relz->Z2ST[nIndex] = 0;

				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			extraz = 0;
			while (bgo_dn == true)
			{
				if (relz->Z2ST[nIndex] == 0 && dem->UpSlope[nIndex] < dDSArea)
				{
					if (dem->Ddir[nIndex] != 5)
					{
						numdn = numdn + 1;
						nIndex = dem->Drec[nIndex] - 1;
					}
					else
					{
						bgo_dn = false;
						extraz = 0;
						str_elev = dem->Elev[nIndex];
						relz->Z2ST[nIndex] = 0;
					}
				}
				else
				{
					if (dem->UpSlope[nIndex] >= dDSArea)
					{
						extraz = 0;
						str_elev = dem->Elev[nIndex];
						relz->Z2ST[nIndex] = 0;
					}
					else
					{
						extraz = relz->Z2ST[nIndex];
						str_elev = dem->Elev[nIndex] - extraz;
						if (str_elev > dem->Elev[nIndex])
						{
							str_elev = dem->Elev[nIndex];
						}
					}
					bgo_dn = false;
				}
			}

			if (numdn > 0)
			{
				nIndex = start_rec;
				for (int j = 0; j < numdn; j++)
				{
					if (dem->Elev[nIndex] > str_elev)
					{
						relz->Z2ST[nIndex] = dem->Elev[nIndex] - str_elev;
					}
					else
					{
						relz->Z2ST[nIndex] = 0;
					}
					nIndex = dem->Drec[nIndex] - 1;
				}
			}
		}
	}
	delete[] pSortedIndex;
}
void FormProcess::go_stream2(DEM *dem, Relz *relz)
{
	int numdn = 0;

	double str_elev = 0;
	double extra_z = 0;
	int current_n = 0;

	int nNumRec = dem->getLength();

	int nLoop = 0;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		//RelzFile.AddRecord();
		relz->SeqNo[nLoop] = dem->SeqNo[nLoop];
		relz->Z2ST[nLoop] = 0;
		relz->N2ST[nLoop] = 0;
	}

	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int[nNumRec];

	int nCounter = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			pSortValue[nCounter] = dem->ShedNow[nLoop] * 10000000000000000 + ((100000 - dem->Elev[nLoop]) * 100000000000) + dem->UpSlope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int nIndex = 0;
	int start_rec = 0;
	bool bgo_dn = 0;
	double extraz = 0;
	int extran = 0;

	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];

		if (dem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if (relz->Z2ST[nIndex] > 0 || dem->UpSlope[nIndex] >= dDSArea)
			{
				if (dem->UpSlope[nIndex] >= dDSArea)
				{
					relz->Z2ST[nIndex] = 0;
					relz->N2ST[nIndex] = 0;

				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			extraz = 0;
			extran = 0;
			while (bgo_dn == true)
			{
				if (relz->Z2ST[nIndex] == 0 && dem->UpSlope[nIndex] < dDSArea)
				{
					if (dem->Ddir[nIndex] != 5)
					{
						numdn = numdn + 1;
						nIndex = dem->Drec[nIndex] - 1;
					}
					else
					{
						bgo_dn = false;
						extraz = 0;
						extran = 0;
						str_elev = dem->Elev[nIndex];
						relz->Z2ST[nIndex] = 0;
						relz->N2ST[nIndex] = 0;

					}

				}
				else
				{
					if (dem->UpSlope[nIndex] >= dDSArea)
					{
						extraz = 0;
						extran = 0;
						str_elev = dem->Elev[nIndex];
						relz->Z2ST[nIndex] = 0;
						relz->N2ST[nIndex] = 0;
					}
					else
					{
						extraz = relz->Z2ST[nIndex];
						extran = relz->N2ST[nIndex];
						str_elev = dem->Elev[nIndex] - extraz;
						if (str_elev > dem->Elev[nIndex])
						{
							str_elev = dem->Elev[nIndex];
						}
					}
					bgo_dn = false;
				}
			}

			if (numdn > 0)
			{
				current_n = numdn + extran;

				nIndex = start_rec;
				for (int j = 0; j < numdn; j++)
				{
					if (dem->Elev[nIndex] > str_elev)
					{
						relz->Z2ST[nIndex] = dem->Elev[nIndex] - str_elev;
					}
					else
					{
						relz->Z2ST[nIndex] = 0;
					}
					relz->N2ST[nIndex] = current_n;
					nIndex = dem->Drec[nIndex] - 1;
					current_n = current_n - 1;
				}
			}
		}
	}
	delete[] pSortedIndex;
}
//
void FormProcess::go_stream3(DEM *dem, Relz *relz, RelzExtraForLen *relzExtra)
{
	int numdn = 0;

	double extra_z = 0;
	int current_n = 0;

	int nNumRec = dem->getLength();

	int nLoop = 0;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		//RelzFile.AddRecord();
		relz->SeqNo[nLoop] = dem->SeqNo[nLoop];
		relz->Z2ST[nLoop] = 0;
		relz->Z2CR[nLoop] = 0;
		relz->Z2PEAK[nLoop] = 0;
		relz->ZTOP2PIT[nLoop] = 0;
		relz->ZPIT2PEAK[nLoop] = 0;
		relz->ZCR2ST[nLoop] = 0;
		relz->Z2TOP[nLoop] = 0;
		relz->N2CR[nLoop] = 0;
		relz->N2ST[nLoop] = 0;
		relz->Z2PIT[nLoop] = 0;
	}

	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int[nNumRec];

	int nCounter = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			pSortValue[nCounter] = dem->ShedNo[nLoop] * 10000000000000000 + ((100000 - dem->Elev[nLoop]) * 100000000000) + dem->UpSlope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int nIndex = 0;
	int start_rec = 0;

	bool bgo_dn;
	double extraz = 0;
	int str_row = 0;
	int str_col = 0;
	double str_elev = 0;
	int extran = 0;
	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		if (dem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if (relz->Z2ST[nIndex] > 0 || dem->UpSlope[nIndex] >= dDSArea)
			{
				if (dem->UpSlope[nIndex] >= dDSArea && relz->Z2ST[nIndex] == 0)
				{
					str_row = dem->Row[nIndex];
					str_col = dem->Col[nIndex];
					str_elev = dem->Elev[nIndex];

					relzExtra->ST_ROW[nIndex] = str_row;
					relzExtra->ST_COL[nIndex] = str_col;
					//pSt_Elev[nIndex] = str_elev;

					relz->Z2ST[nIndex] = 0;
					relz->N2ST[nIndex] = 0;


				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			//extraz = 0;
			extran = 0;
			while (bgo_dn == true)
			{
				if (relz->Z2ST[nIndex] == 0 && dem->UpSlope[nIndex] < dDSArea)
				{
					if (dem->Ddir[nIndex] != 5)
					{
						numdn = numdn + 1;
						nIndex = dem->Drec[nIndex] - 1;
					}
					else
					{
						bgo_dn = false;
						extran = 0;

						str_row = dem->Row[nIndex];
						str_col = dem->Col[nIndex];
						str_elev = dem->Elev[nIndex];

						relzExtra->ST_ROW[nIndex] = str_row;
						relzExtra->ST_COL[nIndex] = str_col;
						/*pSt_Elev[nIndex] = str_elev;*/

						relz->Z2ST[nIndex] = 0;
						relz->N2ST[nIndex] = 0;



					}

				}
				else
				{
					if (dem->UpSlope[nIndex] >= dDSArea)
					{
						extran = 0;
						str_row = dem->Row[nIndex];
						str_col = dem->Col[nIndex];
						str_elev = dem->Elev[nIndex];

						relzExtra->ST_ROW[nIndex] = str_row;
						relzExtra->ST_COL[nIndex] = str_col;
						/*pSt_Elev[nIndex] = str_elev;*/

						relz->Z2ST[nIndex] = 0;
						relz->Z2ST[nIndex] = 0;
					}
					else
					{
						extran = relz->N2ST[nIndex];
						str_row = relzExtra->ST_ROW[nIndex];
						str_col = relzExtra->ST_COL[nIndex];
						str_elev = dem->Elev[nIndex];
						/*str_elev = pSt_Elev[nIndex];*/
					}
					bgo_dn = false;
				}
			}

			if (numdn > 0)
			{
				current_n = numdn + extran;
				nIndex = start_rec;
				for (int j = 0; j < numdn; j++)
				{
					if (dem->Elev[nIndex] > str_elev)
					{
						relz->Z2ST[nIndex] = dem->Elev[nIndex] - str_elev;
						relzExtra->ST_ROW[nIndex] = str_row;
						relzExtra->ST_COL[nIndex] = str_col;
						/*pSt_Elev[nIndex] = str_elev;*/

					}
					else
					{
						relz->Z2ST[nIndex] = 0;
						relzExtra->ST_ROW[nIndex] = str_row;
						relzExtra->ST_COL[nIndex] = str_col;
						/*pSt_Elev[nIndex] = str_elev;*/


					}
					relz->N2ST[nIndex] = current_n;
					current_n--;
					nIndex = dem->Drec[nIndex] - 1;
					if (nIndex == 0)
					{
						nIndex = 0;
					}
				}
			}
		}
	}
	delete[] pSortedIndex;
}
/** Compute pit-related LSM relief derivatives */
void FormProcess::go_pit1(DEM *dem, Pond *pond, Relz *relz)
{
	int nColumn = 0;

	if (pond->getLength() < 10)
	{
		printf("Too few elements in pond object in go_pit1(), exiting ...");
		exit(1);
	}

	int nNumRec = 0, nLoop = 0;


	nNumRec = dem->getLength();

	//pShedNo = dem->ShedNo;

	//pPitRow = pond->PitRow;
	//pPitCol = pond->PitCol;
	//pPitElev = pond->PitElev;
	//pPondShedNo = pond->ShedNo;

	//pz2pit = relz->Z2PIT;
	//relz->Z2ST = relz->Z2ST;
	//pz2cr = relz->Z2CR;

	//Had to do my own sorting, because the QSort is very slow if a lot of keys are the same.
	int* pSortValue, *pSortTemp, *pSortedIndex;

	if (!(pSortValue = new int[nNumRec]))
	{
		printf("Not enough memory for pSortedIndex array, exiting ...");
		exit(1);
	}
	if (!(pSortTemp = new int[nNumRec]))
	{
		printf("Not enough memory for pSortedIndex array, exiting ...");
		exit(1);
	}
	if (!(pSortedIndex = new int[nNumRec]))
	{
		printf("Not enough memory for pSortedIndex array, exiting ...");
		exit(1);
	}

	int nSmallest = 99999;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		pSortValue[nLoop] = dem->ShedNo[nLoop];
		//pSortedIndex[nLoop] = nLoop;
		if (nSmallest > dem->ShedNo[nLoop])
		{
			nSmallest = dem->ShedNo[nLoop];
		}
	}
	int nPointer = 0;
	int nNextHighest = 999999;
	bool bExit = false;
	while (bExit == false)
	{
		for (nLoop = 0; nLoop < nNumRec; nLoop++)
		{
			if (pSortValue[nLoop] == nSmallest)
			{
				pSortTemp[nPointer] = nLoop;
				nPointer++;
			}
			if (pSortValue[nLoop] < nNextHighest && pSortValue[nLoop]> nSmallest)
			{
				nNextHighest = pSortValue[nLoop];
			}
		}
		if (nNextHighest == nSmallest)
		{
			bExit = true;
		}
		nSmallest = nNextHighest;
		nNextHighest = 999999;
	}
	//QuickSort(pSortValue,nNumRec,pSortedIndex);

	delete[] pSortValue;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		pSortedIndex[nLoop] = pSortTemp[nLoop];
	}
	delete[] pSortTemp;
	int nIndex = 0;

	int nStart = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		if (dem->ShedNo[nIndex] > 0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}

	int this_shed = 0;
	int nLoop2 = 0;

	int nPitRow = 0;
	int nPitCol = 0;
	double dPitElev = 0;
	bExit = false;

	//int nPondFileSize = PondFile.GetNumRecords();

	int nPondFileSize = pond->getLength();

	for (nLoop = nStart; nLoop < nNumRec; nLoop++)
	{

		nIndex = pSortedIndex[nLoop];
		this_shed = dem->ShedNo[nIndex];

		for (nLoop2 = 0; nLoop2 < nPondFileSize; nLoop2++)
		{
			if (pond->ShedNo[nLoop2] == this_shed)
			{
				nPitRow = pond->PitRow[nLoop2];
				nPitCol = pond->PitCol[nLoop2];
				dPitElev = pond->PitElev[nLoop2];
			}
		}
		bExit = false;
		while (bExit == false)
		{

			if (dem->Elev[nIndex] != missing_value)
			{
				if (dem->Elev[nIndex] > dPitElev)
				{
					relz->Z2PIT[nIndex] = dem->Elev[nIndex] - dPitElev;
				}
				else
				{
					relz->Z2PIT[nIndex] = 0;
				}
			}
			nLoop = nLoop + 1;
			if (nLoop == nNumRec)
			{
				bExit = true;
			}
			else
			{
				nIndex = pSortedIndex[nLoop];
				if (dem->ShedNo[nIndex] != this_shed)
				{
					bExit = true;
					nLoop--;
				}
			}
		}
	}

	//	PondFile.Empty();
	delete[] pSortedIndex;

}

void FormProcess::go_pit3(DEM *dem, Pond *pond, Relz *relz, RelzExtraForLen *relzExtra)
{
	int nNumRec = dem->getLength();

	int nLoop = 0;

	int* pSortValue = new int[nNumRec];
	int* pSortTemp = new int[nNumRec];
	int* pSortedIndex = new int[nNumRec];
	int nSmallest = 99999;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		pSortValue[nLoop] = dem->ShedNo[nLoop];
		//pSortedIndex[nLoop] = nLoop;
		if (nSmallest > dem->ShedNo[nLoop])
		{
			nSmallest = dem->ShedNo[nLoop];
		}
	}

	int nPointer = 0;
	int nNextHighest = 999999;
	bool bExit = false;
	while (bExit == false)
	{
		for (nLoop = 0; nLoop < nNumRec; nLoop++)
		{
			if (pSortValue[nLoop] == nSmallest)
			{
				pSortTemp[nPointer] = nLoop;
				nPointer++;
			}
			if (pSortValue[nLoop] < nNextHighest && pSortValue[nLoop]> nSmallest)
			{
				nNextHighest = pSortValue[nLoop];
			}
		}
		if (nNextHighest == nSmallest)
		{
			bExit = true;
		}
		nSmallest = nNextHighest;
		nNextHighest = 999999;
	}

	delete[] pSortValue;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		pSortedIndex[nLoop] = pSortTemp[nLoop];
	}
	delete[] pSortTemp;
	int nIndex = 0;

	int nStart = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		if (dem->ShedNo[nIndex] > 0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}

	int this_shed = 0;
	int nLoop2 = 0;

	int nPitRow = 0;
	int nPitCol = 0;
	double dPitElev = 0;
	bExit = false;

	int nPondFileSize = pond->getLength();
	for (nLoop = nStart; nLoop < nNumRec; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		this_shed = dem->ShedNo[nIndex];
		for (nLoop2 = 0; nLoop2 < nPondFileSize; nLoop2++)
		{
			if (pond->ShedNo[nLoop2] == this_shed)
			{
				nPitRow = pond->PitRow[nLoop2];
				nPitCol = pond->PitCol[nLoop2];
				dPitElev = pond->PitElev[nLoop2];
				nLoop2 = nPondFileSize;
			}
		}
		bExit = false;
		while (bExit == false)
		{
			if (dem->Elev[nIndex] != missing_value)
			{
				relzExtra->PIT_ROW[nIndex] = nPitRow;
				relzExtra->PIT_COL[nIndex] = nPitCol;
				/*pRelzFilepit_elev[nIndex] = dPitElev;*/
				if (dem->Elev[nIndex] > dPitElev)
				{
					relz->Z2PIT[nIndex] = dem->Elev[nIndex] - dPitElev;
				}
			}
			nLoop = nLoop + 1;
			if (nLoop == nNumRec)
			{
				bExit = true;
			}
			else
			{
				nIndex = pSortedIndex[nLoop];
				if (dem->ShedNo[nIndex] != this_shed)
				{
					bExit = true;
					nLoop--;

				}
			}
		}
	}
	delete[] pSortedIndex;
}

/** Compute ridge-related LSM relief derivatives */
void FormProcess::go_ridge1(IDEM *idem, Relz *relz)
{
	int nNumRec, nLoop = 0;

	if (idem->getLength() < 5)
	{
		printf("Too few elements in idem object in go_ridge1(), exiting ...");
		exit(1);
	}

	printf("Running run go_ridge1() ...");


	//unsigned _int64 * pSortValue = new unsigned _int64[nNumRec];
	//int* pSortedIndex = new int[nNumRec];

	//for(nLoop=0;nLoop<nNumRec;nLoop++)
	//{
	//	pSortValue[nLoop] = pShedNow[nLoop] * 1000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
	//	pSortedIndex[nLoop] = nLoop;
	//}

	//QuickSort(pSortValue,nNumRec,pSortedIndex);

	//delete pSortValue;

	nNumRec = idem->getLength();

	//pDDir = idem->Ddir;
	//pDrec = idem->Drec;
	//pShedNow = idem->ShedNow;
	//pUpslope = idem->UpSlope;
	//pElev = idem->Elev;

	//pz2cr = relz->Z2CR;

	double cr_elev = 0;

	bool go_dn = false;

	int num_dn = 0;

	int nIndex = 0;
	int start_rec = 0;
	double extra_z = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		//nIndex = pSortedIndex[nLoop];	
		nIndex = nLoop;
		if (idem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if (relz->Z2CR[nIndex] > 0 || idem->UpSlope[nIndex] >= dDSArea)
			{
				if (idem->UpSlope[nIndex] >= dDSArea && relz->Z2CR[nIndex] == 0)
				{
					cr_elev = idem->Elev[nIndex];
					relz->Z2CR[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}

			extra_z = 0;
			while (go_dn == true)
			{
				if (relz->Z2CR[nIndex] == 0 && idem->UpSlope[nIndex] < dDSArea)
				{
					if (idem->Ddir[nIndex] != 5)
					{
						num_dn = num_dn + 1;
						nIndex = idem->Drec[nIndex] - 1;
					}
					else
					{
						go_dn = false;
						extra_z = 0;
						cr_elev = idem->Elev[nIndex];
						relz->Z2CR[nIndex] = 0;
					}
				}
				else
				{
					if (idem->UpSlope[nIndex] >= dDSArea)
					{
						extra_z = 0;
						cr_elev = idem->Elev[nIndex];
						relz->Z2CR[nIndex] = 0;
					}
					else
					{
						extra_z = relz->Z2CR[nIndex];
						cr_elev = idem->Elev[nIndex] - extra_z;
						if (cr_elev > idem->Elev[nIndex])
						{
							cr_elev = idem->Elev[nIndex];
						}
					}
					go_dn = false;
				}

			}

			if (num_dn > 0)
			{
				nIndex = start_rec;
				for (int j = 0; j < num_dn; j++)
				{
					if (idem->Elev[nIndex] > cr_elev)
					{
						relz->Z2CR[nIndex] = idem->Elev[nIndex] - cr_elev;
					}
					else
					{
						relz->Z2CR[nIndex] = 0;
					}
					nIndex = idem->Drec[nIndex] - 1;
				}
			}
			nIndex = start_rec;
		}
	}
}


void FormProcess::go_ridge3(IDEM *idem, Relz *relz, RelzExtraForLen *relzExtra)
{
	int nNumRec = idem->getLength();

	int nLoop = 0;

	double cr_elev = 0;

	bool go_dn = false;

	int num_dn = 0;

	int nIndex = 0;
	int start_rec = 0;
	double extra_z = 0;
	int cr_row = 0;
	int cr_col = 0;
	int chan_cell = 0;
	int extran = 0;
	int current_n = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		nIndex = nLoop;
		if (idem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if (relz->N2CR[nIndex] > 0 || idem->UpSlope[nIndex] >= dDSArea)
			{
				if (idem->UpSlope[nIndex] >= dDSArea && relz->Z2CR[nIndex] == 0)
				{
					cr_row = idem->Row[nIndex];
					cr_col = idem->Col[nIndex];
					/*cr_elev = pElev[nIndex];*/
					relz->Z2CR[nIndex] = 0;


					/*pcr_elev[nIndex] = cr_elev;*/
					relzExtra->CR_ROW[nIndex] = cr_row;
					relzExtra->CR_COL[nIndex] = cr_col;
					relz->N2CR[nIndex] = 0;


				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}

			extran = 0;
			chan_cell = 0;
			while (go_dn == true)
			{

				if (relz->Z2CR[nIndex] == 0 && idem->UpSlope[nIndex] < dDSArea)
				{
					if (idem->Ddir[nIndex] != 5)
					{
						num_dn = num_dn + 1;
						nIndex = idem->Drec[nIndex] - 1;
					}
					else
					{
						go_dn = false;
						extran = 0;

						cr_row = idem->Row[nIndex];
						cr_col = idem->Col[nIndex];
						cr_elev = idem->Elev[nIndex];
						relz->Z2CR[nIndex] = 0;
						relz->N2CR[nIndex] = 0;

						relzExtra->CR_ROW[nIndex] = cr_row;
						relzExtra->CR_COL[nIndex] = cr_col;
						/*pcr_elev[nIndex] = cr_elev;*/
					}
				}
				else
				{
					if (idem->UpSlope[nIndex] >= dDSArea)
					{
						extran = 0;
						cr_elev = idem->Elev[nIndex];
						cr_row = idem->Row[nIndex];
						cr_col = idem->Col[nIndex];
						relz->Z2CR[nIndex] = 0;
						relz->N2CR[nIndex] = 0;
						relzExtra->CR_ROW[nIndex] = cr_row;
						relzExtra->CR_COL[nIndex] = cr_col;
						/*pcr_elev[nIndex] = cr_elev;*/
					}
					else
					{
						extran = relz->N2CR[nIndex];
						/*cr_elev = pcr_elev[nIndex];*/
						cr_row = relzExtra->CR_ROW[nIndex];
						cr_col = relzExtra->CR_COL[nIndex];

					}
					go_dn = false;
				}

			}


			if (num_dn > 0)
			{
				current_n = num_dn + extran;
				nIndex = start_rec;
				for (int j = 0; j < num_dn; j++)
				{
					if (idem->Elev[nIndex] > cr_elev)
					{
						relz->Z2CR[nIndex] = idem->Elev[nIndex] - cr_elev;
						/*pcr_elev[nIndex] = cr_elev;*/
						relzExtra->CR_ROW[nIndex] = cr_row;
						relzExtra->CR_COL[nIndex] = cr_col;

					}
					else
					{
						relz->Z2CR[nIndex] = 0;
						/*pcr_elev[nIndex] = cr_elev;*/
						relzExtra->CR_ROW[nIndex] = cr_row;
						relzExtra->CR_COL[nIndex] = cr_col;
					}
					relz->N2CR[nIndex] = current_n;
					nIndex = idem->Drec[nIndex] - 1;
					current_n = current_n - 1;
				}
			}


			nIndex = start_rec;



		}

	}

}


/** Compute peak related LSM relief derivatives */
void FormProcess::up2peak1(IDEM *idem, Relz *relz)
{
	int nColumn = 0, nNumRec;

	nNumRec = idem->getLength();

	//pz2peak = relz->Z2PEAK;

	//pDDir = idem->Ddir;
	//pDrec = idem->Drec;
	//pShedNow = idem->ShedNow;
	//pUpslope = idem->UpSlope;
	//pElev = idem->Elev;

	double * pSortValue;
	int* pSortedIndex;

	if (!(pSortValue = new double[nNumRec]))
	{
		printf("Not enough memory for pSortValue array, exiting ...");
		exit(1);
	}
	if (!(pSortedIndex = new int[nNumRec]))
	{
		printf("Not enough memory for pSortedIndex array, exiting ...");
		exit(1);
	}
	int nLoop = 0;

	int nCounter = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (idem->Elev[nLoop] != missing_value)
		{
			pSortValue[nCounter] = idem->ShedNow[nLoop] * 10000000000000000 + ((100000 - idem->Elev[nLoop]) * 100000000000) + idem->UpSlope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int start_rec = 0;
	int num_dn = 0;
	bool go_dn = false;
	double extra_z = 0;

	double pk_elev = 0;
	int nIndex = 0;
	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{

		nIndex = pSortedIndex[nLoop];
		if (idem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if (relz->Z2PEAK[nIndex] > 0 || idem->Ddir[nIndex] == 5)
			{
				if (idem->Ddir[nIndex] == 5)
				{
					pk_elev = idem->Elev[nIndex];
					relz->Z2PEAK[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}
			extra_z = 0;

			while (go_dn)
			{
				if (relz->Z2PEAK[nIndex] == 0)
				{
					if (idem->Ddir[nIndex] != 5)
					{
						num_dn++;
						nIndex = idem->Drec[nIndex] - 1;
					}
					else
					{
						go_dn = false;
						extra_z = 0;
						pk_elev = idem->Elev[nIndex];
						relz->Z2PEAK[nIndex] = 0;
					}
				}
				else
				{
					extra_z = relz->Z2PEAK[nIndex];
					pk_elev = idem->Elev[nIndex] - extra_z;
					go_dn = false;
				}
			}

			if (num_dn > 0)
			{
				nIndex = start_rec;
				for (int j = 0; j < num_dn; j++)
				{
					if (idem->Elev[nIndex] > pk_elev)
					{
						relz->Z2PEAK[nIndex] = idem->Elev[nIndex] - pk_elev;
					}
					else
					{
						relz->Z2PEAK[nIndex] = 0;
					}
					nIndex = idem->Drec[nIndex] - 1;
				}
			}
		}
	}
	delete[] pSortedIndex;
}

/** Compute LSM relief quantities */
void FormProcess::calcrelief1(DEM *dem, Relz *relz)
{
	int nColumn = 0, nNumRec;

	double min_elev = 99999999;
	double max_elev = -1;


	/*	if (relz->getLength() != 62750)
			printf("relz is too short, in calcrelief1()!");

		if (dem->getLength() != 62750)

			printf("dem is too short in calcrelief1()!");
			*/

			/*
			nColumn = DEMFile.FindField("Elev");
			assert(nColumn!=-1);
			double* pElev = (double*) DEMFile.ReturnPointer(nColumn);

			int nNumRec = DEMFile.GetNumRecords();
			*/

			//pElev = dem->Elev;

	nNumRec = dem->getLength();

	int nLoop = 0;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] > max_elev)
		{
			max_elev = dem->Elev[nLoop];
		}

		if (dem->Elev[nLoop] < min_elev)
		{
			if (dem->Elev[nLoop] != missing_value)
			{
				min_elev = dem->Elev[nLoop];
			}
		}
	}

	double elev_range = max_elev - min_elev;

	//pShedNow = dem->ShedNow;

	//pUpslope = dem->UpSlope;

	double *pSortValue;
	int* pSortedIndex;

	if (!(pSortValue = new double[nNumRec]))
	{
		printf("Not enough memory for pSortValue array, exiting ...");
		exit(1);
	}
	if (!(pSortedIndex = new int[nNumRec]))
	{
		printf("Not enough memory for pSortValue array, exiting ...");
		exit(1);
	}

	for (nLoop = 0; nLoop < nNumRec; nLoop++) {

		pSortValue[nLoop] = 0.0;
		pSortedIndex[nLoop] = 0;
	}

	int nCounter = 0;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			pSortValue[nCounter] = dem->ShedNow[nLoop] * 10000000000000000.0 + ((100000.0 - dem->Elev[nLoop]) * 100000000000.0) + dem->UpSlope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int nIndex = 0;

	int nStart = 0;

	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{
		if (dem->ShedNow[pSortedIndex[nLoop]] > 0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}


	/*	pzpit2peak = relz->ZPIT2PEAK;
		pz2pit = relz->Z2PIT;
		pz2peak = relz->Z2PEAK;
		relz->Z2ST = relz->Z2ST;
		pz2cr = relz->Z2CR;
		pz2top = relz->Z2TOP;
		pzcr2st = relz->ZCR2ST;
		pztop2pit = relz->ZTOP2PIT;

		ppctz2top = relz->PCTZ2TOP;
		ppctz2st = relz->PCTZ2ST;
		ppctz2pit = relz->PCTZ2PIT;
		ppmin2max = relz->PMIN2MAX;
		*/

		//relz->writeCSV("relz2.csv");

	int this_shed = 0;
	double top_elev = 0;
	bool go_dn = false;
	for (nLoop = nStart; nLoop < nCounter; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];

		this_shed = dem->ShedNow[nIndex];

		top_elev = dem->Elev[nIndex];

		if (dem->Elev[nIndex] == missing_value)
		{
			//nLoop = nNumRec;
		}
		else
		{
			go_dn = true;

			while (go_dn == true)
			{
				relz->ZPIT2PEAK[nIndex] = relz->Z2PIT[nIndex] + relz->Z2PEAK[nIndex];

				relz->ZCR2ST[nIndex] = relz->Z2CR[nIndex] + relz->Z2ST[nIndex];

				relz->Z2TOP[nIndex] = top_elev - dem->Elev[nIndex];

				relz->ZTOP2PIT[nIndex] = relz->Z2TOP[nIndex] + relz->Z2PIT[nIndex];

				if (relz->ZTOP2PIT[nIndex] > 0)
				{
					relz->PCTZ2TOP[nIndex] = (int)(100 - (relz->Z2TOP[nIndex] / relz->ZTOP2PIT[nIndex]) * 100);
				}
				else
				{
					relz->PCTZ2TOP[nIndex] = 0;
				}

				if (relz->ZCR2ST[nIndex] > 0)
				{
					relz->PCTZ2ST[nIndex] = (int)(relz->Z2ST[nIndex] / relz->ZCR2ST[nIndex] * 100);
				}
				else
				{
					relz->PCTZ2ST[nIndex] = 0;
				}

				if (relz->ZPIT2PEAK[nIndex] > 0)
				{
					relz->PCTZ2PIT[nIndex] = (int)((relz->Z2PIT[nIndex] / relz->ZPIT2PEAK[nIndex]) * 100);
				}
				else
				{
					relz->PCTZ2PIT[nIndex] = 0;
				}

				relz->PMIN2MAX[nIndex] = (int)(((dem->Elev[nIndex] - min_elev) / elev_range) * 100);
				///REPLACE b.pmin2max WITH INT(((elev - min_elev)/ elev_range)*100)

				nLoop++;

				if (nLoop < nCounter)
				{
					nIndex = pSortedIndex[nLoop];

					if (dem->ShedNow[nIndex] != this_shed)
					{
						go_dn = false;
						nLoop--;
					}
				}
				else
				{
					go_dn = false;
				}
			}
		}
	}
	delete[] pSortedIndex;
}

void FormProcess::calcrelief3(DEM *dem, Relz* relz)
{
	double min_elev = 99999999;
	double max_elev = -1;


	int nNumRec = dem->getLength();

	int nLoop = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] > max_elev)
		{
			max_elev = dem->Elev[nLoop];
		}

		if (dem->Elev[nLoop] < min_elev)
		{
			if (dem->Elev[nLoop] != missing_value)
			{
				min_elev = dem->Elev[nLoop];
			}
		}
	}


	double elev_range = max_elev - min_elev;


	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int[nNumRec];

	int nCounter = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			pSortValue[nCounter] = dem->ShedNo[nLoop] * 10000000000000000 + ((100000 - dem->Elev[nLoop]) * 100000000000) + dem->UpSlope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue, nCounter, pSortedIndex);

	delete[] pSortValue;

	int nIndex = 0;

	int nStart = 0;
	for (nLoop = 0; nLoop < nCounter; nLoop++)
	{
		if (dem->ShedNow[pSortedIndex[nLoop]] > 0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}

	//Is this part necessary?
	/*for (nLoop = 0;nLoop<nNumRec;nLoop++)
	{
		if (pElev[nLoop] != missing_value)
		{
			pcr_elev[nLoop] = max_elev - pcr_elev[nLoop];
			ppk_elev[nLoop] = max_elev - ppk_elev[nLoop];
		}
		else
		{
			pcr_elev[nLoop] = 0;
			ppk_elev[nLoop] = 0;
		}
	}
	*/

	int this_shed = 0;
	double top_elev = 0;
	bool go_dn = false;
	for (nLoop = nStart; nLoop < nCounter; nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		//nIndex = nLoop;

		this_shed = dem->ShedNow[nIndex];
		top_elev = dem->Elev[nIndex];

		if (dem->Elev[nIndex] == missing_value)
		{
			//nLoop = nNumRec;
		}
		else
		{
			go_dn = true;
			while (go_dn == true)
			{
				relz->ZPIT2PEAK[nIndex] = relz->Z2PIT[nIndex] + relz->Z2PEAK[nIndex];
				relz->ZCR2ST[nIndex] = relz->Z2CR[nIndex] + relz->Z2ST[nIndex];
				//pzcr2pit[nIndex] = pz2cr[nIndex] + pz2pit[nIndex];
				relz->Z2TOP[nIndex] = top_elev - dem->Elev[nIndex];
				relz->ZTOP2PIT[nIndex] = relz->Z2TOP[nIndex] + relz->Z2PIT[nIndex];
				//pncr2st[nIndex] = pn2cr[nIndex] + pn2st[nIndex];

				if (relz->ZTOP2PIT[nIndex] > 0)
				{
					relz->PCTZ2TOP[nIndex] = (short int)(100 - (relz->Z2TOP[nIndex] / relz->ZTOP2PIT[nIndex]) * 100);
				}
				else
				{
					relz->PCTZ2TOP[nIndex] = 0;
				}

				if (relz->ZCR2ST[nIndex] > 0)
				{
					relz->PCTZ2ST[nIndex] = (short int)((relz->Z2ST[nIndex] / relz->ZCR2ST[nIndex]) * 100);
				}
				else
				{
					relz->PCTZ2ST[nIndex] = 0;
				}

				if (relz->ZPIT2PEAK[nIndex] > 0)
				{
					relz->PCTZ2PIT[nIndex] = (short int)(((relz->Z2PIT[nIndex] / relz->ZPIT2PEAK[nIndex]) * 100));
				}
				else
				{
					relz->PCTZ2PIT[nIndex] = 0;
				}

				//Do we need this part?
				/*if (pncr2st[nIndex] > 0)
				{
					ppctn2st[nIndex] = (short int)((pn2st[nIndex] / pncr2st[nIndex]) * 100);
				}
				else
				{
					ppctn2st[nIndex] = 0;
				}*/
				relz->PMIN2MAX[nIndex] = (short int)(((dem->Elev[nIndex] - min_elev) / elev_range) * 100);

				nLoop++;
				if (nLoop < nCounter)
				{
					nIndex = pSortedIndex[nLoop];

					if (dem->ShedNow[nIndex] != this_shed)
					{
						go_dn = false;
						nLoop--;
					}
				}
				else
				{
					go_dn = false;
				}
			}
		}
	}
	delete[] pSortedIndex;

}
void FormProcess::go_peak3(Relz *relz, IDEM* idem, RelzExtraForLen* relzExtra)
{
	int nNumRec = idem->getLength();

	int start_rec = 0;
	int num_dn = 0;
	bool go_dn = false;
	double extra_z = 0;

	int extran = 0;
	int current_n = 0;

	double pk_elev = 0;
	int nIndex = 0;

	int pk_row = 0;
	int pk_col = 0;

	int nLoop = 0;

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		nIndex = nLoop;
		if (idem->Elev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if (relz->Z2PEAK[nIndex] > 0 || idem->Ddir[nIndex] == 5)
			{
				if (idem->Ddir[nIndex] == 5)
				{
					pk_row = idem->Row[nIndex];
					pk_col = idem->Col[nIndex];
					pk_elev = idem->Elev[nIndex];



					relz->Z2PEAK[nIndex] = 0;
					relz->N2PEAK[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}
			extran = 0;

			while (go_dn)
			{
				if (relz->Z2PEAK[nIndex] == 0)
				{
					if (idem->Ddir[nIndex] != 5)
					{
						num_dn++;
						nIndex = idem->Drec[nIndex] - 1;
					}
					else
					{
						go_dn = false;
						extran = 0;

						pk_elev = idem->Elev[nIndex];
						pk_row = idem->Row[nIndex];
						pk_col = idem->Col[nIndex];

						/*ppk_elev[nIndex] = pk_elev;*/
						relzExtra->PK_ROW[nIndex] = pk_row;
						relzExtra->PK_COL[nIndex] = pk_col;


						relz->Z2PEAK[nIndex] = 0;
						relz->N2PEAK[nIndex] = 0;
					}
				}
				else
				{
					extran = relz->N2PEAK[nIndex];
					pk_row = relzExtra->PK_ROW[nIndex];
					pk_col = relzExtra->PK_COL[nIndex];
					/*pk_elev = ppk_elev[nIndex];*/

					go_dn = false;
				}

			}
			if (num_dn > 0)
			{
				current_n = num_dn + extran;
				nIndex = start_rec;
				for (int j = 0; j < num_dn; j++)
				{
					if (idem->Elev[nIndex] > pk_elev)
					{
						relzExtra->PK_ROW[nIndex] = pk_row;
						relzExtra->PK_COL[nIndex] = pk_col;
						//ppk_elev[nIndex] = pk_elev;

						relz->Z2PEAK[nIndex] = idem->Elev[nIndex] - pk_elev;
					}
					else
					{
						relzExtra->PK_ROW[nIndex] = pk_row;
						relzExtra->PK_COL[nIndex] = pk_col;
						//ppk_elev[nIndex] = pk_elev;
						relz->Z2PEAK[nIndex] = 0;
					}
					relz->N2PEAK[nIndex] = current_n;

					nIndex = idem->Drec[nIndex] - 1;
					current_n--;
				}
			}

		}
	}

}
void FormProcess::calc_len3(Len* len, Relz * relz, DEM* dem, RelzExtraForLen* relzExtra)
{
	int nNumRec = dem->getLength();

	int nLoop = 0;

	int lastrec = nNumRec;
	int a_seqno = relz->SeqNo[nNumRec - 1];

	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		len->SeqNo[nLoop] = relz->SeqNo[nLoop];

	}

	int thisrow = 0;
	int thiscol = 0;

	double len2pit = 0;
	double len2peak = 0;
	double pit2peakl = 0;
	double len2str = 0;
	double len2div = 0;
	double str2divl = 0;
	for (nLoop = 0; nLoop < nNumRec; nLoop++)
	{
		if (dem->Elev[nLoop] != missing_value)
		{
			thisrow = dem->Row[nLoop];
			thiscol = dem->Col[nLoop];

			if (thisrow == relzExtra->PIT_ROW[nLoop] && thiscol == relzExtra->PIT_COL[nLoop])
			{
				len2pit = 0;
			}
			else
			{
				len2pit = (sqrt((relzExtra->PIT_COL[nLoop] - thiscol)*(relzExtra->PIT_COL[nLoop] - thiscol) + (relzExtra->PIT_ROW[nLoop] - thisrow)*(relzExtra->PIT_ROW[nLoop] - thisrow))) * dGridSize;
				len2pit = sqrt(len2pit * len2pit + (relz->Z2PIT[nLoop] * dGridSize)*(relz->Z2PIT[nLoop] * dGridSize));
			}

			if (thisrow == relzExtra->PK_ROW[nLoop] && thiscol == relzExtra->PK_COL[nLoop])
			{
				len2peak = 0;
			}
			else
			{
				len2peak = sqrt((relzExtra->PK_COL[nLoop] - thiscol)* (relzExtra->PK_COL[nLoop] - thiscol) + (relzExtra->PK_ROW[nLoop] - thisrow) * (relzExtra->PK_ROW[nLoop] - thisrow)) * dGridSize;
				len2peak = sqrt(len2peak * len2peak + (relz->Z2PEAK[nLoop] * dGridSize)*(relz->Z2PEAK[nLoop] * dGridSize));
			}

			pit2peakl = len2peak + len2pit;
			len->L2PIT[nLoop] = len2pit;
			len->L2PEAK[nLoop] = len2peak;
			len->LPIT2PEAK[nLoop] = pit2peakl;

			if (pit2peakl > 0)
			{
				len->PPIT2PEAKL[nLoop] = (short int)((len2pit / pit2peakl) * 100);
			}
			else
			{
				len->PPIT2PEAKL[nLoop] = 0;
			}

			if (thisrow == relzExtra->ST_ROW[nLoop] && thiscol == relzExtra->ST_COL[nLoop])
			{
				len2str = 0;
			}
			else
			{
				len2str = (sqrt((relzExtra->ST_COL[nLoop] - thiscol) * (relzExtra->ST_COL[nLoop] - thiscol) + (relzExtra->ST_ROW[nLoop] - thisrow)* (relzExtra->ST_ROW[nLoop] - thisrow))) * dGridSize;
				len2str = sqrt((len2str * len2str) + (relz->Z2ST[nLoop] * dGridSize) *(relz->Z2ST[nLoop] * dGridSize));
			}

			if (thisrow == relzExtra->CR_ROW[nLoop] && thiscol == relzExtra->CR_COL[nLoop])
			{
				len2div = 0;
			}
			else
			{
				len2div = sqrt((relzExtra->CR_COL[nLoop] - thiscol) * (relzExtra->CR_COL[nLoop] - thiscol) + (relzExtra->CR_ROW[nLoop] - thisrow) * (relzExtra->CR_ROW[nLoop] - thisrow)) * dGridSize;
				len2div = sqrt(len2div * len2div + (relz->Z2PEAK[nLoop] * dGridSize) * (relz->Z2PEAK[nLoop] * dGridSize));
			}


			str2divl = len2str + len2div;

			len->L2STR[nLoop] = len2str;
			len->L2DIV[nLoop] = len2div;
			len->LSTR2DIV[nLoop] = str2divl;
			if (str2divl > 0)
			{
				len->PSTR2DIVL[nLoop] = (short int)((len2str / str2divl) * 100);
			}
			else
			{
				len->PSTR2DIVL[nLoop] = 0;
			}
		}
		else
		{
			len->L2PIT[nLoop] = 0;
			len->L2PEAK[nLoop] = 0;
			len->LPIT2PEAK[nLoop] = 0;
			len->PPIT2PEAKL[nLoop] = 0;
			len->L2STR[nLoop] = 0;
			len->L2DIV[nLoop] = 0;
			len->LSTR2DIV[nLoop] = 0;
			len->PSTR2DIVL[nLoop] = 0;

		}
	}
}

