#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__

#include "ErrorDef.h"

// forward declaration
class CLfo;
template <class T>
class CRingBuffer;

/**
*  @file    Vibrato.h
*  @author  Joe (nnocae10)
*  @date    3/3/2018
*  @version 1.0
*
*  @brief MUSI 6106, Group 7, Interface for the Vibrato class
*
*  @section DESCRIPTION
*
*  This interface provides a basic vibrato functionality.
*  It takes modulation width, modulation frequency, max modulation 
*  width, number of channels and sampling rate as input parameters 
*  and produces the output with vibrato effect applied.
*
*  The interface provides functions for creating a vibrato handler, 
*  initialization of vibrato class with given parameters, parameter
*  get and update, and processing of the audio buffer with the given
*  parameters.
*
* The design of the interface is in line with the that of the combfilter
* class. It contains the initialization function, getter and setter functions,
* and a process function returning the final results.
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
	/**
	* Create vibrato handler
	*/
    static Error_t create(CVibrato*& cpCVibrato);
	/**
	* Destroy vibrato handler
	*/
    static Error_t destroy(CVibrato*& cpCVibrato);
	/**
	* Initialize the handler with the given parameters, also initialize LFO internally with the given mod frequency
	* @param  iSampleRateInHz given sampling rate
	* @param fMaxWidthInS given maximum width of the vibrato handler
	* @param iNumChannels numbers of channels in the audio file
	* @param fModWidth modulation width or delay of the audio file
	* @param fModFreq given modulation frequency of the sine wave
	*/
    Error_t init(int iSampleRateInHz, float fMaxWdithInS, int iNumChannels, float fModWidth=0.0025, float fModFreq=5);
	/**
	* Reset all the parameters to default values
	*/
    Error_t reset ();
	/**
	* Set the modulation width or the modulation fequency of the vibrato with the given value
	* @param eParam Parameter to be set
	* @param fParamVale Parameter value
	* 
	*/
    Error_t setParam (VibratoParam_t eParam, float fParamValue);
	/**
	* Get the values of the given parameter
	* @param eParam vibrato parameter to get
	* @return the value of the parameter
	*/
    float   getParam (VibratoParam_t eParam) const;

	/**
	* Process the values in the input buffer using LFO and the modulation width, and store the values in the outputBuffer
	* @param ppfInputBuffer given audio samples in the buffer
	* @param ppfOutputBuffer final values of the buffer processed 
	* @param iNumberOfFrames frames/samples  in the input buffer 
	*/
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    Error_t process (const float *pfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames, int channelNum);

private:
    CLfo                *m_pCLfo;
    CRingBuffer<float>  **m_ppCRingBuffer;
    float m_afParam[CVibrato::kNumFilterParams];
    int m_iNumChannels;
    float m_fSampleRate;
    float m_fMaxWidthInS;
    bool m_isInitialized;
    CVibrato();
    ~CVibrato();
};

#endif // #if !defined(__Vibrato_hdr__)

