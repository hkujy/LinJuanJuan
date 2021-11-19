#include "CommonHeaders.h"
#include "DefGloVar.h"
#include "RestoreSchClass.h"
using namespace std;
//int TestMedium();
//int TestAlgorithmPara();
//int TestCSAandGA();
//void ReadDataMain(GRAPH& BaseGraph, NODEPROMATRIX& Pmatrix, LINKPROMATRIX& LinkPmatrix);
//void ReadDataMain(GRAPH& BaseGraph, NODEPROMATRIX& Pmatrix);
void ReadDataMain(GRAPH& BaseGraph);
void OpenAndCleanFiles();
bool ReadModelParas();
void CloseFiles();

int main(int argc, char* argv[])
{
	//isWriteConverge = false;
	isWriteConverge = true;
	ModelIndex = 4; //Paradox network
	if (isWriteConverge) cout << "Output converge file" << endl;
	else cout << "converge file is not written" << endl;
	OpenAndCleanFiles();
	if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	ofstream RemarkFile;
	AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	GRAPH BaseGraph;
	ReadDataMain(BaseGraph);
	UEeps = 0.01;
	BaseGraph.EvaluteGraph();
	cout << "BaseGraph Without Failure = " << BaseGraph.TotalSystemCost << endl;
	//TODO: Read extern failure links data
	BaseGraph.Links.at(6).RequiredRes = 1;
	BaseGraph.Links.at(7).RequiredRes = 1;
	BaseGraph.Links.at(8).RequiredRes = 1;

	BaseGraph.Links.at(6).RecoverTime = 2;
	BaseGraph.Links.at(7).RecoverTime = 2;
	BaseGraph.Links.at(8).RecoverTime = 2;

	ABCAlgorithms MainAlgo;
	MainAlgo.GenerateIni(BaseGraph);


	//Scenario s;
	//// evaluate one link failure scenario
	//s.LinkIds.push_back(6); s.tau.push_back(0);
	//BaseGraph.EvalutateFailureScenarios(s);
	//BaseGraph.RevertFailureScenarios(s);


	//s.LinkIds.clear(); s.tau.clear();
	//s.LinkIds.push_back(7); s.tau.push_back(0);
	//BaseGraph.EvalutateFailureScenarios(s);
	//BaseGraph.RevertFailureScenarios(s);

	//// Step 1: evaluate the network without the link, by setting the capacity to be a large number
	//double TempVal = BaseGraph.Links.at(4).CaInput;
	//BaseGraph.Links.at(4).CaRevise = 0.0000001;
	//BaseGraph.Links.at(4).CaInput = 0.0000001;
	//BaseGraph.EvaluteGraph();
	//cout << "Without the paradox link: Total Cost = " << BaseGraph.TotalSystemCost << endl;
	//BaseGraph.PrintLinks_onscreen();

	//// Step 2: evaluate the network with the link, by setting the capacity to be the same as the input value
	//BaseGraph.Links.at(4).CaInput = TempVal;
	//BaseGraph.Links.at(4).CaRevise = TempVal;
	//BaseGraph.EvaluteGraph();
	//BaseGraph.PrintLinks_onscreen();
	//cout << "With the paradox link: Total Cost = " << BaseGraph.TotalSystemCost << endl;

	//CsaNumPop = 0;
	//CsaCloneBeta = 0.0f;
	//CsaRepRatio = 0.0f;
	//isWriteConverge = true;
	//isWriteConverge = false;
	//GANumChild = 0;
	//GANumPop = 0;
	//StopCriteria = -1;
	//CSACloneMethod = 1; // proportion to the fitness value
	// step 2

	//cout << "Case 1: TestNagurenyNetwork" << endl;
	//cout << "Case 2: TestMedium" << endl;
	//cout << "Case 3: TestAlgorithmPara" << endl;
	//cout << "Case 4: TestCSAandGA" << endl;

	//int ErrMsg = -1;
	////ModelIndex = 1; //SiouxFallsNetwork
	//ModelIndex = 4; //Paradox network
	//if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	//ofstream RemarkFile;
	//AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	//RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	//GRAPH BaseGraph;
	////NODEPROMATRIX NodeProbMatrix;
	////LINKPROMATRIX LinkProbMatrix;
	//ReadDataMain(BaseGraph);
	//BaseGraph.EvaluteGraph();
	//BaseGraph.PrintLinks_onscreen();   
	//cout << "BeforeCase: Total Cost = " << BaseGraph.TotalSystemCost << endl;
	//double TempVal = BaseGraph.Links.at(4).CaInput;
	//BaseGraph.Links.at(4).CaRevise = 0.0000001;
	//BaseGraph.Links.at(4).CaInput = 0.0000001;
	//BaseGraph.EvaluteGraph();
	//BaseGraph.Links.at(4).CaInput = TempVal;
	//cout << "AfterCase: Total Cost = " << BaseGraph.TotalSystemCost << endl;
	//BaseGraph.PrintLinks_onscreen();   
	//TestNagurenyNetwork(); 
	//TestMedium();
	//TestAlgorithmPara();
	//TestCSAandGA();
	//CloseFiles();

	return 0;

}