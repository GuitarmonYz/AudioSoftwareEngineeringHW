#pragma once
#include "ErrorDef.h"
#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"

CVibrato::CVibrato() :
m_pCLfo(0),
m_ppCRingBuffer(0),
m_iNumChannels(0),
m_fSampleRate(0),
m_isInitialized(false)
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

Error_t CVibrato::init(int iSampleRateInHz, float fMaxWidthInS, int iNumChannels, float fModWidth, float fModFreq) {
    m_fSampleRate = iSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_fMaxWidthInS = fMaxWidthInS;
    m_afParam[VibratoParam_t::kParamModWidth] = fModWidth;
    m_afParam[VibratoParam_t::kParamModFreq] = fModFreq;
    m_pCLfo = new CLfo(fModFreq, m_fSampleRate);
    int iMaxWidthInSample = floor(fMaxWidthInS * iSampleRateInHz);
    m_ppCRingBuffer = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i < iNumChannels; i++) {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxWidthInSample*2 + 2);
        m_ppCRingBuffer[i]->setReadIdx(0);
        m_ppCRingBuffer[i]->setWriteIdx(m_afParam[VibratoParam_t::kParamModWidth]*m_fSampleRate);
    }
    m_isInitialized = true;
    return kNoError;
}

Error_t CVibrato::setParam(CVibrato::VibratoParam_t eParam, float fParamValue) {
    switch (eParam) {
        case VibratoParam_t::kParamModWidth :
            if (fParamValue > m_fMaxWidthInS) {
                return kFunctionInvalidArgsError;
            }
            for (int i = 0; i < m_iNumChannels; i++) {
                int cur_write_idx = m_ppCRingBuffer[i]->getWriteIdx();
                if (fParamValue > m_afParam[eParam]) {
                    for (int j = 0; j < (fParamValue*m_fSampleRate - cur_write_idx); j++) {
                        m_ppCRingBuffer[i]->putPostInc(0);
                    }
                } else if (fParamValue < m_afParam[eParam]) {
                    float diff = m_afParam[eParam]*m_fSampleRate - fParamValue*m_fSampleRate;
                    m_ppCRingBuffer[i]->setWriteIdx(cur_write_idx - diff);
                }
            }
            m_pCLfo->reset();
            m_afParam[eParam] = fParamValue;
            break;
        case VibratoParam_t::kParamModFreq :
            m_afParam[eParam] = fParamValue;
            m_pCLfo->setLfoRate(fParamValue);
            break;
        default:
            break;
    }
    return kNoError;
}

float CVibrato::getParam(CVibrato::VibratoParam_t eParam) const {
    return m_afParam[eParam];
}

Error_t CVibrato::reset() {
    for (int i = 0; i < m_iNumChannels; i++) {
        m_ppCRingBuffer[i]->reset();
    }
    m_pCLfo->reset();
    m_afParam[kParamModWidth] = 0;
    m_afParam[kParamModFreq] = 0;
    return kNoError;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {
    if (!m_isInitialized) {
        return kNotInitializedError;
    }
    for (int i = 0; i < m_iNumChannels; i++) {
        for (int j = 0; j < iNumberOfFrames; j++) {
            m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
            ppfOutputBuffer[i][j] = m_ppCRingBuffer[i]->get(m_afParam[VibratoParam_t::kParamModWidth] * m_fSampleRate * m_pCLfo->getLFOVal());
            m_ppCRingBuffer[i]->getPostInc();
        }
    }
    return kNoError;
}

