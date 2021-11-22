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
	//if (isWriteConverge) cout << "Output converge file" << endl;
	//else cout << "converge file is not written" << endl;
	
	//TODO: write the convergence value for each seed and each generation
	//first store the value 
	//then output the values

	OpenAndCleanFiles();
	ABCAlgorithms MainAlgo;
	MainAlgo.ReadData();
	GRAPH BaseGraph;
	ReadDataMain(BaseGraph);
	UEeps = 0.01;
	MainAlgo.ABCMain(BaseGraph);


	//ofstream RemarkFile;
	//AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	//RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	////if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	//UEeps = 0.01;
	//BaseGraph.EvaluteGraph();
	//cout << "BaseGraph Without Failure = " << BaseGraph.TotalSystemCost << endl;
	////TODO: Read extern failure links data
	return 0;

}