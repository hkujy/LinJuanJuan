#include "CommonHeaders.h"
#include "DefGloVar.h"
using namespace std;
void OpenAndCleanFiles();
int TestMedium();
int TestAlgorithmPara();
int TestCSAandGA();
void CloseFiles();
int TestNagurenyNetwork();
void ReadDataMain(GRAPH& BaseGraph, NODEPROMATRIX& Pmatrix);
bool ReadModelParas();
void ReadDataMain(GRAPH& BaseGraph, NODEPROMATRIX& Pmatrix, LINKPROMATRIX& LinkPmatrix);
void ReadDataMain(GRAPH& BaseGraph);
// TODO: Change some input to the format of CSV

int main(int argc, char *argv[])
{
	// Step 1: initialize parameters
	//cout << "Yes : if you want to output converge file" << endl;
	//cout << "No  : if No converge file" << endl;
	//cout << "Converge file is not written, because they are embraced by #if Deubg" << endl;
	//cout << "if want to output convergence file, need to deactive these " << endl;

	////string IsWrite;
	////cin >> IsWrite;
	//if (IsWrite == "Yes") isWriteConverge = true;
	//else if (IsWrite == "No") isWriteConverge = false;
	//else
	//	cout << "Error for isWriteinput";

	isWriteConverge = false;
	//isWriteConverge = true;
	if (isWriteConverge) cout << "Output converge file" << endl;
	else cout << "converge file is not written" << endl;

	CsaNumPop = 0;
	CsaCloneBeta = 0.0f;
	CsaRepRatio = 0.0f;
	//isWriteConverge = true;
	//isWriteConverge = false;
	GANumChild = 0;
	GANumPop = 0;
	StopCriteria = -1;
	CSACloneMethod = 1; // proportion to the fitness value
	// step 2
	OpenAndCleanFiles();

	//cout << "Case 1: TestNagurenyNetwork" << endl;
	//cout << "Case 2: TestMedium" << endl;
	//cout << "Case 3: TestAlgorithmPara" << endl;
	//cout << "Case 4: TestCSAandGA" << endl;

	int ErrMsg = -1;
	//ModelIndex = 1; //SiouxFallsNetwork
	ModelIndex = 4; //Paradox network
	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	ofstream RemarkFile;
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	GRAPH BaseGraph;
	//NODEPROMATRIX NodeProbMatrix;
	//LINKPROMATRIX LinkProbMatrix;
	ReadDataMain(BaseGraph);
	BaseGraph.EvaluteGraph();
	BaseGraph.PrintLinks_onscreen();   
	cout << "BeforeCase: Total Cost = " << BaseGraph.TotalSystemCost << endl;
	double TempVal = BaseGraph.Links.at(4).CaInput;
	BaseGraph.Links.at(4).CaRevise = 0.0000001;
	BaseGraph.Links.at(4).CaInput = 0.0000001;
	BaseGraph.EvaluteGraph();
	BaseGraph.Links.at(4).CaInput = TempVal;
	cout << "AfterCase: Total Cost = " << BaseGraph.TotalSystemCost << endl;
	BaseGraph.PrintLinks_onscreen();   
	//TestNagurenyNetwork(); 
	//TestMedium();
	//TestAlgorithmPara();
	//TestCSAandGA();
	//CloseFiles();

	return 0;

}