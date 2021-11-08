#include "CommonHeaders.h"
using namespace std;

int CsaTestMain(GRAPH &BaseGraph, const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime,
	NODEPROMATRIX &Pmatrix);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix);
bool ReadModelParas();
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix, LINKPROMATRIX &LinkPmatrix);
int ScanNagurenyNetwork(GRAPH &BaseGraph, vector<CHROME> &BestSol, const NODEPROMATRIX &Pmatrix,
	const LINKPROMATRIX &LinkMatrx);
int GATestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
int TestNagurenyNetwork()
{

	int ErrMsg = -1;
	ModelIndex = 2; //  Nagureny2009Network
	cout << "Nagureny 2009 network is testing" << endl;

	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);

	// step 2: Initialize basic parameters
	GRAPH BaseGraph;
	NODEPROMATRIX NodeProbMatrix;
	LINKPROMATRIX LinkProbMatrix;
	ReadDataMain(BaseGraph, NodeProbMatrix, LinkProbMatrix);
	vector<CHROME> BestSol;
	vector<double> CpuTimeVec;
	clock_t ST, ET;
	//cout << "Start test CSA " << endl;

	//ST = clock();
	//ErrMsg = CsaTestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
	//ET = clock();
	//CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	//cout << "CSA Complete " << endl;
	//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);

	cout << "Start Scan NagurenyNetwork " << endl;
	ST = clock();
	ErrMsg = ScanNagurenyNetwork(BaseGraph, BestSol, NodeProbMatrix, LinkProbMatrix);
	ET = clock();
	CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	cout << "Scan NagurenyNetwork Complete " << endl;
	OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);

	return 0;

}