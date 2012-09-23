
// Desktop Version
// Symmetric propagation with central stimulation

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Nhumatr.h" 

const double PI = 3.1415926;

// Saving parameters
#define out_file_name "voltage.%d"
#define out_file_name_current "current.%d"
const int Save_W = 100; //210; // 42; // width of the saved frame
const int Save_H = 100; //560; // 112; // hight of the saved frame
const int Save_dW = 1; // 5; // width resolution of saved elements (Save_W*Save_dW = W MUST !!) 
const int Save_dH = 1; // 5; // height resolution of saved elements (Save_H*Save_dH = H MUST !!)

// Time parameters
const double save_period = 1.0; // milliseconds
const double send_period = 1.0; // milliseconds - For collecting the data from Workers to MASTER for saving - Make equal to save_period !!!
const double time_step = (double) dt; // milliseconds - Defind in const.h that is included in Nhumatr.h
const double max_time = 500.0; // milliseconds - Total time of simulation
double time_to_save = 0.0; // Initialization of saving
double time_to_send = 0.0; // Initialization of sending
double sim_time = 0.0; // Used locally/independently in MASTER and Workers

// Space parameters
//const double Width = 10.0; //10.5; // mm
//const double Height = 10.0; //28.0; // mm
const double dW = 0.1; // mm/node
const double dH = 0.1; // mm/node
const int W = 142; //210; // = Width/dW; // Width, x
const int H = 142; //560; // = Height/dH; // Height, y

// Reaction - Diffusion details
// const double Cm = (double) 100.0e-12;
const double Cm_factor = (double) 1.0/(Cm); 

// Difusion  parameters
struct D_tensor_type
{
	D_tensor_type()
	{
		Dii = 0.0;
		Djj = 0.0;
		Dij = 0.0;
	}
	double Dii, Djj, Dij;
};


// Fibrosis parameters

/*
const double Rm_fibro_low = (double) 5.0e+09; // Ohm Kamkin A. Experimental Physiology 1999
const double Rm_fibro_high = (double) 25.0e+09; // Ohm Kamkin A. Experimental Physiology 1999
const double Cm_fibro_factor_low = (double) 1.0/((double) Rm_fibro_low*Cm);
const double Cm_fibro_factor_high = (double) 1.0/((double) Rm_fibro_high*Cm);
const double Vrm_fibro = (double) -15.9; // mV Kamkin A. Experimental Physiology 1999
*/

// Stimulation parameters
//const double S1_amp = -142.4;//3.0752 -100.0;//-100*Cm; // pA
const double S1_amp = -100.0;//3.0752 -100.0;//-100*Cm; // pA
const double S1_total_time = 5.0; // 50.0 milliseconds
const double S1_begin = 10.0; // milliseconds

/*
const double S2_amp = 0.0;//-100.0;//-3000.0;//-3000.0; //-80.0*Cm; // pA
const double S2_total_time = 4.0; // milliseconds
const double S2_begin = 200.0; // milliseconds
*/
// 3D model

//double *Vm;
double *I_temp;

//out_frame = new (unsigned char) [Save_W*Save_H];



void Show_Vm_Char(int matrix_f[], state_variables* Node)
{
	//int i,j,plane_i,plane_j;
	//double VC;

	for (int i = 0; i < W; i += 4)
	{	
		for (int j = 0; j < H; j += 4)
		{
			double VC = Node[i*H + j].V;
			if (matrix_f[i*H + j]==2)	printf("  ");
			//else if (matrix_f[i*H+j]==1)	cout << ".";
			else if (VC < -80.0) printf("88");
			else if (VC < -70.0) printf("77");
			else if (VC < -60.0) printf("66");
			else if (VC < -50.0) printf("55");
			else if (VC < -40.0) printf("44");
			else if (VC < -30.0) printf("33");
			else if (VC < -20.0) printf("22");
			else if (VC < -10.0) printf("11");
			else if (VC < 0.0) printf("00");
			else if (VC < 5.0) printf("AA");
			else if (VC < 10.0) printf("BB");
			else if (VC < 15.0) printf("CC");
			else if (VC < 20.0) printf("DD");
			else if (VC < 25.0) printf("++");
			else if (VC < 25.0) printf("**");
			else printf("^^");
			
		}
		printf("\n");
	}
	printf("\n"); // Separator
}

