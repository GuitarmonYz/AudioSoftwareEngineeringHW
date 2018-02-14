
// standard headers
#include <limits>

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"

#include "CombFilterIf.h"
#include "CombFilter.h"
#include <iostream>
CCombFilterBase::CCombFilterBase( int iMaxDelayInFrames, int iNumChannels ) :
    m_ppCRingBuffer(0),
    m_iNumChannels(iNumChannels)
{

    assert(iMaxDelayInFrames > 0);
    assert(iNumChannels > 0);
    m_afParam[CCombFilterIf::kParamDelay] = 0;
    m_afParam[CCombFilterIf::kParamGain] = 0;
//    init parameter range
    m_aafParamRange[CCombFilterIf::kParamGain][0] = std::numeric_limits<float>::min();
    m_aafParamRange[CCombFilterIf::kParamGain][1] = std::numeric_limits<float>::max();
    m_aafParamRange[CCombFilterIf::kParamDelay][0] = 0;
    m_aafParamRange[CCombFilterIf::kParamDelay][1] = (float)iMaxDelayInFrames;
//    init ringbuffer for iNumChannels
    m_ppCRingBuffer = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i < iNumChannels; i++) {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxDelayInFrames);
    }

}

CCombFilterBase::~CCombFilterBase()
{
    if (m_ppCRingBuffer) {
        for (int i = 0; i < m_iNumChannels; i++) {
            delete m_ppCRingBuffer[i];
        }
    }
    delete [] m_ppCRingBuffer;
}

Error_t CCombFilterBase::resetInstance()
{
    for (int i = 0; i < m_iNumChannels; i++) {
        m_ppCRingBuffer[i] -> reset();
        m_ppCRingBuffer[i] -> setWriteIdx((int)m_afParam[CCombFilterIf::kParamDelay]);
    }
    return kNoError;
}

Error_t CCombFilterBase::setParam( CCombFilterIf::FilterParam_t eParam, float fParamValue )
{
    if (!isInParamRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;
    std::cout<<fParamValue<<std::endl;
    std::cout<<m_afParam[CCombFilterIf::kParamDelay]<<std::endl;
    if (eParam == CCombFilterIf::kParamDelay) {
        
        int iNumAdditionalTaps  = CUtil::float2int<int>(fParamValue - m_afParam[CCombFilterIf::kParamDelay]);
        if (iNumAdditionalTaps < 0)
        {
            for (int c = 0; c < m_iNumChannels; c++)
            {
                m_ppCRingBuffer[c]->setWriteIdx(CUtil::float2int<int>(fParamValue) + m_ppCRingBuffer[c]->getReadIdx());
            }
        }
        else
        {
            
            for (int c = 0; c < m_iNumChannels; c++)
            {
                for (int i = 0; i < iNumAdditionalTaps; i++)
                {
                    m_ppCRingBuffer[c]->putPostInc(0.F);
                }
            }
        }
    }
    
    m_afParam[eParam] = fParamValue;
    
    return kNoError;
}

float CCombFilterBase::getParam( CCombFilterIf::FilterParam_t eParam ) const
{
    return m_afParam[eParam];
}

bool CCombFilterBase::isInParamRange( CCombFilterIf::FilterParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CCombFilterFir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for (int i = 0; i < m_iNumChannels; i++) {
        for (int j = 0; j < iNumberOfFrames; j++) {
            m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + m_afParam[CCombFilterIf::kParamGain] * m_ppCRingBuffer[i]->getPostInc();
        }
    }
    return kNoError;
}

CCombFilterIir::CCombFilterIir (int iMaxDelayInFrames, int iNumChannels) : CCombFilterBase(iMaxDelayInFrames, iNumChannels)
{
    m_aafParamRange[CCombFilterIf::kParamGain][0] = (float)-1;
    m_aafParamRange[CCombFilterIf::kParamGain][1] = (float) 1;
}

Error_t CCombFilterIir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for (int i = 0; i < m_iNumChannels; i++) {
        for (int j = 0; j < iNumberOfFrames; j++) {
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + m_afParam[CCombFilterIf::kParamGain] * m_ppCRingBuffer[i]->getPostInc();
            m_ppCRingBuffer[i]->putPostInc(ppfOutputBuffer[i][j]);
        }
    }
    return kNoError;
}
