// dataio.cpp
// read/write .tif, .nii, .raw, ...
// developed by Yang Yu (gnayuy@gmail.com)

#include "dataio.h"

//
// BioMedicalDataIO
//
BioMedicalDataIO::BioMedicalDataIO()
{
    inputFileName.clear();
    outputFileName.clear();
    m_Data=NULL;

    m_PixelType = UNKNOWNPIXELTYPE;
    m_DataType = UNKNOWNDATATYPE;
    m_FileFormat = UNKNOWNFILEFORMAT;
}

BioMedicalDataIO::~BioMedicalDataIO()
{
}

bool BioMedicalDataIO::checkFileExtension(char* filename, const char* extension)
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

bool BioMedicalDataIO::checkFileFormat(char* filename)
{
    if(checkFileExtension(filename, ".tif"))
    {
        m_FileFormat = TIFFFormat;
    }
    else if(checkFileExtension(filename, ".nii") || checkFileExtension(filename, ".nii.gz"))
    {
        m_FileFormat = NIFTIFormat;
    }
    else if(checkFileExtension(filename, ".h5"))
    {
        m_FileFormat = HDF5Format;
    }
    else if(checkFileExtension(filename, ".raw"))
    {
        m_FileFormat = RAWFormat;
    }
    else if(checkFileExtension(filename, ".nrrd"))
    {
        m_FileFormat = NRRDFormat;
    }
    else
    {
        cout<<"Invalid file format"<<endl;
        return false;
    }

    return true;
}

// file names
char* BioMedicalDataIO::getFileName()
{
    return this->m_FileName;
}

void BioMedicalDataIO::setFileName(char* fileName)
{
    if(!fileName)
    {
        cout<<"Invalid file name"<<endl;
        return;
    }

    new1dp<char, int>(this->m_FileName, strlen(fileName) + 1);
    strcpy(this->m_FileName,fileName);

    return;
}

// dimensions
long BioMedicalDataIO::getDimx()
{
    return this->dimx;
}

long BioMedicalDataIO::getDimy()
{
    return this->dimy;
}

long BioMedicalDataIO::getDimz()
{
    return this->dimz;
}

long BioMedicalDataIO::getDimc()
{
    return this->dimc;
}

long BioMedicalDataIO::getDimt()
{
    return this->dimt;
}

void BioMedicalDataIO::setDimx(long x)
{
    if(x<=0)
    {
        cout << "invalid x dimension"<<endl;
    }
    this->dimx = x;
}

void BioMedicalDataIO::setDimy(long y)
{
    if(y<=0)
    {
        cout << "invalid y dimension"<<endl;
    }
    this->dimy = y;
}

void BioMedicalDataIO::setDimz(long z)
{
    if(z<=0)
    {
        cout << "invalid z dimension"<<endl;
    }
    this->dimz = z;
}

void BioMedicalDataIO::setDimc(long c)
{
    if(c<=0)
    {
        cout << "invalid c dimension"<<endl;
    }
    this->dimc = c;
}

void BioMedicalDataIO::setDimt(long t)
{
    if(t<=0)
    {
        cout << "invalid t dimension"<<endl;
    }
    this->dimt = t;
}

float BioMedicalDataIO::getResX()
{
    return resx;
}

float BioMedicalDataIO::getResY()
{
    return resy;
}

float BioMedicalDataIO::getResZ()
{
    return resz;
}

void BioMedicalDataIO::setResX(float resolution_x)
{
    if(resolution_x<=0)
    {
        cout<<"Invalide x resolution"<<endl;
        return;
    }
    resx = resolution_x;
    return;
}

void BioMedicalDataIO::setResY(float resolution_y)
{
    if(resolution_y<=0)
    {
        cout<<"Invalide y resolution"<<endl;
        return;
    }
    resy = resolution_y;
    return;
}

void BioMedicalDataIO::setResZ(float resolution_z)
{
    if(resolution_z<=0)
    {
        cout<<"Invalide z resolution"<<endl;
        return;
    }
    resz = resolution_z;
    return;
}

int BioMedicalDataIO::getDataType()
{
    return m_DataType;
}

void BioMedicalDataIO::setDataType(int dt)
{
    this->m_DataType = (DataType)dt;
}

int BioMedicalDataIO::readData(string filename)
{
    inputFileName.assign(filename);

    ifstream in(filename.c_str());
    if(!in.good())
    {
        cout<<"The input file "<<filename<<" does not exist."<<endl;
        return -1;
    }

    if(checkFileFormat(const_cast<char *>(filename.c_str())))
    {
        if(m_FileFormat==TIFFFormat)
        {
            TiffIO tif;

            if(!tif.canReadFile(const_cast<char*>(inputFileName.c_str())))
            {
                cout<<"Fail to read TIFF image."<<endl;
                return -2;
            }
            tif.read();

            dimx = tif.getDimx();
            dimy = tif.getDimy();
            dimz = tif.getDimz();
            dimc = tif.getDimc();
            dimt = tif.getDimt();

            m_DataType = (DataType)tif.getDataType();

            resx = tif.resx;
            resy = tif.resy;
            resz = tif.resz;
        }
        else if(m_FileFormat==NIFTIFormat)
        {
            NiftiIO nii;

            if(!nii.canReadFile(const_cast<char*>(inputFileName.c_str())))
            {
                cout<<"Fail to read Nifti image."<<endl;
                return -2;
            }
            nii.read();

            dimx = nii.getDimx();
            dimy = nii.getDimy();
            dimz = nii.getDimz();
            dimc = nii.getDimc();
            dimt = nii.getDimt();

            m_DataType = (DataType)nii.getDataType();

            resx = nii.resx;
            resy = nii.resy;
            resz = nii.resz;
        }
        else
        {
            cout<<"Unsupported Data Formats."<<endl;
            return -1;
        }
    }

    //
    return 0;
}

int BioMedicalDataIO::writeData(string filename)
{
    outputFileName.assign(filename);

    if(m_Data)
    {
        if(checkFileFormat(const_cast<char *>(filename.c_str())))
        {
            if(m_FileFormat==TIFFFormat)
            {
                TiffIO tif;

                if(!tif.canWriteFile(const_cast<char*>(outputFileName.c_str())))
                {
                    cout<<"Fail to write TIFF image."<<endl;
                    return -2;
                }
                tif.write();

            }
            else if(m_FileFormat==NIFTIFormat)
            {
                NiftiIO nii;

                if(!nii.canWriteFile(const_cast<char*>(outputFileName.c_str())))
                {
                    cout<<"Fail to write Nifti image."<<endl;
                    return -2;
                }
                nii.write();
            }
            else
            {
                cout<<"Unsupported Image Formats."<<endl;
                return -1;
            }
        }

    }
    else
    {
        cout<<"Empty data!"<<endl;
        return -1;
    }

    //
    return 0;
}

//
// TIFFIO
//
TiffIO::TiffIO() : BioMedicalDataIO()
{
    // supported file suffix
    // .tif
    
    m_TiffImage = NULL;
    this->m_FileName = NULL;
    
    inputFileName.clear();
    outputFileName.clear();
    m_Data=NULL;

    m_PixelType = UNKNOWNPIXELTYPE;
    m_DataType = UNKNOWNDATATYPE;
    m_FileFormat = UNKNOWNFILEFORMAT;
    
    config = (uint16) -1;
    width = (uint32) 0;
    length = (uint32) 0;
    tilewidth = (uint32) -1;
    tilelength = (uint32) -1;
    rowsperstrip = (uint32) 0;
    
    compression = (uint16) -1;
    
    bICC = false;
    bINKS = false;
    bInkName = false;
    bPG = false;
    
    bTiled = false;
    
    dimx = 1;
    dimy = 1;
    dimz = 1;
    dimc = 1;
    dimt = 1;
    
    resx = 1.0;
    resy = 1.0;
    resz = 1.0;
    resc = 1.0;
    rest = 1.0;
    
    m_SubFiles = 0;
    m_IgnoredSubFiles = 0;
    
    m_NumberOfPages = 0;
    m_RESOLUTIONUNIT = 1;
    m_XRESOLUTION = 1;
    m_YRESOLUTION = 1;
}

