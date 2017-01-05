// test biomedicaldataio
// imageReadWrite <input> <output>

#include "biomedicaldataio.h"

int main(int argc, char *argv[])
{
    //
    if(argc<3)
    {
        cout<<"need at least input and output file names\n";
        return -1;
    }

    string input(argv[1]);
    string output(argv[2]);

    // read
    BioMedicalDataIO bmdata;

    if(bmdata.readData(input)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    // write
    return bmdata.writeData(output);
}
