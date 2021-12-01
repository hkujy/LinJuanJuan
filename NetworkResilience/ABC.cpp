#include "CommonHeaders.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "ABC.h"
#include <assert.h>     /* assert */
#include <string>
using namespace std;

bool ReadSeedVec(std::vector<int>& SeedVec,
	FILE* fin) {
	int SeedValue;
	SeedVec.clear();
	if (nullptr == fin) {
		perror("Read Seed File Fails \n");
		return false;
	}
	else
	{
		while (!feof(fin))
		{
			fscanf_s(fin, "%i",
				&SeedValue);
			if (SeedValue != EOF) {
				SeedVec.push_back(SeedValue);
			}
		}
	}
	return true;
}

void ABCAlgorithms::ABCMain()
{
	ofstream ConvergeFile;
	ConvergeFile.open("..//OutPut//ABCConverge.txt", ios::app);
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		GenRan.seed((unsigned)SeedVecVal.at(s));
		// start the process of one seed operation
		GenerateIni();
		ConvergeMeasure.assign(MaxIter, -1);
		Prob.assign(NumEmployedBee, 0.0);
		GlobalBest.Fitness = std::numeric_limits<double>::max();
		ScountCounter.assign(NumEmployedBee, 0);
		for (int i = 0; i < MaxIter; i++)
		{
			cout << "---------------------ABC iter = " << i << "--------------" << endl;
			EmployBeePhase();
			cout << "---------------------ABC iter" << i << " complete employed bee phase" << endl;
			GetProb();
			cout << "---------------------ABC iter" << i << " complete get prob" << endl;
			OnlookerPhase();
			cout << "---------------------ABC iter" << i << " complete on looker phase" << endl;
			ScoutPhase();
			cout << "---------------------ABC iter" << i << " complete on scout phase" << endl;
			ConvergeMeasure.at(i) = GlobalBest.Fitness;
			if (isWriteConverge)
			{
				ConvergeFile << s << "," << i << "," << fixed << setprecision(2) << ConvergeMeasure.at(i) << endl;
			}
		}
		this->PrintFinal(s);
		PrintOperator(s);
	}

	cout << "*************************ABC completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
}

void ABCAlgorithms::GenerateIni()
{
	// improve the solutions
	for (int i = 0; i < NumEmployedBee; i++)
	{
		cout << "-----------------------Generate Ini Sol= " << i << "-------------------" << endl;
		//SCHCLASS news;
		Sols.push_back(SCHCLASS(i));
		Sols.back().GenerateIniSch(*Graph, FailureLinks);
		Sols.back().AlignStartTime(ResourceCap);
		cout << "----------Print solution after solution alignment--------" << endl;
		Sols.back().print();
		cout << "----------End print solution after solution alignment--------" << endl;
		// Wang links
		//Sols.back().Links.at(0) = &Graph->Links.at(15);
		//Sols.back().Links.at(1) = &Graph->Links.at(2);
		//Sols.back().Links.at(2) = &Graph->Links.at(8);
		//Sols.back().Links.at(3) = &Graph->Links.at(12);
		//Sols.back().Links.at(4) = &Graph->Links.at(7);
		//Sols.back().Links.at(0) = &Graph->Links.at(8);
		//Sols.back().Links.at(1) = &Graph->Links.at(7);
		//Sols.back().Links.at(2) = &Graph->Links.at(5);
		//Sols.back().Links.at(3) = &Graph->Links.at(3);
		//Sols.back().Links.at(4) = &Graph->Links.at(6);
	/*	Sols.back().GenerateTimeFromOrder(ResourceCap);
		Sols.back().Evaluate(*Graph);
		cout << Sols.back().Fitness << endl;
		Sols.back().Links.at(0) = &Graph->Links.at(8);
		Sols.back().Links.at(1) = &Graph->Links.at(5);
		Sols.back().Links.at(2) = &Graph->Links.at(3);
		Sols.back().Links.at(3) = &Graph->Links.at(7);
		Sols.back().Links.at(4) = &Graph->Links.at(6);
		Sols.back().GenerateTimeFromOrder(ResourceCap);*/
		Sols.back().Evaluate(*Graph);
		cout << Sols.back().Fitness << endl;
		if (Sols.back().Fitness < GlobalBest.Fitness)
		{
			GlobalBest = Sols.back();
		}
	}
	for (auto s : Sols) cout << s.ID << "," << s.Fitness << endl;
}

