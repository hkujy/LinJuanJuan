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
		if (fields[0] == "SingleOpIndex") TestSingleOpIndex = stoi(fields[1]);
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

void ComputeBaressNetUnpm(ABCAlgorithms alg)
{
	ofstream  OutFile;
	UEeps = 0.0001;
	Zero = 1.0e-6f;
	UEmaxIter = 1000;
	OutFile.open("..//OutPut//BaressNetUnpm.txt", ios::trunc);
	OutFile << "Link,Unpm,Impact,Cost,CostIncr,UEcost" << endl;
	alg.Graph->EvaluteGraph();
	double BaseCost = alg.Graph->TotalSystemCost;
	double BaseUnpm = alg.Graph->UNPM;
	Scenario s;


	for (int l = 0; l < alg.setOfFailureLinks.size(); l++)
	{
		int linkId = alg.setOfFailureLinks.at(l);
		//if (linkId!=5)continue;
		s.LinkIds.push_back(linkId); s.tau.push_back(0);
		alg.Graph->EvalutateFailureScenarios(s);
		alg.Graph->Links.at(linkId).IniCap();
		OutFile << alg.setOfFailureLinks.at(l) << ",";
		OutFile << alg.Graph->UNPM<< ",";
		OutFile << (BaseUnpm-alg.Graph->UNPM)/BaseUnpm<< ",";
		OutFile << alg.Graph->TotalSystemCost << ",";
		OutFile << alg.Graph->TotalSystemCost - BaseCost << ",";
		OutFile << alg.Graph->OdPairs.at(0).MinCost;
		OutFile<<endl;
		s.LinkIds.clear(); s.tau.clear();
	}
	OutFile << "Base Cost = " << BaseCost << endl;
	OutFile << "Base UNPM =" << BaseUnpm << endl;
	OutFile.close();
}


void CheckScenario(ABCAlgorithms alg)
{
	ofstream  OutFile;
	UEeps = 1.0e-6f;
	Zero = 1.0e-6f;
	UEmaxIter = 1000;
	OutFile.open("..//OutPut//checkscneario.txt", ios::trunc);
	OutFile << "Link,Unpm,Impact,Cost,CostIncr,UEcost" << endl;
	alg.Graph->EvaluteGraph();
	double BaseCost = alg.Graph->TotalSystemCost;
	double BaseUnpm = alg.Graph->UNPM;
	Scenario s;
	s.LinkIds.push_back(3); s.tau.push_back(0);
	s.LinkIds.push_back(7); s.tau.push_back(0);
	s.LinkIds.push_back(6); s.tau.push_back(0);
	alg.Graph->EvalutateFailureScenarios(s);
	cout << alg.Graph->TotalSystemCost << endl;
	//for (int l = 0; l < alg.setOfFailureLinks.size(); l++)
	//{
	//	int linkId = alg.setOfFailureLinks.at(l);
	//	//if (linkId!=5)continue;
	//	s.LinkIds.push_back(linkId); s.tau.push_back(0);
	//	alg.Graph->EvalutateFailureScenarios(s);
	//	alg.Graph->Links.at(linkId).IniCap();
	//	OutFile << alg.setOfFailureLinks.at(l) << ",";
	//	OutFile << alg.Graph->UNPM << ",";
	//	OutFile << (BaseUnpm - alg.Graph->UNPM) / BaseUnpm << ",";
	//	OutFile << alg.Graph->TotalSystemCost << ",";
	//	OutFile << alg.Graph->TotalSystemCost - BaseCost << ",";
	//	OutFile << alg.Graph->OdPairs.at(0).MinCost;
	//	OutFile << endl;
	//	s.LinkIds.clear(); s.tau.clear();
	//}
	//OutFile << "Base Cost = " << BaseCost << endl;
	//OutFile << "Base UNPM =" << BaseUnpm << endl;
	//OutFile.close();
}

void CompareBaressNet(ABCAlgorithms alg)
{
	ofstream  OutFile;
	UEeps = 1.0e-6f;
	Zero = 1.0e-6f;
	UEmaxIter = 1000;
	OutFile.open("..//OutPut//CompareBaressNet.txt", ios::trunc);
	OutFile << "Link,Unpm,Impact,Cost,CostIncr" << endl;
	alg.Graph->EvaluteGraph();
	alg.Sols.push_back(SCHCLASS());
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(8));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(5));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(3));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(7));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(6));
	alg.Sols.back().AlignStartTime(alg.setResourceCap);
	alg.Sols.back().Evaluate(*alg.Graph);

	alg.GlobalBest = alg.Sols.back();
	alg.PrintFinal(0);
	OutFile << "Seq:8,5,3,7,6, total cost = " << alg.Sols.back().Fitness << endl;

	alg.Sols.push_back(SCHCLASS());
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(8));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(7));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(5));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(3));
	alg.Sols.back().Links.push_back(&alg.Graph->Links.at(6));
	alg.Sols.back().AlignStartTime(alg.setResourceCap);
	alg.Sols.back().Evaluate(*alg.Graph);
	OutFile << "Seq:8,7,5,3,6, total cost = " << alg.Sols.back().Fitness << endl;
	alg.GlobalBest = alg.Sols.back();
	alg.PrintFinal(1);


}
void TestBaraess(ABCAlgorithms alg)
{
	cout << "Remark: To obtain results in paper from LinJuan, the parameters has be set as follows" << endl;
	cout << "UEeps = 0.0001; " << endl;
	cout << "Zero = 1.0e-6f;" << endl;
	cout << "Particularly, the value of zero has effects on the objetive value" << endl;
	UEeps = 0.0001;
	Zero = 1.0e-6f;
	UEmaxIter = 1000;

	ComputeBaressNetUnpm(alg);
	//CheckScenario(alg);
	CompareBaressNet(alg);

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
	MainAlgo.Ini(BaseGraph); //contains all the ini procedure for the algorithm
	//MainAlgo.ReadData(BaseGraph);
	std::cout << "# complete read graph data" << endl;
	//MainAlgo.ComputeFailureLinkEI();
	//MainAlgo.printLinkEI();

	TestBaraess(MainAlgo);
	return 0;

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