
#ifndef __CANDIDATE__
#define __CANDIDATE__

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define CANDIDATE_MAX_NAME_LENGTH 256

class Candidate
{
public:
	Candidate(int nIndex, int nHStart, int nWStart, int nHEnd, int nWEnd);
	virtual ~Candidate();	
	char* GetFullName();

private:
	void CreateFibroblastBorders();
	void CreateFibroblastPatch();
	
public:	
	double** m_pFibroblastMat;
	double** m_pResult1;
	double** m_pResult2;		
	int m_nIndex;
	unsigned long int m_cost;
	int m_nHStart;
	int m_nWStart;
	int m_nHEnd;
	int m_nWEnd;
	
private:
	char m_cCandidateFullName[CANDIDATE_MAX_NAME_LENGTH];
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool CandidateCompare(Candidate* pFirstCandidate, Candidate* pSecondCandidate);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __CANDIDATE__

