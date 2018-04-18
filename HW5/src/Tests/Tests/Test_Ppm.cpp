#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>

#include "UnitTest++.h"
#include "Ppm.h"
#include "Synthesis.h"

SUITE(Ppm)
{
    struct PpmData {
        PpmData() {
            CPpm::createInstance(pPpm, sampleRate, numOfChannels);
            
        }
        ~PpmData() {
            CPpm::destroyInstance(pPpm);
            if (testData != nullptr) {
                for (int i = 0; i < numOfChannels; i++) {
                    delete [] testData[i];
                }
                delete [] testData;
            }
            if (outputBuffer != nullptr) {
                delete [] outputBuffer;
            }
            if (groundTruth != nullptr) {
                delete [] groundTruth;
            }
        }
        int const sampleRate = 44100;
        int const numOfChannels = 2;
        int const blockSize = 1024;
        CPpm* pPpm = 0;
        float** testData = 0;
        float* outputBuffer = 0;
        float* groundTruth = 0;
        int numOfBlocks = 2;
    };
    
    TEST_FIXTURE(PpmData, ValidFunctionArgs) {
        pPpm->init();
        CHECK_EQUAL(kFunctionInvalidArgsError, pPpm->process(testData, outputBuffer, 3));
    }
    
    TEST_FIXTURE(PpmData, IsInitialized) {
        CHECK_EQUAL(kNotInitializedError, pPpm->process(testData, outputBuffer, 3));
    }
    
    TEST_FIXTURE(PpmData, ZeroInput) {
        pPpm->init();
        testData = new float*[numOfChannels];
        outputBuffer = new float[numOfChannels];
        groundTruth = new float[numOfChannels];
        for (int i = 0; i < numOfChannels; i++) {
            groundTruth[i] = 0;
            testData[i] = new float[blockSize*numOfBlocks];
            for (int j = 0; j < blockSize*numOfBlocks; j++) {
                testData[i][j] = 0;
            }
        }
        for (int i = 0; i < numOfChannels; i++) {
            outputBuffer[i] = 0;
        }
        for (int i = 0; i < numOfBlocks; i++) {
            pPpm->process(testData, outputBuffer, blockSize);
            CHECK_ARRAY_CLOSE(groundTruth, outputBuffer, numOfChannels, 1e-3);
        }
    }
    
    TEST_FIXTURE(PpmData, DcInput) {
        pPpm->init();
        testData = new float*[numOfChannels];
        outputBuffer = new float[numOfChannels];
        groundTruth = new float[numOfChannels];
        for (int i=0; i < numOfChannels; i++) {
            groundTruth[i] = 1;
            testData[i] = new float[blockSize*numOfBlocks];
            for (int j = 0; j < blockSize*numOfBlocks; j++) {
                testData[i][j] = 1;
            }
        }
        for (int i = 0; i < numOfChannels; i++) {
            outputBuffer[i] = 0;
        }
        pPpm->setAlphaAT(0.1);
        for (int i =0; i < numOfBlocks; i++) {
            pPpm->process(testData, outputBuffer, blockSize);
            CHECK_ARRAY_CLOSE(groundTruth, outputBuffer, numOfChannels, 1e-3);
        }
    }
    
    TEST_FIXTURE(PpmData, SinInput) {
        pPpm->init();
        testData = new float*[numOfChannels];
        outputBuffer = new float[numOfChannels];
        groundTruth = new float[numOfChannels];
        for (int i = 0; i < numOfChannels; i++) {
            testData[i] = new float[blockSize*numOfBlocks];
            groundTruth[i] = 1;
            CSynthesis::generateSine(testData[i], 440, 44100, blockSize*numOfBlocks);
        }
        for (int i = 0; i < numOfChannels; i++) {
            outputBuffer[i] = 0;
        }
        pPpm->setAlphaAT(1);
        pPpm->setAlphaRT(0);
        for (int i = 0; i < numOfBlocks; i++) {
            pPpm->process(testData, outputBuffer, blockSize);
            CHECK_ARRAY_CLOSE(groundTruth, outputBuffer, numOfChannels, 1e-3);
        }
    }
    
    TEST_FIXTURE(PpmData, SawInput) {
        pPpm->init();
        testData = new float*[numOfChannels];
        outputBuffer = new float[numOfChannels];
        groundTruth = new float[numOfChannels];
        for (int i = 0; i < numOfChannels; i++) {
            testData[i] = new float[blockSize*numOfBlocks];
            groundTruth[i] = 1;
            for (int j = 0; j < blockSize; j++) {
                testData[i][j] = j * (1.0/1024.0);
            }
            for (int j = blockSize; j < blockSize*numOfBlocks; j++) {
                testData[i][j] = 2 - j * (1.0/1024.0);
            }
        }
        for (int i = 0; i < numOfChannels; i++) {
            outputBuffer[i] = 0;
        }
        pPpm->setAlphaRT(1);
        pPpm->setAlphaAT(1);
        for (int i = 0; i < numOfBlocks; i++) {
            pPpm->process(testData, outputBuffer, blockSize);
            CHECK_ARRAY_CLOSE(groundTruth, outputBuffer, numOfChannels, 1e-3);
        }
    }
}

#endif //WITH_TESTS
