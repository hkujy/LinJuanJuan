//new function created in 2021-Nov-8. the objetive is create example for the baraess paradox network

#include "CommonHeaders.h"
using namespace std;

//void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime, NODEPROMATRIX &Pmatrix);
void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix);
bool ReadModelParas();
// void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix, LINKPROMATRIX &LinkPmatrix);
void ReadDataMain(GRAPH &BaseGraph);
int ScanNagurenyNetwork(GRAPH &BaseGraph, vector<CHROME> &BestSol, const NODEPROMATRIX &Pmatrix,
	const LINKPROMATRIX &LinkMatrx);
int TestBaressParadoxNetwork()
{
	int ErrMsg = -1;
	//ModelIndex = 4;   // Baraess Paradox Network
	ModelIndex = 2; //  Nagureny2009Network
	cout << "Now is testing Baress Paradox network" << endl;

	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);

	// step 2: Initialize basic parameters
	GRAPH BaseGraph;
	ReadDataMain(BaseGraph);
	vector<CHROME> BestSol;
	vector<double> CpuTimeVec;
	clock_t ST, ET;
	cout << "Start Scan Paradox Network " << endl;
	BaseGraph.EvaluteGraph();
	//cout << "Start test CSA " << endl;

	//ST = clock();
	//ErrMsg = CsaTestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
	//ET = clock();
	//CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	//cout << "CSA Complete " << endl;
	//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);

	cout << "Start Scan NagurenyNetwork " << endl;
	ST = clock();
	//ErrMsg = ScanNagurenyNetwork(BaseGraph, BestSol, NodeProbMatrix, LinkProbMatrix);
	ET = clock();
	CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	cout << "Scan NagurenyNetwork Complete " << endl;
	//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);

	return 0;

}