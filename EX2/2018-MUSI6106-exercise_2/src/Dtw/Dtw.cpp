
#include "Vector.h"
#include "Util.h"

#include "Dtw.h"

CDtw::CDtw( void )
{

}

CDtw::~CDtw( void )
{

}

Error_t CDtw::init( int iNumRows, int iNumCols )
{
    m_numCols = iNumCols;
    m_numRows = iNumRows;
    costMatrix = new float*[iNumRows];
    for (int i = 0; i < iNumRows; i++) {
        costMatrix[i] = new float[iNumCols];
        for (int j = 0; j < iNumCols; j++) {
            costMatrix[i][j] = 0;
        }
    }
    directMatrix = new int*[iNumRows];
    for (int i = 0; i < iNumRows; i++) {
        directMatrix[i] = new int[iNumCols];
        for (int j = 0; j < iNumCols; j++) {
            directMatrix[i][j] = 0;
        }
    }
    resMatrix = new int*[iNumRows + iNumCols - 2];
    for (int i = 0; i < iNumRows; i++) {
        resMatrix[i] = new int[2];
        for (int j = 0; j < 2; j++) {
            resMatrix[i][j] = 0;
        }
    }
    
    return kNoError;
}

Error_t CDtw::reset()
{
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    costMatrix[0][0] = ppfDistanceMatrix[0][0];
    for (int i = 1; i < m_numCols; i++) {
        costMatrix[0][i] = costMatrix[0][i-1] + ppfDistanceMatrix[0][i];
    }
    for (int i = 1; i < m_numRows; i++) {
        costMatrix[i][0] = costMatrix[i-1][0] + ppfDistanceMatrix[i][0];
    }
    for (int i = 1; i < m_numRows; i++) {
        for (int j = 1; j < m_numCols; j++) {
            float minVal = std::min(costMatrix[i-1][j], std::min(costMatrix[i][j-1], costMatrix[i-1][j-1]));
            
            costMatrix[i][j] = minVal + ppfDistanceMatrix[i][j];
            
            if (minVal == costMatrix[i-1][j]) {
                directMatrix[i][j] = kVert;
            } else if (minVal == costMatrix[i][j-1]) {
                directMatrix[i][j] = kHoriz;
            } else {
                directMatrix[i][j] = kDiag;
            }
        }
    }
    
    for (int i = m_numRows + m_numCols - 2, curRow=m_numRows, curCol=m_numCols; i >= 0; i--) {
        resMatrix[i][0] = curRow;
        resMatrix[i][1] = curCol;
        int direct = directMatrix[curRow][curCol];
        if (direct == kHoriz) {
            curCol--;
        } else if (direct == kVert) {
            curRow--;
        } else {
            curCol--;
            curRow--;
        }
        if (curRow==0 && curCol==0) {
            pathStartIdx = i;
        }
    }
    return kNoError;
}

int CDtw::getPathLength()
{    
    return m_numRows + m_numCols - 2 - pathStartIdx;
}

float CDtw::getPathCost() const
{
    float totalCost = 0;
    for (int i = pathStartIdx; i < m_numRows + m_numCols - 2; i++) {
        totalCost += costMatrix[resMatrix[i][0]][resMatrix[i][1]];
    }
    return totalCost;
}

Error_t CDtw::getPath( int **ppiPathResult ) const
{
    for (int i = 0; i < m_numRows + m_numCols - 2 - pathStartIdx; i++) {
        ppiPathResult[i][0] = resMatrix[pathStartIdx+i][0];
        ppiPathResult[i][1] = resMatrix[pathStartIdx+i][1];
    }
    return kNoError;
}

