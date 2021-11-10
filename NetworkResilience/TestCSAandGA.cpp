#include "CommonHeaders.h"
using namespace std;

void OpenAndCleanFiles();
void CloseFiles();
int CsaTestMain(GRAPH &BaseGraph, const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
//void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime, NODEPROMATRIX &Pmatrix);
void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix);
bool ReadModelParas();
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix, LINKPROMATRIX &LinkPmatrix);
int ScanNagurenyNetwork(GRAPH &BaseGraph, vector<CHROME> &BestSol, const NODEPROMATRIX &Pmatrix,
	const LINKPROMATRIX &LinkMatrx);
int GATestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
int TestCSAandGA()
{
	int ErrMsg = -1;
	ModelIndex = 3; //SiouxFallsNetwork
	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	// step 2: Initialize basic parameters
	GRAPH BaseGraph;
	NODEPROMATRIX NodeProbMatrix;
	LINKPROMATRIX LinkProbMatrix;
	//ReadDataMain(BaseGraph, Pmatrix);
	ReadDataMain(BaseGraph, NodeProbMatrix, LinkProbMatrix);

	vector<CHROME> BestSol;
	vector<double> CpuTimeVec;
	vector<double> BestSolSummary;

	//ErrMsg = GATestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);

	/// TEST CSA
		//StopCriteria = 0;  fixed number of iter
		//StopCriteria = 1;  fixed number of ue
	for (StopCriteria = 0; StopCriteria < 2; StopCriteria++)
	{
		//CSACloneMethod = 0;  // fixed number of clone solutions
		CSACloneMethod = 1; // proportion to the fitness
		BestSol.clear();
		CpuTimeVec.clear();
		ErrMsg = CsaTestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
		//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);
		OutputSummary(BestSol, BaseGraph, CpuTimeVec);
		for (int i = 0; i < (int)BestSol.size(); i++)
		{
			BestSolSummary.push_back(BestSol[i].Fitness);
		}
	}

	/// TEST GA
	for (StopCriteria = 0; StopCriteria < 2; StopCriteria++)
	{
		BestSol.clear();
		CpuTimeVec.clear();
		ErrMsg = GATestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
		//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);
		OutputSummary(BestSol, BaseGraph, CpuTimeVec);
		for (int i = 0; i < (int) BestSol.size(); ++i)
		{
			BestSolSummary.push_back(BestSol[i].Fitness);
		}
	}


	std::ofstream BestSolSumFile;
	BestSolSumFile.open("..//OutPut//BestSolSummar.txt", ios::trunc);
	BestSolSumFile << "CSAStop1,CSAStop2,GAstop1,GAStop4" << endl;

	int NumSeed =(int) BestSolSummary.size() / 4;
	for (int i = 0; i < NumSeed; i++)
	{
		BestSolSumFile << BestSolSummary.at(i) << ",";
		BestSolSumFile << BestSolSummary.at(i + NumSeed) << ",";
		BestSolSumFile << BestSolSummary.at(i + 2 * NumSeed) << ",";
		BestSolSumFile << BestSolSummary.at(i + 3 * NumSeed) << ",";
		BestSolSumFile << endl;
	}
	BestSolSumFile << "For check sol" << endl;
	for (auto j = BestSolSummary.begin(); j != BestSolSummary.end(); ++j)
	{
		BestSolSumFile << *j << endl;
	}

	return 0;

}