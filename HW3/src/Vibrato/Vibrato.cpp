
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"

CVibrato::CVibrato() :
m_pCLfo(0),
m_ppCRingBuffer(0),
m_iNumChannels(0),
m_fSampleRate(0)
{
    
}

CVibrato::~CVibrato(){
    if (m_ppCRingBuffer) {
        for (int i = 0; i < m_iNumChannels; i++) {
            delete m_ppCRingBuffer[i];
        }
    }
    delete [] m_ppCRingBuffer;
}

Error_t CVibrato::create(CVibrato *&cpCVibrato) {
    cpCVibrato = new CVibrato();
    return kNoError;
}

Error_t CVibrato::destroy(CVibrato *&cpCVibrato) {
    delete cpCVibrato;
    return kNoError;
}

Error_t CVibrato::init(float fSampleRateInHz, float fMaxWidthInS, int iNumChannels, float fModWidth, float fModFreq) {
    m_fSampleRate = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_afParam[VibratoParam_t::kParamModWidth] = fModWidth;
    m_afParam[VibratoParam_t::kParamModFreq] = fModFreq;
    int iMaxWidthInSample = floor(fMaxWidthInS * fSampleRateInHz);
    m_ppCRingBuffer = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i < iNumChannels; i++) {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxWidthInSample*2 + 2);
    }
    return kNoError;
}

Error_t CVibrato::setParam(CVibrato::VibratoParam_t eParam, float fParamValue) {
    switch (eParam) {
        case VibratoParam_t::kParamModWidth :
            m_afParam[eParam] = fParamValue;
            for (int i = 0; i < m_iNumChannels; i++) {
                m_ppCRingBuffer[i]->setReadIdx(0);
                m_ppCRingBuffer[i]->setReadIdx(m_afParam[VibratoParam_t::kParamModWidth]);
            }
            break;
        case VibratoParam_t::kParamModFreq :
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
            ppfOutputBuffer[i][j] = m_ppCRingBuffer[i]->get(m_afParam[VibratoParam_t::kParamModWidth] * m_pCLfo->getLFOVal());
            m_ppCRingBuffer[i]->getPostInc();
        }
    }
    return kNoError;
}