void ABCAlgorithms::UpdateOperatorMeaures(int _id, bool isImproved)
{
	//TODO: write the update the counters of the operators
	assert(_id <= NumOperators);
	assert(_id >= 1);
	if (isImproved)
	{
		this->Operators.at(_id - 1).CounterGood++;
	}
	else
	{
		this->Operators.at(_id - 1).CounterBad++;
	}
	this->Operators.at(_id - 1).CounterSum++;
}

bool ABCAlgorithms::CompareTwoSolsAndReplace(SCHCLASS& lhs, SCHCLASS& rhs, int NeiOperatorId)
{
	//Compare the left hand side and the right hand side solutions 
	//if the right hand side is better 
	//then replace the lhs solution with the right hand solutions 
	//return true, if the rhs hand side is better
	bool isBetter = false;
	if (rhs.Fitness < lhs.Fitness)
	{
		cout << rhs.Fitness << " is less than" << lhs.Fitness << endl;
		rhs.print();
		lhs = rhs;
		isBetter = true;
		if (rhs.Fitness < GlobalBest.Fitness) GlobalBest = rhs;
	}

	return isBetter;

}

void ABCAlgorithms::EmployBeePhase()
{
	for (int i = 0; i < NumEmployedBee; i++)
	{
		cout << "******Employed Bee = " << i << "**************" << endl;
		SCHCLASS Nei(this->Sols.at(i));
		bool isImproved = false;
		cout << "Eb = " << i << endl;
		//if (i == 2)
			cout << "wtf" << endl;
		int OpId = -1;
		this->Sols.at(i).GenNei(Nei, *Graph, OpId, FailureLinks, ResourceCap);
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(i), Nei, OpId);
		if (isImproved) ScountCounter.at(i) = 0;
		else ScountCounter.at(i)++;
		UpdateOperatorMeaures(OpId, isImproved);
	}
}

void ABCAlgorithms::OnlookerPhase()
{
	for (int i = 0; i < NumOnlookers; i++)
	{
		cout << "******Onlooker Bee = " << i << "**************" << endl;
		size_t Selected = Select_Basedon_Prob();
		cout << "******Selected Bee = " << Selected << "**************" << endl;
		//if (i == 4 && Selected == 2)
		//{
		//	cout << "wtf" << endl;
		//	wtf = true;
		//}
		SCHCLASS Nei(this->Sols.at(Selected));
		int OpId = -1;
		this->Sols.at(Selected).GenNei(Nei, *Graph, OpId, FailureLinks, ResourceCap);
		bool isImproved = false;
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(Selected), Nei, OpId);
		UpdateOperatorMeaures(OpId, isImproved);
	}
}

void ABCAlgorithms::ScoutPhase()
{
	for (size_t t = 0; t < NumEmployedBee; t++)
	{
		if (ScountCounter.at(t) > MaxScountCount)
		{

			cout << "******Scout selected employed bee = " << t << "**************" << endl;
			this->Sols.at(t).GenerateIniSch(*Graph, FailureLinks);
			this->Sols.at(t).AlignStartTime(ResourceCap);
			this->Sols.at(t).Evaluate(*Graph);
			if (this->Sols.at(t).Fitness < GlobalBest.Fitness)
			{
				GlobalBest = this->Sols.at(t);
			}
			ScountCounter.at(t) = 0;
		}
	}
}

//get the fitness probability vector
void ABCAlgorithms::GetProb()  // compute probability based on the fitness values
{
	double sumFit = 0.0;
	for (auto s : Sols) sumFit += s.Fitness;
	assert(sumFit > 0);
	Prob.at(0) = 0.0;
	for (size_t i = 0; i < NumEmployedBee - 1; i++)
	{
		Prob.at(i + 1) = Prob.at(i) + this->Sols.at(i).Fitness / sumFit;
	}
}

size_t ABCAlgorithms::Select_Basedon_Prob()
{
	size_t selected = -1;
	double f = GenRandomReal();
	for (size_t i = 0; i < Prob.size() - 1; i++)
	{
		if (f >= Prob.at(i) && f < Prob.at(i + 1))
		{
			selected = static_cast<int>(i);
			break;
		}
	}
	if (f >= Prob.back()) selected = Prob.size() - 1;
	assert(selected >= 0);
	return selected;
}

