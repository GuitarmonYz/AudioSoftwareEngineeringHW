
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
int    test_1(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int test_signal_length);
int    test_2(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal);
/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    // You don't need to use an audio file this time, though you can try if you want.
    // std::string             sInputFilePath,                 //!< file paths
    //                         sOutputFilePath;
    // static const int        kBlockSize          = 1024;
    // float                   **ppfAudioData      = 0;
    // CAudioFileIf            *phAudioFile        = 0;
    // CAudioFileIf::FileSpec_t stFileSpec;
    // long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    CRingBuffer<float>      *pCRingBuffer       = 0;
    float                   *pfTestSignal       = 0;
    float                   fGain               = 1.f;
    int                     buffer_length       = atoi(argv[1]);
    int                     test_signal_length  = atoi(argv[2]);
    
    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    pCRingBuffer = new CRingBuffer<float>(buffer_length);
    
    // fill the test signal (e.g., a unit impulse)
    pfTestSignal = new float[test_signal_length];
    for (int i = 0; i < test_signal_length; i++) {
        pfTestSignal[i] = i;
    }
    //////////////////////////////////////////////////////////////////////////////
    // do processing and tests
    if (test_1(pCRingBuffer, pfTestSignal, test_signal_length) == 0)
        cout << "test case 1 passed" << endl;
    test_2(pCRingBuffer, pfTestSignal);
    cout << "processing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    delete [] pfTestSignal;
    delete pCRingBuffer;
    return 0;
}

int test_1 (CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int test_signal_length) {
    for (int i = 0; i < test_signal_length; i++) {
        pCRingBuffer->putPostInc(pfTestSignal[i]);
        float read_res = pCRingBuffer->getPostInc();
        if (read_res != pfTestSignal[i]) {
            cout << "read and write sample value doesn't match" << endl;
            return -1;
        }
    }
    
    return 0;
}

int test_2 (CRingBuffer<float> *pCRingBuffer, float *pfTestSignal) {
    pCRingBuffer->getPostInc();
    return 0;
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

