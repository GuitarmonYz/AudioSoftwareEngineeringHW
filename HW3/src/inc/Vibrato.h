#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__

#include "ErrorDef.h"

// forward declaration
class CLfo;
template <class T>
class CRingBuffer;

/*
 * brief explanation about your class-interface design
 */

class CVibrato
{
public:
    enum FilterParam_t
    {
        kParamModWidth,
        kParamModFreq,
        kNumFilterParams
    };
    static Error_t create(CVibrato*& cpCVibrato);
    static Error_t destroy(CVibrato*& cpCVibrato);
    Error_t initLFO ();
    Error_t init(float fSampleRateInHz, float fMaxWithInS, int iNumChannels, float fModWidth=0.01, float fModFreq=20);
    Error_t reset ();
    Error_t setParam (FilterParam_t eParam, float fParamValue);
    float   getParam (FilterParam_t eParam) const;
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
private:
    CLfo                *m_pCLfo;
    CRingBuffer<float>  **m_ppCRingBuffer;
    float m_afParam[CVibrato::kNumFilterParams];
    int m_iNumChannels;
    float m_fSampleRate;
    bool m_isInitialized;
    CVibrato();
    ~CVibrato();
};

#endif // #if !defined(__Vibrato_hdr__)