TiffIO::~TiffIO()
{
    close();
}

void TiffIO::close()
{
    if( m_TiffImage )
    {
        TIFF* tif = (TIFF*)m_TiffImage;
        TIFFClose( tif );
        m_TiffImage = NULL;
    }

//    if(m_Data)
//    {
//        if(m_DataType==UCHAR)
//        {
//            unsigned char *p = (unsigned char *)(m_Data);
//            del1dp<unsigned char>(p);
//        }
//        else if(m_DataType==USHORT)
//        {
//            unsigned short *p = (unsigned short *)(m_Data);
//            del1dp<unsigned short>(p);
//        }
//        else
//        {
//            // other data type
//        }
//    }
    
    return;
}

void TiffIO::changeImageOrder(bool io)
{
    // io == true read a tiff image
    // [CXYZT] -> [XYZCT]
    // io == false write a tiff image
    // [XYZCT] -> [CXYZT]
    
    
    // orientation
    // BOTLEFT (4) <-> TOPLEFT (1)
    
    //
    if(dimc>1)
    {
        if(m_DataType==UCHAR)
        {
            unsigned char *p = (unsigned char *)m_Data;
            convertImageOrder<unsigned char, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==CHAR)
        {
            char *p = (char *)m_Data;
            convertImageOrder<char, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==USHORT)
        {
            unsigned short *p = (unsigned short *)m_Data;
            convertImageOrder<unsigned short, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==SHORT)
        {
            short *p = (short *)m_Data;
            convertImageOrder<short, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==UINT)
        {
            unsigned int *p = (unsigned int *)m_Data;
            convertImageOrder<unsigned int, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==INT)
        {
            int *p = (int *)m_Data;
            convertImageOrder<int, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==ULONG)
        {
            unsigned long *p = (unsigned long *)m_Data;
            convertImageOrder<unsigned long, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==LONG)
        {
            long *p = (long *)m_Data;
            convertImageOrder<long, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==FLOAT)
        {
            float *p = (float *)m_Data;
            convertImageOrder<float, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else if(m_DataType==DOUBLE)
        {
            double *p = (double *)m_Data;
            convertImageOrder<double, long>(p, dimx, dimy, dimz, dimc, io);
        }
        else
        {
            cout<<"Invalid datatype."<<m_DataType<<" "<<UCHAR<<endl;
            return;
        }
    }
    else
    {
        // silent
        //cout<<"No need to convert image order for grayscale images."<<endl;
    }
    
    return;
}

bool TiffIO::canReadFile(char *fileNameToRead)
{
    //
    setFileName(fileNameToRead);
    
    //
    close();
    
    if(strcmp(fileNameToRead, "") == 0)
    {
        return false;
    }
    
    // suppress the warning messages
    TIFFErrorHandler WarnHandler = TIFFSetWarningHandler(0);
    TIFFSetWarningHandler(WarnHandler);

    //
    TIFF* tif = TIFFOpen( const_cast<char *>(m_FileName), "r" );
    if(tif == NULL)
    {
        TIFFClose( tif );
        return false;
    }
    m_TiffImage = tif;
    
    // read header
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length);
    
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    TIFFGetFieldDefaulted(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetFieldDefaulted(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    
    if (compression == COMPRESSION_JPEG)
    {
        // Force conversion to RGB
        TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
    }
    else if (photometric == PHOTOMETRIC_YCBCR)
    {
        // Otherwise, can't handle subsampled input
        uint16 subsamplinghor,subsamplingver;
        
        TIFFGetFieldDefaulted(tif, TIFFTAG_YCBCRSUBSAMPLING, &subsamplinghor, &subsamplingver);
        if (subsamplinghor!=1 || subsamplingver!=1)
        {
            cout<< "Can't copy/convert subsampled image."<<endl;
            return false;
        }
    }
    
    TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
    
    TIFFGetFieldDefaulted(tif, TIFFTAG_ORIENTATION, &orientation);
    switch (orientation)
    {
        case ORIENTATION_BOTRIGHT:
        case ORIENTATION_RIGHTBOT:	/* XXX */
            TIFFWarning(TIFFFileName(tif), "using bottom-left orientation");
            orientation = ORIENTATION_BOTLEFT;
            /* fall thru... */
        case ORIENTATION_LEFTBOT:	/* XXX */
        case ORIENTATION_BOTLEFT:
            break;
        case ORIENTATION_TOPRIGHT:
        case ORIENTATION_RIGHTTOP:	/* XXX */
        default:
            TIFFWarning(TIFFFileName(tif), "using top-left orientation");
            orientation = ORIENTATION_TOPLEFT;
            /* fall thru... */
        case ORIENTATION_LEFTTOP:	/* XXX */
        case ORIENTATION_TOPLEFT:
            break;
    }
    
    bTiled = TIFFIsTiled(tif);
    
    if(bTiled)
    {
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength);
    }
    else
    {
        if (rowsperstrip == (uint32) 0)
        {
            TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
            if (rowsperstrip > length && rowsperstrip != (uint32)-1)
                rowsperstrip = length;
        }
        else if (rowsperstrip == (uint32) -1)
        {
            rowsperstrip = length;
        }
    }
    
    //
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
    
    //
    if (samplesperpixel <= 4)
        TIFFGetField(tif, TIFFTAG_TRANSFERFUNCTION, &transferR, &transferG, &transferB, &transferA);
    TIFFGetField(tif, TIFFTAG_COLORMAP, &colormapR, &colormapG, &colormapB, &colormapA);
    
    //
    bICC = TIFFGetField(tif, TIFFTAG_ICCPROFILE, &lenICC, &dataICC);
    
    //
    bINKS = TIFFGetField(tif, TIFFTAG_NUMBEROFINKS, &ninks);
    
    if(bINKS)
    {
        bInkName = TIFFGetField(tif, TIFFTAG_INKNAMES, &inknames);
        if(bInkName)
        {
            inknameslen = strlen(inknames) + 1;
            const char* cp = inknames;
            while (ninks > 1)
            {
                cp = strchr(cp, '\0');
                cp++;
                inknameslen += (strlen(cp) + 1);
                ninks--;
            }
        }
    }
    
    //
    m_NumberOfPages = TIFFNumberOfDirectories(tif);
    bPG = TIFFGetField(tif, TIFFTAG_PAGENUMBER, &m_CurrentPage, &m_NumberOfPages);
    
    if ( this->m_NumberOfPages > 1 )
    {
        for ( unsigned int page = 0; page < m_NumberOfPages; page++ )
        {
            m_SUBFILETYPE = 6;
            if ( TIFFGetField(tif, TIFFTAG_SUBFILETYPE, &m_SUBFILETYPE) )
            {
                if ( m_SUBFILETYPE == 0 )
                {
                    m_SubFiles += 1;
                }
                else if ( m_SUBFILETYPE & FILETYPE_REDUCEDIMAGE || m_SUBFILETYPE & FILETYPE_MASK )
                {
                    ++m_IgnoredSubFiles;
                }
                
            }
            TIFFReadDirectory(tif);
        }
        TIFFSetDirectory(tif, 0);
    }
    
    //
    TIFFGetField(tif, TIFFTAG_THRESHHOLDING, &m_THRESHHOLDING);
    TIFFGetField(tif, TIFFTAG_DOCUMENTNAME, &m_DOCUMENTNAME);
    TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &m_IMAGEDESCRIPTION);
    TIFFGetField(tif, TIFFTAG_MAKE, &m_MAKE);
    TIFFGetField(tif, TIFFTAG_MODEL, &m_MODEL);
    TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &m_MINSAMPLEVALUE);
    TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &m_MAXSAMPLEVALUE);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &m_XRESOLUTION);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION,	&m_YRESOLUTION);
    TIFFGetField(tif, TIFFTAG_PAGENAME, &m_PAGENAME);
    TIFFGetField(tif, TIFFTAG_XPOSITION, &m_XPOSITION);
    TIFFGetField(tif, TIFFTAG_YPOSITION, &m_YPOSITION);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &m_RESOLUTIONUNIT);
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &m_SOFTWARE);
    TIFFGetField(tif, TIFFTAG_DATETIME, &m_DATETIME);
    TIFFGetField(tif, TIFFTAG_ARTIST, &m_ARTIST);
    TIFFGetField(tif, TIFFTAG_HOSTCOMPUTER, &m_HOSTCOMPUTER);
    TIFFGetField(tif, TIFFTAG_WHITEPOINT, &m_WHITEPOINT);
    TIFFGetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, &m_PRIMARYCHROMATICITIES);
    TIFFGetField(tif, TIFFTAG_HALFTONEHINTS, &m_HALFTONEHINTS1, &m_HALFTONEHINTS2);
    TIFFGetField(tif, TIFFTAG_INKSET, &m_INKSET);
    TIFFGetField(tif, TIFFTAG_DOTRANGE, &m_DOTRANGE1, &m_DOTRANGE2);
    TIFFGetField(tif, TIFFTAG_TARGETPRINTER, &m_TARGETPRINTER);
    TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &m_SAMPLEFORMAT);
    TIFFGetField(tif, TIFFTAG_YCBCRCOEFFICIENTS, &m_YCBCRCOEFFICIENTS);
    TIFFGetField(tif, TIFFTAG_YCBCRSUBSAMPLING, &m_YCBCRSUBSAMPLING1, &m_YCBCRSUBSAMPLING2);
    TIFFGetField(tif, TIFFTAG_YCBCRPOSITIONING, &m_YCBCRPOSITIONING);
    TIFFGetField(tif, TIFFTAG_REFERENCEBLACKWHITE,	&m_REFERENCEBLACKWHITE);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &m_EXTRASAMPLES1, &m_EXTRASAMPLES2);
    TIFFGetField(tif, TIFFTAG_SMINSAMPLEVALUE,	&m_SMINSAMPLEVALUE);
    TIFFGetField(tif, TIFFTAG_SMAXSAMPLEVALUE,	&m_SMAXSAMPLEVALUE);
    TIFFGetField(tif, TIFFTAG_STONITS, &m_STONITS);
    
    
    // datatype
    if ( bitspersample <= 8 )
    {
        if ( m_SAMPLEFORMAT == 2 ) // SAMPLEFORMAT_INT = 2
        {
            m_DataType = CHAR;
        }
        else // SAMPLEFORMAT_UINT = 1
        {
            m_DataType = UCHAR;
        }
    }
    else if ( bitspersample == 32 )
    {
        if ( m_SAMPLEFORMAT == 3 ) // SAMPLEFORMAT_IEEEFP = 3
        {
            m_DataType = FLOAT;
        }
    }
    else
    {
        if ( m_SAMPLEFORMAT == 2 )
        {
            m_DataType = SHORT;
        }
        else
        {
            m_DataType = USHORT;
        }
    }
    
    // dimensions
    dimx = width; // x
    dimy = length; // y
    
    if(bTiled)
    {
        m_NumberOfTiles = TIFFNumberOfTiles(tif);
        if ( this->m_NumberOfTiles > 1 )
        {
            dimz = m_NumberOfPages; // (m_NumberOfTiles*tilewidth*tilelength)/(width*length);
        }
    }
    else
    {
        if ( m_NumberOfPages-m_IgnoredSubFiles > 1 )
        {
            if ( m_SubFiles > 0 )
            {
                dimz = m_SubFiles;
            }
            else
            {
                dimz = m_NumberOfPages - m_IgnoredSubFiles;
            }
        }
    }
    
    switch ( photometric )
    {
        case PHOTOMETRIC_RGB:
        case PHOTOMETRIC_YCBCR:
            dimc = samplesperpixel;
            break;
        case PHOTOMETRIC_MINISWHITE:
        case PHOTOMETRIC_MINISBLACK:
            dimc = 1;
            break;
        case PHOTOMETRIC_PALETTE:
            for (unsigned short cc = 0; cc < 256; cc++ )
            {
                if ( (colormapR+cc) != (colormapG+cc) || (colormapR+cc) != (colormapB+cc) )
                {
                    dimc = 3;
                    break;
                }
            }
            dimc = 1;
            break;
        default:
            dimc = 4;
    }
    
    // spacing
    if ( m_RESOLUTIONUNIT > 0 && m_XRESOLUTION > 0 && m_YRESOLUTION > 0 )
    {
        if ( m_RESOLUTIONUNIT == 2 ) // inches
        {
            resx = 25.4 / m_XRESOLUTION;
            resy = 25.4 / m_YRESOLUTION;
        }
        else if ( m_RESOLUTIONUNIT == 3 ) // cm
        {
            resx = 10.0 / m_XRESOLUTION;
            resy = 10.0 / m_YRESOLUTION;
        }
    }
    
    //
    return true;
}

