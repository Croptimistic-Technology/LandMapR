/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
\brief Formprocess class definition
*/
#ifndef _FORMPROCESS_H_
#define _FORMPROCESS_H_
#include <iostream>
#include <string>
#include <memory>
#include "idem.h"
#include "dem.h"
//#include "pit.h"
//#include "ipit.h"
#include "form.h"
#include "relz.h"
#include "len.h"
#include "pond.h"
#include "RelzExtraForLen.h"
/** The FormProcess Class definition */
class FormProcess
{
public:

	void calc_len3(Len* len, Relz * relz, DEM* dem, RelzExtraForLen* relzExtra);
	void calcrelief1(DEM *dem, Relz *relz);
	/// <summary>
	/// Populate Relz object with relief derivatives (LSM)
	/// </summary>
	void calcrelief3(DEM *dem, Relz* relz);
	void go_peak3(Relz *relz, IDEM* idem, RelzExtraForLen* relzExtra);
	void go_ridge1(IDEM *idem, Relz *relz);
	/// <summary>
	/// Compute ridge-related relief quantities (LSM)
	/// </summary>
	void go_ridge3(IDEM *idem, Relz *relz, RelzExtraForLen *relzExtra);
	void go_pit1(DEM *dem, Pond *pond, Relz *relz);
	/// <summary>
	/// Compute pit-related relief qunatities (LSM)
	/// </summary>
	void go_pit3(DEM *dem, Pond *pond, Relz *relz, RelzExtraForLen *relzExtra);
	void go_stream1(DEM *dem, Relz *relz, int &numup, int &numdn);	
	void go_stream2(DEM *dem, Relz *relz);
	/// <summary>
	/// Compute channel/stream -related quantities (LSM)
	/// Procedure to go to each grid cell starting at the highest cell in each watershed(the peak cell) and to follow a flow path from each cell DOWNSLOPE from it to the first channel cell to which it is connected by downstream
	/// </summary>
	void go_stream3(DEM *dem, Relz *relz, RelzExtraForLen* relzExtra);
	/// <summary>
	/// Compute relief quantities (LSM)
	/// </summary>
	void fcalcRelz1(DEM *dem, Pond *pond, IDEM *idem, Relz *relz);	
	/// <summary>
	/// Compute relief quantities (BC-PEM)
	/// </summary>
	void fcalcRelz2(DEM *dem, Relz *relz);
	/// <summary>
	/// compute relief quantities (full set)
	/// </summary>
	void fcalcRelz3(DEM *dem, Pond *pond, Relz *relz, Len *len, IDEM *idem, RelzExtraForLen *relzExtra);
	/// <summary>
	/// Compute Form and Relz objects from given outputs of flow topology objects (LSM)
	/// </summary>
	bool fprocess1(DEM *dem, Pond *pond, Relz *relz, IDEM *idem, Form *form, string m_WorkingDirectory);	
	/// <summary>
	/// Compute Form and Relz objects from given outputs of flow topology objects (BC-PEM)
	/// </summary>
	bool fprocess2(DEM *dem, Relz *relz, Form *form, string m_WorkingDirectory);
	/// <summary>
	/// Compute a full set of all possible derivatives
	/// </summary>
	bool fprocess3(string& resultJsonPath, DEM *dem, Pond *pond, Relz *relz, IDEM *idem, Form *form, Len *len, RelzExtraForLen *relzExtra, string m_WorkingDirectory, int numRows, int numCols, double xll, double yll);
	/// <summary>
	/// Compute peak-related derivatives  (LSM)
	/// </summary>
	void up2peak1(IDEM *idem, Relz *relz); 
	/// <summary>
	/// Calculate Qarea and Wetting indices according to Quinn et al. (1991).
	/// </summary>
	void calc_quinn(int numneigh, double cent_z, double cent_qa, int* zrec, double* deltax, double* pElev,
		int* pDDir, int* pDrec, int centrec, double* l, double* pQArea, double* pQweti);	
	/// <summary>
	/// Find relative elevations of neighbouring cells for \latexonly\citet{quinn91}\endlatexonly  algorithm for Qarea and Wetting index
	/// </summary>
	void qfindneig(int cent_x, int cent_y, double cent_z, double cent_qa, int& numneigh, int* zrec, double* l,
		double* deltax, double orthogonal, double diagonal, int maxcols, int maxrows, double l1, double l2);
	/// <summary>
	/// Calculate the wetting area (LSM)
	/// </summary>
	void fcalcweti(DEM *dem, Form *form, int nNum);	
	void pad_edges(double* pSlope, int* pAspect, double* pProf, double* pPlan, int no_cols, int no_rows);
	double max_runoff;
	double max_vol;
	double max_area;
	double max_depth;
	/// <summary>
	/// Numerical value of a pseudo elevation value for grid cells outside the bounary of region of interest
	/// </summary>
	double missing_value;	
	/// <summary>
	/// Compute gradient, aspect, profile and plan curvature according to \latexonly\citet{eyton91}\endlatexonly
	/// </summary>
	void eyton_form(double* row1z, double* row2z, double* row3z, int this_col, double* pSlope, int* pAspect, double* pProf, double* pPlan, int nIndex); 
	/// <summary>
	/// Eyton form processing wrapper method
	/// </summary>
	void fcalcForm(DEM *dem, Form *form);	
	/// <summary>
	/// Threshold upslope area for downslope flow
	/// </summary>
	double dDSArea;	
	/// <summary>
	/// Threshold upslope area for "upslope flow"
	/// </summary>
	double dUpslopeArea;	
	/// <summary>
	/// Grid Size in m
	/// </summary>
	double dGridSize;	
	/// <summary>
	/// Default constructor
	/// </summary>
	FormProcess();	
	/// <summary>
	/// Default destructor
	/// </summary>
	virtual ~FormProcess();	
};

#endif 
