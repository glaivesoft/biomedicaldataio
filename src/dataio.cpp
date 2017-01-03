// dataio.cpp
// read/write .tif, ...
// developed by Yang Yu (gnayuy@gmail.com)

#include "dataio.h"

// tiff
TiffIO::TiffIO()
{
    // supported file suffix
    // .tif
    
    m_TiffImage = NULL;
    this->m_FileName = NULL;
    
    m_DataType = UNKNOWNDATATYPE;
    
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

// file names
char* TiffIO::getFileName()
{
    return this->m_FileName;
}

void TiffIO::setFileName(char* fileName)
{
    if(!fileName)
    {
        cout<<"Invalid file name"<<endl;
        return;
    }

    if(!this->m_FileName || strcmp (this->m_FileName,fileName) != 0)
    {
        new1dp<char, int>(this->m_FileName, strlen(fileName) + 1);
        strcpy(this->m_FileName,fileName);
    }
    
    return;
}

// dimensions
long TiffIO::getDimX()
{
    return this->dimx;
}

long TiffIO::getDimY()
{
    return this->dimy;
}

long TiffIO::getDimZ()
{
    return this->dimz;
}

long TiffIO::getDimC()
{
    return this->dimc;
}

long TiffIO::getDimT()
{
    return this->dimt;
}

void TiffIO::setDimX(long x)
{
    if(x<=0)
    {
        cout << "invalid x dimension"<<endl;
    }
    this->dimx = x;
}

void TiffIO::setDimY(long y)
{
    if(y<=0)
    {
        cout << "invalid y dimension"<<endl;
    }
    this->dimy = y;
}

void TiffIO::setDimZ(long z)
{
    if(z<=0)
    {
        cout << "invalid z dimension"<<endl;
    }
    this->dimz = z;
}

void TiffIO::setDimC(long c)
{
    if(c<=0)
    {
        cout << "invalid c dimension"<<endl;
    }
    this->dimc = c;
}

void TiffIO::setDimT(long t)
{
    if(t<=0)
    {
        cout << "invalid t dimension"<<endl;
    }
    this->dimt = t;
}

float TiffIO::getResX()
{
    return resx;
}

float TiffIO::getResY()
{
    return resy;
}

float TiffIO::getResZ()
{
    return resz;
}

void TiffIO::setResX(float resolution_x)
{
    if(resolution_x<=0)
    {
        cout<<"Invalide x resolution"<<endl;
        return;
    }
    resx = resolution_x;
    return;
}

void TiffIO::setResY(float resolution_y)
{
    if(resolution_y<=0)
    {
        cout<<"Invalide y resolution"<<endl;
        return;
    }
    resy = resolution_y;
    return;
}

void TiffIO::setResZ(float resolution_z)
{
    if(resolution_z<=0)
    {
        cout<<"Invalide z resolution"<<endl;
        return;
    }
    resz = resolution_z;
    return;
}

int TiffIO::getDataType()
{
    return m_DataType;
}

void TiffIO::setDataType(int dt)
{
    this->m_DataType = (DataType)dt;
}

void* TiffIO::getData()
{
    return m_Data;
}
void TiffIO::setData(void *p)
{
    m_Data = p;
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

