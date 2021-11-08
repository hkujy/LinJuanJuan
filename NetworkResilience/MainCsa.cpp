#include "CommonHeaders.h"
#include "RandomFuncs.h"
using namespace std;
void BruteForceFor3NodeSmall(NODEPROMATRIX &Prob,
	GRAPH &Graph);
bool ReadSeedVec(std::vector<int> &SeedVec,
	FILE *fin){
	int SeedValue;
	SeedVec.clear();
	if (nullptr == fin){
		perror("Read Seed File Fails \n");
		return false;
	}
	else
	{
		while (!feof(fin))
		{
			fscanf_s(fin, "%i",
				&SeedValue);
			if (SeedValue != EOF){
				SeedVec.push_back(SeedValue);
			}
		}
	}
	return true;
}
int CsaTestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix,const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
					vector<double> &CpuTimeVec)
{
	try
	{
		vector<int> SeedVec;
		FILE * fin;
		//step 1: read CSA seed parameter
		if (ModelIndex == 3)
		{
			fopen_s(&fin, "..//Input//SiouxFallsNetwork//Seed.txt", "r");
			if (!ReadSeedVec(SeedVec, fin)) TRACE("Read Seed File Fails \n");
		}
		else if (ModelIndex == 1)
		{
			fopen_s(&fin, "..//Input//MediumNetwork//Seed.txt", "r");
			if (!ReadSeedVec(SeedVec, fin)) TRACE("Read Seed File Fails \n");
		}
		else
		{
			cout << "Check Csa Seed Open file data" << endl;
			system("pause");
		}
		
		fin = nullptr;
		ofstream ConvergeFile;
		ConvergeFile.open("..//OutPut//CSAconverge.txt", ios::app);
		assert(ConvergeFile.is_open());
		
		int NumClone = (int)(CsaCloneBeta*CsaNumPop);
		int NumRep = (int)(CsaRepRatio*CsaNumPop);
		if (NumClone == 0 || NumRep == 0)
		{
			cerr << "Csa Num population is not read" << endl;
		}
		// initialize Csa Parameters
		Algorithms CsaTest(CsaNumPop, NumClone, NumRep);
		for (int i = 0; i < NumNodes; i++)
		{
			CsaTest.NodeVarSet.push_back(i);
		}
		for (unsigned int i = 0; i < NodeProbMatrix.Dof.size(); i++)
		{
			CsaTest.NodeDofVarSet.push_back(NodeProbMatrix.Dof.at(i));
		}
		BaseGraph.EvaluteGraph();
		CsaTest.BaseUNPM = BaseGraph.UNPM;

		for (unsigned int i = 0; i < SeedVec.size();i++)
		{
			SeedRunCount = i;
			clock_t ST = clock();
			GenRan.seed((unsigned)SeedVec.at(i));
			//GenRan.seed((unsigned)time(NULL));
			if (isWriteConverge)
			{
				ofstream SolConvergeFile;
				SolConvergeFile.open("..//OutPut//CSAConvergeBasedOnSol.txt", ios::app);
				//ConvergeFile << "********Seed Num = " << (unsigned)SeedVec.at(i) << "********" << endl;
				//SolConvergeFile<< "********Seed Num = " << (unsigned)SeedVec.at(i) << "********" << endl;
				SolConvergeFile.close();
			}

			CsaTest.CSAmain(BaseGraph, CsaNumPop, NumClone, NumRep, NodeProbMatrix, LinkProbMatrix, ConvergeFile);
			BestSol.push_back(CsaTest.Chroms.at(0));
			clock_t ET = clock();
			CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
		}
		ConvergeFile.close();


		return 1;
	}
	catch (exception& e)
	{
		TRACE("%s", e);
		return 0;
	}
}