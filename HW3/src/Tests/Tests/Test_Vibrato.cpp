#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <iostream>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "ErrorDef.h"

#include "Vibrato.h"

SUITE(Vibrato)
{
    struct VibratoData
    {
        VibratoData()
        {
            // setup
            // e.g., allocate a vibrato object and test signal (newly created for each test case)
            iSampleRateInHz = 44100;
            iTestSigLength = 8192;
            iNumChannels = 2;
            iBlockLength = 1024;
            fMaxWidthInS = 0.1f;
//            fWidth = 0.0025f;
//            fModFreq = 5;
            CVibrato::create(pcVibrato);
            
            ppfTemp = new float*[iNumChannels];
            ppfTestSig = new float*[iNumChannels];
            for (int i = 0; i < iNumChannels; i++) {
                ppfTemp[i] = new float[iTestSigLength];
                ppfTestSig[i] = new float[iTestSigLength];
            }
            pcVibrato->init(iSampleRateInHz, fMaxWidthInS, iNumChannels);
//            pcVibrato->setParam(CVibrato::kParamModWidth, fWidth);
//            pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        }

        ~VibratoData()
        {
            // teardown
            // e.g., deallocate the vibrato object and test signal
            CVibrato::destroy(pcVibrato);
            for (int i = 0; i < iNumChannels; i++) {
                delete [] ppfTemp[i];
                delete [] ppfTestSig[i];
            }
            delete[] ppfTemp;
            delete[] ppfTestSig;
        }

        // e.g., a reusable process() function

        // e.g., a member vibrato object to be reused in each test
        CVibrato* pcVibrato = 0;
        int iSampleRateInHz;
        int iTestSigLength;
        int iNumChannels;
        int iBlockLength;
        float fMaxWidthInS;
        float fWidth;
        float fModFreq;
        float **ppfTestSig = 0;
        float **ppfTemp = 0;
        
        void resetTempBuffer() {
            for (int i = 0; i < iNumChannels; i++) {
                for (int j = 0; j < iTestSigLength; j++) {
                    ppfTemp[i][j] = 0;
                }
            }
        }
    };
    
    TEST_FIXTURE(VibratoData, ModulationWithZero) {
        resetTempBuffer();
        for (int i = 0; i < iNumChannels; i++) {
            CSynthesis::generateSine(ppfTestSig[i], 300, iSampleRateInHz, iTestSigLength);
        }
        fWidth = 0;
        fModFreq = 5;
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        VibratoData::pcVibrato->process(ppfTestSig, ppfTemp, iTestSigLength);
        for (int i = 0; i < iNumChannels; i++) {
            CHECK_ARRAY_CLOSE(ppfTestSig[i], ppfTemp[i], iTestSigLength, 1e-3);
        }
        std::cout << "ModualtionWithZero Passed" << std::endl;
    }
    
    TEST_FIXTURE(VibratoData, DCInput) {
        resetTempBuffer();
        pcVibrato->reset();
        for (int i = 0; i < iNumChannels; i++) {
            CSynthesis::generateDc(ppfTestSig[i], iTestSigLength);
        }
        fWidth = 0.0025f;
        fModFreq = 5.0f;
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);

        VibratoData::pcVibrato->process(ppfTestSig, ppfTemp, iTestSigLength);
        for (int i = 0; i < iNumChannels; i++) {
            CHECK_ARRAY_CLOSE(ppfTestSig[i], ppfTemp[i], iTestSigLength, 1e-3);
        }
        std::cout << "DCInput Passed" << std::endl;
    }
    
    TEST_FIXTURE(VibratoData, ZeroInput) {
        resetTempBuffer();
        pcVibrato->reset();
        for (int i = 0; i < iNumChannels; i++) {
            for (int j = 0; j < iTestSigLength; j++) {
                ppfTestSig[i][j] = 0;
            }
        }
        fWidth = 0.0025f;
        fModFreq = 5;
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        VibratoData::pcVibrato->process(ppfTestSig, ppfTemp, iTestSigLength);
        for (int i = 0; i < iNumChannels; i++) {
            CHECK_ARRAY_CLOSE(ppfTestSig[i], ppfTemp[i], iTestSigLength, 1e-3);
        }
        std::cout << "ZeroInput passed" << std::endl;
    }
    
    TEST_FIXTURE(VibratoData, VaryingBlockSize) {
        resetTempBuffer();
        pcVibrato->reset();
        
    }
    
	TEST_FIXTURE(VibratoData, vibratoParamUpdate) {
		resetTempBuffer();
		pcVibrato->reset();
		fWidth = 0.05;
		float fNewWidth = 0.06;
		pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
		CHECK_CLOSE(pcVibrato->getParam(CVibrato::VibratoParam_t::kParamModWidth), fWidth, 1e-4);
		pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fNewWidth);
		std::cout << "Assigning modulation width test passed";
		getchar();
	}
    
    
}

#endif //WITH_TESTS
