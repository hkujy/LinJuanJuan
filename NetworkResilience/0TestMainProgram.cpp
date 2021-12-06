#include "CommonHeaders.h"
#include "DefGloVar.h"
#include "RestoreSchClass.h"
#include "ABC.h"
using namespace std;
void OpenAndCleanFiles();
bool ReadModelParas();
void CloseFiles();
void ReadModelPara()
{
	ifstream fin_Model,fin_Net;
	string line;
	vector<string> fields;

	fin_Model.open("..//InPut//ModelPara.txt");
	while (getline(fin_Model, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		//cout << fields[1] << endl;
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "ModelIndex")	ModelIndex = stoi(fields[1]);
		if (fields[0] == "UseMyOwn")
		{
			if (fields[1] == "True")
			{
				UseMyOwnAlgo = true;
			}
			else if (fields[1] == "False")
			{
				UseMyOwnAlgo = false;
			}
		}
		//if (fields[0] == "MaxNumSol")	MaxNumSolEval = stoi(fields[1]);
		//if (fields[0] == "StopCriteria")	StopCriteria = stoi(fields[1]);
	}
	std::cout << "OneDimEsp = " << OneDimEsp << endl;
	fin_Model.close();

	if (ModelIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl;
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
		fin_Net.open("..//InPut//SiouxFallsNetwork//NetPara.txt");
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		//fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		//fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (ModelIndex == 4)
	{
		fin_Net.open("..//InPut//ParadoxNet//NetPara.txt");
	}
	else if (ModelIndex == 5)
	{
		fin_Net.open("..//InPut//WangNetwork//NetPara.txt");
	}
	else
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
	}

	while (getline(fin_Net, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		//cout << fields[1] << endl;
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
	}
	fin_Model.close();
}
void ReproduceWang(GRAPH& g)
{
	if (ModelIndex != 5)
		cout << "Warning: reproduce Wang's work need to set the model index to be 5" << endl;
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
	//ModelIndex = 3;  //Sioux Fall Network
	//ModelIndex = 4; //Paradox network
	//ModelIndex = 5; //Wang David network
	OpenAndCleanFiles();
	ReadModelPara();
	ABCAlgorithms MainAlgo;
	GRAPH BaseGraph;
	std::cout << "# start to read graph data" << endl;
	BaseGraph.ReadDataMain();
	std::cout << "# complete read graph data" << endl;
	std::cout << "# start to read algo data" << endl;
	MainAlgo.ReadData(BaseGraph);
	std::cout << "# complete read graph data" << endl;
	UEeps = 0.01;
	Zero = 1.0e-6f;
	UEmaxIter = 500;
	BaseGraph.EvaluteGraph();
	if (!UseMyOwnAlgo)
	{
		vector<int> InputVec;
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				//cout<< "i="<<i<<","<<atoi(argv[i]) << endl;
				InputVec.push_back(MainAlgo.FailureLinks.at(atoi(argv[i])));
			}
		}
		if (UseMyOwnAlgo)
		{
			std::cout << "c++: Warning: Use My Own Algo is not well defined" << endl;
		}
		MainAlgo.ReadSolAndEvaluate(InputVec);
	}
	else
	{
		//ReproduceWang(BaseGraph); // This function is for reproducing Wang's work
		MainAlgo.ABCMain();
	}

	//ofstream RemarkFile;
	//RemarkFile.open("..//OutPut//TestRemark.txt", ios::trunc);
	////if (!ReadModelParas()) cerr << "Read Model Fails" << endl;// Must before 
	//UEeps = 0.01;
	//BaseGraph.EvaluteGraph();
	//cout << "BaseGraph Without Failure = " << BaseGraph.TotalSystemCost << endl;
	return 0;
}