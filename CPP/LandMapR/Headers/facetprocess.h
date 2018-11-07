/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* \brief
FacetProcess.h: interface for the facetprocess class.
*/
#ifndef _FACETPROCESS_H_
#define _FACETPROCESS_H_

#include "dbffile.h"
#include <string>
#include <cassert>
#include "dem.h"
#include "form.h"
#include "relz.h"
#include "arule.h"
#include "crule.h"
#include "clsm.h"

class FacetProcess
{
public:

	/** A fuzzy classification function for the condensed LSM mode of output (for each grid cell).
	*
	* @param no_models Number of fuzzy models used in the fuzzy attributes file (it counts those models based on all attribute source files)
	* @param nIndex The index of the current grid cell (in the grid) for which classification is being carried out
	* @param infield Array holding pointers to the input field arrayss
	* @param model	One of the five fuzzy classification rules model as given in \latexonly\citet{macmillan03}\endlatexonly
	* @param B Array to hold values of B corresponding to each fuzzy classification model
	* @param b_low Array to hold values of b_low as given in the fuzzy classification model
	* @param b_hi Array to hold values of the b_hi values as given in the fuzzy clasification model
	* @param b1 Array to hold values of the b1 values as given in the fuzzy classification model
	* @param b2 Array to hold values of the b2 values as given in the fuzzy classification model
	* @param D Array to hold values of the dispersion coeffcient as given in the classification model
	* @param outattr Array to hold output attributes
	* @param Max_Code A dynamic integer array to hold value that identify the most likely fuzzy out put class \latexonly\citet{macmillan03}\endlatexonly
	* @param Max_Class A dynamic array of strings to hold alphanumeric code of the landform class with the highest fuzzy membership for the given grid cells
	* @param Max_Value A dynamic array to hold the highest fuzzy membership value corresponding to a grid cell
	* @param attr_wt Array to hold attribute weights, given in the classification rules file
	* @param fuzzy_attr Array to hold attribute names such as hi_above, near_level, etc
	* @param no_facets Equivalent to the number of entries in the classification rules file (cf. length), it is a legacy class attribute
	* @param Facet_Name Array to hold facet names (facet names such as FSL, TSL, LSM, etc)
	* @param Facet_Code Array to hold facet codes (integers)
	* @param fuzz_attrIndex  Fuzzy attributes index
	* @param no_attrib Array to hold attribute numbers
	* @param dim Count of input attributes
	*/
	void calc_class3(int no_models, int nIndex, double** infield, int* model, double* B, double* b_low, double* b_hi, double* b1, double* b2, double *D, std::string* outattr, double &Max_Code, std::string &Max_Class, double &Max_Value, double* attr_wt, std::string* fuzz_attr, int no_facets, std::string* Facet_Name, int* Facet_Code, int* fuzz_attrIndex, int* no_attrib, int dim);

	/** Wrapper around calc_class3(), to compute fuzzy classification of landform in the condensed LSM mode.
	*
	* @param dem, the DEM object as computed by the flow topography module
	* @param form, the Form object as computed by the terrain derivative computing module
	* @param Relz the relief object that is computed by the terrain derivative computing module
	* @param arule the fuzzy rules attribute (Arule) object
	* @param crule the fuzzy classification rules (Crule) object
	*/
	Clsm *calc_lsm(DEM *dem, Form *form, Relz *relz, Arule *arule, Crule *crule);
	/// <summary>
	/// Facet process method
	/// </summary>
	FacetProcess();
	/// <summary>
	/// Destructor
	/// </summary>
	virtual ~FacetProcess();
};

#endif 
