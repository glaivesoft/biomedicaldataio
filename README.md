# biomedicaldataio
biomedicaldataio is a software tool for reading and writing biomedical data using standardized, open formats.

Follow the Occam's razor principle, here we design a simple biomedical data structure and I/O for reading and writing biomedical data with standard file formats. Simply, our biomedical data structure consists of (1) 1D data pointer and (2) meta/header information. Meta information consists of (1) origin offsets, (2) spacing resolution/voxelsize, and (3) size information. Currently we design this data structure with 5D and is stored in the specified order XYZCT.

### data structure

```
BioMedicalData: 1D pointer + meta

meta: origin + spacing + size

origin:  x,y,z,c,t
spacing: x,y,z,c,t
size:    x,y,z,c,t
```

## Release 0.1.0 (1/19/2017)
The release 0.1.0 supports [LibTIFF][](.tif), [NIfTI][](.nii, .nii.gz), and RAW (.raw) file formats. RAW can be your customized file format that need come with a pain text file or command line parameters explaining the image's meta information, for example, the image size, voxelsize, saving order, etc.

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
bmData->setDataType(datatype);
bmData->origin.setXYZCT(ox,oy,oz,oc,ot);
bmData->spacing.setXYZCT(vx,vy,vz,vc,vt);
bmData->size.setXYZCT(sx,sy,sz,sc,st);
bmData->zeros();

BioMedicalDataIO bmDataIO;
bmDataIO.setData(bmData);
```

##
[LibTIFF]: http://libtiff.maptools.org/
[NIfTI]: https://nifti.nimh.nih.gov/
