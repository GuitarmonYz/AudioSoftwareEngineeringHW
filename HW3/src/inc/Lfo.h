#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"

class CLfo
{
public:
    CLfo (int bufferLength) {
        m_pCRingBuffer = new CRingBuffer<float>(bufferLength);
        float waveTableBuffer[bufferLength];
        CSynthesis::generateSine(waveTableBuffer, modFreq, sampleRateInHz, bufferLength);
            for (int i = 0; i < bufferLength; i++) {
                m_pCRingBuffer-> putPostInc(waveTableBuffer[i]);
            }
    }
    
    ~CLfo () {
        if (m_pCRingBuffer) {
            delete [] m_pCRingBuffer;
        }
    }
    
    Error_t reset() {
        
        return kNoError;
    }
    
    float getLFOVal() {
        return m_pCRingBuffer->getPostInc();
    }
private:
    CRingBuffer<float>  *m_pCRingBuffer;
    float               modFreq;
    float               sampleRateInHz;
};

#endif // __Lfo_hdr__
