#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"

class CLfo
{
public:
    CLfo (float modFreq):
    m_fModFreq(modFreq),
    m_fCurPos(0)
    {
        m_iFixedBufferLength = m_fFixedSampleRateInHz / m_fFixedModFreq;
        m_fIncrement = m_iFixedBufferLength / (1 / modFreq);
        m_pCRingBuffer = new CRingBuffer<float>(m_iFixedBufferLength);
        float waveTableBuffer[m_iFixedBufferLength];
        CSynthesis::generateSine(waveTableBuffer, m_fFixedModFreq, m_fFixedSampleRateInHz, m_iFixedBufferLength);
        for (int i = 0; i < m_iFixedBufferLength; i++) {
            m_pCRingBuffer -> putPostInc(waveTableBuffer[i]);
        }
    }
    
    ~CLfo () {
        if (m_pCRingBuffer) {
            delete [] m_pCRingBuffer;
        }
    }
    
    Error_t setLfoRate(float modFreq) {
        m_fModFreq = modFreq;
        m_fIncrement = m_iFixedBufferLength / (1 / modFreq);
        return kNoError;
    }
    
    Error_t reset() {
        
        return kNoError;
    }
    
    float getLFOVal() {
        float res = m_pCRingBuffer->get(m_fCurPos);
        m_fCurPos += m_fIncrement;
        if (m_fCurPos >= m_iFixedBufferLength) {
            m_fCurPos -= m_iFixedBufferLength;
        }
        return res;
    }
private:
    CRingBuffer<float>  *m_pCRingBuffer;
    const float         m_fFixedModFreq = 10;
    const float         m_fFixedSampleRateInHz = 44100;
    int                 m_iFixedBufferLength;
    float               m_fModFreq;
    float               m_fIncrement;
    float               m_fCurPos;
};

#endif // __Lfo_hdr__

