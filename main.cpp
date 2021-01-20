#include "gdal_priv.h"
#include "cpl_conv.h" //for CPLMalloc()
 
int main()
{
    //注册文件格式
    GDALAllRegister();
   
    const char* pszFile = "lianggongchunri_de_youyv-009.jpg";
    GDALDataset *poDataset;
    //使用只读方式打开图像
    poDataset = (GDALDataset*) GDALOpen( pszFile,GA_ReadOnly );
    if( poDataset == NULL )
    {
        printf( "File: %s不能打开！\n",pszFile);
        return 0;
    }
 
    //输出图像的格式信息
    printf( "Driver:%s/%s\n",
        poDataset->GetDriver()->GetDescription(),
        poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME) );
 
    //输出图像的大小和波段个数
    printf( "Size is%dx%dx%d\n",
        poDataset->GetRasterXSize(),poDataset->GetRasterYSize(),
        poDataset->GetRasterCount());
 
    //输出图像的投影信息
    if( poDataset->GetProjectionRef() != NULL )
        printf( "Projectionis `%s'\n", poDataset->GetProjectionRef() );
 
    //输出图像的坐标和分辨率信息
    double adfGeoTransform[6];
    if( poDataset->GetGeoTransform( adfGeoTransform) == CE_None )
    {
        printf( "Origin =(%.6f,%.6f)\n",
            adfGeoTransform[0], adfGeoTransform[3]);
 
        printf( "PixelSize = (%.6f,%.6f)\n",
            adfGeoTransform[1], adfGeoTransform[5]);
    }
 
    GDALRasterBand *poBand;
    int            nBlockXSize, nBlockYSize;
    int            bGotMin, bGotMax;
    double         adfMinMax[2];
 
    //读取第一个波段
    poBand = poDataset->GetRasterBand( 1 );
 
    //获取图像的块大小并输出
    poBand->GetBlockSize(&nBlockXSize, &nBlockYSize );
    printf( "Block=%dx%dType=%s, ColorInterp=%s\n",
        nBlockXSize, nBlockYSize,
        GDALGetDataTypeName(poBand->GetRasterDataType()),
        GDALGetColorInterpretationName(
        poBand->GetColorInterpretation()));
 
    //获取该波段的最大值最小值，如果获取失败，则进行统计
    adfMinMax[0] = poBand->GetMinimum( &bGotMin);
    adfMinMax[1] = poBand->GetMaximum( &bGotMax);
 
    if( ! (bGotMin&& bGotMax) )
        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
 
    printf( "Min=%.3fd,Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
 
    //输出图像的金字塔信息
    if( poBand->GetOverviewCount() > 0 )
        printf( "Band has%d overviews.\n", poBand->GetOverviewCount() );
 
    //输出图像的颜色表信息
    if( poBand->GetColorTable() != NULL)
        printf( "Band hasa color table with %d entries.\n",
        poBand->GetColorTable()->GetColorEntryCount() );
 
    float *pafScanline;
    int   nXSize = poBand->GetXSize();
   
    //读取图像的第一行数据
    pafScanline = (float*) CPLMalloc(sizeof(float)*nXSize);
    poBand->RasterIO(GF_Read, 0, 0, nXSize,1, 
        pafScanline, nXSize,1, GDT_Float32, 0, 0 );
 
    CPLFree(pafScanline);
 
    //关闭文件
    GDALClose((GDALDatasetH)poDataset);
}