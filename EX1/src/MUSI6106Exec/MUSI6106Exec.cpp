
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;

    static const int        kBlockSize = 1024;
    
    int                     blockLength = 0;
    int                     hopLength = 0;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CFft                    *pcFft = 0;
    CRingBuffer<float>*      hopRingBuffer = 0;
    float*                   hopBuffer = 0;
    float*                   spectrumBuffer = 0;
    
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 4)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        blockLength = atoi(argv[3]);
        hopLength =atoi(argv[4]);
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }
    
    ///////////////////////////
    // fft related
    CFft::createInstance(pcFft);
    pcFft->initInstance(blockLength, 1, CFft::WindowFunction_t::kWindowHann, CFft::Windowing_t::kPreWindow);
    hopRingBuffer = new CRingBuffer<float>(blockLength);
    hopBuffer = (float*)calloc(blockLength, sizeof(float));
    spectrumBuffer = (float*)calloc(blockLength, sizeof(float));
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    long long iNumFrames = blockLength;
    phAudioFile->readData(ppfAudioData, iNumFrames);
    hopRingBuffer->put(ppfAudioData[0], iNumFrames);
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = hopLength;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        hopRingBuffer->putPostInc(ppfAudioData[0], iNumFrames);
        
        hopRingBuffer->getPostInc(hopBuffer, blockLength);
//        for (int i = 0; i < blockLength; i++) {
//            cout << hopBuffer[i] << endl;
//        }
        pcFft->doFft(spectrumBuffer, hopBuffer);
//        cout << "\r" << "reading and writing";
        

        for (int i = 0; i < iNumFrames; i++)
        {
            

            hOutputFile << spectrumBuffer[i] << " ";
        }
        hOutputFile << endl;
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