void ABCAlgorithms::ReadData(GRAPH& g)
{
	this->Graph = &g;

	ifstream fin, fabc, fl;
	FILE* fseedin;
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
		fabc.open("..//InPut//ParadoxNet//ABCPara.txt");
		fl.open("..//InPut//ParadoxNet//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//ParadoxNet//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else if (ModelIndex == 5)
	{
		fabc.open("..//InPut//WangNetwork//ABCPara.txt");
		fl.open("..//InPut//WangNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//WangNetwork//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
	}

	cout << "Complte open files" << endl;
	cout << "Start to read para" << endl;
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


	cout << "complete to read para" << endl;
	cout << "start to read abc para" << endl;
	while (getline(fabc, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		if (fields[0] == "NumEmployBee")	NumEmployedBee = stoi(fields[1]);
		if (fields[0] == "NumOnlookerBee")	NumOnlookers = stoi(fields[1]);
		if (fields[0] == "MaxScountCount")	MaxScountCount = stoi(fields[1]);
		if (fields[0] == "MaxABCIter")	MaxIter = stoi(fields[1]);
	}
	fabc.close();
	cout << "complete read abc para" << endl;
	cout << "Start to read failure link data" << endl;
	while (getline(fl, line))
	{
		splitf(fields, line, ",");
		if (fields.size() == 3)
		{
			int linkId = stoi(fields[0]);
			int length = stoi(fields[1]);
			int res = stoi(fields[2]);
			FailureLinks.push_back(linkId);
			(*Graph).Links.at(linkId).RecoverTime = length;
			(*Graph).Links.at(linkId).RequiredRes = res;
		}
		else if (fields.size() == 2)
		{
			if (fields[0]._Equal("res"))
			{
				ResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
			}
			else
				cout << "ERR: Read failure links warning" << endl;
		}
		else
			cout << "ERR: Read failure links warning" << endl;

	}
	fl.close();

	cout << "compete read failure link data" << endl;

	/// <summary>
	/// print and check the model parameters input
	/// </summary>
	ofstream fout;
	fout.open("..//OutPut//ModelPara.txt");
	fout << "NumNodes" << "," << NumNodes << endl;
	fout << "NumOD" << "," << NumOD << endl;
	fout << "NumLinks" << "," << NumLinks << endl;
	fout << "StopCriteria" << "," << StopCriteria << endl;
	fout << "MaxNumSol" << "," << MaxNumSolEval << endl;
	fout << "OneDimEsp" << "," << OneDimEsp << endl;
	fout << "UEmaxIter" << "," << UEmaxIter << endl;
	fout << "NumEmployBee" << "," << NumEmployedBee << endl;
	fout << "NumOnlookerBee" << "," << NumOnlookers << endl;
	fout << "MaxScountCount" << "," << MaxScountCount << endl;
	fout << "MaxABCIter" << "," << MaxIter << endl;
	fout.close();
}


void ABCAlgorithms::PrintFinal(int sd)
{
	ofstream sf;
	sf.open("..//OutPut//PrintSols.txt", ios::app);
	for (size_t t = 0; t < GlobalBest.Links.size(); t++)
	{
		sf << sd << "," << GlobalBest.Links.at(t)->ID << "," << GlobalBest.StartTime.at(t) << "," <<
			GlobalBest.EndTime.at(t) << endl;
	}
	sf.close();


	sf.open("..//OutPut//PrintPeriod.txt", ios::app);

	for (size_t t = 0; t < GlobalBest.TravelTime.size(); t++)
	{
		sf << sd << "," << t << "," << GlobalBest.TravelTime.at(t) << endl;
	}
	sf.close();
}
OperatorClass::OperatorClass()
{
	id = -1; CounterSum = 0; CounterBad = 0; CounterGood = 0;
}
OperatorClass::~OperatorClass()
{
	id = -1; CounterSum = 0; CounterBad = 0; CounterGood = 0;
}

/// <summary>
///  print the summary of the operators
/// </summary>
void ABCAlgorithms::PrintOperator(int seedid)
{
	ofstream OutFile;
	OutFile.open("..//OutPut//OperatorsMeasure.txt", ios::app);
	for (int i = 0; i < NumOperators; ++i)
	{
		assert(Operators.at(i).CounterSum > 0);
		OutFile << seedid<< ",";
		OutFile << i<< ",";
		OutFile << Operators.at(i).CounterGood << ",";
		OutFile << Operators.at(i).CounterBad << ",";
		OutFile << Operators.at(i).CounterSum << ",";
		OutFile << double(Operators.at(i).CounterGood)/ double(Operators.at(i).CounterSum) << ",";
		OutFile << double(Operators.at(i).CounterBad)/double(Operators.at(i).CounterSum)<<endl;
	}
}
