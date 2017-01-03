/* biomedicaldataio.h
 * Developed by Yang Yu, 2017
 */

#ifndef __BioMedicalDataIO_H__
#define __BioMedicalDataIO_H__

// BioMedicalDataIO defines a simple data structure for biomedical data

//
#include <deque>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <complex>
#include <float.h>
//#include <codecvt>
using namespace std;

//
#ifndef INF
#define INF 1E10
#endif

#ifndef EPS
#define EPS 1E-10
#endif

//
#define MIN_HEADER_SIZE 348
#define NII_HEADER_SIZE 352

#define FILE_TYPE_NIFTI 100
#define FILE_TYPE_NIFTI_GZ 101

#define META_DATA_BYTE_COUNTS 50838 /* private tag registered with Adobe */
#define META_DATA 50839 /* private tag registered with Adobe */

#define PHC_RAW_HEADER_SIZE_4BYTE 43
#define PHC_RAW_HEADER_SIZE_2BYTE 35

typedef enum { UNKNOWNDATATYPE, UCHAR, CHAR, USHORT, SHORT,
               UINT, INT, ULONG, LONG, FLOAT, DOUBLE } DataType;

typedef enum { UNKNOWNPIXELTYPE, SCALAR, RGB, RGBA, OFFSET, VECTOR,
               POINT, COVARIANTVECTOR, SYMMETRICSECONDRANKTENSOR,
               DIFFUSIONTENSOR3D, COMPLEX, FIXEDARRAY, MATRIX }  IOPixelType;

typedef enum { UNKNOWNFILEFORMAT, TIFFFormat, NIFTIFormat, HDF5Format, RAWFormat, V3DRAWFormat,
               NRRDFormat, CSVFormat, InsightTransformFileFormat, JsonFormat } IOFileFormat;

typedef enum { NoCompression, PackBits, JPEG, Deflate, LZW } CompressionType;

typedef enum { UNKNOWNRAWTYPE, RAW4BYTE, RAW2BYTE } V3DRawType;

//
#define foreach(count, iter) 	\
    for(long iter=0; iter<count; iter++)

// abs
template <class T>
T y_abs(T x)
{
    return (x<(T)0)?-x:x;
}

// delete 1d pointer
template <class T>
void del1dp(T *&p)
{
    if(p) {delete []p; p=NULL;}
    return;
}

// new 1d pointer
template<class T, class Tidx>
void new1dp(T *&p, Tidx n)
{
    //
    del1dp<T>(p);

    //
    try
    {
        p = new T [n];
    }
    catch(...)
    {
        cout<<"Attempt to allocate memory failed!"<<endl;
        del1dp<T>(p);
        return;
    }
    return;
}

//
template <class T>
void y_Debug(T v)
{
    cout<<"\n... y_Debug ... ";
    cout<<v;
    cout<<" ...\n";
}

// BioMedicalDataIO class
class BioMedicalDataIO
{
public:
    BioMedicalDataIO();
    ~BioMedicalDataIO();

public:
    int readData(string filename);
    int writeData(string filename);
    bool checkFileFormat(char *filename);
    bool checkFileExtension(char* filename, const char* extension);

    // filename
    char* getFileName();
    void setFileName(char* fileName);

    // dimensions
    long getDimx();
    long getDimy();
    long getDimz();
    long getDimc();
    long getDimt();

    void setDimx(long x);
    void setDimy(long y);
    void setDimz(long z);
    void setDimc(long c);
    void setDimt(long t);

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

public:
    string inputFileName,outputFileName;

    IOFileFormat m_FileFormat;
    DataType m_DataType;
    IOPixelType m_PixelType;

    long dimx, dimy, dimz, dimc, dimt; // dimensions
    float resx, resy, resz, resc, rest; // spacing

    void *m_Data;
    char* m_FileName;
};

#endif // __BioMedicalDataIO_H__
