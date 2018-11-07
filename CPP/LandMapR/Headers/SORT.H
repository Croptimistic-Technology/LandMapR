/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SORT_H_
#define _SORT_H_

#include <iostream>
#include <fstream>
#include <omp.h>
//#include "logger.h"

#define low_limit 100

using namespace std;


/// <summary>
/// Quicksort implentation #1. most preferred implemantation of Quicksort (De Sande, 2004), omp parallelized 
/// </summary>
template <typename T>
void QuickSortSande(T *v, int *tempSort, int first, int last, bool ascending = true) {
	int start[2], end[2], i, tempi;
	T temp, pivot;

	if (first < last) {
		start[1] = first;
		end[0] = last;
		pivot = v[(first + last) / 2];
		while (start[1] <= end[0]) {
			while (v[start[1]] < pivot)
				start[1]++;
			while (pivot < v[end[0]])
				end[0]--;
			if (start[1] <= end[0]) {

				temp = v[start[1]];
				v[start[1]] = v[end[0]];
				v[end[0]] = temp;

				tempi = tempSort[start[1]];
				tempSort[start[1]] = tempSort[end[0]];
				tempSort[end[0]] = tempi;

				start[1]++;
				end[0]--;
			}
		}
		start[0] = first;
		end[1] = last;

#pragma omp parallel 
		{
#pragma omp for nowait
			for (i = 0; i <= 1; i++) {
				QuickSortSande(v, tempSort, start[i], end[i], ascending);
			}
		}
	}
}

template <typename T>
void QuickSort(T *data, int n, int *order, bool ascending = true)
{
	omp_set_dynamic(0);
#pragma omp parallel num_threads(2)
	{
#pragma omp single nowait 
		QuickSortSande(&data[0], order, 0, n - 1, ascending);
	}
}



//Quicksort implementation #2: (original Quicksort implementation) could be used in case a serial implementation is required (to acommodate very large DEM files)
/*
QuickSortRecursive - core of algorithm, do not call it, use QuickSort */
/*
void QuickSortRecursiveD(double *pArr, int *piOrder, int d, int h, bool ascendOrdescend)
{

int i, j;
double str;

i = h;
j = d;

str = pArr[((int)((d + h) / 2))];

do {

if (ascendOrdescend) {		//ascending
while (pArr[j] < str) j++;
while (pArr[i] > str) i--;
}
else {
while (pArr[j] > str) j++;
while (pArr[i] < str) i--;
}

if (i >= j) {

if (i != j) {
double zal;
int iTmp;

zal = pArr[i];
pArr[i] = pArr[j];
pArr[j] = zal;

iTmp = piOrder[i];
piOrder[i] = piOrder[j];
piOrder[j] = iTmp;
}

i--;
j++;
}
} while (j <= i);

if (d < i)
QuickSortRecursiveD(pArr, piOrder, d, i, ascendOrdescend);
if (j < h)
QuickSortRecursiveD(pArr, piOrder, j, h, ascendOrdescend);
}


bool QuickSortD(double *pArr, int iSize, int *piOrder, bool bAscending)
{

bool rc = true;
int		i, j, k, l;
double		tTmp;
int		iTmp;

if (iSize > 1) {

try {

int	low = 0,
high = iSize - 1;

#pragma omp parallel
{
#pragma omp single nowait
QuickSortRecursiveD(pArr, piOrder, low, high, bAscending);
}

j = 1;
tTmp = pArr[0];
for (i = 1; i < iSize; i++)
{
if (pArr[i] != tTmp)
{
for (k = i - j; k < i - 1; k++)
{
for (l = k + 1; l < i; l++)
{
if (piOrder[l] > piOrder[k])
{
iTmp = piOrder[l];
piOrder[l] = piOrder[k];
piOrder[k] = iTmp;
}
}
}
j = 1;
tTmp = pArr[i];
}
else
j++;

}
if (j > 1)
{
for (k = i - j; k < i - 1; k++)
{
for (l = k + 1; l < i; l++)
{
if (piOrder[l] > piOrder[k])
{
iTmp = piOrder[l];
piOrder[l] = piOrder[k];
piOrder[k] = iTmp;
}
}
}
}

}
catch (...) {
Logger::logMessage("Unable to complete Quicksort().\n");
rc = false;
}

}
else {
//if (!singleValOk)
//{
//Logger::logMessage("Invalid parameter in Quicksort() for iSize <= 1.\n");
//}

rc = false;
}

return rc;
}


//int version

void QuickSortRecursiveI(int *pArr, int *piOrder, int d, int h, bool ascendOrdescend)
{

int i, j;
int str;

i = h;
j = d;

str = pArr[((int)((d + h) / 2))];

do {

if (ascendOrdescend) {			//ascending
while (pArr[j] < str) j++;
while (pArr[i] > str) i--;
}
else {
while (pArr[j] > str) j++;
while (pArr[i] < str) i--;
}

if (i >= j) {

if (i != j) {
int zal;
int iTmp;

zal = pArr[i];
pArr[i] = pArr[j];
pArr[j] = zal;

iTmp = piOrder[i];
piOrder[i] = piOrder[j];
piOrder[j] = iTmp;
}

i--;
j++;
}
} while (j <= i);

if (d < i) QuickSortRecursiveI(pArr, piOrder, d, i, ascendOrdescend);
if (j < h) QuickSortRecursiveI(pArr, piOrder, j, h, ascendOrdescend);
}

//int version
bool QuickSortI(int *pArr, int iSize, int *piOrder, bool bAscending/* = true, bool singleValOk = true*//*)
{

bool rc = true;
int		i, j, k, l;
int		tTmp;
int		iTmp;

if (iSize > 1) {

try {

int	low = 0,
high = iSize - 1;

QuickSortRecursiveI(pArr, piOrder, low, high, bAscending);

j = 1;
tTmp = pArr[0];
for (i = 1; i < iSize; i++)
{
if (pArr[i] != tTmp)
{
for (k = i - j; k < i - 1; k++)
{
for (l = k + 1; l < i; l++)
{
if (piOrder[l] > piOrder[k])
{
iTmp = piOrder[l];
piOrder[l] = piOrder[k];
piOrder[k] = iTmp;
}
}
}
j = 1;
tTmp = pArr[i];
}
else
j++;

}
if (j > 1)
{
for (k = i - j; k < i - 1; k++)
{
for (l = k + 1; l < i; l++)
{
if (piOrder[l] > piOrder[k])
{
iTmp = piOrder[l];
piOrder[l] = piOrder[k];
piOrder[k] = iTmp;
}
}
}
}

}
catch (...) {
Logger::logMessage("Unable to complete Quicksort().\n");
rc = false;
}

}
else {
//if (!singleValOk)
//{
//Logger::logMessage("Invalid parameter in Quicksort() for iSize <= 1.\n");
//}

rc = false;
}

return rc;
}
*/






