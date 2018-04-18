#include "ErrorDef.h"
#include "Ppm.h"
#include <math.h>

Error_t CPpm::createInstance(CPpm *&pCPpm, int sampleRate, int numOfChannels)
{
    pCPpm = new CPpm (sampleRate, numOfChannels);
    if (!pCPpm)
        return kUnknownError;

    return kNoError;
}

Error_t CPpm::destroyInstance(CPpm *&pCPpm)
{
    if (!pCPpm)
        return kUnknownError;

    delete pCPpm;
    pCPpm = 0;

    return kNoError;
}

CPpm::CPpm(int sampleRate, int numOfChannels):
m_fs(sampleRate),
m_numOfChannels(numOfChannels)
{
    m_alpha_AT = 1 - exp(-2.2 / (m_fs * 0.01));
    m_alpha_RT = 1 - exp(-2.2 / (m_fs * 1.5));
    m_preBlockVal = new float[numOfChannels];
    for (int i = 0; i < numOfChannels; i++) {
        m_preBlockVal[i] = 0;
    }
}

CPpm::~CPpm()
{
    delete [] m_preBlockVal;
}

Error_t CPpm::setAlphaAT(float val) {
    m_alpha_AT = val;
    return kNoError;
}

Error_t CPpm::setAlphaRT(float val) {
    m_alpha_RT = val;
    return kNoError;
}

Error_t CPpm::process(float **ppfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames)
{
    for (int i = 0; i < m_numOfChannels; i++) {
        float pre_val = m_preBlockVal[i];
        float max_val = -__FLT_MAX__;
        float ppm_out = 0;
        for (int j = 0; j < iNumberOfFrames; j++) {
            float abs_input = abs(ppfInputBuffer[i][j]);
            if (pre_val > abs_input) {
                ppm_out = (1-m_alpha_RT) * pre_val;
            } else {
                ppm_out = m_alpha_AT * abs_input + (1 - m_alpha_AT) * pre_val;
            }
            pre_val = ppm_out;
            max_val = fmax(max_val, ppm_out);
        }
        m_preBlockVal[i] = ppfInputBuffer[i][iNumberOfFrames-1];
        pfOutputBuffer[i] = max_val;
    }
    return kNoError;
}
