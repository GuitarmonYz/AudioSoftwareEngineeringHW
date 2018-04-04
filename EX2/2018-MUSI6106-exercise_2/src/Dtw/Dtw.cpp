
#include "Vector.h"
#include "Util.h"

#include "Dtw.h"

CDtw::CDtw( void )
{

}

CDtw::~CDtw( void )
{
    for (int i = 0; i < m_numRows; i++) {
        delete [] distanceMatrix[i];
        delete [] costMatrix[i];
        delete [] directMatrix[i];
    }
    delete [] distanceMatrix;
    delete [] costMatrix;
    delete [] directMatrix;
    for (int i = 0; i < m_numCols + m_numRows; i++) {
        delete [] resMatrix[i];
    }
    delete [] resMatrix;
}

Error_t CDtw::init( int iNumRows, int iNumCols )
{
    if (iNumRows <= 0 || iNumCols <= 0) return kFunctionInvalidArgsError;
    
    m_numCols = iNumCols;
    m_numRows = iNumRows;
    distanceMatrix = new float*[iNumRows];
    for (int i = 0; i < iNumRows; i++) {
        distanceMatrix[i] = new float[iNumCols];
        for (int j = 0; j < iNumCols; j++) {
            distanceMatrix[i][j] = 0;
        }
    }
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
    resMatrix = new int*[iNumRows + iNumCols];
    for (int i = 0; i < iNumRows + iNumCols; i++) {
        resMatrix[i] = new int[2];
        for (int j = 0; j < 2; j++) {
            resMatrix[i][j] = 0;
        }
    }
    m_initialized = true;
    return kNoError;
}

Error_t CDtw::reset()
{
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    if (!m_initialized) return kNotInitializedError;
    if (ppfDistanceMatrix == NULL) return kFunctionInvalidArgsError;
    costMatrix[0][0] = ppfDistanceMatrix[0][0];
    distanceMatrix[0][0] = ppfDistanceMatrix[0][0];
    for (int i = 1; i < m_numCols; i++) {
        distanceMatrix[0][i] = ppfDistanceMatrix[0][i];
        costMatrix[0][i] = costMatrix[0][i-1] + ppfDistanceMatrix[0][i];
        directMatrix[0][i] = kHoriz;
    }
    for (int i = 1; i < m_numRows; i++) {
        distanceMatrix[i][0] = ppfDistanceMatrix[i][0];
        costMatrix[i][0] = costMatrix[i-1][0] + ppfDistanceMatrix[i][0];
        directMatrix[i][0] = kVert;
    }
    for (int i = 1; i < m_numRows; i++) {
        for (int j = 1; j < m_numCols; j++) {
            distanceMatrix[i][j] = ppfDistanceMatrix[i][j];
//            float minVal = std::min(costMatrix[i-1][j], std::min(costMatrix[i][j-1], costMatrix[i-1][j-1]));
            
//            costMatrix[i][j] = minVal + ppfDistanceMatrix[i][j];
            
//            if (minVal == costMatrix[i-1][j]) {
//                directMatrix[i][j] = kVert;
//            } else if (minVal == costMatrix[i][j-1]) {
//                directMatrix[i][j] = kHoriz;
//            } else {
//                directMatrix[i][j] = kDiag;
//            }
            if (costMatrix[i-1][j] < costMatrix[i][j-1] && costMatrix[i-1][j] < costMatrix[i-1][j-1]){
                directMatrix[i][j] = kVert;
                costMatrix[i][j] = costMatrix[i-1][j] + ppfDistanceMatrix[i][j];
            } else if (costMatrix[i][j-1] < costMatrix[i-1][j] && costMatrix[i][j-1] < costMatrix[i-1][j-1]) {
                directMatrix[i][j] = kHoriz;
                costMatrix[i][j] = costMatrix[i][j-1] + ppfDistanceMatrix[i][j];
            } else {
                directMatrix[i][j] = kDiag;
                costMatrix[i][j] = costMatrix[i-1][j-1] + ppfDistanceMatrix[i][j];
            }
        }
    }
    for (int i = m_numRows + m_numCols - 1, curRow=m_numRows-1, curCol=m_numCols-1; !(curRow == 0 && curCol == 0); i--) {
        resMatrix[i][0] = curRow;
        resMatrix[i][1] = curCol;
        int direct = directMatrix[curRow][curCol];
        switch (direct) {
            case kHoriz:
                curCol--;
                break;
            case kVert:
                curRow--;
                break;
            default:
                curCol--;
                curRow--;
                break;
        }
        pathStartIdx++;
    }
    pathStartIdx = m_numRows + m_numCols - 1 - pathStartIdx;
    m_processed = true;
    return kNoError;
}

int CDtw::getPathLength()
{
    if (m_processed) return m_numRows + m_numCols - pathStartIdx;
    else return 0;
}

float CDtw::getPathCost() const
{
    float totalCost = 0;
    for (int i = pathStartIdx; i < m_numRows + m_numCols; i++) {
        if (m_numRows != 1 && m_numCols != 1) {
            totalCost += distanceMatrix[resMatrix[i][0]][resMatrix[i][1]];
        } else if (m_numRows == 1) {
            totalCost += distanceMatrix[0][resMatrix[i][1]];
        } else if (m_numCols == 1) {
            totalCost += distanceMatrix[resMatrix[i][0]][0];
        }
        
    }
    return totalCost;
}

Error_t CDtw::getPath( int **ppiPathResult ) const
{
    for (int i = 0; i < m_numRows + m_numCols - pathStartIdx; i++) {
        ppiPathResult[0][i] = resMatrix[pathStartIdx+i][0];
        ppiPathResult[1][i] = resMatrix[pathStartIdx+i][1];
    }
    return kNoError;
}

