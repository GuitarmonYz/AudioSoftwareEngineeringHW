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
    
}

Error_t CPpm::init() {
    m_alpha_AT = 1 - exp(-2.2 / (m_fs * 0.01));
    m_alpha_RT = 1 - exp(-2.2 / (m_fs * 1.5));
    m_preVal = new float[m_numOfChannels];
    for (int i = 0; i < m_numOfChannels; i++) {
        m_preVal[i] = 0;
    }
    m_isInitialized = true;
    return kNoError;
}



CPpm::~CPpm()
{
    m_isInitialized = false;
    if (*m_preVal != 0)
        delete [] m_preVal;
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
    if (!m_isInitialized) return kNotInitializedError;
    if (ppfInputBuffer == nullptr || pfOutputBuffer == nullptr || iNumberOfFrames < 0) return kFunctionInvalidArgsError;

    for (int i = 0; i < m_numOfChannels; i++) {
        float max_val = -MAXFLOAT;
        float ppm_out = 0;
        for (int j = 0; j < iNumberOfFrames; j++) {
            float abs_input = fabsf(ppfInputBuffer[i][j]);
            if (m_preVal[i] > abs_input) {
                ppm_out = (1-m_alpha_RT) * m_preVal[i];
            } else {
                ppm_out = m_alpha_AT * abs_input + (1 - m_alpha_AT) * m_preVal[i];
            }
            m_preVal[i] = ppm_out;
            max_val = (max_val < ppm_out) ? ppm_out : max_val;
        }
        pfOutputBuffer[i] = max_val;
    }
    return kNoError;
}