int TiffIO::read()
{
    if ( !m_TiffImage )
    {
        if ( !this->canReadFile(m_FileName) )
        {
            cout<< "Cannot open file: " << this->m_FileName << endl;
            return -1;
        }
    }
    
    if(bitspersample != 8 && samplesperpixel > 1)
    {
        cout<<"Can not handle bits/sample != 8"<<endl;
        return -2;
    }
    
    tsize_t scanlinesize = TIFFRasterScanlineSize(m_TiffImage);
    long bytes = (long)scanlinesize * (long)length*dimz*dimc*dimt;
    
    try
    {
        m_Data = malloc(bytes);
    }
    catch (...)
    {
        cout<<"Fail to allocate memory for image buffer"<<endl;
        return -2;
    }
    
    //tsize_t scanlinesize = TIFFScanlineSize(m_TiffImage);
    if (!scanlinesize)
    {
        cout<<"Scanline size is zero"<<endl;
        return -4;
    }
    
    int bychunk = 0;
    uint32 irps = (uint32) -1L;
    TIFFGetField(m_TiffImage, TIFFTAG_ROWSPERSTRIP, &irps);
    
    if(irps==rowsperstrip) bychunk = 1;
    
    for(uint32 z=0; z<dimz; z++)
    {
        // tif image [x,y,c,z,t]
        uint8* bufp = (uint8*) m_Data + z*dimx*dimy*dimc*bitspersample/8;
        
        if ( m_IgnoredSubFiles > 0 )
        {
            int32 subfiletype = 6;
            if ( TIFFGetField(m_TiffImage, TIFFTAG_SUBFILETYPE, &subfiletype) )
            {
                if ( subfiletype & FILETYPE_REDUCEDIMAGE || subfiletype & FILETYPE_MASK )
                {
                    // skip subfile
                    TIFFReadDirectory(m_TiffImage);
                    continue;
                }
            }
        }
        
        if(config==PLANARCONFIG_CONTIG)
        {
            if(bTiled)
            {
                // ContigTiles
                tsize_t tilesize = TIFFTileSize(m_TiffImage);
                tdata_t tilebuf = NULL;
                uint32 imagew = TIFFScanlineSize(m_TiffImage);
                uint32 tilew  = TIFFTileRowSize(m_TiffImage);
                int iskew = imagew - tilew;
                
                try
                {
                    tilebuf = _TIFFmalloc(tilesize);
                    _TIFFmemset(tilebuf, 0, tilesize); // init
                    
                    uint32 row;
                    for (row = 0; row < length; row += tilelength)
                    {
                        uint32 nrow = (row+tilelength > length) ? length-row : tilelength;
                        uint32 colb = 0;
                        
                        uint32 col;
                        for (col = 0; col < width; col += tilewidth)
                        {
                            if (TIFFReadTile(m_TiffImage, tilebuf, col, row, 0, 0) < 0)
                            {
                                TIFFError(TIFFFileName(m_TiffImage), "Error, can't read tile at %lu %lu", (unsigned long) col, (unsigned long) row);
                                _TIFFfree(tilebuf);
                                return -3;
                            }
                            if (colb + tilew > imagew)
                            {
                                uint32 width = imagew - colb;
                                uint32 oskew = tilew - width;
                                
                                uint8* out = bufp + colb;
                                uint8* in = (uint8*) tilebuf;
                                
                                uint32 i = nrow;
                                
                                while (i-- > 0)
                                {
                                    uint32 j = width;
                                    while (j-- > 0)
                                        *out++ = *in++;
                                    out += (oskew + iskew);
                                    in += oskew;
                                }
                            }
                            else
                            {
                                uint8* out = bufp + colb;
                                uint8* in = (uint8*) tilebuf;
                                
                                uint32 i = nrow;
                                
                                while (i-- > 0)
                                {
                                    uint32 j = tilew;
                                    while (j-- > 0)
                                        *out++ = *in++;
                                    out += iskew;
                                }
                            }
                            colb += tilew;
                        }
                        bufp += imagew * nrow;
                    }
                    _TIFFfree(tilebuf);
                }
                catch(...)
                {
                    cout<<"Fail to allocate tilebuf"<<endl;
                    return -1;
                }
            }
            else
            {
                // ContigStrips
                if(bychunk)
                {
                    tsize_t stripsize  = TIFFStripSize(m_TiffImage);
                    
                    _TIFFmemset(bufp, 0, stripsize); // init
                    
                    tstrip_t s, ns = TIFFNumberOfStrips(m_TiffImage);
                    uint32 row = 0;
                    
                    //cout<<"ns "<<ns<<" stripsize "<<stripsize<<" rowsperstrip "<<rowsperstrip<<endl;
                    
                    for (s = 0; s < ns; s++)
                    {
                        tsize_t cc = (row + rowsperstrip > length) ? TIFFVStripSize(m_TiffImage, length - row) : stripsize;
                        if (TIFFReadEncodedStrip(m_TiffImage, s, bufp, cc) < 0)
                        {
                            TIFFError(TIFFFileName(m_TiffImage), "Error, can't read strip %lu", (unsigned long) s);
                        }
                        row += rowsperstrip;
                        bufp += cc;
                        
                        //cout<<"s "<<s<<" row "<<row<<" cc "<<cc<<endl;
                    }
                }
                else
                {
                    uint32 row;
                    for (row = 0; row < length; row++)
                    {
                        if (TIFFReadScanline(m_TiffImage, (tdata_t) bufp, row, 0) < 0)
                        {
                            TIFFError(TIFFFileName(m_TiffImage),"Error, can't read scanline %lu",(unsigned long) row);
                            return -3;
                        }
                        bufp += scanlinesize;
                    }
                    
                }
                
            }
        }
        else if(config==PLANARCONFIG_SEPARATE)
        {
            if(bTiled)
            {
                // SeparateTiles
                uint32 imagew = TIFFRasterScanlineSize(m_TiffImage);
                uint32 tilew = TIFFTileRowSize(m_TiffImage);
                int iskew  = imagew - tilew*samplesperpixel;
                tsize_t tilesize = TIFFTileSize(m_TiffImage);
                tdata_t tilebuf = NULL;
                uint16 bytes_per_sample;
                
                try
                {
                    tilebuf = _TIFFmalloc(tilesize);
                    _TIFFmemset(tilebuf, 0, tilesize);
                    
                    if( bitspersample % 8 != 0 )
                    {
                        cout<<"Invalid bitspersample"<<endl;
                        return -5;
                    }
                    bytes_per_sample = bitspersample/8;
                    
                    uint32 row;
                    for (row = 0; row < length; row += tilelength)
                    {
                        uint32 nrow = (row+tilelength > length) ? length-row : tilelength;
                        uint32 colb = 0;
                        uint32 col;
                        
                        for (col = 0; col < width; col += tilewidth)
                        {
                            tsample_t s;
                            
                            for (s = 0; s < samplesperpixel; s++)
                            {
                                if (TIFFReadTile(m_TiffImage, tilebuf, col, row, 0, s) < 0)
                                {
                                    TIFFError(TIFFFileName(m_TiffImage),"Error, can't read tile at %lu %lu, ""sample %lu",(unsigned long) col,(unsigned long) row,(unsigned long) s);
                                    _TIFFfree(tilebuf);
                                    return -3;
                                }
                                
                                // Tile is clipped horizontally.  Calculate visible portion and skewing factors.
                                if (colb + tilew*samplesperpixel > imagew)
                                {
                                    uint32 width = imagew - colb;
                                    int oskew = tilew*samplesperpixel - width;
                                    
                                    uint8* out = bufp+colb+s*bytes_per_sample;
                                    uint8* in = (uint8*) tilebuf;
                                    
                                    uint32 i = nrow;
                                    
                                    while (i-- > 0)
                                    {
                                        uint32 j = width/(samplesperpixel*bytes_per_sample);
                                        while (j-- > 0)
                                        {
                                            int n = bytes_per_sample;
                                            
                                            while( n-- )
                                            {
                                                *out++ = *in++;
                                            }
                                            out += (samplesperpixel-1)*bytes_per_sample;
                                        }
                                        out += oskew + iskew;
                                        in += oskew/samplesperpixel;
                                    }
                                }
                                else
                                {
                                    uint8* out = bufp+colb+s*bytes_per_sample;
                                    uint8* in = (uint8*) tilebuf;
                                    
                                    uint32 i = nrow;
                                    
                                    while (i-- > 0)
                                    {
                                        uint32 j = tilewidth;
                                        while (j-- > 0)
                                        {
                                            int n = bytes_per_sample;
                                            
                                            while( n-- )
                                            {
                                                *out++ = *in++;
                                            }
                                            out += (samplesperpixel-1)*bytes_per_sample;
                                        }
                                        out += iskew;
                                    }
                                }
                            }
                            colb += tilew*samplesperpixel;
                        }
                        bufp += imagew * nrow;
                    }
                    _TIFFfree(tilebuf);
                }
                catch(...)
                {
                    cout<<"Fail to allocate tilebuf"<<endl;
                    return -1;
                }
            }
            else
            {
                // SeparateStrips
                tdata_t scanline = NULL;
                try
                {
                    scanline = _TIFFmalloc(scanlinesize);
                    _TIFFmemset(scanline, 0, scanlinesize); // init
                    
                    uint32 row;
                    for (row = 0; row < length; row++)
                    {
                        // merge channels
                        tsample_t s;
                        for (s = 0; s < samplesperpixel; s++) {
                            uint8* bp = bufp + s;
                            tsize_t n = scanlinesize;
                            uint8* sbuf = (uint8*) scanline;
                            
                            if (TIFFReadScanline(m_TiffImage, scanline, row, s) < 0)
                            {
                                TIFFError(TIFFFileName(m_TiffImage),"Error, can't read scanline %lu",(unsigned long) row);
                                _TIFFfree(scanline);
                                return -3;
                            }
                            while (n-- > 0)
                                *bp = *sbuf++, bp += samplesperpixel;
                        }
                        bufp += scanlinesize * samplesperpixel;
                    }
                    _TIFFfree(scanline);
                }
                catch(...)
                {
                    cout<<"Fail to allocate scanline"<<endl;
                    return -1;
                }
            }
        }
        else
        {
            cout<<"Unkown configuration!"<<endl;
            return -3;
        }
        TIFFReadDirectory(m_TiffImage); // read the next 2d image
    }
    
    //
    changeImageOrder(true);
    
    //
    return 0;
}

