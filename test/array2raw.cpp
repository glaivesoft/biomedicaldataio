// convert a 2D array into a raw file
// developed by Yang Yu (gnayuy@gmail.com)
// Usage: array2raw <input> <output>

// g++ -o array2raw array2raw.cpp

#include <cstdio>
#include <iostream>
#include <fstream>
//#include <ofstream>
//#include <ifstream>

using namespace std;

//
int main(int argc, char *argv[])
{
    //
    if(argc<3)
    {
        cout<<"please enter at least 2 file names\n";
        cout<<"\te.g. array2raw array.txt array.raw\n";
        return -1;
    }
    
    //
    ifstream arrayFile;
    arrayFile.open(argv[1]);
    
    ofstream rawFile;
    rawFile.open(argv[2], ios::out | ios::app | ios::binary);
    
    //
    float v;
    long count = 0;
    
    //
    if (arrayFile.is_open() && rawFile.is_open()) {
        while (!arrayFile.eof()) {

            arrayFile >> v;
            rawFile << v;
            
            count++;
        }
    }
    arrayFile.close();
    rawFile.close();
    
    // test the size of raw file
    
    cout<<"size of data "<<count<<" * sizeof(float)"<<sizeof(float)<<endl;
    
    arrayFile.open(argv[2]);
    
    float *p=NULL;
    
    try{
        p = new float [count];
    }
    catch(...){
        cout<<"Fail to allocate memory\n";
        return -1;
    }
    
    if(arrayFile.is_open()) {
        
//        size_t begin = arrayFile.tellg();
//        arrayFile.seekg (0, ios::end);
//        size_t end = arrayFile.tellg();
//        cout << "size is: " << (end-begin) << " bytes.\n";
        
        
        if(!arrayFile.read ((char*)(p), count*sizeof(float)))
        {
            cout<<"Fail to read data."<<endl;
            arrayFile.close();
            return -1;
        }
        
    }
    arrayFile.close();
    
    //
    return 0;
}

