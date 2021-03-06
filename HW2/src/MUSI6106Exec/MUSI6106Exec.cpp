
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
void    test_1(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, int delayInSample, int sampleRate, int testNumOfChannel);
void    test_2(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel);
void    test_3(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pIIRInstance, CCombFilterIf* pFIRInstance, int delayInSample, int sampleRate, int testNumOfChannel);
void    test_4(CCombFilterIf* pIIRInstance);
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
    CAudioFileIf            *pOutputFile = 0;
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
        
        CAudioFileIf::create(pOutputFile);
        
        
        if (!phAudioFile->isOpen())
        {
            cout << "Wave file open error!"<<endl;
            return -1;
        }
        phAudioFile->getFileSpec(stFileSpec);
        pOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
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
        test_1(testSignal, testOutputData, testSigLenInSample, pFIRInstance, testSigDelayInSample, testSampleRate, testNumOfChannel);
        test_2(testSignal, testOutputData, testSigLenInSample, pIIRInstance, testSigDelayInSample, testSampleRate, testNumOfChannel);
        test_3(testSignal, testOutputData, testSigLenInSample, pIIRInstance, pFIRInstance, testSigDelayInSample, testSampleRate, testNumOfChannel);
        test_4(pIIRInstance);
    } else {
        while (!phAudioFile->isEof())
        {
            long long iNumFrames = kBlockSize;
            phAudioFile->readData(ppfAudioData, iNumFrames);
            pInstance->process(ppfAudioData, ppfOutputData, iNumFrames);
            pOutputFile->writeData(ppfOutputData, iNumFrames);
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
        CAudioFileIf::destroy(pOutputFile);
        CCombFilterIf::destroy(pInstance);
        
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
void test_1(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pFIRInstance, int delayInSample, int sampleRate, int testNumOfChannel) {
    float error_threshold = 0.0001;
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pFIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pFIRInstance->setParam(CCombFilterIf::kParamDelay, delayInSample/(float)sampleRate);
    pFIRInstance->process(testSignal, testOutputData, sig_len);
    bool isPass = true;
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < delayInSample; j++) {
            if (testOutputData[i][j] < 1 - error_threshold || testOutputData[i][j] > 1 + error_threshold ) isPass = false;
        }
        for (int j = delayInSample + 1; j < sig_len; j++) {
            if (testOutputData[i][j] < 1.5 - error_threshold || testOutputData[i][j] > 1.5 + error_threshold) isPass = false;
        }
    }
    if (isPass) {
        cout << "test case 1 passed" << endl;
    } else {
        cout << "test case 1 failed" << endl;
    }
}
// test with IIR, given unit signal and normal gain(-1,1)
void test_2(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pIIRInstance, int delayInSample, int sampleRate, int testNumOfChannel) {
    float error_threshold = 0.0001;
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pIIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pIIRInstance->setParam(CCombFilterIf::kParamDelay, delayInSample/(float)sampleRate);
    pIIRInstance->process(testSignal, testOutputData, sig_len);
    bool isPass = true;
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < delayInSample; j++) {
            if (testOutputData[i][j] < 1 - error_threshold || testOutputData[i][j] > 1 + error_threshold ) isPass = false;
        }
        for (int j = delayInSample + 1; j < delayInSample * 2; j++) {
            if (testOutputData[i][j] < 1.5 - error_threshold || testOutputData[i][j] > 1.5 + error_threshold) isPass = false;
        }
        for (int j = delayInSample * 2 + 1; j < sig_len; j++ ) {
            if (testOutputData[i][j] < 1.75 - error_threshold || testOutputData[i][j] > 1.75 + error_threshold) isPass = false;
        }
    }
    if (isPass) {
        cout << "test case 2 passed" << endl;
    } else {
        cout << "test case 2 failed" << endl;
    }
}
// test on FIR, check reset function by varying delay length
void test_3(float** testSignal, float** testOutputData, int sig_len, CCombFilterIf* pIIRInstance, CCombFilterIf* pFIRInstance, int delayInSample, int sampleRate, int testNumOfChannel) {
    float error_threshold = 0.0001;
    bool isPass = true;
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pIIRInstance->reset();
    pIIRInstance->init(CCombFilterIf::kCombIIR, 1, sampleRate, testNumOfChannel);
    pFIRInstance->reset();
    pFIRInstance->init(CCombFilterIf::kCombFIR, 1, sampleRate, testNumOfChannel);
    pFIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pFIRInstance->setParam(CCombFilterIf::kParamDelay, sig_len / (float)sampleRate);
    pFIRInstance->process(testSignal, testOutputData, sig_len);
    
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < sig_len; j++) {
            if (testOutputData[i][j] < 1 - error_threshold || testOutputData[i][j] > 1 + error_threshold) isPass = false;
        }
    }
    
    resetTestOutput(testOutputData, sig_len, testNumOfChannel);
    pIIRInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pIIRInstance->setParam(CCombFilterIf::kParamDelay, sig_len / (float)sampleRate);
    pIIRInstance->process(testSignal, testOutputData, sig_len);
    for (int i = 0; i < testNumOfChannel; i++) {
        for (int j = 0; j < sig_len; j++) {
            if (testOutputData[i][j] < 1 - error_threshold || testOutputData[i][j] > 1 + error_threshold) isPass = false;
        }
    }
    //    pFIRInstance->setParam(CCombFilterIf::kParamDelay, delayInSample/(float)sampleRate/2.0);
    if (isPass) {
        cout << "test case 3 passed" << endl;
    } else {
        cout << "test case 3 failed" << endl;
    }
}

// test whether the in range check prevent iir from exploding
void test_4(CCombFilterIf* pIIRInstance){
    float highGain = 12;
    if (pIIRInstance->setParam(CCombFilterIf::kParamGain, highGain)==kNoError) {
        cout << "test case 4 failed" << endl;
    } else {
        
        cout << "test case 4 passed" << endl;
    }
}




void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

