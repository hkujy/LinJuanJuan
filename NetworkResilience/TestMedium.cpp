#include "CommonHeaders.h"
using namespace std;

int ScanMedium(GRAPH &BaseGraph, vector<CHROME> &BestSol, const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkMatrx);
int CsaTestMain(GRAPH &BaseGraph, const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
//void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTime, NODEPROMATRIX &Pmatrix);
void OutputSummary(vector<CHROME> &BestSol, GRAPH &Graph, vector<double> &CpuTim);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix);
bool ReadModelParas();
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix, LINKPROMATRIX &LinkPmatrix);
int GATestMain(GRAPH &BaseGraph, const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, vector<CHROME> &BestSol,
	vector<double> &CpuTimeVec);
int TestMedium()
{
	int ErrMsg = -1;
	ModelIndex = 1; //medium size network is test
	cout << "medium size network is testing" << endl;

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

	//cout << "Start test CSA on Medium " << endl;
	string testindicator;
	cout << "1: Test Medium csa" << endl;
	cout << "2: Test Medium scan" << endl;
	cout << "3: Test Medium both" << endl;
	cin >> testindicator;
	ofstream  fSummay;

	if (testindicator.compare("2") == 0) goto Scan;

	cout << "start test CSA " << endl;
	clock_t ST = clock();
	ErrMsg = CsaTestMain(BaseGraph, NodeProbMatrix, LinkProbMatrix, BestSol, CpuTimeVec);
	clock_t ET = clock();
	CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	cout << "CSA Complete " << endl;
	//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);
	OutputSummary(BestSol, BaseGraph, CpuTimeVec);

	fSummay.open("..//OutPut//Summary.txt", ios::app);
	fSummay << "The following results from scan" << endl;

	if (testindicator.compare("1") == 0) return 0;
Scan:
	cout << "Start Scan Medium " << endl;
	ST = clock();
	fSummay << "start time is " << ST << endl;
	ErrMsg = ScanMedium(BaseGraph, BestSol, NodeProbMatrix, LinkProbMatrix);
	ET = clock();
	fSummay << "end time is " << ET << endl;
	CpuTimeVec.push_back((double)(ET - ST) / CLOCKS_PER_SEC); // time unit is second
	cout << "Scan Medium Complete " << endl;
	fSummay << "duration is " << (double)(ET - ST) / CLOCKS_PER_SEC<<" seconds" << endl;
	
	fSummay << "The following output contains both CSA and scan: the last row is scan results"<< endl;

	//OutputSummary(BestSol, BaseGraph, CpuTimeVec, NodeProbMatrix);
	OutputSummary(BestSol, BaseGraph, CpuTimeVec);

	return 0;

}