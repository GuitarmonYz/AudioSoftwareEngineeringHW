
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"

CVibrato::CVibrato(float fSampleRateInHz, float iMaxDelayInS,int iNumChannels) :
    m_pCLfo(0),
    m_ppCRingBuffer(0),
    m_iNumChannels(iNumChannels),
    m_fSampleRate(fSampleRateInHz)
{
    int iMaxDelayInFrames = iMaxDelayInS * fSampleRateInHz;
    m_ppCRingBuffer = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i < iNumChannels; i++) {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxDelayInFrames);
    }
}

CVibrato::~CVibrato() {
    if (m_ppCRingBuffer) {
        for (int i = 0; i < m_iNumChannels; i++) {
            delete m_ppCRingBuffer[i];
        }
    }
    delete [] m_ppCRingBuffer;
}

Error_t CVibrato::setParam(CVibrato::FilterParam_t eParam, float fParamValue) {
    switch (eParam) {
        case FilterParam_t::kParamModWidth :
            m_afParam[eParam] = fParamValue;
            break;
        case FilterParam_t::kParamDelay :
            m_afParam[eParam] = fParamValue;
            
            break;
        case FilterParam_t::kParamModFreq :
            m_afParam[eParam] = fParamValue;
            break;
        default:
            break;
    }
    return kNoError;
}

Error_t CVibrato::initLFO() {
    int lfoBufferLengthInSample = m_fSampleRate / m_afParam[kParamModFreq];
    m_pCLfo = new CLfo(lfoBufferLengthInSample);
    return kNoError;
}


Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {
    for (int i = 0; i < m_iNumChannels; i++) {
        for (int j = 0; j < iNumberOfFrames; j++) {
            m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
            ppfOutputBuffer[i][j] = m_ppCRingBuffer[i]->get(m_afParam[FilterParam_t::kParamModWidth] * m_pCLfo->getLFOVal());
        }
    }
    return kNoError;
}
