#include "gdal_priv.h"
#include<iostream>  
using namespace std;
int main()
{
	const char* pszFile;
	GDALAllRegister();
    pszFile = "lianggongchunri_de_youyv-009.jpg";
	GDALDataset *poDataset = (GDALDataset*)GDALOpen(pszFile, GA_ReadOnly);
	GDALRasterBand *poBand = poDataset->GetRasterBand(1);
	int xsize = poBand->GetXSize();
	int ysize = poBand->GetYSize();
	cout << xsize << endl;
	cout << ysize << endl;
	system("pause");
 
	return 0;
}