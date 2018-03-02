
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Vibrato.h"

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

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   **ppfOutputData = 0;

    CAudioFileIf            *phAudioFile = 0;
    CAudioFileIf            *phOutputFile = 0;
//    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CVibrato                *pcVibrato = 0;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
//    if (argc < 2)
//    {
//        cout << "Missing audio input path!";
//        return -1;
//    }
//    else
//    {
//        sInputFilePath = argv[1];
//        sOutputFilePath = sInputFilePath + ".txt";
//    }
    sInputFilePath = "./piano.wav";
    sOutputFilePath = "./output.wav";
    int iSampleRateInHz = 44100;
    float fMaxWidthInS = 0.1f;
    float fWidth = 0.0002f;
    float fModFreq = 5;

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    CAudioFileIf::create(phOutputFile);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
    phOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    int iNumChannels = stFileSpec.iNumChannels;
    
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
//    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
//    if (!hOutputFile.is_open())
//    {
//        cout << "Text file open error!";
//        return -1;
//    }
    ///////////////////
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
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pcVibrato->process(ppfAudioData, ppfOutputData, iNumFrames);
        phOutputFile->writeData(ppfOutputData, iNumFrames);
    }


    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phOutputFile);

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfOutputData[i];
    delete[] ppfOutputData;
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

