#include <iostream>
#include <ctime>
#include "dirent.h"
#include <fstream>
#include "MUSI6106Config.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
int    listFiles ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
    sOutputFilePath;
    
    long long               iInFileLength       = 0;        //!< length of input file
    
    long long               blockLength         = 1024LL;
    
    long long               curPos              = 0;
    
    clock_t                 time                = 0;
    
    float                   **ppfAudioData      = 0;
    
    CAudioFileIf::FileSpec_t fileSpec;
    
    CAudioFileIf            *phAudioFile        = 0;
    
    std::ofstream outdata;
    
    showClInfo ();
    
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argv[1] != NULL && argv[2] != NULL) {
        sInputFilePath = argv[1];
        sOutputFilePath = argv[2];
    } else {
        cout << "input argument is illegal" << endl;
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    phAudioFile->getFileSpec(fileSpec);
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time = clock();
    ppfAudioData = new float*[fileSpec.iNumChannels];
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        ppfAudioData[i] = new float[blockLength];
    }
    // get audio data and write it to the output file
    outdata.open(sOutputFilePath);
    long long writePos = 0;
    while (!phAudioFile->isEof()) {
        phAudioFile->readData(ppfAudioData, blockLength);
        phAudioFile->getPosition(curPos);
        for (int j = 0; j < blockLength; j++) {
            //cout << ppfAudioData[0][j] << " " << ppfAudioData[1][j] << endl;
            for (int k = 0; k < fileSpec.iNumChannels; k++) {
                outdata << ppfAudioData[k][j] << " ";
            }
            outdata << endl;
        }
        writePos = curPos;
    }
    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;
    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout
    phAudioFile->getLength(iInFileLength);
    cout << "File Length: " << iInFileLength << endl;
    cout << "File Format: " << fileSpec.eFormat << endl;
    cout << "File Bit stream Type " << fileSpec.eBitStreamType << endl;
    cout << "File Sample Rate " << fileSpec.fSampleRateInHz << endl;
    cout << "File Number of Channels " << fileSpec.iNumChannels << endl;
    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Foo processing" << endl << endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    phAudioFile->destroy(phAudioFile);
    outdata.close();
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete [] ppfAudioData[i];
    }
    delete [] ppfAudioData;
    return 0;
    
}


void showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;
    
    return;
}

int listFiles() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            printf ("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return -1;
    }
    return 0;
}