bool TiffIO::canWriteFile(char *fileNameToWrite)
{
    setFileName(fileNameToWrite);
    
    const char *mode;
    mode = "w";
    
    long totalpixels = dimx*dimy*dimz*dimc*dimt;
    
    const long oneKiloByte = 1024;
    const long oneMegaByte = 1024 * oneKiloByte;
    const long oneGigaByte = 1024 * oneMegaByte;
    const long twoGigaBytes = 2 * oneGigaByte;
    
    if( totalpixels > twoGigaBytes )
    {
#ifdef TIFF_INT64_T  // detect if libtiff4, enables the use of big tiff
        mode = "w8";
#else
        cout<<"Size of image exceeds the limit of libtif."<<endl;
#endif
    }
    
    m_TiffImage = TIFFOpen(this->m_FileName, mode);
    if ( !m_TiffImage )
    {
        cout<< "Cannot open file: " << this->m_FileName << endl;
        close();
        return false;
    }
    
    return true;
}

int TiffIO::write()
{
    if ( !m_TiffImage )
    {
        if ( !this->canWriteFile(this->m_FileName) )
        {
            cout<< "Cannot open file: " << this->m_FileName << endl;
            close();
            return -1;
        }
    }
    TIFF* tif = (TIFF*)m_TiffImage;
    
    if(!m_Data || dimx<1 || dimy<1 || dimz<1 || dimc<1 || dimt<1 || m_DataType<1)
    {
        cout<<"Invalid data for output!"<<endl;
        return -2;
    }
    else
    {
        //
        changeImageOrder(false);
    }
    
    //
    uint32 page;
    
    uint32 width =  dimx;
    uint32 height = dimy;
    uint32 pages = dimz;
    uint32 scomponents = dimc;
    uint32 rowsperstrip = ( uint32 ) - 1;
    
    switch ( m_DataType )
    {
        case UCHAR:
            bitspersample = 8;
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            break;
        case CHAR:
            bitspersample = 8;
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
            break;
        case USHORT:
            bitspersample = 16;
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            break;
        case SHORT:
            bitspersample = 16;
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
            break;
        case FLOAT:
            bitspersample = 32;
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
            break;
        default:
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_VOID);
            cout<< "TIFF image only supports unsigned/signed char, unsigned/signed short, and float."<<endl;
            close();
            return -1;
    }
    
    uint32 w = width;
    uint32 h = height;
    
    if ( dimz > 1 )
    {
        TIFFCreateDirectory(tif);
    }
    
    char *p = (char *)m_Data;
    for ( page = 0; page < pages; page++ )
    {
        TIFFSetDirectory(tif, page);
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, scomponents);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        switch (m_DataType) {
            case UCHAR:
            case USHORT:
                TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
                break;
            case CHAR:
            case SHORT:
                TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
                break;
            case FLOAT:
                TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
                break;
            default:
                TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_VOID);
                break;
        }
        
        TIFFSetField(tif, TIFFTAG_SOFTWARE, "vigl"); //
        
        if ( scomponents > 3 )
        {
            // if number of scalar components is greater than 3, that means we assume there is alpha.
            uint16  extra_samples = scomponents - 3;
            uint16 *sample_info = NULL;
            
            new1dp<uint16, uint32>(sample_info, scomponents - 3);
            
            sample_info[0] = EXTRASAMPLE_ASSOCALPHA;
            uint16 cc;
            for ( cc = 1; cc < scomponents - 3; cc++ )
            {
                sample_info[cc] = EXTRASAMPLE_UNSPECIFIED;
            }
            TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, extra_samples, sample_info);
            del1dp<uint16>(sample_info);
        }
        
        int compressiontype;
        
        switch ( compression )
        {
            case PackBits:
                compressiontype = COMPRESSION_PACKBITS; break;
            case JPEG:
                compressiontype = COMPRESSION_JPEG; break;
            case Deflate:
                compressiontype = COMPRESSION_DEFLATE; break;
            case LZW:
                compressiontype = COMPRESSION_LZW; break;
            default:
                compressiontype = COMPRESSION_NONE;
        }
        
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compressiontype); // Fix for compression
        
        uint16 photometric = ( scomponents == 1 ) ? PHOTOMETRIC_MINISBLACK : PHOTOMETRIC_RGB;
        
        int predictor;
        if ( compression == COMPRESSION_JPEG )
        {
            TIFFSetField(tif, TIFFTAG_JPEGQUALITY, 75); // Parameter
            TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
            photometric = PHOTOMETRIC_YCBCR;
        }
        else if ( compression == COMPRESSION_LZW )
        {
            predictor = 2;
            TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
            cout<< "LZW compression is patented outside US so it is disabled"<<endl;
        }
        else if ( compression == COMPRESSION_DEFLATE )
        {
            predictor = 2;
            TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
        }
        
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric); // Fix for scomponents
        
        TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, rowsperstrip) );
        
        // spacing
        if ( resx > 0 && resy > 0 )
        {
            m_XRESOLUTION = resx / 0.254;
            m_YRESOLUTION = resy / 0.254;
            
            TIFFSetField(tif, TIFFTAG_XRESOLUTION, m_XRESOLUTION);
            TIFFSetField(tif, TIFFTAG_YRESOLUTION, m_YRESOLUTION);
            TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
        }
        
        if ( dimz > 1 )
        {
            TIFFSetField(tif, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
            TIFFSetField(tif, TIFFTAG_PAGENUMBER, page, pages);
        }
        int rowLength; // in bytes
        
        switch ( m_DataType )
        {
            case UCHAR:
                rowLength = sizeof( unsigned char );
                break;
            case USHORT:
                rowLength = sizeof( unsigned short );
                break;
            case CHAR:
                rowLength = sizeof( char );
                break;
            case SHORT:
                rowLength = sizeof( short );
                break;
            case FLOAT:
                rowLength = sizeof( float );
                break;
            default:
                cout << "TIFF image only supports unsigned/signed char, unsigned/signed short, and float"<<endl;
                close();
                return -1;
        }
        
        rowLength *= dimc;
        rowLength *= width;
        
        int row = 0;
        for ( unsigned int ih = 0; ih < height; ih++ )
        {
            if ( TIFFWriteScanline(tif, const_cast< char * >( p ), row, 0) < 0 )
            {
                cout<< "Error out of disk space" <<endl;
                break;
            }
            p += rowLength;
            row++;
        }
        
        if ( dimz > 1)
        {
            TIFFWriteDirectory(tif);
        }
    }
    
    //
    close();
    
    //
    return 0;
}

