#include "CommonHeaders.h"
using namespace std;

int CsaTestMain(GRAPH &BaseGraph, const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
//void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime, NODEPROMATRIX &Pmatrix);
void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix);
bool ReadModelParas();
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix, LINKPROMATRIX &LinkPmatrix);
int GATestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
int TestAlgorithmPara()
{
	int ErrMsg = -1;
	ModelIndex = 3; //SiouxFallsNetwork
	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	ofstream RemarkFile;
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	GRAPH BaseGraph;
	NODEPROMATRIX NodeProbMatrix;
	LINKPROMATRIX LinkProbMatrix;
	ReadDataMain(BaseGraph, NodeProbMatrix, LinkProbMatrix);
	
	//RemarkFile << "This test is adjust CsaConle Beta from 0.2 to 1.0" << endl;
	RemarkFile << "This test is adjust CsaRepRatio from 0.2 to 1.0" << endl;
	CsaCloneBeta = 0.2f;
	for (int i = 0; i < 5;i++)
	{
		CsaRepRatio = 0.2f + i*0.2f;
		vector<CHROME> BestSol;
		vector<double> CpuTimeVec;
		ErrMsg = CsaTestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
		//ErrMsg = GATestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
		//DebugMain();
		//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);
		OutputSummary(BestSol, BaseGraph, CpuTimeVec);
	}
	return 0;

}