//Quicksort implementation #3 (parallel)

//Taken from a solaris studio documetation
//works but crashes for large DEM files.

/*
//works for farm-plots okay, but crashes for Baffin Island 400 m x 400 m grid

template <typename T>
void seq_quick_sort(T *data, int *order, int p, int r, bool ascending = true)
{
if (p < r) {
int q = partition(data, order, p, r, ascending);
seq_quick_sort(data, order, p, q - 1, ascending);
seq_quick_sort(data, order, q + 1, r, ascending);
}
}

template <typename T>
void quick_sort(T *data, int *order, int p, int r, bool ascending = true)
{
if (p < r) {
if ((r - p) < low_limit) {
seq_quick_sort(data, order, p, r, ascending);
}
else {
int q = partition(data, order, p, r, ascending);
#pragma omp task firstprivate(data, order, r, q)
quick_sort(data, order, p, q - 1, ascending);
#pragma omp task firstprivate(data, order, r, q)
quick_sort(data, order, q + 1, r, ascending);
}
}
}

//template <typename T> void par_quick_sort(T *data, int *order, int n, int low_limit, bool ascending)
template <typename T>
void QuickSort(T *data,  int n, int *order, bool ascending = true)
{
omp_set_dynamic(0);
#pragma omp parallel num_threads(6)
{
#pragma omp single nowait
quick_sort(&data[0], order, 0, n - 1, ascending);
//quick_sort(data, order, 0, n - 1, ascending);
}
}

template <typename T>
int partition(T *data, int *order, int p, int r, bool ascending = true)	//ascending flag is here only for the sake of compatibility, it does nothing for the moment
{
T x = data[p];
int k = p, tempi;
int l = r + 1;
T t;

while (1) {
do
k++;
while ((data[k] <= x) && (k < r));
do
l--;
while (data[l] > x);
while (k < l) {
t = data[k];
data[k] = data[l];
data[l] = t;

tempi = order[k];
order[k] = order[l];
order[l] = tempi;
do
k++;
while (data[k] <= x);
do
l--;
while (data[l] > x);
}
t = data[p];
data[p] = data[l];
data[l] = t;

tempi = order[p];
order[p] = order[l];
order[l] = tempi;
return l;
}
}
*/


#endif
