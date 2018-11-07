/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
Floating point Grid file I/O class
According to ArcGIS website, a floating-point file is a binary file of floating-point values that represent raster data.
Single file—extension *.flt
Read-only (Write—Only via Raster To Float tool, or developer code)
32-bit floating point
Multi-spectral: No
*/
#include "../Headers/stdafx.h"
#include "../Headers/EsriFlt.h"


EsriFlt::EsriFlt()
{
}

EsriFlt::EsriFlt(string fname)
{
	size_t loc;
	long i = 0, j = 0, k = 0, validLength = 0;
	char buffer[32];
	double elev = 0.0;
	string name, buf, buf1, buf2, hdrname;
	bitset <32> inbits;
	bitset <32> reverse;
	//NCOLS xxx
	//NROWS xxx
	//XLLCENTER xxx or XLLCORNER xxx
	//YLLCENTER xxx or YLLCORNER xxx
	//CELLSIZE xxx
	//[the length of one side of a square grid cell in same units as coordinate system used to locate the lower left center]
	//NODATA_VALUE xxx
	//BYTEORDER MSBFIRST or BYTEORDER LSBFIRST

	loc = fname.find('.', 0);
	name = fname.substr(0, loc);
	hdrname = name + ".hdr";

	ifstream hdr;
	hdr.open(hdrname, ios::in);
	if (hdr.is_open())
	{
		hdr.seekg(ios::beg);
		buf = nextString(hdr);
		if (_stricmp(buf.c_str(), "NCOLS") == 0)
			setNcols(stol(nextString(hdr)));
		else {
			printf("Could not read number of columns from header file, exiting...");
			exit(1);
		}
		buf = nextString(hdr);
		if (_stricmp(buf.c_str(), "NROWS") == 0)
			setNrows(stol(nextString(hdr)));
		else {
			printf("Could not read number of rows from header file, exiting...");
			exit(1);
		}
		buf1 = nextString(hdr);
		if (_stricmp(buf1.c_str(), "XLLCORNER") == 0 || _stricmp(buf1.c_str(), "XLLCENTER") == 0)
			setXllCorner(stof(nextString(hdr)));
		else {
			printf("Could not read lower-left x-coordinate from header file, exiting...");
			exit(1);
		}
		buf2 = nextString(hdr);
		if (_stricmp(buf2.c_str(), "YLLCORNER") == 0 || _stricmp(buf2.c_str(), "YLLCENTER") == 0)
			setYllCorner(stof(nextString(hdr)));
		else {
			printf("Could not read lower-left y-coordinate from header file, exiting...");
			exit(1);
		}
		buf = nextString(hdr);
		if (_stricmp(buf.c_str(), "CELLSIZE") == 0)
			setCellSize(stof(nextString(hdr)));
		else {
			printf("Could not read cell size from header file, exiting...");
			exit(1);
		}
		if (_stricmp(buf1.c_str(), "XLLCENTER") == 0)
			setXllCorner(getXllCorner() - (getCellSize() / 2.0));
		else
			;
		if (_stricmp(buf2.c_str(), "YLLCENTER") == 0)
			setYllCorner(getYllCorner() - (getCellSize() / 2.0));
		else
			;
		buf = nextString(hdr);
		if (_stricmp(buf.c_str(), "NODATA_value") == 0)
			setNoDataVal(stof(nextString(hdr)));
		else {
			printf("Could not read NODATA_value from header file, exiting...");
			exit(1);
		}
		buf = nextString(hdr);
		if (_stricmp(buf.c_str(), "BYTEORDER") == 0)
			setByteOrder(nextString(hdr));
		else {
			printf("Could not read byte order from header file, exiting...");
			exit(1);
		}
		hdr.close();
	}
	else
	{
		printf("Error opening header file, exiting ...");
		exit(1);
	}

	long length = getNcols() * getNrows();
	initData(length);

	//BYTEORDER MSBFIRST or BYTEORDER LSBFIRST
	ifstream fltfile(name + ".flt", ios::in | ios::binary);
	if (fltfile.is_open())
	{
		fltfile.seekg(0, ios::beg);
		while (!fltfile.eof())
		{
			fltfile.read(buffer, 4);
			bitset<32> temp("11111111111111111111111111111111");
			bitset<32> temp1("11111111111111111111111111111111");
			bitset<32> temp2("11111111111111111111111111111111");
			bitset<32> temp3("11111111111111111111111111111111");
			bitset<32> temp4("11111111111111111111111111111111");

			bitset<32>bits[4];
			for (k = 0; k < 4; k++)
			{
				if ((int)buffer[k] < 0)
				{
					bits[k] = int2bin(buffer[k]);
				}
				else
					bits[k] = buffer[k];
			}
			temp1 &= bits[0];
			temp2 &= bits[1];
			temp3 &= bits[2];
			temp4 &= bits[3];

			if (byteOrder == "MSBFIRST")
			{
				temp &= (temp4 | (temp3 << 8) | (temp2 << 16) | (temp1 << 24));
				for (j = 0; j < 32; j++)
				{
					reverse[j] = temp[31 - j];
				}
				setDataVal(bin2SPfloat(reverse), i++);
			}
			else if (byteOrder == "LSBFIRST")
			{
				temp &= (temp1 | (temp2 << 8) | (temp3 << 16) | (temp4 << 24));
				for (j = 0; j < 32; j++)
				{
					reverse[j] = temp[31 - j];
				}
				elev = bin2SPfloat(reverse);
				setDataVal(elev, i++);
			}
			else if (byteOrder == "VMS_FFLOAT")		//not tested, but should most likely work
			{
				temp &= (temp2 | (temp1 << 8) | (temp4 << 16) | (temp3 << 24));
				for (j = 0; j < 32; j++)
				{
					reverse[j] = temp[31 - j];
				}
				elev = bin2SPfloat(reverse);
				setDataVal(elev, i++);
			}
			else
			{
				printf("Invalid byte order, exiting ...");
				exit(1);
			}
		}
		fltfile.close();
	}
	else
	{
		printf("Error opening float data file, exiting ...");
		exit(1);
	}
}

