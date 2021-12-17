#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
#include "MutationFuncHeader.h"
#include "RandomFuncs.h"
using namespace std;

int CaseIndex(double Fit, double MaxFit, double MinFit){
	double Fp = MaxFit - MinFit;
	assert(Fp >= 0.0f);
	if (Fit <= MaxFit&&Fit > MaxFit - 0.05f*Fp) return 0;
	if (Fit <= MaxFit - 0.05f*Fp&&Fit > MaxFit - 0.2f*Fp) return 1;
	if (Fit <= MaxFit - 0.20f*Fp&&Fit > MaxFit - 0.4f*Fp) return 2;
	if (Fit <= MaxFit - 0.4f*Fp&&Fit > MaxFit - 0.6f*Fp) return 3;
	if (Fit <= MaxFit - 0.6f*Fp&&Fit > MaxFit - 0.8f*Fp) return 4;
	if (Fit <= MaxFit - 0.8f*Fp&&Fit >= MinFit) return 5;
	return InvaildInt;
}

void Algorithms::FirstProcedure(CHROME &Chrom){
	FirstProcedureFunc(Chrom, this->NodeDofVarSet);
}

void Algorithms::addNewNode(CHROME &Chrom){
	addNewNodeFunc(Chrom, this->NodeDofVarSet);
}

void Algorithms::removeNodeDof(CHROME &Chrom){
	removeNodeDofFunc(Chrom);
}

void Algorithms::exchangeNodeDof(CHROME &Chrom){
	exchangeNodeDofFunc(Chrom,this->NodeDofVarSet);
}

void Algorithms::SecondProcedure(CHROME &Chrom, double Ratio){
	
	int NumOfRepeate = (int)((double)NumNodes*Ratio);
	std::vector<int> v = { 1, 2, 3 }; //a,b,c, three operators
	for (int i = 0; i < NumOfRepeate; i++)
	{
		int Index = GenRandomInt(v);
		//cout << "index = " << Index<<endl;
		switch (Index)
		{
			//add a new failed node and assign it dof value
		case 1: addNewNode(Chrom); break;
		case 2: removeNodeDof(Chrom); break;
		case 3: exchangeNodeDof(Chrom); break;
		default:
			TRACE("SecondProcedure generate wrong Index");
			system("PAUSE");
			break;
		}
	}
	
}
void Algorithms::HyperMutateMain(CHROME &Chrom){

	int MutatorIndex;  // from 1 to 5
	double f;
	switch (AlgorithmIndex)
	{
		case CSA:
		f = GenRandomReal();
			if (f < double(1.0f / 6.0f))
			{
				MutatorIndex = 0;	break;
			}
			if (f < double(2.0f / 6.0f) && f >= double(1.0f / 6.0f))
			{
				MutatorIndex = 1;	break;
			}
			if (f < double(3.0f / 6.0f) && f >= double(2.0f / 6.0f))
			{
				MutatorIndex = 2;	break;
			}
			if (f < double(4.0f / 6.0f) && f >= double(3.0f / 6.0f))
			{
				MutatorIndex = 3;	break;
			}
			if (f < double(5.0f / 6.0f) && f >= double(4.0f / 6.0f))
			{
				MutatorIndex = 4;	break;
			}
			if (f <= double(6.0f / 6.0f) && f >= double(5.0f / 6.0f))
			{
				MutatorIndex = 5;
				break;
			}
			//MutatorIndex = CaseIndex(Chrom.Fitness, MaxFitValue, MinFitValue); break;
		case GA:
			f = GenRandomReal();
			if (f < double(1.0f / 6.0f))
			{
				MutatorIndex = 0;	break;
			}
			if (f < double(2.0f / 6.0f) && f >= double(1.0f / 6.0f))
			{
				MutatorIndex = 1;	break;
			}
			if (f < double(3.0f / 6.0f) && f >= double(2.0f / 6.0f))
			{
				MutatorIndex = 2;	break;
			}
			if (f < double(4.0f / 6.0f) && f >= double(3.0f / 6.0f))
			{
				MutatorIndex = 3;	break;
			}
			if (f < double(5.0f / 6.0f) && f >= double(4.0f / 6.0f)) 
			{
				MutatorIndex = 4;	break;
			}
			if (f <= double(6.0f / 6.0f) && f >= double(5.0f / 6.0f))
			{
				MutatorIndex = 5;
				break;
			}
		
	default:
		cerr << "Algorithm index is not proper defined" << endl;
		system("PAUSE");
		break;
	}
	
	switch (MutatorIndex)
	{
	case 0:
		for (unsigned int i = 0; i < Chrom.NodeDof.size();i++)
		{
			if (GenRandomReal()>=0.5f) increasdof(Chrom.NodeDof.at(i), NodeDofVarSet);
			else decreaseDof(Chrom.NodeDof.at(i), NodeDofVarSet);
		}
		break;
	case 1:
		if (GenRandomReal() <= 0.1f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.2f);
		break;
	case 2:
		if (GenRandomReal() <= 0.2f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.4f);
		break;
	case 3:
		if (GenRandomReal() <= 0.3f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.6f);
		break;
	case 4:
		if (GenRandomReal() <= 0.4f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.8f);
		break;
	case 5:
		if (GenRandomReal() <= 0.5f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 1.0f);
		break;
	default:	
		TRACE("CSA Hypermutation main generates wrong Index");
		system("PAUSE");
		break;
	}

};