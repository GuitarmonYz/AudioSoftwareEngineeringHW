#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
*/
template <class T> 
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples),
        m_iReadIdx(0),                          
        m_iWriteIdx(0),
        m_iCount(0),
        m_ptBuff(0)
    {
        assert(iBufferLengthInSamples > 0);

        m_ptBuff        = new T [m_iBuffLength];
        reset();
    }

    virtual ~CRingBuffer ()
    {
        delete [] m_ptBuff;
        m_ptBuff    = 0;
    }

    /*! add a new value of type T to write index
    \param tNewValue the new value
    \return void
    */
    void put(T tNewValue)
    {
        // to be implemented
        int cur_idx = getWriteIdx();
        m_ptBuff[cur_idx] = tNewValue;
    }
    
    /*! return the value at the index with an optional arbitrary offset
    \param iOffset: read at offset from read index
    \return type T the value from the read index
    */
    T get (int iOffset = 0) const
    {
        // to be implemented
        int cur_read_idx = getReadIdx();
        if (cur_read_idx < 0) {
            cur_read_idx = m_iBuffLength - abs(cur_read_idx % m_iBuffLength);
        } else if (cur_read_idx >= m_iBuffLength) {
            cur_read_idx %= m_iBuffLength;
        }
        return m_ptBuff[cur_read_idx];
    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx () const
    {
        // to be implemented
        return m_iWriteIdx;
    }

    /*! move the write index to a new position
    \param iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx (int iNewWriteIdx)
    {
        // to be implemented
        m_iWriteIdx = iNewWriteIdx;
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx () const
    {
        // to be implemented
        return m_iReadIdx;
    }

    /*! move the read index to a new position
    \param iNewReadIdx: new position
    \return void
    */
    void setReadIdx (int iNewReadIdx)
    {
        // to be implemented
        m_iReadIdx = iNewReadIdx;
    }

    /*! add a new value of type T to write index and increment write index
    \param tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        // to be implemented
        put(tNewValue);
        int cur_write_idx = getWriteIdx();
        cur_write_idx++;
        
        if (m_iCount < m_iBuffLength) {
            m_iCount++;
        } else {
            setReadIdx(getReadIdx() + 1);
        }
        
        if (cur_write_idx < m_iBuffLength) {
            setWriteIdx(cur_write_idx);
        } else if (cur_write_idx == m_iBuffLength) {
            setWriteIdx(0);
        }
    }

    /*! return the value at the current read index and increment the read pointer
    \return type T the value from the read index
    */
    T getPostInc ()
    {
        // to be implemented
        T cur_read_val = get();
        int cur_read_idx = getReadIdx();
        cur_read_idx++;
        m_iCount--;
        if (m_iCount < 0) {
            std::cout<< "warning: reading samples haven't writen" << std::endl;
        }
        if (cur_read_idx < m_iBuffLength) {
            setReadIdx(cur_read_idx);
        } else if (cur_read_idx == m_iBuffLength) {
            setReadIdx(0);
        }
        return cur_read_val;
    }
    
    /*! set buffer content and indices to 0
    \return void
    */
    void reset ()
    {
        memset (m_ptBuff, 0, sizeof(T)*m_iBuffLength);
        m_iReadIdx  = 0;
        m_iWriteIdx = 0;
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer () const
    {
        return (m_iWriteIdx - m_iReadIdx + m_iBuffLength)%m_iBuffLength;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength () const
    {
        return m_iBuffLength;
    }
private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);

    int m_iBuffLength,              //!< length of the internal buffer
        m_iReadIdx,                 //!< current read index
        m_iWriteIdx,                //!< current write index
        m_iCount;                   //!< length of unread data

    T   *m_ptBuff;                  //!< data buffer
};
#endif // __RingBuffer_hdr__
