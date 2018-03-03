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
    enum VibratoParam_t
    {
        kParamModWidth,
        kParamModFreq,
        kNumFilterParams
    };
    static Error_t create(CVibrato*& cpCVibrato);
    static Error_t destroy(CVibrato*& cpCVibrato);
    Error_t init(int iSampleRateInHz, float fMaxWdithInS, int iNumChannels, float fModWidth=0.0025, float fModFreq=5);
    Error_t reset ();
    Error_t setParam (VibratoParam_t eParam, float fParamValue);
    float   getParam (VibratoParam_t eParam) const;
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

