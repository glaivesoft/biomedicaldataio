/* NeuCVData.h
 * Developed by Yang Yu, 2009-2015
 */

#ifndef __NeuCVDATA_H__
#define __NeuCVDATA_H__

#include "NeuCVDataStructure.h"

// NeuCVDataIO class can read/write .tif using libtiff library and .nii using nifti library

//
#define MIN_HEADER_SIZE 348
#define NII_HEADER_SIZE 352

#define FILE_TYPE_NIFTI 100
#define FILE_TYPE_NIFTI_GZ 101

#define META_DATA_BYTE_COUNTS 50838 /* private tag registered with Adobe */
#define META_DATA 50839 /* private tag registered with Adobe */

#define PHC_RAW_HEADER_SIZE_4BYTE 43
#define PHC_RAW_HEADER_SIZE_2BYTE 35

typedef enum { UNKNOWNPIXELTYPE, SCALAR, RGB, RGBA, OFFSET, VECTOR,
               POINT, COVARIANTVECTOR, SYMMETRICSECONDRANKTENSOR,
               DIFFUSIONTENSOR3D, COMPLEX, FIXEDARRAY, MATRIX }  IOPixelType;

typedef enum { UNKNOWNFILEFORMAT, TIFFIMAGE, NIFTIIMAGE, HDF5IMAGE, V3DRAWIMAGE, NRRDIMAGE, CSVFILE, InsightTransformFile } IOFileFormat;
typedef enum { NoCompression, PackBits, JPEG, Deflate, LZW } CompressionType;

typedef enum { UNKNOWNRAWTYPE, RAW4BYTE, RAW2BYTE } V3DRawType;

// NeuCVDataIO class
class NeuCVDataIO
{
public:
    NeuCVDataIO();
    ~NeuCVDataIO();

public:
    int readData(string filename);
    int writeData(string filename);
    bool checkFileFormat(char *filename);
    bool checkFileExtension(char* filename, const char* extension);

    // filename
    char* getFileName();
    void setFileName(char* fileName);

    // dimensions
    LONG64 getDimx();
    LONG64 getDimy();
    LONG64 getDimz();
    LONG64 getDimc();
    LONG64 getDimt();

    void setDimx(LONG64 x);
    void setDimy(LONG64 y);
    void setDimz(LONG64 z);
    void setDimc(LONG64 c);
    void setDimt(LONG64 t);

    // resolutions
    void setResX(REAL resolution_x);
    void setResY(REAL resolution_y);
    void setResZ(REAL resolution_z);

    // datatype
    int getDataType();
    void setDataType(int dt);

public:
    string inputFileName,outputFileName;

    IOFileFormat m_FileFormat;
    DataType m_ComponentType;
    IOPixelType m_PixelType;

    LONG64 dimx, dimy, dimz, dimc, dimt; // dimensions
    REAL resx, resy, resz, resc, rest; // spacing

    void *m_Data;
    char* m_FileName;
};

#endif // __NeuCVDATA_H__
