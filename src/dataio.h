// dataio.h
// read/write .tif, ...
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __DATAIO_H__
#define __DATAIO_H__

#include "image.hpp"

//
extern "C" {
#include "tiff.h"
#include "tiffio.h"

#include "nifti1_io.h"
#include "znzlib.h"

};

//
#ifdef Use_OpenMP
#include <omp.h>
#endif

// types
typedef enum { UNKNOWNDATATYPE, UCHAR, CHAR, USHORT, SHORT, UINT, INT, ULONG, LONG, FLOAT, DOUBLE } DataType;

typedef enum { UNKNOWNPIXELTYPE, SCALAR, RGB, RGBA, OFFSET, VECTOR,
    POINT, COVARIANTVECTOR, SYMMETRICSECONDRANKTENSOR,
    DIFFUSIONTENSOR3D, COMPLEX, FIXEDARRAY, MATRIX }  IOPixelType;

typedef enum { UNKNOWNFILEFORMAT, TIFFIMAGE, VDBIMAGE, RLEIMAGE } IOFileFormat;

typedef enum { NoCompression, PackBits, JPEG, Deflate, LZW } CompressionType;

//
template <class Tdata, class Tidx>
int convertImageOrder(Tdata *&p, Tidx dimx, Tidx dimy, Tidx dimz, Tidx dimc, bool cxyz2xyzc)
{
    //
    /// convert image order [CXYZ] <-> [XYZC]
    //
    
    //
    if(!p)
    {
        cout<<"Invalid pointer for convertImageOrder."<<endl;
        return -1;
    }
    
    //
    Tidx offset_tif_x = dimc;
    Tidx offset_tif_y = offset_tif_x*dimx;
    Tidx offset_tif_z = offset_tif_y*dimy;
    
    Tidx offset_ilia_y = dimx;
    Tidx offset_ilia_z = offset_ilia_y*dimy;
    Tidx offset_ilia_c = offset_ilia_z*dimz;
    
    Tidx imagesz = dimc*offset_ilia_c;
    
    //
    Tdata *pTmp = NULL;
    new1dp<Tdata, Tidx>(pTmp, imagesz);
    memcpy(pTmp, p, imagesz * sizeof(Tdata) );
    // std::copy(&p[0], &p[imagesz-1], pTmp);
    
    //
    for (Tidx c=0; c<dimc; c++)
    {
        Tidx offset_out_c = c*offset_ilia_c;
        for (Tidx z=0; z<dimz; z++)
        {
            Tidx offset_in_z = z*offset_tif_z;
            Tidx offset_out_z = z*offset_ilia_z + offset_out_c;
            for (Tidx y=0; y<dimy; y++)
            {
                Tidx offset_in_y = y*offset_tif_y + offset_in_z;
                Tidx offset_out_y = y*offset_ilia_y + offset_out_z;
                for (Tidx x=0; x<dimx; x++)
                {
                    Tidx idx_in = offset_in_y + x*offset_tif_x + c;
                    Tidx idx_out = offset_out_y + x;
                    
                    if(cxyz2xyzc)
                    {
                        // CXYZ -> XYZC
                        p[idx_out] = pTmp[idx_in];
                    }
                    else
                    {
                        // XYZC -> CXYZ
                        p[idx_in] = pTmp[idx_out];
                    }
                    
                }// x
            }// y
        }// z
    }// c
    
    // de-alloc
    del1dp<Tdata>(pTmp);
    
    //
    return 0;
}

template <class T>
bool checkFileExtension(T* filename, const T* extension)
{
    if(filename == NULL || extension == NULL)
        return false;
    
    if(strlen(filename) == 0 || strlen(extension) == 0)
        return false;
    
    if(strchr(filename, '.') == NULL || strchr(extension, '.') == NULL)
        return false;
    
    for(unsigned int i = 0; i < strlen(filename); i++)
    {
        if(tolower(filename[strlen(filename) - i - 1]) == tolower(extension[strlen(extension) - i - 1]))
        {
            if(i == strlen(extension) - 1)
                return true;
        } else
            break;
    }
    
    return false;
}

