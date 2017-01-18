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
#include <sys/time.h>
//#include <codecvt>
using namespace std;

//
#ifdef Use_OpenMP
#include <omp.h>
#endif

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
        memset(p, 0, n);
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

//
/// Timer
//
class Timer
{
public:
    Timer();
    ~Timer();

public:
    void start();
    double getEclipseTime();

private:
    struct timeval m_startTime, m_endTime;
};

//
/// Tuplet
//
template <class T>
class Tuplet
{
public:
    Tuplet(){}
    Tuplet(T x, T y, T z)
    {
        setXYZ(x,y,z);
    }
    ~Tuplet(){}

public:
    T getX(){return x;}
    T getY(){return y;}
    T getZ(){return z;}
    void setX(T v){x = v;}
    void setY(T v){y = v;}
    void setZ(T v){z = v;}
    void setXYZ(T xx, T yy, T zz)
    {
        x = xx;
        y = yy;
        z = zz;
    }
    T size(){return x*y*z;}

public:
    T x, y, z;
};

//
/// Quadruplet
//
template <class T>
class Quadruplet
{
public:
    Quadruplet(){}
    Quadruplet(T x, T y, T z, T c)
    {
        setXYZC(x,y,z,c);
    }
    ~Quadruplet(){}

public:
    T getX(){return x;}
    T getY(){return y;}
    T getZ(){return z;}
    T getC(){return c;}
    void setX(T v){x = v;}
    void setY(T v){y = v;}
    void setZ(T v){z = v;}
    void setC(T v){c = v;}
    void setXYZC(T xx, T yy, T zz, T cc)
    {
        x = xx;
        y = yy;
        z = zz;
        c = cc;
    }
    T size(){return x*y*z*c;}

public:
    T x, y, z, c;
};

//
/// Quintuplet
//
template <class T>
class Quintuplet
{
public:
    Quintuplet(){}
    Quintuplet(T x, T y, T z, T c, T t)
    {
        setXYZCT(x,y,z,c,t);
    }
    ~Quintuplet(){}

public:
    T getX(){return x;}
    T getY(){return y;}
    T getZ(){return z;}
    T getC(){return c;}
    T getT(){return t;}
    void setX(T v){x = v;}
    void setY(T v){y = v;}
    void setZ(T v){z = v;}
    void setC(T v){c = v;}
    void setT(T v){t = v;}
    void setXYZCT(T xx, T yy, T zz, T cc, T tt)
    {
        x = xx;
        y = yy;
        z = zz;
        c = cc;
        t = tt;
    }
    T size(){return x*y*z*c*t;}

public:
    T x, y, z, c, t;
};

//
/// BioMedicalData
//
class BioMedicalData
{
public:
    BioMedicalData();
    ~BioMedicalData();

public:
    // data
    void * data();
    void setData(void *data);
    int zeros(long bytes);
    long bytes();
    void clear();

    // datatype
    DataType dataType();
    void setDataType(DataType type);

public:
    void *p; // 1d data pointer
    Quintuplet<float> origin; // origin (offset)
    Quintuplet<float> spacing; // voxelsize (spacing)
    Quintuplet<long> size; // size XYZCT (dimension)
    DataType dt; // data type
};

//
/// BioMedicalDataIO
//
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

    // data
    BioMedicalData * data();
    void setData(BioMedicalData *data);

public:
    string inputFileName,outputFileName;

    IOFileFormat m_FileFormat;
    IOPixelType m_PixelType;

    char* m_FileName;

    BioMedicalData *m_Data;
};

#endif // __BioMedicalDataIO_H__