//
// NiftiIO
//

NiftiIO::NiftiIO() : BioMedicalDataIO()
{
    // supported file suffix
    // .nia .nii .nii.gz
    // .hdr .img .img.gz

    m_NiftiImage = NULL;

    m_FileName = NULL;
    inputFileName.clear();
    outputFileName.clear();
    m_Data=NULL;

    m_PixelType = UNKNOWNPIXELTYPE;
    m_DataType = UNKNOWNDATATYPE;
    m_FileFormat = UNKNOWNFILEFORMAT;

    m_LegacyAnalyze75Mode = true;

    m_RescaleIntercept = 0.0;
    m_RescaleSlope = 1.0;

    dimx = 1;
    dimy = 1;
    dimz = 1;
    dimc = 1;
    dimt = 1;

    resx = 1.0;
    resy = 1.0;
    resz = 1.0;
    resc = 1.0;
    rest = 1.0;
}

NiftiIO::~NiftiIO()
{
    nifti_image_free(this->m_NiftiImage);
}

//
bool NiftiIO::mustRescale()
{
    return y_abs<double>(this->m_RescaleSlope) > numeric_limits< double >::epsilon()
            && ( y_abs<double>(this->m_RescaleSlope - 1.0) > numeric_limits< double >::epsilon()
                 || y_abs<double>(this->m_RescaleIntercept) > numeric_limits< double >::epsilon() );
}

