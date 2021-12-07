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
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "NetworkIndex")	NetworkIndex = stoi(fields[1]);
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
	}
	fin_Model.close();

	if (NetworkIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//MediumNetwork//Para.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (NetworkIndex == 2)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (NetworkIndex == 3)
	{
		fin_Net.open("..//InPut//SiouxFallsNetwork//NetPara.txt");
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		//fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		//fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (NetworkIndex == 4)
	{
		fin_Net.open("..//InPut//ParadoxNet//NetPara.txt");
	}
	else if (NetworkIndex == 5)
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
		if (fields.size() != 2) continue;
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
	}
	fin_Net.close();
}
/// <summary>
/// Code to reporduce Wang's results
/// </summary>
/// <param name="g"></param>
void ReproduceWang(GRAPH& g)
{
	if (NetworkIndex != 5)
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
	//TOOD: Change NetworkIndex to NetIndex
	/* remarks on the network model index
	* NetworkIndex = 3;  //Sioux Fall Network
	  NetworkIndex = 4; //Paradox network
	  NetworkIndex = 5; //Wang David network
	*/
	Zero = 1.0e-6f;
	cout << "Remarks: Maximum Restore Periods is " << MaxNumOfSchPeriod << endl;
	isWriteConverge = true;
	OpenAndCleanFiles();
	ReadModelPara();
#ifdef _DEBUG
	cout << "c++:Read Model Para Complete" << endl;
#endif // _DEBUG
	ABCAlgorithms MainAlgo;
	GRAPH BaseGraph;
	std::cout << "# start to read graph data" << endl;
	BaseGraph.ReadDataMain();
	std::cout << "# complete read graph data" << endl;
	std::cout << "# start to read algorithm data" << endl;
	MainAlgo.ReadData(BaseGraph);
	std::cout << "# complete read graph data" << endl;
	MainAlgo.ComputeFailureLinkEI();
	MainAlgo.printLinkEI();

	//BaseGraph.EvaluteGraph();
	if (!UseMyOwnAlgo)
	{
		vector<int> InputVec;
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				InputVec.push_back(MainAlgo.setOfFailureLinks.at(atoi(argv[i])));
			}
		}
		if (UseMyOwnAlgo)
		{
			std::cout << "c++: Warning: Use My Own Algorithm is not well defined" << endl;
		}
		MainAlgo.ReadSolAndEvaluate(InputVec);
	}
	else
	{
		//ReproduceWang(BaseGraph); // This function is for reproducing Wang's work
		MainAlgo.ABCMain();
	}
	CloseFiles();
	return 0;
}