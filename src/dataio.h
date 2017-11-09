// dataio.h
// read/write .tif, .nii, .raw, ...
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __DATAIO_H__
#define __DATAIO_H__

#include "biomedicaldataio.h"

//
extern "C" {
#include "tiff.h"
#include "tiffio.h"

#include "nifti1_io.h"
#include "znzlib.h"
};

#include "klb_imageIO.h"

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

// TIFF
class TiffIO : public BioMedicalDataIO
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

// NIfTI
class NiftiIO : public BioMedicalDataIO
{

public:
    NiftiIO();
    ~NiftiIO();

public:

    // reading
    bool canReadFile(char *fileNameToRead);
    int read();

    // writing
    bool canWriteFile(char *fileNameToWrite);
    int write();
    int write(const void *buffer, long sx, long sy, long sz, long sc, long st, int datatype, float vx, float vy, float vz);

    // compressed ?
    int isCompressed(const char * filename);

private:
    bool mustRescale();
    void setPixelType(IOPixelType pt);

private:
    znzFile m_GZFile;
    nifti_image *m_NiftiImage;

    double m_RescaleSlope;
    double m_RescaleIntercept;

    bool m_LegacyAnalyze75Mode;

    IOPixelType m_PixelType;
};

// KLB
class KLBIO : public BioMedicalDataIO
{

public:
    KLBIO();
    ~KLBIO();

public:
    // reading
    bool canReadFile(char *fileNameToRead);

    // writing
    bool canWriteFile(char *fileNameToWrite);

private:
    KLB_COMPRESSION_TYPE compressionType;
    int numThreads;
    uint32_t xyzct[KLB_DATA_DIMS], blockSize[KLB_DATA_DIMS];
    float32_t voxelsize[KLB_DATA_DIMS];
    KLB_DATA_TYPE datatype;
    char metadata[KLB_METADATA_SIZE];
};

// RAW
class RawIO : public BioMedicalDataIO
{
public:
    RawIO();
    ~RawIO();

public:

    // reading
    bool canReadFile(char *fileNameToRead);
    int read();

    // writing
    bool canWriteFile(char *fileNameToWrite);
    int write();

};


#endif // __DATAIO_H__