/*****************************************************************************/
/*
int	Save_activity_byte(int frame_number)
{

	int out_frame[Save_W*Save_H];
	int i;
	char file_name[200];

	// Generating a vector 0-255 of chars

	sprintf(file_name,out_file_name,frame_number);
	FILE *pFile;
	pFile=fopen(file_name,"w");
	for (i=0; i<Save_W*Save_H; i++)
	{
		fprintf(pFile,"%f ",Vm[i]);   // 3 - (non boundary cell), 2 - out of tissue (boundary cell), 1 - fibrotic tissue, 0 - regular media
	}
	fclose(pFile);

	sprintf(file_name,out_file_name_current,frame_number);
	
	pFile=fopen(file_name,"w");
	for (i=0; i<Save_W*Save_H; i++)
	{
		fprintf(pFile,"%f ",I_temp[i]);   // 3 - (non boundary cell), 2 - out of tissue (boundary cell), 1 - fibrotic tissue, 0 - regular media
	}
	fclose(pFile);
	
	printf("File %d saved at time %f ms \n",frame_number,sim_time);;
	return 0;
}
*/
void SaveToFile(double sim_time, state_variables* Node)
{
	double sim_time_to_save = floor(sim_time + 0.5);
	char fileName[1024] = {0};
	sprintf(fileName, "Output\\ModelOutput_at_%.2f.txt", sim_time_to_save);
	FILE* pFile = fopen(fileName, "w");
	if(pFile == NULL)
	{
		return;
	}

	for (int i=1; i<W-1; i++)
	{	
		for (int j=1; j<H-1; j++)
		{
			fprintf(pFile, "%4.3f ", Node[i*H+j].V);
		}
		fprintf(pFile, "\n");
	}	
	fclose(pFile);
}

/*****************************************************************************/

const int Protocol = 1; // Can be either 1 or 2
/*
const int TargetCenterH = 50;
const int TargetCenterW = 40;
const int TargetHeight = 15;
const int TargetWidth = 25;
*/

const int TargetCenterH = 0;
const int TargetCenterW = 0;
const int TargetHeight = 0;
const int TargetWidth = 0;

#define INVALID_RISE_TIME	1000.0

void SaveProtFile(double* Stim)
{
	char fileName[1024] = {0};
	sprintf(fileName, "Output\\Protocol%d.txt", Protocol);
	FILE* pFile = fopen(fileName, "w");
	if(pFile == NULL)
	{
		return;
	}

	for (int i=0; i<W; i++)	
	{	
		for (int j=0; j<H; j++)
		{
			fprintf(pFile, "%4.3f ", Stim[i*H+j]);
		}
		fprintf(pFile, "\n");
	}	
	fclose(pFile);
}

void SaveTensorToFile(D_tensor_type* D_tensor)
{
	char fileName[1024] = {0};
	sprintf(fileName, "Output\\D_tensor_ii.txt");
	FILE* pFile = fopen(fileName, "w");
	if(pFile == NULL)
	{
		return;
	}

	for (int i=0; i<W; i++)	
	{	
		for (int j=0; j<H; j++)
		{
			fprintf(pFile, "%4.3f ", D_tensor[i*H+j].Dii);
		}
		fprintf(pFile, "\n");
	}	
	fclose(pFile);

	sprintf(fileName, "Output\\D_tensor_jj.txt");
	pFile = fopen(fileName, "w");
	if(pFile == NULL)
	{
		return;
	}

	for (int i=0; i<W; i++)	
	{	
		for (int j=0; j<H; j++)
		{
			fprintf(pFile, "%4.3f ", D_tensor[i*H+j].Djj);
		}
		fprintf(pFile, "\n");
	}	
	fclose(pFile);

	sprintf(fileName, "Output\\D_tensor_ij.txt");
	pFile = fopen(fileName, "w");
	if(pFile == NULL)
	{
		return;
	}

	for (int i=0; i<W; i++)	
	{	
		for (int j=0; j<H; j++)
		{
			fprintf(pFile, "%4.3f ", D_tensor[i*H+j].Dij);
		}
		fprintf(pFile, "\n");
	}	
	fclose(pFile);
}

