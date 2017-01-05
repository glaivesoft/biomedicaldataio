# biomedicaldataio
biomedicaldataio is a software tool for reading and writing biomedical data using standardized, open formats.

Follow the Occam's razor principle, here we design a simple biomedical data structure and I/O for reading and writing biomedical data with standard file formats. Simply, our biomedical data structure consists of (1) 1D data pointer and (2) meta/header information. Meta information consists of (1) origin offsets, (2) spacing resolution/voxelsize, and (3) size information. Currently we design this data structure with 5D and is stored in the specified order XYZCT.

### data structure

```
1D pointer + meta

meta: origin + spacing + size

size: x,y,z,c,t
```

## Release 0.1
The release 0.1 supports [LibTIFF][], [NIfTI][], and RAW file formats.

An example of usage of biomedicaldataio can be found in the file test/imageReadWrite.

```
    // read
    BioMedicalDataIO bmData;

    if(bmData.readData(input)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    // write
    return bmData.writeData(output);
```

##
[LibTIFF]: http://libtiff.maptools.org/
[NIfTI]: https://nifti.nimh.nih.gov/