long EsriFlt::getNcols()
{
	return ncols;
}

long EsriFlt::getNrows()
{
	return nrows;
}

double EsriFlt::getXllCorner()
{
	return xllcorner;
}

double EsriFlt::getYllCorner()
{
	return yllcorner;
}

double EsriFlt::getCellSize()
{
	return cellsize;
}

double EsriFlt::getNoDataVal()
{
	return noDataVal;
}

double EsriFlt::getData(long idx)
{
	return data[idx];
}

double *EsriFlt::getWholeData()
{
	return data;
}

bool EsriFlt::setNcols(long c)
{
	if (ncols = c) return true;
	else return false;
}

bool EsriFlt::setNrows(long r)
{
	if (nrows = r) return true;
	else return false;
}

bool EsriFlt::setXllCorner(double xllc)
{
	if (xllcorner = xllc) return true;
	else return false;
}

bool EsriFlt::setYllCorner(double yllc)
{
	if (yllcorner = yllc) return true;
	else return false;
}

bool EsriFlt::setCellSize(double cs)
{
	if (cellsize = cs)	return true;
	else return false;
}

bool  EsriFlt::initData(long leng)
{
	if ((data = new double[leng]) == NULL)
	{
		printf("Unable to allocate memory for data array, exiting...");
		exit(1);
	}
	return true;
}

bool EsriFlt::setNoDataVal(double nodat)
{
	if (noDataVal = nodat) return true;
	else return false;
}

void EsriFlt::setDataVal(double val, long idx)
{
	data[idx] = val;
}

void EsriFlt::setByteOrder(string byteorder)
{
	byteOrder.append(byteorder);
}

string EsriFlt::getByteOrder()
{
	return byteOrder;
}

EsriFlt::~EsriFlt()
{
	//delete[] data;
}

string EsriFlt::nextString(ifstream &in)
{
	string out;
	char c = '\0';
	//leading wspaces
	while (true)
	{
		c = in.get();
		if (c == EOF)
			return out;
		if (!isspace(c))
			break;
	}
	in.putback(c);

	do {
		c = in.get();
		if (c == EOF) break;
		if (!isspace(c))
			out.push_back(c);
	} while (!isspace(c));
	return out;
}

double EsriFlt::bin2SPfloat(bitset <32> in) const
{
	long i, sign, exponent;
	string temp;
	double out;
	bitset<8> exp;
	bitset<23> mant;

	if (in[0] == 1) sign = -1;
	else sign = 1;

	for (i = 8; i > 0; i--) exp[8 - i] = in[i];	//exponent

	exponent = exp.to_ulong();
	temp.clear();

	exponent -= 127;
	for (i = 9; i < 32; i++)
	{
		mant[i - 9] = in[i];
	}

	out = 1.0;
	for (i = 0; i < 23; i++)
	{
		out += mant[i] * pow(2, (-1)*(i + 1));
	}

	out *= pow(2, exponent);
	return out *= sign;
}

bitset<32> EsriFlt::spfloat2bin(double flt)
{
	bitset<32> out;
	//unnecessary for this purpose, will implement in the future

	return out;
}

bitset <8> EsriFlt::twosComplement(bitset<8> in)
{
	int i;
	bitset<8>temp2;

	if (in[0] == 0) temp2[0] = 1; else temp2[0] = 0;
	for (i = 1; i < 8; i++)
	{
		temp2[i] = (in[i] == 0) ? 1 : 0;
	}
	return temp2;
}

bitset <32> EsriFlt::int2bin(int in)
{
	int i = 0;
	bitset <32> temp("00000000000000000000000000000000");
	if (in < 0) in = 256 + in;
	// a bit of recursion	
	while (in >= 1 && i < 8)
	{
		temp[i] = in % 2;
		in /= 2;
		i++;
	}
	temp[i] = in;
	return temp;
}
