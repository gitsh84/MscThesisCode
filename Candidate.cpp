
#include "defs.h"
#include "Candidate.h"
#include "Mat.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Candidate::Candidate(int nIndex, int nHStart, int nWStart, int nHEnd, int nWEnd)
{
	// Create the mats.
	m_pFibroblastMat = CreateMat();	
	m_pResult1 = CreateMat();
	m_pResult2 = CreateMat();
		
	// Init the vars.
	m_nIndex = nIndex;	
	m_nHStart = nHStart;
	m_nWStart = nWStart;
	m_nHEnd = nHEnd;
	m_nWEnd = nWEnd;
	m_cost = 0;
	
	memset(m_cCandidateFullName, 0, sizeof(m_cCandidateFullName));
	
	// Now create the fibroblats.
	CreateFibroblastBorders();
	CreateFibroblastPatch();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Candidate::~Candidate()
{
	DestroyMat(m_pFibroblastMat);
	DestroyMat(m_pResult1);
	DestroyMat(m_pResult2);
}
	
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Candidate::CreateFibroblastBorders()
{
	for (int iH = 0; iH < Nh_with_border; ++iH)
	{
		for (int iW = 0; iW < Nw_with_border; ++iW)
		{								
			if((iH == 0) || (iH == Nh+1) || (iW == 0) || (iW == Nw+1))
			{
				m_pFibroblastMat[iH][iW] = 1.0;
			}
			else
			{
				m_pFibroblastMat[iH][iW] = 0.0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Candidate::CreateFibroblastPatch()
{
	for (int iH = m_nHStart; iH <= m_nHEnd; ++iH)
	{	
		for (int iW = m_nWStart; iW <= m_nWEnd; ++iW)
		{
			m_pFibroblastMat[iH][iW] = 1.0;
		}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

char* Candidate::GetFullName()
{ 
	sprintf(m_cCandidateFullName,"(%d,%d) -> (%d,%d)", m_nHStart, m_nWStart, m_nHEnd, m_nWEnd);
	return m_cCandidateFullName; 
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool CandidateCompare(Candidate* pFirstCandidate, Candidate* pSecondCandidate)
{
	return (pFirstCandidate->m_cost < pSecondCandidate->m_cost);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

