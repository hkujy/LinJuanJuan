#include "CommonHeaders.h"
#include "RandomFuncs.h"
using namespace std;
bool ReadSeedVec(std::vector<int> &SeedVec, FILE *fin);
int GATestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
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
		ofstream ConvergeFile ;
	
		ConvergeFile.open("..//OutPut//GAconverge.txt", ios::app);

	
		Algorithms GATest(GANumPop,GANumChild);
		for (int i = 0; i < NumNodes; i++)
		{
			GATest.NodeVarSet.push_back(i);
		}
		for (unsigned int i = 0; i < NodeProbMatrix.Dof.size(); i++)
		{
			GATest.NodeDofVarSet.push_back(NodeProbMatrix.Dof.at(i));
		}
		BaseGraph.EvaluteGraph();
		GATest.BaseUNPM = BaseGraph.UNPM;

		for (unsigned int i = 0; i < SeedVec.size(); i++)
		{
			SeedRunCount = i;
			clock_t ST = clock();
			GenRan.seed((unsigned)SeedVec.at(i));
			

			if (isWriteConverge)
			{
				ofstream SolConvergeFile;
				SolConvergeFile.open("..//OutPut//GAConvergeBasedOnSol.txt", ios::app);
				//ConvergeFile << "********Seed Num = " << (unsigned)SeedVec.at(i) << "********" << endl;
				//SolConvergeFile << "********Seed Num = " << (unsigned)SeedVec.at(i) << "********" << endl;
				SolConvergeFile.close();
			}

			GATest.GAmain(BaseGraph, GANumPop, GANumChild,  NodeProbMatrix, LinkProbMatrix, ConvergeFile);
			BestSol.push_back(GATest.Chroms.at(0));
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