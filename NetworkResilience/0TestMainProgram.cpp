#include "CommonHeaders.h"
#include "ABC.h"
using namespace std;
void OpenAndCleanFiles();
//bool ReadModelParas();
void CloseFiles();
void CompareBaressNet(Algorithm alg);
void TestBaraess(Algorithm alg);
//bool PrintModelParas();
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
		if (fields[0] == "oneDimEps")	oneDimEps = stof(fields[1]);
		if (fields[0] == "epsUE")	epsUE = stof(fields[1]);
		if (fields[0] == "maxIterUE")	maxIterUE = stoi(fields[1]);
		if (fields[0] == "networkIndex")	networkIndex = stoi(fields[1]);
		if (fields[0] == "SingleOpIndex") testSingleOperatorIndex = stoi(fields[1]);
		if (fields[0] == "isTestSingleOp")
		{
			if (fields[1]._Equal("True"))
			{
				isTestSingleOperator = true;
			}
			else if (fields[1]._Equal("False"))
			{
				isTestSingleOperator = false;
			}
			else
			{
				cout << "C++: Warning:isTestSingleOperator is not read properly" << endl;
			}
		}
		if (fields[0] == "isUseMyOwn")
		{
			if (fields[1] == "True")
			{
				isUseMyOwnAlgo = true;
			}
			else if (fields[1] == "False")
			{
				isUseMyOwnAlgo = false;
			}
		}
	}
	fin_Model.close();

	if (networkIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//MediumNetwork//Para.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (networkIndex == 2)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (networkIndex == 3)
	{
		fin_Net.open("..//InPut//SiouxFallsNetwork//NetPara.txt");
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		//fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		//fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (networkIndex == 4)
	{
		fin_Net.open("..//InPut//ParadoxNet//NetPara.txt");
	}
	else if (networkIndex == 5)
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
		if (fields[0] == "NumNodes")	numNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	numOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	numLinks = stoi(fields[1]);
	}
	fin_Net.close();
	//bool flag = PrintModelParas();
}
/// <summary>
/// Code to reproduce Wang's results
/// </summary>
/// <param name="g"></param>
void ReproduceWang(GraphClass& g)
{
	if (networkIndex != 5)
		cout << "Warning: reproduce Wang's work need to set the model index to be 5" << endl;
	cout << "--Start Reproduce David Work--- " << endl;
	ScenarioClass s;
	s.LinkIds.push_back(15); s.tau.push_back(0);
	epsUE = 0.0000000001;
	zero = 1.0e-16f;
	maxIterUE = 5000;
	g.EvalutateFailureScenarios(s);
	cout << "Total Cost = " << g.TotalSystemCost << endl;
	cout << "--Complete Reproduce David Work--- " << endl;
}

int main(int argc, char* argv[])
{	
	//TOOD: Change networkIndex to NetIndex
	/* remarks on the network model index
	* networkIndex = 3;  //Sioux Fall Network
	  networkIndex = 4; //Paradox network
	  networkIndex = 5; //Wang David network
	*/
	//zero = 1.0e-16f;
	cout << "Remarks: Maximum Restore Periods is " << MAX_NUM_OF_SCH_PERIOD << endl;
	isWriteConverge = true;
	OpenAndCleanFiles();
	ReadModelPara();
#ifdef _DEBUG
	cout << "c++:Read Model Para Complete" << endl;
#endif // _DEBUG
	Algorithm MainAlgo;
	GraphClass BaseGraph;
	std::cout << "# start to read graph data" << endl;
	BaseGraph.ReadDataMain();
	std::cout << "# complete read graph data" << endl;
	std::cout << "# start to read algorithm data" << endl;
	//contains all the ini procedure for the algorithm
	MainAlgo.Ini(BaseGraph);
	std::cout << "# complete read graph data" << endl;
	//MainAlgo.ComputeFailureLinkEI();
	//MainAlgo.printLinkEI();
	//TestBaraess(MainAlgo);
	//return 0;
	//BaseGraph.EvaluteGraph();

	// this used for evaluating the method when using python
	if (!isUseMyOwnAlgo)   
	{
		vector<int> InputVec;
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				InputVec.push_back(MainAlgo.SetOfFailureLinks.at(atoi(argv[i])));
			}
		}
		if (isUseMyOwnAlgo)
		{
			std::cout << "c++: Warning: Use My Own Algorithm is not well defined" << endl;
		}
		MainAlgo.ReadSolAndEvaluate(InputVec, BaseGraph);
	}
	else
	{
		zero = 1.0e-6f;
		MainAlgo.name = "ABC";
		//ReproduceWang(BaseGraph); // This function is for reproducing Wang's work
		MainAlgo.ABCMain();
	}
	CloseFiles();
	return 0;
}