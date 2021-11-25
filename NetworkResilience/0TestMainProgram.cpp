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
//void ReadDataMain(GRAPH& BaseGraph);
void OpenAndCleanFiles();
bool ReadModelParas();
void CloseFiles();
void ReadModelPara()
{
	ifstream fin;
	if (ModelIndex == 1)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//MediumNetwork//Para.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (ModelIndex == 2)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (ModelIndex == 3)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		//fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (ModelIndex == 4)
	{
		fin.open("..//InPut//ParadoxNet//Para.txt");
	}
	else if (ModelIndex == 5)
	{
		fin.open("..//InPut//WangNetwork//Para.txt");
	}
	else
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
	}
	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		//cout << fields[1] << endl;
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
		if (fields[0] == "MaxNumSol")	MaxNumSolEval = stoi(fields[1]);
		if (fields[0] == "StopCriteria")	StopCriteria = stoi(fields[1]);
	}
	cout << "OneDimEsp = " << OneDimEsp << endl;
	fin.close();
}

int main(int argc, char* argv[])
{
	isWriteConverge = true;
	//ModelIndex = 4; //Paradox network
	ModelIndex = 5; //Wang Zhiwei network
	OpenAndCleanFiles();
	ReadModelPara();
	ABCAlgorithms MainAlgo;
	GRAPH BaseGraph;
	BaseGraph.ReadDataMain();
	Scenario s;
	s.LinkIds.push_back(15); s.tau.push_back(0);
	BaseGraph.EvalutateFailureScenarios(s);
	cout << "BaseGraph Without Failure = " << BaseGraph.TotalSystemCost << endl;

	return 0;

	UEeps = 0.01;
	BaseGraph.EvaluteGraph();
	MainAlgo.ReadData(BaseGraph);
	UEeps = 0.01;
	MainAlgo.ABCMain();

	//ofstream RemarkFile;
	//AssertLog.open("..//OutPut//AssertLog.txt", ios::trunc);
	//RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	////if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	//UEeps = 0.01;
	//BaseGraph.EvaluteGraph();
	//cout << "BaseGraph Without Failure = " << BaseGraph.TotalSystemCost << endl;
	return 0;

}