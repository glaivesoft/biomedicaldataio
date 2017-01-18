# biomedicaldataio
biomedicaldataio is a software tool for reading and writing biomedical data using standardized, open formats.

Follow the Occam's razor principle, here we design a simple biomedical data structure and I/O for reading and writing biomedical data with standard file formats. Simply, our biomedical data structure consists of (1) 1D data pointer and (2) meta/header information. Meta information consists of (1) origin offsets, (2) spacing resolution/voxelsize, and (3) size information. Currently we design this data structure with 5D and is stored in the specified order XYZCT.

### data structure

```
1D pointer + meta

meta: origin + spacing + size

size: x,y,z,c,t
```

## Release 0.1.0 (1/19/2017)
The release 0.1 supports [LibTIFF][](.tif), [NIfTI][](.nii, .nii.gz), and RAW (.raw) file formats. RAW can be your customized file format that need come with a pain text file or command line parameters explaining the image's meta information, for example, the image size, voxelsize, saving order, etc.

An example of usage of biomedicaldataio can be found in the file test/imageReadWrite.

```
// read
BioMedicalDataIO bmDataIO;

if(bmDataIO.readData(input)!=0){
    cout<<"Fail to read data!"<<endl;
    return -1;
}

...

// write
bmDataIO.writeData(output);

```

Init a new empty data:

```
BioMedicalData *bmData = new BioMedicalData();
bmData->zeros(bytes); // bytes=bmData->size.size();
bmData->origin.setXYZCT(x,y,z,c,t);
bmData->spacing.setXYZCT(x,y,z,c,t);
bmData->size.setXYZCT(x,y,z,c,t);

BioMedicalDataIO bmDataIO;
bmDataIO.setData(bmData);
```

##
[LibTIFF]: http://libtiff.maptools.org/
[NIfTI]: https://nifti.nimh.nih.gov/