void NiftiIO::setPixelType(IOPixelType pt)
{
    this->m_PixelType = pt;
}

//
int NiftiIO::isCompressed(const char * filename)
{
    const char *tempextension = nifti_find_file_extension( filename );
    if ( tempextension == NULL )
    {
        cout << "Bad Nifti file name. No extension found for file: " << this->getFileName() << endl;
    }
    const std::string ExtensionName(tempextension);

    const std::string::size_type ext = ExtensionName.rfind(".gz");
    int iscompressed = ( ext == std::string::npos ) ? 0 : 1;

    return iscompressed;
}

// reading
bool NiftiIO::canReadFile(char *fileNameToRead)
{
    setFileName(fileNameToRead);

    // is_nifti_file returns
    //       > 0 for a nifti file
    //      == 0 for an analyze file,
    //       < 0 for an error,

    const char * filename = reinterpret_cast<const char *>(fileNameToRead);

    const int image_FTYPE = is_nifti_file(filename);

    if ( image_FTYPE > 0 )
    {
        return true;
    }
    else if (image_FTYPE == 0)
    {
        this->m_LegacyAnalyze75Mode=true;
        return true;
    }

    /* image_FTYPE < 0 */
    return false;
}

int NiftiIO::read()
{
    const char * filename = reinterpret_cast<const char *>(this->getFileName());
    m_GZFile = znzopen(filename, "rb", isCompressed(filename));
    if (znz_isnull(m_GZFile))
    {
        cout << "Error: failed to read file " << filename << endl;
        return -1;
    }

    this->m_NiftiImage = nifti_image_read(filename, 1);

    static std::string prev;
    if ( prev != this->getFileName() )
    {
        prev = this->getFileName();
    }
    if ( this->m_NiftiImage == 0 )
    {
        cout << this->getFileName() << " is not recognized as a NIFTI file!" <<endl;
        return -1;
    }

    //Check the intent code, it is a vector image, or matrix image, then this is not true.
    int dims = 0;

    cout<<"test ... ndim "<<this->m_NiftiImage->ndim<<endl;

    if ( this->m_NiftiImage->intent_code == NIFTI_INTENT_VECTOR || this->m_NiftiImage->intent_code == NIFTI_INTENT_SYMMATRIX )
    {
        dimc = this->m_NiftiImage->dim[5];

        if(this->m_NiftiImage->dim[5] > 1)
        {
            dims = 5;
        }
        else if ( this->m_NiftiImage->dim[4] > 1 )
        {
            dims = 4;
        }
        else if ( this->m_NiftiImage->dim[3] > 1 )
        {
            dims = 3;
        }
        else if ( this->m_NiftiImage->dim[2] > 1 )
        {
            dims = 2;
        }
        else
        {
            dims = 1;
        }
    }
    else if ( this->m_NiftiImage->intent_code == NIFTI_INTENT_GENMATRIX )
    {
        //TODO:  NEED TO DEAL WITH CASE WHERE NIFTI_INTENT_MATRIX
        cout<< this->getFileName() << " has an intent code of NIFTI_INTENT_GENMATRIX which is not yet implemented in ITK" <<endl;
        return -1;
    }
    else
    {
        cout<<"Simple Scalar Image"<<endl;

        // Simple Scalar Image
        //
        // Apparently some straight-from-the-scanner files report as 4D
        // with T = 1; this causes ImageFileReader to erroneously ignore the
        // reported
        // direction cosines.
        unsigned floatdim;
        for ( floatdim = this->m_NiftiImage->dim[0];
              this->m_NiftiImage->dim[floatdim] == 1 && floatdim > 3;
              floatdim-- )
        {}
        dims = floatdim;
        this->dimc = 1;
    }

    cout<<"dims ... "<<dims<<" dimt ... "<<this->m_NiftiImage->nt<<endl;

    switch ( this->m_NiftiImage->datatype )
    {
    case NIFTI_TYPE_INT8:
        this->m_DataType = CHAR;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_UINT8:
        this->m_DataType = UCHAR;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_INT16:
        this->m_DataType = SHORT;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_UINT16:
        this->m_DataType = USHORT;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_INT32:
        this->m_DataType = INT;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_UINT32:
        this->m_DataType = UINT;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_FLOAT32:
        this->m_DataType = FLOAT;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_FLOAT64:
        this->m_DataType = DOUBLE;
        this->m_PixelType = SCALAR;
        break;
    case NIFTI_TYPE_COMPLEX64:
        this->m_DataType = FLOAT;
        this->m_PixelType = COMPLEX;
        this->dimc = 2;
        break;
    case NIFTI_TYPE_COMPLEX128:
        this->m_DataType = DOUBLE;
        this->m_PixelType = COMPLEX;
        this->dimc = 2;
        break;
    case NIFTI_TYPE_RGB24:
        this->m_DataType = UCHAR;
        this->m_PixelType = RGB;
        this->dimc = 3;
        break;
    case NIFTI_TYPE_RGBA32:
        this->m_DataType = UCHAR;
        this->m_PixelType = RGBA;
        this->dimc = 4;
        break;
    default:
        break;
    }

    // there are a wide variety of intents we ignore
    // but a few wee need to care about
    switch ( this->m_NiftiImage->intent_code )
    {
    case NIFTI_INTENT_NONE:
        break;
    case NIFTI_INTENT_CORREL:
        break;
    case NIFTI_INTENT_TTEST:
        break;
    case NIFTI_INTENT_FTEST:
        break;
    case NIFTI_INTENT_ZSCORE:
        break;
    case NIFTI_INTENT_CHISQ:
        break;
    case NIFTI_INTENT_BETA:
        break;
    case NIFTI_INTENT_BINOM:
        break;
    case NIFTI_INTENT_GAMMA:
        break;
    case NIFTI_INTENT_POISSON:
        break;
    case NIFTI_INTENT_NORMAL:
        break;
    case NIFTI_INTENT_FTEST_NONC:
        break;
    case NIFTI_INTENT_CHISQ_NONC:
        break;
    case NIFTI_INTENT_LOGISTIC:
        break;
    case NIFTI_INTENT_LAPLACE:
        break;
    case NIFTI_INTENT_UNIFORM:
        break;
    case NIFTI_INTENT_TTEST_NONC:
        break;
    case NIFTI_INTENT_WEIBULL:
        break;
    case NIFTI_INTENT_CHI:
        break;
    case NIFTI_INTENT_INVGAUSS:
        break;
    case NIFTI_INTENT_EXTVAL:
        break;
    case NIFTI_INTENT_PVAL:
        break;
    case NIFTI_INTENT_LOGPVAL:
        break;
    case NIFTI_INTENT_LOG10PVAL:
        break;
    case NIFTI_INTENT_ESTIMATE:
        break;
    case NIFTI_INTENT_LABEL:
        break;
    case NIFTI_INTENT_NEURONAME:
        break;
    case NIFTI_INTENT_GENMATRIX:
        break;
    case NIFTI_INTENT_SYMMATRIX:
        this->setPixelType(SYMMETRICSECONDRANKTENSOR);
        break;
    case NIFTI_INTENT_DISPVECT:
        break;
    case NIFTI_INTENT_VECTOR:
        this->setPixelType(VECTOR);
        break;
    case NIFTI_INTENT_POINTSET:
        break;
    case NIFTI_INTENT_TRIANGLE:
        break;
    case NIFTI_INTENT_QUATERNION:
        break;
    case NIFTI_INTENT_DIMLESS:
        break;
    case NIFTI_INTENT_TIME_SERIES:
        break;
    case NIFTI_INTENT_NODE_INDEX:
        break;
    case NIFTI_INTENT_RGB_VECTOR:
        break;
    case NIFTI_INTENT_RGBA_VECTOR:
        break;
    case NIFTI_INTENT_SHAPE:
        break;
    }
    // set slope/intercept
    if ( this->m_NiftiImage->qform_code == 0 && this->m_NiftiImage->sform_code == 0 )
    {
        this->m_RescaleSlope = 1;
        this->m_RescaleIntercept = 0;
    }
    else
    {
        if ( ( this->m_RescaleSlope = this->m_NiftiImage->scl_slope ) == 0 )
        {
            this->m_RescaleSlope = 1;
        }
        this->m_RescaleIntercept = this->m_NiftiImage->scl_inter;
    }

    //
    // if rescale is necessary, promote type reported
    // to ImageFileReader to float
    if ( this->mustRescale() )
    {
        if ( this->m_DataType == CHAR
             || this->m_DataType == UCHAR
             || this->m_DataType == SHORT
             || this->m_DataType == USHORT
             || this->m_DataType == INT
             || this->m_DataType == UINT
             || this->m_DataType == LONG
             || this->m_DataType == ULONG )
        {
            this->m_DataType = FLOAT;
        }
    }
    //
    // set up the dimension stuff
    double spacingscale = 1.0; //default to mm
    switch ( XYZT_TO_SPACE(this->m_NiftiImage->xyz_units) )
    {
    case NIFTI_UNITS_METER:
        spacingscale = 1e3;
        break;
    case NIFTI_UNITS_MM:
        spacingscale = 1e0;
        break;
    case NIFTI_UNITS_MICRON:
        spacingscale = 1e-3;
        break;
    }

    double timingscale = 1.0; //Default to seconds
    switch ( XYZT_TO_TIME(this->m_NiftiImage->xyz_units) )
    {
    case NIFTI_UNITS_SEC:
        timingscale = 1.0;
        break;
    case NIFTI_UNITS_MSEC:
        timingscale = 1e-3;
        break;
    case NIFTI_UNITS_USEC:
        timingscale = 1e-6;
        break;
    }

    switch ( dims )
    {
    case 7:
        //this->SetDimensions(6, this->m_NiftiImage->nw);
        //this->SetSpacing(6, this->m_NiftiImage->dw);
    case 6:
        //this->SetDimensions(5, this->m_NiftiImage->nv);
        //this->SetSpacing(5, this->m_NiftiImage->dv);
    case 5:
        this->setDimc(this->m_NiftiImage->nu);
        this->resc = this->m_NiftiImage->du;
    case 4:
        this->setDimt(this->m_NiftiImage->nt);
        this->rest = this->m_NiftiImage->dt * timingscale;
    case 3:
        this->setDimz(this->m_NiftiImage->nz);
        this->resz = this->m_NiftiImage->dz * spacingscale;
    case 2:
        this->setDimy(this->m_NiftiImage->ny);
        this->resy = this->m_NiftiImage->dy * spacingscale;
    case 1:
        this->setDimx(this->m_NiftiImage->nx);
        this->resx = this->m_NiftiImage->dx * spacingscale;
        break;
    default:
        cout << this->getFileName() << " has " << dims << " dimensions, and is not supported or invalid!" <<endl;
        return -1;
    }

    // dims = m_NiftiImage->dim[0];
    // dimx = m_NiftiImage->dim[1] = m_NiftiImage->nx;
    // dimy = m_NiftiImage->dim[2] = m_NiftiImage->ny;
    // dimz = m_NiftiImage->dim[3] = m_NiftiImage->nz;
    // dimt = m_NiftiImage->dim[4] = m_NiftiImage->nt;
    // dimc = m_NiftiImage->dim[5] = m_NiftiImage->nu; // vector

    //
    unsigned long pixelSize = this->m_NiftiImage->nbyper;
    unsigned long imSize = dimx*dimy*dimz*dimc*dimt;

    switch ( this->m_DataType )
    {
    case CHAR:
        new1dp< char, unsigned long >((char*&)m_Data, imSize);
        break;
    case UCHAR:
        new1dp< unsigned char, unsigned long >((unsigned char*&)m_Data, imSize);
        break;
    case SHORT:
        new1dp< short, unsigned long >((short*&)m_Data, imSize);
        break;
    case USHORT:
        new1dp< unsigned short, unsigned long >((unsigned short*&)m_Data, imSize);
        break;
    case INT:
        new1dp< int, unsigned long >((int*&)m_Data, imSize);
        break;
    case UINT:
        new1dp< unsigned int, unsigned long >((unsigned int*&)m_Data, imSize);
        break;
    case LONG:
        new1dp< long, unsigned long >((long*&)m_Data, imSize);
        break;
    case ULONG:
        new1dp< unsigned long, unsigned long >((unsigned long*&)m_Data, imSize);
        break;
    case FLOAT:
        new1dp< float, unsigned long >((float*&)m_Data, imSize);
        break;
    case DOUBLE:
        new1dp< double, unsigned long >((double*&)m_Data, imSize);
        break;
    case UNKNOWNDATATYPE:
        cout << "Bad OnDiskComponentType UNKNOWNDATATYPE" <<endl;
        return -1;
    }

    //
    const char *       niftibuf = reinterpret_cast<const char *>(this->m_NiftiImage->data);
    char *             p = (char *)m_Data;

    const long rowdist = dimx;
    const long slicedist = rowdist * dimy;
    const long volumedist = slicedist * dimz;
    const long seriesdist = volumedist * dimt;
    long t,z,y,x,c;

    for ( t = 0; t < dimt; t++ )
    {
        for ( z = 0; z < dimz; z++ )
        {
            for ( y = 0; y < dimy; y++ )
            {
                for ( x = 0; x < dimx; x++ )
                {
                    for ( c = 0; c < dimc; c++ )
                    {
                        const long nifti_index = ( c * seriesdist + volumedist * t + slicedist * z + rowdist * y + x ) * pixelSize;
                        const long index =( ( t * volumedist * dimc  + volumedist * c + slicedist * z + rowdist * y + x )  ) * pixelSize;

                        memcpy(p + index, niftibuf + nifti_index, pixelSize);
                    }
                }
            }
        }
    }

    //m_Data = (void *)p;

    // de-alloc
    nifti_image_free(this->m_NiftiImage);
    this->m_NiftiImage = 0;

    //
    return 0;
}

