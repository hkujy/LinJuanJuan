#include "CommonHeaders.h"
#include "RestoreSchClass.h"
#include "ABC.h"
#include <string>
using namespace std;

//Compute the ranking of different failure links
void ComputeBaressNetUnpm(Algorithm alg)
{
	ofstream  OutFile;
	epsUE = 1.0e-10f;
	zero = 1.0e-26f;
	maxIterUE = 5000;
	OutFile.open("..//OutPut//BaressNetUnpm.txt", ios::trunc);
	OutFile << "Link,Unpm,Impact,Cost,CostIncr,UEcost" << endl;
	alg.Graph->EvaluteGraph();
	double BaseCost = alg.Graph->TotalSystemCost;
	double BaseUnpm = alg.Graph->UNPM;
	ScenarioClass s;


	for (int l = 0; l < alg.SetOfFailureLinks.size(); l++)
	{
		int linkId = alg.SetOfFailureLinks.at(l);
		//if (linkId!=5)continue;
		s.LinkIds.push_back(linkId); s.tau.push_back(0);
		alg.Graph->EvalutateFailureScenarios(s);
		alg.Graph->Links.at(linkId).IniCap();
		OutFile << alg.SetOfFailureLinks.at(l) << ",";
		OutFile << alg.Graph->UNPM << ",";
		OutFile << (BaseUnpm - alg.Graph->UNPM) / BaseUnpm << ",";
		OutFile << alg.Graph->TotalSystemCost << ",";
		OutFile << alg.Graph->TotalSystemCost - BaseCost << ",";
		OutFile << alg.Graph->OdPairs.at(0).MinCost;
		OutFile << endl;
		s.LinkIds.clear(); s.tau.clear();
	}
	OutFile << "Base Cost = " << BaseCost << endl;
	OutFile << "Base UNPM =" << BaseUnpm << endl;
	OutFile.close();
}
// check the combination of solution scenarios 
void CheckReadSolScenario_2_period(Algorithm alg)
{
	epsUE = 1.0e-10f;
	zero = 1.0e-20f;
	maxIterUE = 1000;

	alg.Graph->EvaluteGraph();
	double BaseCost = alg.Graph->TotalSystemCost;
	double BaseUnpm = alg.Graph->UNPM;

	// Step 1: read data from check file 
	ifstream fin;
	ofstream fout;
	fout.open("..//OutPut//CheckSol.txt", ios::trunc);
	fout << "Num,Cost" << endl;
	fin.open("..//InPut//ParadoxNet//CheckSol.txt");
	
	cout << "Warning and Remark: Need to check whether each link needs 1 or 2 period" << endl;

	vector<int> lv(5,-1);
	vector<int> st(5,-1);
	vector<int> et(5,-1);
	int SolNumCounter = 0;
	//Sols.push_back(SCHCLASS(i));
	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 10) continue;
		for (int i = 0; i < 5; i++) {
			lv.at(i) = stoi(fields[i]);
			st.at(i) = static_cast<int>(stoi(fields[i + 5]))-1;
			et.at(i) = st.at(i) + 2;
		}
		alg.Sols.push_back(SCHCLASS());
		for (int i = 0; i < 5; i++)
		{
			//alg.Sols.back().LinkID.push_back(&alg.Graph->Links.at(lv.at(i)));
			alg.Sols.back().LinkID.push_back(lv.at(i));
			alg.Sols.back().StartTime.push_back(st.at(i));
			alg.Sols.back().EndTime.push_back(et.at(i));
		}

		alg.Sols.back().Evaluate(*alg.Graph);
		fout << SolNumCounter << "," << alg.Sols.back().Fitness << endl;
		alg.PrintFinal(SolNumCounter);
		SolNumCounter++;
		alg.Sols.clear();
	}
	cout << alg.Graph->TotalSystemCost << endl;
	fout.close();
	fin.close();
}
void CheckReadSolScenario_1_period(Algorithm alg)
{
	epsUE = 1.0e-10f;
	zero = 1.0e-20f;
	maxIterUE = 1000;

	alg.Graph->EvaluteGraph();
	double BaseCost = alg.Graph->TotalSystemCost;
	double BaseUnpm = alg.Graph->UNPM;

	// Step 1: read data from check file 
	ifstream fin;
	ofstream fout;
	fout.open("..//OutPut//CheckSol.txt", ios::trunc);
	fout << "Num,Cost" << endl;
	fin.open("..//InPut//ParadoxNet//CheckSol.txt");

	cout << "Warning and Remark: Need to check whether each link needs 1 or 2 period" << endl;

	vector<int> lv(5, -1);
	vector<int> st(5, -1);
	vector<int> et(5, -1);
	int SolNumCounter = 0;
	//Sols.push_back(SCHCLASS(i));
	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 10) continue;
		for (int i = 0; i < 5; i++) {
			lv.at(i) = stoi(fields[i]);
			st.at(i) = static_cast<int>(stoi(fields[i + 5])) - 1;
			et.at(i) = st.at(i) + 1;
		}
		alg.Sols.push_back(SCHCLASS());
		for (int i = 0; i < 5; i++)
		{
			//alg.Sols.back().LinkID.push_back(&alg.Graph->Links.at(lv.at(i)));
			alg.Sols.back().LinkID.push_back(lv.at(i));
			alg.Sols.back().StartTime.push_back(st.at(i));
			alg.Sols.back().EndTime.push_back(et.at(i));
		}

		alg.Sols.back().Evaluate(*alg.Graph);
		fout << SolNumCounter << "," << alg.Sols.back().Fitness << endl;
		alg.PrintFinal(SolNumCounter);
		SolNumCounter++;
		alg.Sols.clear();
	}
	cout << alg.Graph->TotalSystemCost << endl;
	fout.close();
	fin.close();
}
// compare two solutions, one is based on ranking 
// the other is based on the optimal solution
void CompareBaressNet(Algorithm alg)
{
	ofstream  OutFile;
	epsUE = 1.0e-10f;
	zero = 1.0e-20f;
	maxIterUE = 1000;
	OutFile.open("..//OutPut//CompareBaressNet.txt", ios::trunc);
	OutFile << "Link,Unpm,Impact,Cost,CostIncr" << endl;
	alg.Graph->EvaluteGraph();
	alg.Sols.push_back(SCHCLASS());
	alg.Sols.back().LinkID.push_back(8);
	alg.Sols.back().LinkID.push_back(3);
	alg.Sols.back().LinkID.push_back(5);
	alg.Sols.back().LinkID.push_back(7);
	alg.Sols.back().LinkID.push_back(6);
	vector<int> st = { 1,2,3,4,5 };
	for (int i = 0; i < 5; i++)
	{
		alg.Sols.back().StartTime.push_back(st.at(i) - 1);
		alg.Sols.back().EndTime.push_back(st.at(i) - 1 + 1);
	}
	alg.Sols.back().Evaluate(*alg.Graph);

	alg.GlobalBest = alg.Sols.back();
	alg.PrintFinal(0);
	alg.Graph->PrintLinks_onscreen();
	OutFile << "Seq:8,5,3,7,6, total cost = " << alg.Sols.back().Fitness << endl;

	alg.Sols.push_back(SCHCLASS());
	alg.Sols.back().LinkID.push_back(5);
	alg.Sols.back().LinkID.push_back(8);
	alg.Sols.back().LinkID.push_back(7);
	alg.Sols.back().LinkID.push_back(3);
	alg.Sols.back().LinkID.push_back(6);
	st = { 1,2,3,4,5 };
	for (int i = 0; i < 5; i++)
	{
		alg.Sols.back().StartTime.push_back(st.at(i) - 1);
		alg.Sols.back().EndTime.push_back(st.at(i) - 1 + 1);
	}
	alg.Sols.back().Evaluate(*alg.Graph);
	OutFile << "Seq:8,7,5,3,6, total cost = " << alg.Sols.back().Fitness << endl;
	alg.GlobalBest = alg.Sols.back();
	alg.PrintFinal(1);
	alg.Graph->PrintLinks_onscreen();
}
void Check2PeriodSol(Algorithm alg)
{
	//ofstream  OutFile;
	epsUE = 1.0e-10f;
	zero = 1.0e-20f;
	maxIterUE = 1000;
	//OutFile.open("..//OutPut//CompareBaressNet.txt", ios::trunc);
	//OutFile << "Link,Unpm,Impact,Cost,CostIncr" << endl;
	//alg.Graph->EvaluteGraph();
	alg.Sols.push_back(SCHCLASS());
	alg.Sols.back().LinkID.push_back(3);
	alg.Sols.back().LinkID.push_back(5);
	alg.Sols.back().LinkID.push_back(8);
	alg.Sols.back().LinkID.push_back(6);
	alg.Sols.back().LinkID.push_back(7);
	vector<int> st = { 1,1,3,5,5 };
	for (int i = 0; i < 5; i++)
	{
		alg.Sols.back().StartTime.push_back(st.at(i)-1);
		alg.Sols.back().EndTime.push_back(st.at(i)-1+2);
	}
	alg.Sols.back().Evaluate(*alg.Graph);

	alg.GlobalBest = alg.Sols.back();
	alg.PrintFinal(0);
	//alg.Sols.push_back(SCHCLASS());
	//alg.Sols.back().Links.push_back(&alg.Graph->Links.at(6));
	//alg.Sols.back().Links.push_back(&alg.Graph->Links.at(3));
	//alg.Sols.back().Links.push_back(&alg.Graph->Links.at(7));
	//alg.Sols.back().Links.push_back(&alg.Graph->Links.at(5));
	//alg.Sols.back().Links.push_back(&alg.Graph->Links.at(8));

	//st = { 1,3,3,5,5 };
	//for (int i = 0; i < 5; i++)
	//{
	//	alg.Sols.back().StartTime.push_back(st.at(i) - 1);
	//	alg.Sols.back().EndTime.push_back(st.at(i) - 1 + 2);
	//}
	//alg.Sols.back().Evaluate(*alg.Graph);
	//alg.GlobalBest = alg.Sols.back();
	//alg.PrintFinal(1);
}


void TestBaraess(Algorithm alg)
{
	ComputeBaressNetUnpm(alg);
	//CheckReadSolScenario_2_period(alg);
	//CheckReadSolScenario_1_period(alg);

	//CompareBaressNet(alg);
	//Check2PeriodSol(alg);
}