int main(int argc, char *argv[])
{
	//int i,j;       // dummy int
	char * fibrosis_name;
	//double DiffW = 0.04*1.4; // was 0.013
	//double DiffH = 0.04*1.4; // was 0.013
	//D_tensor_type D_tensor[W*H];
	D_tensor_type* D_tensor = (D_tensor_type*) malloc(W*H*sizeof(D_tensor_type));
	double Diff = 0.04*1.4; // was 0.013
	int out_file_number=0;
	//Vm = (double*) malloc(W*H*sizeof(double));

	//int fibrosis_matrix[W*H];
	int* fibrosis_matrix = (int*) malloc(W*H*sizeof(int));
	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			fibrosis_matrix[i*H + j] = 0;
			D_tensor[i*H + j].Dii = Diff;
			D_tensor[i*H + j].Djj = Diff;
			D_tensor[i*H + j].Dij = 0.0;
		}
	}
	
	// Set the borders
	for(int iBorder = 0; iBorder < W; ++iBorder)
	{
		fibrosis_matrix[iBorder] = 2;
		fibrosis_matrix[(H-1)*W + iBorder] = 2;

		D_tensor[iBorder].Dii = 0.0;
		D_tensor[iBorder].Djj = 0.0;
		D_tensor[(H-1)*W + iBorder].Dii = 0.0;
		D_tensor[(H-1)*W + iBorder].Djj = 0.0;
	}
	for(int iBorder = 0; iBorder < H; ++iBorder)
	{
		fibrosis_matrix[iBorder*W] = 2;
		fibrosis_matrix[(iBorder+1)*W - 1] = 2;

		D_tensor[iBorder*W].Dii = 0.0;
		D_tensor[iBorder*W].Djj = 0.0;
		D_tensor[(iBorder+1)*W - 1].Dii = 0.0;
		D_tensor[(iBorder+1)*W - 1].Djj = 0.0;
	}		
	SaveTensorToFile(D_tensor);

	// Set the fibroblast patch.
	int m_nCenterH = TargetCenterH;
	int m_nCenterW = TargetCenterW;
	int m_nHeight = TargetHeight;
	int m_nWidth = TargetWidth;	
	int nhStart = m_nCenterH;
	int nhEnd = m_nCenterH+m_nHeight;
	int nwStart = m_nCenterW;
	int nwEnd = m_nCenterW+m_nWidth;
	for (int iH = nhStart; iH < nhEnd; ++iH)
	{	
		for (int iW = nwStart; iW < nwEnd; ++iW)
		{
			fibrosis_matrix[iH*H + iW] = 2;
			D_tensor[iH*H + iW].Dii = 0.0;
			D_tensor[iH*H + iW].Djj = 0.0;
		}
	}
	
	//double outRiseTimeMat[W*H] = {0.0};
	double* outRiseTimeMat = (double*) malloc(W*H*sizeof(double));

	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			outRiseTimeMat[i*H + j] = INVALID_RISE_TIME;
			if(fibrosis_matrix[i*H + j] == 2)
			{
				outRiseTimeMat[i*H + j] = -1.0;
			}
		}
	}

	FILE* pFile = fopen("Output\\TargetFibroblastMatReadable.txt", "w");
	if(pFile != NULL)
	{		
		for (int i = 0; i < W; i++)
		{	
			for (int j = 0; j < H; j++)
			{
				fprintf(pFile, "%d", fibrosis_matrix[i*H + j]);
			}
			fprintf(pFile, "\n");
		}	
		fclose(pFile);
	}

	pFile = fopen("Output\\TargetFibroblastMat.txt", "w");
	if(pFile != NULL)
	{		
		for (int i = 1; i < W-1; i++)
		{	
			for (int j = 1; j < H-1; j++)
			{
				fprintf(pFile, "%.3f ", (double)fibrosis_matrix[i*H + j]);
			}
			fprintf(pFile, "\n");
		}	
		fclose(pFile);
		pFile = NULL;
	}
	
	fibrosis_name="fibrosis_matrix.txt";
	Model* Models = (Model*) malloc(W*H*sizeof(Model));
	state_variables* Node = (state_variables*) malloc(W*H*sizeof(state_variables));
	state_variables* pNewNode = (state_variables*) malloc(W*H*sizeof(state_variables));
	state_variables* pTempNode = NULL;
	double dFirstRiseTime = 0.0;
	
	//double dVm[W*H] = {0};
	//double Stim[W*H] = {0};
	double* dVm = (double*) malloc(W*H*sizeof(double));
	double* Stim = (double*) malloc(W*H*sizeof(double));
	
	double Vm_init_all;
	//int x,y;
	double dVm_plus_x, dVm_minus_x, dVm_plus_y, dVm_minus_y;	
	I_temp = (double*) malloc(W*H*sizeof(double));
		
	// Initialize each CPU individually
	Get_state_variables_initial_condition(); // Put initial conditions in the vector state[...]
	for (int ij = 0; ij < W*H; ij++)
	{
		Assign_node_initial_condition(Node[ij]);
		Assign_node_initial_condition(pNewNode[ij]);		
	}

	//Vm_init_all = Node[0].V; // Just take Vm from one of the nodes
	for (int ij = 0; ij < W*H; ij++)
	{
		Stim[ij] = 0.0;
		//Vm[ij] = Vm_init_all;
	}
	if(Protocol == 1)
	{	
		for (int i = 0; i <= 2; i++)
			//for (int i = 10; i <= 12; i++)
		{
			for (int j = 0; j < H; j++) 
			{
				Stim[i*H + j] = S1_amp;
			}
		}
	}				
	else if(Protocol == 2)
	{					
		for (int i = 0; i < W; i++) 
		{
			for (int j = 0; j <= 2; j++)
				//for (int j = 10; j <= 12; j++)
			{
				Stim[i*H + j] = S1_amp;
			}
		}
	}
	SaveProtFile(Stim);

	clock_t iterationStartingTime = clock();
	double bStim = false;
	// time loop
	for (sim_time = 0; sim_time < (max_time+time_step); sim_time += time_step)
	{
		bStim = ((sim_time >= S1_begin) && (sim_time < S1_begin + S1_total_time));
	
		if (sim_time >= time_to_send)
		{
			clock_t iterationEndingTime = clock();
			double iterationRunningTime = (iterationEndingTime - iterationStartingTime)/double(CLOCKS_PER_SEC);

			printf("Time: %.2f (duration from last print: %.2f)\n", sim_time, iterationRunningTime);
			SaveToFile(sim_time, Node);
			Show_Vm_Char(fibrosis_matrix, Node);
			time_to_send += send_period;
			iterationStartingTime = clock();
		}

		for (int x = 0; x < W; x++) 
		{
			for (int y = 0; y < H; y++)
			{
				int nCurIndex = x*H+y;
				if (fibrosis_matrix[nCurIndex] == 0)
				{
					if((Node[nCurIndex].V > 0.0) && (outRiseTimeMat[nCurIndex] == INVALID_RISE_TIME))
					{				
						if(dFirstRiseTime == 0.0)
						{
							dFirstRiseTime = sim_time;
						}
						outRiseTimeMat[nCurIndex] = sim_time - dFirstRiseTime;
					}

					dVm_plus_x = (2.0*D_tensor[x*H+y].Dii*D_tensor[(x+1)*H+y].Dii/
						(D_tensor[x*H+y].Dii+D_tensor[(x+1)*H+y].Dii))*(Node[(x+1)*H+y].V-Node[x*H+y].V)/dW;
					//dVm_plus_x+=((D_tensor[x*H+y].Dij*D_tensor[(x+1)*H+y].Dii + D_tensor[x*H+y].Dii*D_tensor[(x+1)*H+y].Dij)/
					//	(D_tensor[x*H+y].Dii+D_tensor[(x+1)*H+y].Dii))*(Node[x*H+y+1].V+Node[(x+1)*H+y+1].V-Node[x*H+y-1].V-Node[(x+1)*H+y-1].V)/(4.0*dH);
					dVm_minus_x = (2.0*D_tensor[(x-1)*H+y].Dii*D_tensor[x*H+y].Dii/
						(D_tensor[(x-1)*H+y].Dii + D_tensor[x*H+y].Dii))*(Node[x*H+y].V-Node[(x-1)*H+y].V)/dW;
					//dVm_minus_x+=((D_tensor[(x-1)*H+y].Dij*D_tensor[x*H+y].Dii + D_tensor[(x-1)*H+y].Dii*D_tensor[x*H+y].Dij)/
					//	(D_tensor[(x-1)*H+y].Dii+D_tensor[x*H+y].Dii))*(Node[x*H+y+1].V+Node[(x-1)*H+y+1].V-Node[x*H+y-1].V-Node[(x-1)*H+y-1].V)/(4.0*dH);
					
					dVm_plus_y = (2.0*D_tensor[x*H+y].Djj*D_tensor[x*H+y+1].Djj/
						(D_tensor[x*H+y].Djj+D_tensor[x*H+y+1].Djj))*(Node[x*H+y+1].V-Node[x*H+y].V)/dH;
					//dVm_plus_y+=((D_tensor[x*H+y].Dij*D_tensor[x*H+y+1].Djj + D_tensor[x*H+y].Djj*D_tensor[x*H+y+1].Dij)/
					//	(D_tensor[x*H+y].Djj+D_tensor[x*H+y+1].Djj))*(Node[(x+1)*H+y].V+Node[(x+1)*H+y+1].V-Node[(x-1)*H+y].V-Node[(x-1)*H+y+1].V)/(4.0*dW);
					dVm_minus_y = (2.0*D_tensor[x*H+y-1].Djj*D_tensor[x*H+y].Djj/
						(D_tensor[x*H+y-1].Djj + D_tensor[x*H+y].Djj))*(Node[x*H+y].V-Node[x*H+y-1].V)/dH;
					//dVm_minus_y+=((D_tensor[x*H+y-1].Dij*D_tensor[x*H+y].Djj + D_tensor[x*H+y-1].Djj*D_tensor[x*H+y].Dij)/
					//	(D_tensor[x*H+y-1].Djj+D_tensor[x*H+y].Djj))*(Node[(x+1)*H+y].V+Node[(x+1)*H+y-1].V-Node[(x-1)*H+y].V-Node[(x-1)*H+y-1].V)/(4.0*dW);
					
					dVm[x*H+y]=((dVm_plus_x-dVm_minus_x)*dH+(dVm_plus_y-dVm_minus_y)*dW)/(dW*dH);

					double dStim = bStim ? Stim[nCurIndex] : 0.0;
					I_temp[nCurIndex] = Models[nCurIndex].Total_transmembrane_currents(Node[nCurIndex], pNewNode[nCurIndex], dStim);
					pNewNode[nCurIndex].V = Node[nCurIndex].V + time_step*(dVm[nCurIndex] - I_temp[nCurIndex]);
					I_temp[nCurIndex] = I_temp[nCurIndex] - dStim;
					//Vm[nCurIndex] = pNewNode[nCurIndex].V;

					/*
					if(x == 1)
					{
						if(y == 1 || y == H-2)
						{
							printf("x=%d, y=%d: New.V=%4.3f, dVm=%4.3f, I=%4.3f, Stim=%4.3f\n", x, y, pNewNode[nCurIndex].V, dVm[x*H+y], I_temp[nCurIndex], dStim);
						}
					}
					*/
				}
			}
		}
						
		bool bStopSim = true;
		if(Protocol == 1)
		{
			int i = (W-2);
			for (int j = 1; j < H-1; j++) 
			{							
				if(outRiseTimeMat[i*H + j] == INVALID_RISE_TIME)
				{
					bStopSim = false;
					break;
				}
			}			
		}
		else if(Protocol == 2)
		{
			for (int i = 1; i < W-1; i++)
			{			
				int j = H-2;
				if(outRiseTimeMat[i*H + j] == INVALID_RISE_TIME)
				{
					bStopSim = false;
					break;
				}
			}
		}
		else
		{
			bStopSim = false;
		}
		if(bStopSim)
		{
			break;
		}		

		pTempNode = Node;
		Node = pNewNode;
		pNewNode = Node;

	} // sim_time loop

	char targetFilename[1024] = {0};
	sprintf(targetFilename, "Output\\TargetFibroblastMatResults%d.txt", Protocol);
	FILE* pFibroBlastFile = fopen(targetFilename, "w");
	if(pFibroBlastFile != NULL)
	{		
		for (int i = 1; i < W-1; i++)
		{	
			for (int j = 1; j < H-1; j++)
			{
				fprintf(pFibroBlastFile, "%.3f ", outRiseTimeMat[i*H + j]);
			}
			fprintf(pFibroBlastFile, "\n");
		}	
		fclose(pFibroBlastFile);
		pFibroBlastFile = NULL;
	}

	return 0;
} // of main