// writing
bool NiftiIO::canWriteFile(char *fileNameToWrite)
{
    const char * filename = reinterpret_cast<const char *>(fileNameToWrite);
    const int ValidFileNameFound = nifti_is_complete_filename(filename) > 0;

    this->setFileName(fileNameToWrite);

    return ValidFileNameFound;
}

int NiftiIO::write()
{
    return write(m_Data, dimx, dimy, dimz, dimc, dimt, m_DataType, resx, resy, resz);
}

int NiftiIO::write(const void *buffer, long sx, long sy, long sz, long sc, long st, int datatype, float srx, float sry, float srz)
{
    if ( this->m_NiftiImage == 0 )
    {
        this->m_NiftiImage = nifti_simple_init_nim();
    }

    const char * filename = reinterpret_cast<const char *>(this->getFileName());
    const char *tempextension = nifti_find_file_extension( filename );
    if ( tempextension == NULL )
    {
        cout << "Bad Nifti file name. No extension found for file: " << this->getFileName() <<endl;
    }
    const std::string ExtensionName(tempextension);

    const char *tempbasename = nifti_makebasename( filename );

    const std::string::size_type ext = ExtensionName.rfind(".gz");
    const bool IsCompressed = ( ext == std::string::npos ) ? false : true;

    bool b_nii = false; // hdr/img
    if ( ExtensionName == ".nii" || ExtensionName == ".nii.gz" )
    {
        this->m_NiftiImage->nifti_type = NIFTI_FTYPE_NIFTI1_1;
        b_nii = true;
    }
    else if ( ExtensionName == ".nia" )
    {
        this->m_NiftiImage->nifti_type = NIFTI_FTYPE_ASCII;
    }
    else if ( ExtensionName == ".hdr" || ExtensionName == ".img" || ExtensionName == ".hdr.gz" || ExtensionName == ".img.gz" )
    {
        this->m_NiftiImage->nifti_type = NIFTI_FTYPE_NIFTI1_2;
        //this->m_NiftiImage->nifti_type = NIFTI_FTYPE_ANALYZE; //NOTE: OREINTATION IS NOT WELL DEFINED IN THIS FORMAT.
    }
    else
    {
        cout << "Bad Nifti file name: " << this->getFileName() << endl;
        return -1;
    }
    this->m_NiftiImage->fname = nifti_makehdrname(tempbasename, this->m_NiftiImage->nifti_type, false, IsCompressed);
    this->m_NiftiImage->iname = nifti_makeimgname(tempbasename, this->m_NiftiImage->nifti_type, false, IsCompressed);

    if(this->m_PixelType==VECTOR)
        this->m_NiftiImage->intent_code = NIFTI_INTENT_VECTOR;

    //
    this->m_NiftiImage->nx = sx;
    this->m_NiftiImage->ny = sy;
    this->m_NiftiImage->nz = sz;
    this->m_NiftiImage->nt = st;
    this->m_NiftiImage->nu = sc;

    if(sc>1)
    {
        this->m_NiftiImage->nvox = sc;
        this->m_NiftiImage->ndim = 5;
    }
    else if(st>1)
    {
        this->m_NiftiImage->nvox = st;
        this->m_NiftiImage->ndim = 4;
    }
    else if (sz>1)
    {
        this->m_NiftiImage->nvox = sz;
        this->m_NiftiImage->ndim = 3;
    }
    else if (sy>1)
    {
        this->m_NiftiImage->nvox = sy;
        this->m_NiftiImage->ndim = 2;
    }
    else
    {
        this->m_NiftiImage->nvox = sx;
        this->m_NiftiImage->ndim = 1;
    }

    cout<<" ndim ... "<<this->m_NiftiImage->ndim<<endl;

    //
    switch ( datatype )
    {
    case UCHAR:
        this->m_NiftiImage->datatype = NIFTI_TYPE_UINT8;
        this->m_NiftiImage->nbyper = 1;
        break;
    case CHAR:
        this->m_NiftiImage->datatype = NIFTI_TYPE_INT8;
        this->m_NiftiImage->nbyper = 1;
        break;
    case USHORT:
        this->m_NiftiImage->datatype = NIFTI_TYPE_UINT16;
        this->m_NiftiImage->nbyper = 2;
        break;
    case SHORT:
        this->m_NiftiImage->datatype = NIFTI_TYPE_INT16;
        this->m_NiftiImage->nbyper = 2;
        break;
    case ULONG:
    case UINT:
        this->m_NiftiImage->datatype = NIFTI_TYPE_UINT32;
        this->m_NiftiImage->nbyper = 4;
        break;
    case LONG:
    case INT:
        this->m_NiftiImage->datatype = NIFTI_TYPE_INT32;
        this->m_NiftiImage->nbyper = 4;
        break;
    case FLOAT:
        this->m_NiftiImage->datatype = NIFTI_TYPE_FLOAT32;
        this->m_NiftiImage->nbyper = 4;
        break;
    case DOUBLE:
        this->m_NiftiImage->datatype = NIFTI_TYPE_FLOAT64;
        this->m_NiftiImage->nbyper = 8;
        break;
    case UNKNOWNDATATYPE:
    default:
        cout<< "Nifti image only supports unsigned/signed char, unsigned/signed short, and float."<<endl;
        return -1;
    }
    this->m_NiftiImage->scl_slope = 1.0f;
    this->m_NiftiImage->scl_inter = 0.0f;

    this->m_NiftiImage->xyz_units = static_cast< int >(NIFTI_UNITS_MICRON | NIFTI_UNITS_SEC);
    this->m_NiftiImage->pixdim[0] = 0;
    this->m_NiftiImage->pixdim[1] = this->m_NiftiImage->dx = 1e3*srx; // mm by default
    this->m_NiftiImage->pixdim[2] = this->m_NiftiImage->dy = 1e3*sry; // mm by default
    this->m_NiftiImage->pixdim[3] = this->m_NiftiImage->dz = 1e3*srz; // mm by default

    // data
    unsigned long numVoxels = sx*sy*sz*sc*st;
    unsigned long buffer_size = numVoxels*(unsigned long)(this->m_NiftiImage->nbyper);

    char *nifti_buf = NULL;
    new1dp<char,unsigned long>(nifti_buf,buffer_size);

    const char *const buf = reinterpret_cast<const char *>(buffer);

    // nifti_layout[vec][t][z][y][x]
    const long rowdist = sx;
    const long slicedist = rowdist * sy;
    const long volumedist = slicedist * sz;
    const long seriesdist = volumedist * st;
    long t,z,y,x,c;

    for ( t = 0; t < st; t++ )
    {
        for ( z = 0; z < sz; z++ )
        {
            for ( y = 0; y < sy; y++ )
            {
                for ( x = 0; x < sx; x++ )
                {
                    for ( c = 0; c < sc; c++ )
                    {
                        const long nifti_index = ( c * seriesdist + volumedist * t + slicedist * z + rowdist * y + x ) * (long)(m_NiftiImage->nbyper);
                        const long index =( ( t * volumedist * sc  + volumedist * c + slicedist * z + rowdist * y + x )  ) * (long)(m_NiftiImage->nbyper);

                        memcpy(nifti_buf + nifti_index, buf + index, m_NiftiImage->nbyper);
                    }
                }
            }
        }
    }

    //
    m_GZFile = znzopen(filename, "wb", isCompressed(this->m_NiftiImage->fname));
    if (znz_isnull(m_GZFile))
    {
        cout << "Error: failed to write file " << this->m_NiftiImage->fname << endl;
        return -1;
    }
    else
    {
        if(b_nii == 1)
        {
            // 0 = do not write data and close (do not open data file)
            // 1 = write data and close
            // 2 = do not write data and leave data file open
            // 3 = write data and leave data file open
            nifti_image_write_hdr_img2(m_NiftiImage,2,"wb",m_GZFile,NULL);
            nifti_write_buffer(m_GZFile, nifti_buf, buffer_size);
        }
        else
        {
            nifti_image_write_hdr_img(m_NiftiImage,0,"wb");
            nifti_write_buffer(m_GZFile, buf, buffer_size);
        }
    }
    znzclose( m_GZFile );

    // de-alloc
    del1dp<char>(nifti_buf);

    //
    return 0;
}
