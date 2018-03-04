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
        int startPoint = fWidth * 2 * iSampleRateInHz;
        for (int i = 0; i < iNumChannels; i++) {
            for (int j = startPoint; j < iTestSigLength; j++) {
                CHECK_CLOSE(ppfTestSig[i][j], ppfTemp[i][j], 1e-3);
            }
        }
        std::cout << "DCInput Passed" << std::endl;
    }
    
    TEST_FIXTURE(VibratoData, VaryingBlockSize) {
        resetTempBuffer();
        pcVibrato->reset();
        for (int i = 0; i < iNumChannels; i++) {
            CSynthesis::generateDc(ppfTestSig[i], iTestSigLength);
        }
        fWidth = 0.0005f;
        fModFreq = 5;
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        float** inputBuffer = new float*[iNumChannels];
        float** outputBuffer = new float*[iNumChannels];
        float** totalOutputBuffer = new float*[iNumChannels];
        iBlockLength = 1024;
        for (int i = 0; i < iNumChannels; i++) {
            inputBuffer[i] = new float[iBlockLength];
            outputBuffer[i] = new float[iBlockLength];
            totalOutputBuffer[i] = new float[iTestSigLength];
        }
        for (int i = 0; i < iNumChannels; i++) {
            for (int j = 0; j < iTestSigLength; j+=iBlockLength) {
                for (int k = j, t=0; t < iBlockLength; k++, t++) {
                    inputBuffer[i][t] = ppfTestSig[i][k];
                }
                VibratoData::pcVibrato->process(inputBuffer, outputBuffer, iBlockLength);
                for (int k = j, t=0; t < iBlockLength; k++, t++) {
                    totalOutputBuffer[i][k] = outputBuffer[i][t];
                    
                }
            }
        }
        pcVibrato->reset();
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
        pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModFreq, fModFreq);
        VibratoData::pcVibrato->process(ppfTestSig, ppfTemp, iTestSigLength);
        for (int i = 0; i < iNumChannels; i++) {
            for (int j = fWidth * iSampleRateInHz * 2; j < iTestSigLength; j++)
                CHECK_CLOSE(totalOutputBuffer[i][j], ppfTemp[i][j], 1e-3);
        }
        std::cout << "varying block length test pass" << std::endl;
        for (int i = 0; i < iNumChannels; i++) {
            delete[] inputBuffer[i];
            delete[] outputBuffer[i];
            delete[] totalOutputBuffer[i];
        }
        delete[] inputBuffer;
        delete [] outputBuffer;
        delete [] totalOutputBuffer;
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
    
    
    
	TEST_FIXTURE(VibratoData, vibratoParamModWidthUpdate) {
		resetTempBuffer();
		pcVibrato->reset();
		fWidth = 0.05;
		float fIncreaseWidth = 0.06;
		float fDecreaseWidth = 0.04;
		pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fWidth);
		CHECK_CLOSE(pcVibrato->getParam(CVibrato::VibratoParam_t::kParamModWidth), fWidth, 1e-4);
		pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fIncreaseWidth);
		CHECK_CLOSE(pcVibrato->getParam(CVibrato::VibratoParam_t::kParamModWidth), fIncreaseWidth, 1e-4);
		pcVibrato->setParam(CVibrato::VibratoParam_t::kParamModWidth, fDecreaseWidth);
		CHECK_CLOSE(pcVibrato->getParam(CVibrato::VibratoParam_t::kParamModWidth), fDecreaseWidth, 1e-4);
		std::cout << "Assigning modulation width test passed";
		getchar();
	}
    
	TEST_FIXTURE(VibratoData, vibratoParamModFreqUpdate) {

	}
    
}

#endif //WITH_TESTS
