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


int ABCAlgorithms::SelectOperIndex()
{
	if (this->SelectOp == SelectOperatorType::Uniform)
	{
		return GenRandomInt(0, NumOperators-1);
	}
	if (this->SelectOp == SelectOperatorType::ALNS)
	{
		return SelectOperator_ALNS();
	}
	TRACE("Select Operator does not return an index");
	return -999;
}
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
void ABCAlgorithms::IniOperatorProb_ANLS()
{
	for (int i = 0; i < Operators.size(); i++)
	{
		Operators.at(i).Prob = 1.0 / NumOperators;
		Operators.at(i).Weight = 1.0;
		Operators.at(i).Score = 1.0;
		Operators.at(i).TotalCounterBad = 0;
		Operators.at(i).TotalCounterGood = 0;
		Operators.at(i).TotalCounterSum = 0;
	}

}
void ABCAlgorithms::IniOperatorProb()
{
	CumProbForSelectNei.assign(NumOperators, 0.0);
	IniOperatorProb_ANLS();
}

void ABCAlgorithms::ABCMain()
{
	ofstream ConvergeFile;
	ConvergeFile.open("..//OutPut//ABCConverge.txt", ios::app);
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		GenRan.seed((unsigned)SeedVecVal.at(s));
		// start the process of one seed operation
		ConvergeMeasure.clear();
		CumProbForSelectOnlooker.assign(NumEmployedBee, 0.0);
		GlobalBest.Fitness = std::numeric_limits<double>::max();
		ScountCounter.assign(NumEmployedBee, 0);
		GenerateIni();
		IniOperatorProb();

		for (int i = 0; i < MaxIter; i++)
		{
			if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorProb();

			cout << "---------------------ABC iter = " << i << "--------------" << endl;
			EmployBeePhase();
			cout << "---------------------ABC iter" << i << " complete employed bee phase" << endl;
			GetProb();
			cout << "---------------------ABC iter" << i << " complete get prob" << endl;
			OnlookerPhase();
			cout << "---------------------ABC iter" << i << " complete on looker phase" << endl;
			ScoutPhase();
			cout << "---------------------ABC iter" << i << " complete on scout phase" << endl;
			//ConvergeMeasure.at(i) = GlobalBest.Fitness;
			ConvergeMeasure.push_back(GlobalBest.Fitness);
			if (ConvergeMeasure.back() > std::numeric_limits<double>::max() - 1)
			{
				cout << "wtf: can not find a feasible?" << endl;
				system("Pause");
			}
			if (isWriteConverge)
			{
				ConvergeFile << s << "," << i << "," << fixed << setprecision(2) << ConvergeMeasure.back() << endl;
			}
			if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorWeight();
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
#ifdef _DEBUG
		cout << "----------Print solution after solution alignment--------" << endl;
		Sols.back().print();
		cout << "----------End print solution after solution alignment--------" << endl;
#endif // _DEBUG
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
		if (Sols.back().Fitness < GlobalBest.Fitness)
		{
			GlobalBest = Sols.back();
		}
	}
	//for (auto s : Sols) cout << s.ID << "," << s.Fitness << endl;
}

void ABCAlgorithms::UpdateOperatorMeaures(int _id, bool isImproved)
{
	//TODO: write the update the counters of the operators
	assert(_id <= NumOperators-1);
	assert(_id >= 0);
	if (isImproved)
	{
		this->Operators.at(_id).TotalCounterGood++;
	}
	else
	{
		this->Operators.at(_id).TotalCounterBad++;
	}
	this->Operators.at(_id).TotalCounterSum++;
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
#ifdef _DEBUG
		cout << rhs.Fitness << " is less than" << lhs.Fitness << endl;
		rhs.print();
#endif // _DEBUG
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
		int OpId = SelectOperIndex();
		this->Sols.at(i).GenNei(Nei, *Graph, OpId, FailureLinks, ResourceCap);
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, this->Sols.at(i).Fitness, GlobalBest.Fitness);
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
		size_t Selected = SelectOnLookerBasedonProb();
		cout << "******Selected Bee = " << Selected << "**************" << endl;
		SCHCLASS Nei(this->Sols.at(Selected));
		int OpId = SelectOperIndex();
		this->Sols.at(Selected).GenNei(Nei, *Graph, OpId, FailureLinks, ResourceCap);
		bool isImproved = false;
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, this->Sols.at(SelectOp).Fitness, GlobalBest.Fitness);
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
	CumProbForSelectOnlooker.at(0) = 0.0;
	for (size_t i = 0; i < NumEmployedBee - 1; i++)
	{
		CumProbForSelectOnlooker.at(i + 1) = CumProbForSelectOnlooker.at(i) + this->Sols.at(i).Fitness / sumFit;
	}
}

void ABCAlgorithms::UpdateOperatorProb()
{
	UpdateOperatorProb_ALNS();
}

void ABCAlgorithms::UpdateOperatorProb_ALNS()
{
	double sumWeight = 0.0;
	for (auto o : Operators) sumWeight += o.Weight;
	assert(sumWeight > 0);
	for (int i = 0; i < Operators.size(); i++)
	{
		Operators.at(i).Prob = Operators.at(i).Weight / sumWeight;
	}
	CumProbForSelectNei.at(0) = 0.0;
	for (size_t i = 0; i < Operators.size()-1; i++)
	{
		CumProbForSelectNei.at(i + 1) = CumProbForSelectNei.at(i) + Operators.at(i).Prob;
	}
}

