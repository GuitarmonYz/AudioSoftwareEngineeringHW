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
        kParamDelay,                    //!< delay in seconds for specification of comb width
        kParamModWidth,
        kParamModFreq,
        kNumFilterParams
    };
    CVibrato(float fSampleRateInHz, float iMaxDelayInS,int iNumChannels);
    ~CVibrato();
    Error_t initLFO ();
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
};

#endif // #if !defined(__Vibrato_hdr__)
