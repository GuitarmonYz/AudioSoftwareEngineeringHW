
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void    test_1();
void    test_2();

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
    
//    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   **ppfOutputData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf   *pInstance = 0;
    CCombFilterIf   *pFIRInstance = 0;
    CCombFilterIf   *pIIRInstance = 0;
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 2)
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
//        sOutputFilePath = sInputFilePath;
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!"<<endl;
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

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    
    if (argc < 2) {
        pFIRInstance->init(CCombFilterIf::kCombFIR, delay, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        pIIRInstance->init(CCombFilterIf::kCombIIR, delay, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
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
        cout << delay << endl;
        cout << pInstance->getParam(CCombFilterIf::kParamDelay) << endl;
        
    }
    
    
    //time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    if (argc < 2) {
        // test　 cases
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
//                    cout<< ppfOutputData[c][i] << endl;
                    hOutputFile << ppfOutputData[c][i] << "\t";
                    
                }
                hOutputFile << endl;
            }
        }
    }
    

    //cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    if (argc < 2) {
        CCombFilterIf::destroy(pFIRInstance);
        CCombFilterIf::destroy(pIIRInstance);
    } else {
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

void test_1() {
    // test ??
}

void test_2() {
    // test ??
}

void test_3() {
    // test ??
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

