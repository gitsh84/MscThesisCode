#ifndef __GA_H__
#define __GA_H__

#include <mpi.h>
#include "safeJobVector.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class Ga
{
public:
	Ga();	
	void RunGa();
	void Test();
	
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

private:
	void InitGa();
	bool ReadTargetMeasurements();
	void CreateTargetMeasurements();
	void CreateJobs();
	void ProcessJobs();
	void ProcessResults();
	void ProcessResults(Candidate* pCandidate);
	int GetMate();
	void CreateChild(Candidate* pParent1, Candidate* pParent2, Candidate* pChild);
	bool FindSimilarCandidate(int nIndex);
	void CreateMutations();

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	
public:
	static int nNumberOfMachines;	
	
private:	
	int m_nCurIteration;
	double* m_pTargetMeasurement1;
	double* m_pTargetMeasurement2;
	unsigned long int* MinCost;
	double* Rank;
	vector<Candidate*> Population;
	CSafeJobVector m_jobVector;
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __GA_H__

