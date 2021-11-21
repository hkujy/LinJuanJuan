#include "CommonHeaders.h"
#include "DefGloVar.h"
#include "RestoreSchClass.h"
#include "ABC.h"
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
	//TODO: Read and Print the input parameters
	//TODO: 1. ABC Parameters
	//TODO: 2. Link Failure Value
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
	MainAlgo.ABCMain(BaseGraph);



	return 0;

}