
#ifndef __SB__MODEL__
#define __SB__MODEL__

#include "Protocols.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class CSBModel
{
public:
	CSBModel();
	virtual ~CSBModel();
	void ExecuteModel(double** inFibroblastMat, double** outRiseTimeMat, const ProtocolParams& protParams, char* outputFolder = NULL);
	void SetDiffusion(double dDiffusion) {m_dDiffusion = dDiffusion;}
	void SetJ(double j) {m_dj = j;}
	
private:
	void InitModel();
	void DeleteModel();
	void CleanupModel();
	void CalculateDer(int iH, int iW, double** inFibroblastMat);	

private:
	double m_dDiffusion;
	double m_dj;
	double dVdh;
	double dVdw;
	double dW2;
	double dH2;
	double** mat;
	double** new_mat;
	double** s_mat;
	double** new_s_mat;
	double** f_mat;
	double** new_f_mat;
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __SB__MODEL__