int RouletteSelect(vector<double> cumProb)
{
	int selected = -1;
	double f = GenRandomReal();
	for (size_t i = 0; i < cumProb.size() - 1; i++)
	{
		if (f >= cumProb.at(i) && f < cumProb.at(i + 1))
		{
			selected = static_cast<int>(i);
			break;
		}
	}
	if (f >= cumProb.back()) selected = static_cast<int>(cumProb.size() - 1);
	if (selected < 0)
	{
		cout << f << endl;
		for (auto v : cumProb) cout << v << endl;
		cout << "wtf" << endl;
	}
	assert(selected >= 0);
	return selected;
}

int ABCAlgorithms::SelectOperator_ALNS()
{
	return RouletteSelect(CumProbForSelectNei);
}

void ABCAlgorithms::UpdateOperatorScore(int OpId, double ResultFit, double LocalFit, double GlobalFit)
{
	UpdateOperatorScore_ALNS(OpId, ResultFit, LocalFit, GlobalFit);
}

size_t ABCAlgorithms::SelectOnLookerBasedonProb()
{
	return RouletteSelect(CumProbForSelectOnlooker);
	//size_t selected = -1;
	//double f = GenRandomReal();
	//for (size_t i = 0; i < CumProbForSelectOnlooker.size() - 1; i++)
	//{
	//	if (f >= CumProbForSelectOnlooker.at(i) && f < CumProbForSelectOnlooker.at(i + 1))
	//	{
	//		selected = static_cast<int>(i);
	//		break;
	//	}
	//}
	//if (f >= CumProbForSelectOnlooker.back()) selected = CumProbForSelectOnlooker.size() - 1;
	//assert(selected >= 0);
	//return selected;
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
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		fabc.open("..//InPut//SiouxFallsNetwork//ABCPara.txt");
		fl.open("..//InPut//SiouxFallsNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//SiouxFallsNetwork//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
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
		if (fields[0] == "RewardImproveGlobal") RewardImproveGlobal = stof(fields[1]);
		if (fields[0] == "RewardImproveLocal") RewardImproveLocal = stof(fields[1]);
		if (fields[0] == "RewardWorse") RewardWorse = stof(fields[1]);
		if (fields[0] == "ReactionFactor") ReactionFactor = stof(fields[1]);
		if (fields[0] == "SelectOperator")
		{
			if (fields[1]._Equal("ALNS"))
				SelectOp = SelectOperatorType::ALNS;
			if (fields[1]._Equal("Uni"))
				SelectOp = SelectOperatorType::Uniform;
		}
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
	fout << "RewardImproveGlobal" << "," << RewardImproveGlobal << endl;
	fout << "RewardImproveLocal" << "," << RewardImproveLocal << endl;
	fout << "RewardWorse" << ","<<RewardWorse << endl;;
	fout << "ReactionFactor," << ReactionFactor << endl;
	if (SelectOp == SelectOperatorType::ALNS)
	{
		fout << "SelectOperator,ANLS" << endl;
	}
	else if (SelectOp == SelectOperatorType::Uniform)
	{
		fout << "SelectOperator,Uni" << endl;
	}
	else
	{
		cout << "Warning: SelectOp does not properly" << endl;
	}
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
/// <summary>
/// update the score of an operator
/// </summary>
/// <param name="OpId">Id of the operator</param>
/// <param name="ImprovedFit"> if negative, then it is an improvement</param>
void ABCAlgorithms::UpdateOperatorScore_ALNS(int OpId, double ResultFit, double LocalFit, double GlobalFit)
{
	//TODO: write function to read reward 
	//      change the prob for selecting nei
	//      write python to read paramemter 
	//      and select which nei select method is used 
	assert(OpId >= 0); assert(OpId <= NumOperators-1);
	if (ResultFit<GlobalFit)
	{
		Operators.at(OpId).Score += RewardImproveGlobal;
		return;
	}
	if (ResultFit < LocalFit)
	{
		Operators.at(OpId).Score += RewardImproveLocal;
	}
	else
	{
		Operators.at(OpId).Score += RewardWorse;
	}

}
OperatorClass::OperatorClass()
{
	id = -1; TotalCounterSum = 0; TotalCounterBad = 0; TotalCounterGood = 0;
	Score = 1; Prob = 1.0/NumOperators;
	Weight = 1;
}
OperatorClass::~OperatorClass()
{
	id = -1; TotalCounterSum = 0; TotalCounterBad = 0; TotalCounterGood = 0;
	Score = 1; Prob = 1.0/NumOperators;
	Weight = 1;
}
/// <summary>
/// cal the weight 
/// </summary>
void OperatorClass::calWeight(double r)
{
	if (TotalCounterSum > 0)
	{
		Weight = (1 - r) * Weight + r * Score / TotalCounterSum;
	}

}
/// <summary>
/// update the weight of all operators
/// </summary>
void ABCAlgorithms::UpdateOperatorWeight_ALNS()
{
	for (int i = 0; i < Operators.size(); i++)
	{
		Operators.at(i).calWeight(ReactionFactor);
	}
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
		assert(Operators.at(i).TotalCounterSum > 0);
		OutFile << seedid<< ",";
		OutFile << i<< ",";
		OutFile << Operators.at(i).TotalCounterGood << ",";
		OutFile << Operators.at(i).TotalCounterBad << ",";
		OutFile << Operators.at(i).TotalCounterSum << ",";
		OutFile << double(Operators.at(i).TotalCounterGood)/ double(Operators.at(i).TotalCounterSum) << ",";
		OutFile << double(Operators.at(i).TotalCounterBad)/double(Operators.at(i).TotalCounterSum)<<",";
		OutFile << Operators.at(i).Prob <<",";
		OutFile << Operators.at(i).Score << ",";
		OutFile << Operators.at(i).Weight;
		OutFile << endl;
	}
}

void ABCAlgorithms::UpdateOperatorWeight()
{
	UpdateOperatorWeight_ALNS();
}
