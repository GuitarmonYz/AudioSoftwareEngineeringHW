
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Vibrato.h"
#include <thread>

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void callFromThread(CAudioFileIf *phAudioFile, CAudioFileIf *phOutputFile, int kBlockSize, float** ppfAudioData, float** ppfOutputData, CVibrato* pcVibrato );

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;

    static const int        kBlockSize = 1024;

    float                   **ppfAudioData = 0;
    float                   **ppfOutputData = 0;

    CAudioFileIf            *phAudioFile = 0;
    CAudioFileIf            *phOutputFile = 0;
    CAudioFileIf::FileSpec_t stFileSpec;
    CVibrato                *pcVibrato = 0;
    
    int                     iNumChannels = 0;
    int                     iSampleRateInHz = 44100;
    float                   fMaxWidthInS = 0.1f;
    float                   fWidth = 0.0005f;
    float                   fModFreq = 5;
    
    sInputFilePath = "./sweep.wav";
    sOutputFilePath = "./output.wav";
    
    showClInfo();
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
    iNumChannels = stFileSpec.iNumChannels;
    //////////////////////////////////////////////////////////////////////////////
    // prepare the output wav file
    CAudioFileIf::create(phOutputFile);
    phOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    //////////////////////////////////////////////////////////////////////////////
    // viberato related initialization
    CVibrato::create(pcVibrato);
    pcVibrato->init(iSampleRateInHz, fMaxWidthInS, iNumChannels);
    pcVibrato->setParam(CVibrato::kParamModWidth, fWidth);
    pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory for input and output data buffer
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    ppfOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfAudioData[i] = new float[kBlockSize];
        ppfOutputData[i] = new float[kBlockSize];
    }
    //////////////////////////////////////////////////////////////////////////////
    // process file
//    while (!phAudioFile->isEof())
//    {
//        long long iNumFrames = kBlockSize;
//        phAudioFile->readData(ppfAudioData, iNumFrames);
//        pcVibrato->process(ppfAudioData, ppfOutputData, iNumFrames);
//        phOutputFile->writeData(ppfOutputData, iNumFrames);
//    }
    
    std::thread t1(callFromThread);
    std::cout << "executing" << std::endl;
    
    t1.join();
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phOutputFile);
    CVibrato::destroy(pcVibrato);
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfOutputData[i];
    delete[] ppfOutputData;
    ppfAudioData = 0;
    ppfOutputData = 0;
    return 0;
}

void callFromThread(CAudioFileIf *phAudioFile, CAudioFileIf *phOutputFile, int kBlockSize, float** ppfAudioData, float** ppfOutputData, CVibrato* pcVibrato ) {
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pcVibrato->process(ppfAudioData, ppfOutputData, iNumFrames);
        phOutputFile->writeData(ppfOutputData, iNumFrames);
    }
    
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;
    return;
}

