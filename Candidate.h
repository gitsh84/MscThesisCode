
#ifndef __CANDIDATE__
#define __CANDIDATE__

#include "FkModel.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class Candidate
{
public:
	Candidate(int nIndex);
	virtual ~Candidate();	
	void CreateFibroblasts();
	void Mutate();
	void UnMutate();

private:
	void ClearMat();

public:	
	double** m_mat;
	double** m_pResult1;
	double** m_pResult2;		
	int m_nIndex;
	double m_cost;
	int m_nCenterH;
	int m_nCenterW;
	int m_nHeight;
	int m_nWidth;

	char m_str[256];
	
	// Mutation.
	int m_nParam1;
	int m_nParam2;
	int m_nVal1;
	int m_nVal2;
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool CandidateCompare(Candidate* pFirstCandidate, Candidate* pSecondCandidate);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __CANDIDATE__

