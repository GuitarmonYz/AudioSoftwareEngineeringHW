
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void    generateUnit(float** testSig, int sig_len, int testNumOfChannel);
void    resetTestOutput (float** testSignal, int sig_len, int testNumOfChannel);
void    test_1(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel);
void    test_2(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;
    float                   gain = 0;
    float                   delay = 0;
    std::string             filter_type = "";
    static const int        kBlockSize = 1024;
    static const float      maxDelay = 1;
    static const int        testSigLenInSample = 1024;
    static const int        testSampleRate = 44100;
    static const int        testNumOfChannel = 2;
    float                   testSigDelayInSample = 441;
    

    float                   **testSignal = 0;
    float                   **testOutputData = 0;
    float                   **ppfAudioData = 0;
    float                   **ppfOutputData = 0;
    
    
    bool                    isTestMode = false;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf   *pInstance = 0;
    CCombFilterIf   *pFIRInstance = 0;
    CCombFilterIf   *pIIRInstance = 0;
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 2) isTestMode = true;
    
    if (isTestMode)
    {
        CCombFilterIf::create(pFIRInstance);
        CCombFilterIf::create(pIIRInstance);
    }
    else
    {
        CCombFilterIf::create(pInstance);
        sInputFilePath = argv[1];
        sOutputFilePath = argv[2];
        gain = atof(argv[3]);
        delay = atof(argv[4]);
        filter_type = argv[5];
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    if (!isTestMode) {
        CAudioFileIf::create(phAudioFile);
        phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
        if (!phAudioFile->isOpen())
        {
            cout << "Wave file open error!"<<endl;
            return -1;
        }
        phAudioFile->getFileSpec(stFileSpec);
        hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
        if (!hOutputFile.is_open())
        {
            cout << "Text file open error!";
            return -1;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    
    if (isTestMode) {
        testSignal = new float*[testNumOfChannel];
        testOutputData = new float*[testNumOfChannel];
        for (int i = 0; i < testNumOfChannel; i++) {
            testSignal[i] = new float[testSigLenInSample];
            testOutputData[i] = new float[testSigLenInSample];
        }
        generateUnit(testSignal, testSigLenInSample, testNumOfChannel);
        pFIRInstance->init(CCombFilterIf::kCombFIR, maxDelay, testSampleRate, testNumOfChannel);
        pIIRInstance->init(CCombFilterIf::kCombIIR, maxDelay, testSampleRate, testNumOfChannel);
    } else {
        ppfAudioData = new float*[stFileSpec.iNumChannels];
        ppfOutputData = new float*[stFileSpec.iNumChannels];
        for (int i = 0; i < stFileSpec.iNumChannels; i++) {
            ppfAudioData[i] = new float[kBlockSize];
            ppfOutputData[i] = new float[kBlockSize];
        }
        if (filter_type.compare("fir") == 0){
            pInstance->init(CCombFilterIf::kCombFIR, maxDelay, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
            
        } else if (filter_type.compare("iir") == 0) {
            pInstance->init(CCombFilterIf::kCombIIR, maxDelay, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        }
        pInstance->setParam(CCombFilterIf::kParamGain, gain);
        pInstance->setParam(CCombFilterIf::kParamDelay, delay);
    }

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    if (isTestMode) {
        test_1(testSignal, testOutputData, testSigLenInSample, pFIRInstance, pIIRInstance, testSigDelayInSample, testSampleRate, testNumOfChannel);
        test_2(testSignal, testOutputData, testSigLenInSample, pFIRInstance, pIIRInstance, testSigDelayInSample, testSampleRate, testNumOfChannel);
    } else {
        while (!phAudioFile->isEof())
        {
            long long iNumFrames = kBlockSize;
            phAudioFile->readData(ppfAudioData, iNumFrames);
            pInstance->process(ppfAudioData, ppfOutputData, iNumFrames);
            for (int i = 0; i < iNumFrames; i++)
            {
                for (int c = 0; c < stFileSpec.iNumChannels; c++)
                {
                    hOutputFile << ppfOutputData[c][i] << "\t";
                }
                hOutputFile << endl;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    
    if (isTestMode) {
        CCombFilterIf::destroy(pFIRInstance);
        CCombFilterIf::destroy(pIIRInstance);
        for (int i = 0; i < testNumOfChannel; i++) {
            delete[] testSignal[i];
        }
        delete[] testSignal;
        for (int i = 0; i < testNumOfChannel; i++) {
            delete [] testOutputData[i];
        }
        delete[] testOutputData;
    } else {
        CAudioFileIf::destroy(phAudioFile);
        CCombFilterIf::destroy(pInstance);
        hOutputFile.close();
        
        for (int i = 0; i < stFileSpec.iNumChannels; i++)
            delete[] ppfAudioData[i];
        delete[] ppfAudioData;
        for (int i = 0; i < stFileSpec.iNumChannels; i++)
            delete[] ppfOutputData[i];
        delete[] ppfOutputData;
        ppfAudioData = 0;
        ppfOutputData = 0;
        
    }
    
    return 0;

}

void resetTestOutput (float** testSignal, int sig_len, int testNumOfChannel) {
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < sig_len; j++) {
            testSignal[i][j] = 0;
        }
    }
}

void generateUnit (float** testSignal, int sig_len, int testNumOfChannel) {
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < sig_len; j++) {
            testSignal[i][j] = 1;
        }
    }
}

// test with FIR, given unit signal and given normal gain(-1,1)
void test_1(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel) {
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pFIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pFIRInstance->setParam(CCombFilterIf::kParamDelay, delayInSample/(float)sampleRate);
    pFIRInstance->process(testSignal, testOutputData, sig_len);
    bool isPass = true;
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < delayInSample; j++) {
            if (testOutputData[i][j] != 1 ) isPass = false;
        }
        for (int j = delayInSample + 1; j < sig_len; j++) {
            if (testOutputData[i][j] != 1.5) isPass = false;
        }
    }
    if (isPass) {
        cout << "test case 1 passed" << endl;
    } else {
        cout << "test case 1 failed" << endl;
    }
}
// test with IIR, given unit signal and normal gain(-1,1)
void test_2(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel) {
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pIIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pIIRInstance->setParam(CCombFilterIf::kParamDelay, delayInSample/(float)sampleRate);
    pIIRInstance->process(testSignal, testOutputData, sig_len);
    bool isPass = true;
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < delayInSample; j++) {
            if (testOutputData[i][j] != 1 ) isPass = false;
        }
        for (int j = delayInSample + 1; j < delayInSample * 2; j++) {
            if (testOutputData[i][j] != 1.5) isPass = false;
        }
        for (int j = delayInSample * 2 + 1; j < sig_len; j++ ) {
            if (testOutputData[i][j] != 1.75) isPass = false;
        }
    }
    if (isPass) {
        cout << "test case 2 passed" << endl;
    } else {
        cout << "test case 2 failed" << endl;
    }
}
// test on FIR, with different delay length, see if setParam works
void test_3() {
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

