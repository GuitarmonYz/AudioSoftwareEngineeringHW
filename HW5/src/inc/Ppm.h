#if !defined(__Ppm_hdr__)
#define __Ppm_hdr__

#include "ErrorDef.h"

class CPpm
{
public:
    static Error_t createInstance (CPpm*& pCPpm, int sampleRate, int numOfChannels);
    static Error_t destroyInstance (CPpm*& pCPpm);

    Error_t process (float **ppfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames);
    Error_t setAlphaAT(float val);
    Error_t setAlphaRT(float val);

protected:
    CPpm (int sampleRate, int numOfChannels);
    virtual ~CPpm ();

private:
    float m_alpha_AT;
    float m_alpha_RT;
    float* m_preBlockVal;
    int m_fs;
    int m_numOfChannels;
};

#endif // #if !defined(__Ppm_hdr__)