// TIFF image I/O class
class TiffIO
{
public:
    TiffIO();
    ~TiffIO();
    
public:
    
    // reading
    bool canReadFile(char *fileNameToRead);
    int read();
    
    // writing
    bool canWriteFile(char *fileNameToWrite);
    int write();
    
    //
    void close();
    
    //
    void changeImageOrder(bool io);
    
    // filename
    char* getFileName();
    void setFileName(char* fileName);
    
    // dimensions
    long getDimX();
    long getDimY();
    long getDimZ();
    long getDimC();
    long getDimT();
    
    void setDimX(long x);
    void setDimY(long y);
    void setDimZ(long z);
    void setDimC(long c);
    void setDimT(long t);
    
    // resolutions
    float getResX();
    float getResY();
    float getResZ();

    void setResX(float resolution_x);
    void setResY(float resolution_y);
    void setResZ(float resolution_z);
    
    // datatype
    int getDataType();
    void setDataType(int dt);
    
    // data
    void* getData();
    void setData(void *p);
    
public:
    string inputFileName,outputFileName;
    
    IOFileFormat m_FileFormat;
    DataType m_DataType;
    IOPixelType m_PixelType;
    
    long dimx, dimy, dimz, dimc, dimt; // dimensions
    float resx, resy, resz, resc, rest; // spacing
    
    void *m_Data;
    char* m_FileName;
    
private:
    uint16 config;
    uint16 bitspersample, samplesperpixel;
    uint16 compression, photometric;
    uint32 width, length;
    uint16 orientation;
    uint32 tilewidth, tilelength, rowsperstrip;
    uint16 transferR, transferG, transferB, transferA;
    uint16 colormapR, colormapG, colormapB, colormapA;
    
    bool bICC;
    uint32 lenICC;
    void** dataICC;
    
    bool bINKS, bInkName;
    uint16 ninks;
    const char* inknames;
    int inknameslen;
    
    bool bPG;
    unsigned short m_CurrentPage, m_NumberOfPages;
    
    bool bTiled;
    uint16 fillorder;
    
    uint32 m_NumberOfTiles;
    
private:
    uint32 m_SUBFILETYPE;
    uint16 m_THRESHHOLDING;
    char* m_DOCUMENTNAME;
    char* m_IMAGEDESCRIPTION;
    char* m_MAKE;
    char* m_MODEL;
    uint16 m_MINSAMPLEVALUE;
    uint16 m_MAXSAMPLEVALUE;
    float m_XRESOLUTION;
    float m_YRESOLUTION;
    char* m_PAGENAME;
    float m_XPOSITION;
    float m_YPOSITION;
    uint16 m_RESOLUTIONUNIT;
    char* m_SOFTWARE;
    char* m_DATETIME;
    char* m_ARTIST;
    char* m_HOSTCOMPUTER;
    float* m_WHITEPOINT;
    float* m_PRIMARYCHROMATICITIES;
    uint16 m_HALFTONEHINTS1, m_HALFTONEHINTS2;
    uint16 m_INKSET;
    uint16 m_DOTRANGE1, m_DOTRANGE2;
    char* m_TARGETPRINTER;
    uint16 m_SAMPLEFORMAT;
    float* m_YCBCRCOEFFICIENTS;
    uint16 m_YCBCRSUBSAMPLING1, m_YCBCRSUBSAMPLING2;
    uint16 m_YCBCRPOSITIONING;
    float* m_REFERENCEBLACKWHITE;
    uint16 m_EXTRASAMPLES1;
    uint16* m_EXTRASAMPLES2;
    double m_SMINSAMPLEVALUE;
    double m_SMAXSAMPLEVALUE;
    double m_STONITS;
    
    unsigned int   m_SubFiles;
    unsigned int   m_IgnoredSubFiles;
    
    //
    TIFF* m_TiffImage;
};

#endif // __DATAIO_H__
