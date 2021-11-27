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
void ReproduceWang(GRAPH& g)
{
	cout << "--Start Reproduce David Work--- " << endl;
	Scenario s;
	s.LinkIds.push_back(15); s.tau.push_back(0);
	UEeps = 0.0000000001;
	Zero = 1.0e-16f;
	UEmaxIter = 5000;
	g.EvalutateFailureScenarios(s);
	cout << "Total Cost = " << g.TotalSystemCost << endl;
	cout << "--Complete Reproduce David Work--- " << endl;
}
int main(int argc, char* argv[])
{
	wtf = false;
	isWriteConverge = true;
	ModelIndex = 4; //Paradox network
	//ModelIndex = 5; //Wang David network
	OpenAndCleanFiles();
	ReadModelPara();
	ABCAlgorithms MainAlgo;
	GRAPH BaseGraph;
	cout << "# start to read graph data" << endl;
	BaseGraph.ReadDataMain();
	cout << "# complete read graph data" << endl;
	cout << "# start to read algo data" << endl;
	MainAlgo.ReadData(BaseGraph);
	cout << "# complete read graph data" << endl;
	UEeps = 0.0001;
	Zero = 1.0e-6f;
	UEmaxIter = 500;
	//ReproduceWang(BaseGraph);
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