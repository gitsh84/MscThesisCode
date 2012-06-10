
#ifndef __CANDIDATE__
#define __CANDIDATE__

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class Candidate
{
public:
	Candidate(int nIndex);
	Candidate(int nCenterH, int nCenterW, int nHeight, int nWidth);
	virtual ~Candidate();	
	void CreateFibroblasts();
	void Mutate();
	void UnMutate();
	char* GetFullName() 
	{ 
		sprintf(m_cCandidateFullName,"(%d,%d) %dX%d", m_nCenterH, m_nCenterW, m_nHeight, m_nWidth);
		return m_cCandidateFullName; 
	}
	
private:
	void ClearMat();

public:	
	double** m_pFibroblastMat;
	double** m_pResult1;
	double** m_pResult2;		
	int m_nIndex;
	unsigned long int m_cost;
	int m_nCenterH;
	int m_nCenterW;
	int m_nHeight;
	int m_nWidth;
	
	// Mutation.
	int m_nParam1;
	int m_nParam2;
	int m_nVal1;
	int m_nVal2;
	
private:
	char m_cCandidateFullName[256];
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool CandidateCompare(Candidate* pFirstCandidate, Candidate* pSecondCandidate);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __CANDIDATE__

