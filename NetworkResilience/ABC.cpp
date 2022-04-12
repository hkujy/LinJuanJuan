#include "CommonHeaders.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "ABC.h"
#include <cassert>     /* assert */
#include <string>
#include <unordered_map>
#include <iomanip>
#include "DefGloVar.h" /*all the global variables*/
using namespace std;
// read seed vector
bool ReadSeedVec(std::vector<int>& SeedVec, FILE* fin) {
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
	ofstream fout;
	fout.open("..//OutPut//CheckSeed.txt");
	for (int i = 0; i < SeedVec.size(); i++)
	{
		fout << SeedVec.at(i) << endl;
	}
	fout.close();
	return true;
}

int Algorithm::SelectOperatorIndex()
{
	//return 8;
	if (isTestSingleOperator) return testSingleOperatorIndex;
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
void Algorithm::IniOperatorProb_ANLS()
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
void Algorithm::IniOperatorProb()
{
	CumProbForSelectNei.assign(NumOperators, 0.0);
	if (SelectOp == SelectOperatorType::ALNS) IniOperatorProb_ANLS();
	else if (SelectOp == SelectOperatorType::Uniform) { ; }
	else 
	{
		std::cout << "C++ Warning: iniOperatorProb type is not defined" << endl;
	}
}

void Algorithm::ABCMain()
{
	ofstream ConvergeFile;
	ConvergeFile.open("..//OutPut//ABCConverge.txt", ios::app);
	vector<double> CpuTimes;
	vector<size_t> sizeOfarchive;
	clock_t St, Et;
	int BestSeedNum = -1;
	double BestSeedVal = std::numeric_limits<double>::max();

	for (int s = 0; s < seedVecVal.size(); s++)
	{
		St = clock();
		GenRan.seed((unsigned)seedVecVal.at(s));
		// start the process of one seed operation
		ConvergeMeasure.clear();
		CumProbForSelectOnlooker.assign(NumEmployedBee, 0.0);
		GlobalBest.Fitness = std::numeric_limits<double>::max();
		ScoutCounter.assign(NumEmployedBee, 0);
		IniOperatorProb();
		IniPattern();
		IniSolArchive();
		GenerateIniSol();

		for (int i = 0; i < MaxIter; i++)
		{
			if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorProb();
#ifdef _DEBUG
			cout << "---------------------ABC iter = " << i << "--------------" << endl;
#endif // _DEBUG
			EmployBeePhase();
#ifdef _DEBUG
			cout << "---------------------ABC iter" << i << " complete employed bee phase" << endl;
#endif // _DEBUG
			GetProb();
#ifdef _DEBUG
			cout << "---------------------ABC iter" << i << " complete get prob" << endl;
#endif // _DEBUG
			OnlookerPhase();
#ifdef _DEBUG
			cout << "---------------------ABC iter" << i << " complete on looker phase" << endl;
#endif // _DEBUG
			ScoutPhase();
#ifdef _DEBUG
			cout << "---------------------ABC iter" << i << " complete on scout phase" << endl;
#endif // _DEBUG
			//ConvergeMeasure.at(i) = GlobalBest.Fitness;
			ConvergeMeasure.push_back(GlobalBest.Fitness);
			if (isWriteConverge) ConvergeFile << s << "," << i << "," << fixed << setprecision(2) << ConvergeMeasure.back() << endl;
			if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorWeight();

			PrintOperator(s,i);
		}
		sizeOfarchive.push_back(m_str_val_solArchive.size());
		Et = clock();
		CpuTimes.push_back((double)((Et - St)/CLOCKS_PER_SEC)); // time unit is second
		PrintFinal(s); 
		printPattern(s);
		printDomRelation(s);

		if (GlobalBest.Fitness < BestSeedVal)
		{
			BestSeedVal = GlobalBest.Fitness;
			BestSeedNum = s;
		}
	}
	ofstream CpuTimeFile;
	CpuTimeFile.open("..//OutPut//CpuTime.txt", ios::trunc);
	CpuTimeFile << "Seed,Time" << endl;
	for (int s= 0;s< seedVecVal.size();s++)
	{
		CpuTimeFile << s << "," << fixed << setprecision(2) <<CpuTimes[s]<< endl;
	}
	CpuTimeFile.close();

	ofstream Archive;
	Archive.open("..//OutPut//ArchiveSize.txt", ios::trunc);
	Archive << "Seed,Size" << endl;
	for (int s = 0; s < seedVecVal.size(); s++)
	{
		Archive << s << "," << sizeOfarchive[s] << endl;
	}
	Archive.close();

	ofstream bs;
	bs.open("..//OutPut//BestSeed.txt", ios::trunc);
	bs << "Seed,Val" << endl;
	bs << BestSeedNum << "," << BestSeedVal << endl;
	bs.close();

#ifdef _DEBUG
	cout << "*************************ABC completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
#endif // _DEBUG
}

//Evaluate solution
//if it is a new solution, then add to archive 
//if not, then evaluate the solution
void Algorithm::evaluateOneSol(SCHCLASS& Sol, GraphClass& g)
{
	Sol.key.assign(getMapStrFromSol(Sol));
	if (isNeedToEvaluateSol(Sol))
	{
		Sol.Evaluate(g);
		m_str_val_solArchive.insert(std::make_pair(Sol.key, Sol.Fitness));
	}
	else
	{
		Sol.Fitness = m_str_val_solArchive[Sol.key];
	}
}

void Algorithm::GenerateIniSol()
{
	clearSols();
	// improve the solutions
	for (int i = 0; i < NumEmployedBee; i++)
	{
#ifdef _DEBUG
		cout << "-----------------------Generate Ini Sol= " << i << "-------------------" << endl;
#endif // _DEBUG
		Sols.push_back(SCHCLASS(i));
		Sols.back().GenerateIniSch(*Graph, SetOfFailureLinks);
		Sols.back().AlignStartTime(SetOfResourceCap,*Graph);
#ifdef _DEBUG
		cout << "----------Print solution after solution alignment--------" << endl;
		Sols.back().print();
		cout << "----------End Print solution after solution alignment--------" << endl;
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
	
		evaluateOneSol(Sols[i],*Graph);
		//Sols.back().Evaluate(*Graph);

		if (Sols.back().Fitness < GlobalBest.Fitness)
		{
			GlobalBest = Sols.back();
		}
	}
	//for (auto s : Sols) cout << s.Id << "," << s.Fitness << endl;
}


bool Algorithm::CompareTwoSolsAndReplace(SCHCLASS& lhs, SCHCLASS& rhs, int NeiOperatorId)
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
		if (rhs.Fitness < GlobalBest.Fitness)
		{
			//LearnPattern_Score(rhs, true);
			LearnPatternRelation_Score(rhs, true);
			GlobalBest = rhs;
		}
		else
		{
			//LearnPattern_Score(rhs, false);
			LearnPatternRelation_Score(rhs, false);
		}
	}
	return isBetter;
}

void Algorithm::EmployBeePhase()
{
	for (int i = 0; i < NumEmployedBee; i++)
	{
		SCHCLASS Nei(this->Sols.at(i));
		bool isImproved = false;
#ifdef _DEBUG
		cout << "******Employed Bee = " << i << "**************" << endl;
		cout << "Eb = " << i << endl;
#endif // _DEBUG
		//if (i == 2)
		int OpId = SelectOperatorIndex();
		this->Sols.at(i).GenNei(Nei, *Graph, OpId, SetOfFailureLinks, SetOfResourceCap,Pattern, CompareScoreMethod);
		evaluateOneSol(Nei, *Graph);
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, this->Sols.at(i).Fitness, GlobalBest.Fitness);
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(i), Nei, OpId);
		if (isImproved) ScoutCounter.at(i) = 0;
		else ScoutCounter.at(i)++;
		UpdateOperatorMeasure(OpId, isImproved);
	}
}

void Algorithm::OnlookerPhase()
{
	for (int i = 0; i < NumOnlookers; i++)
	{
		size_t Selected = SelectOnLookerBasedOnProb();
#ifdef _DEBUG
		cout << "******Onlooker Bee = " << i << "**************" << endl;
		cout << "******Selected Bee = " << Selected << "**************" << endl;
#endif // _DEBUG
		SCHCLASS Nei(this->Sols.at(Selected));
		int OpId = SelectOperatorIndex();
		this->Sols.at(Selected).GenNei(Nei, *Graph, OpId, SetOfFailureLinks, SetOfResourceCap,Pattern,CompareScoreMethod);
		evaluateOneSol(Nei, *Graph);
		bool isImproved = false;
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, 
			this->Sols.at(Selected).Fitness, GlobalBest.Fitness);
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(Selected), Nei, OpId);
		if (isImproved) ScoutCounter.at(Selected) = 0;
		else ScoutCounter.at(Selected)++;
		UpdateOperatorMeasure(OpId, isImproved);
	}
}

void Algorithm::ScoutPhase()
{
	for (size_t t = 0; t < NumEmployedBee; t++)
	{
		if (ScoutCounter.at(t) > MaxScoutCount)
		{
#ifdef _DEBUG
			cout << "******Scout selected employed bee = " << t << "**************" << endl;
#endif // _DEBUG
			this->Sols.at(t).GenerateIniSch(*Graph, SetOfFailureLinks);
			this->Sols.at(t).AlignStartTime(SetOfResourceCap,*Graph);
			evaluateOneSol(this->Sols.at(t), *Graph);
			//this->Sols.at(t).Evaluate(*Graph);
			if (this->Sols.at(t).Fitness < GlobalBest.Fitness)
			{
				GlobalBest = this->Sols.at(t);
				LearnPattern_Score(GlobalBest, true);
			}
			ScoutCounter.at(t) = 0;
		}
	}
}

//Get the fitness probability vector
void Algorithm::GetProb()  // compute probability based on the fitness values
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

void Algorithm::UpdateOperatorProb()
{
	if (SelectOp == SelectOperatorType::ALNS)
	{
		UpdateOperatorProb_ALNS();
	}
	else { std::cout << "C++ Warning: UpdateOperator Type is not specified" << endl; }
}

void Algorithm::UpdateOperatorProb_ALNS()
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
int RouletteSelect(const vector<double>& cumProb);

int Algorithm::SelectOperator_ALNS()
{
	return RouletteSelect(CumProbForSelectNei);
}

void Algorithm::UpdateOperatorScore(int opId, double resultFit, double localFit, double globalFit)
{
	UpdateOperatorScore_ALNS(opId, resultFit, localFit, globalFit);
}

size_t Algorithm::SelectOnLookerBasedOnProb()
{
	return RouletteSelect(CumProbForSelectOnlooker);
}

void Algorithm::IniPattern()
{
	if (Pattern.size() > 0) Pattern.clear();
	for (int l = 0; l < SetOfFailureLinks.size(); l++)
	{
		Pattern.push_back(PatternClass());
		Pattern.back().id = l;
		Pattern.back().LinkId = SetOfFailureLinks.at(l);
		for (int k = 0; k < SetOfFailureLinks.size(); k++)
		{
			Pattern.back().Next.push_back(SetOfFailureLinks.at(k));
		}
		Pattern.back().AbsProb.assign(SetOfFailureLinks.size(), 0.0);
		Pattern.back().AbsScore.assign(SetOfFailureLinks.size(), 1.0);

		Pattern.back().AveProb.assign(SetOfFailureLinks.size(), 0.0);
		Pattern.back().AveScore.assign(SetOfFailureLinks.size(), 1.0);
		Pattern.back().Count.assign(SetOfFailureLinks.size(), 0);

		// relationship
		int _FirstLink = SetOfFailureLinks[l];
		for (int j = 0; j < SetOfFailureLinks.size(); j++)
		{
			int _ComparedLink = SetOfFailureLinks[j];
			Pattern.back().Relation.push_back(RelationClass(_ComparedLink));
		}

		//Update the score to set the diagonal vector value = 0
		for (int i = 0; i < SetOfFailureLinks.size(); i++)
		{
			Pattern.back().AbsScore.at(l) = 0;
			Pattern.back().AveScore.at(l) = 0;
		}
	}

	for (auto& p : Pattern)
	{
		p.updateProb();
	}
}
//Ini Algorithm
void Algorithm::Ini(GraphClass& g)
{
	ReadData(g);
	for (int l = 0; l < SetOfFailureLinks.size(); l++)
	{
		Pattern.push_back(PatternClass());
		Pattern.back().id = l;
		Pattern.back().LinkId = SetOfFailureLinks.at(l);
		for (int k =0;k<SetOfFailureLinks.size();k++)
		{
			Pattern.back().Next.push_back(SetOfFailureLinks.at(k));
		}
		Pattern.back().AbsProb.assign(SetOfFailureLinks.size(), 0.0);
		Pattern.back().AbsScore.assign(SetOfFailureLinks.size(), 1.0);

		Pattern.back().AveProb.assign(SetOfFailureLinks.size(), 0.0);
		Pattern.back().AveScore.assign(SetOfFailureLinks.size(), 1.0);
		Pattern.back().Count.assign(SetOfFailureLinks.size(), 0);
			
		// TODO. update the score to set the diagonal vector value =1
		for (int i = 0; i < SetOfFailureLinks.size(); i++)
		{
			Pattern.back().AbsScore.at(l) = 0;
			Pattern.back().AveScore.at(l) = 0;
		}
	}
	for (auto& p : Pattern)
	{
		p.updateProb();
	}
}

void Algorithm::ReadData(GraphClass& g)
{
	this->Graph = &g;
	ifstream f_ABCpara, f_FailLinks;
	FILE* fseedin;
	if (networkIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//MediumNetwork//Para.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (networkIndex == 2)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (networkIndex == 3)
	{
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		f_ABCpara.open("..//InPut//SiouxFallsNetwork//ABCPara.txt");
		f_FailLinks.open("..//InPut//SiouxFallsNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(seedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else if (networkIndex == 4)
	{
		f_ABCpara.open("..//InPut//ParadoxNet//ABCPara.txt");
		f_FailLinks.open("..//InPut//ParadoxNet//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(seedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else if (networkIndex == 5)
	{
		f_ABCpara.open("..//InPut//WangNetwork//ABCPara.txt");
		f_FailLinks.open("..//InPut//WangNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(seedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
	}
	std::cout << "Complete open files" << endl;
	std::cout << "Start to read para" << endl;
	string line;
	vector<string> fields;
	while (getline(f_ABCpara, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "NumEmployBee")	NumEmployedBee = stoi(fields[1]);
		if (fields[0] == "NumOnlookerBee")	NumOnlookers = stoi(fields[1]);
		if (fields[0] == "MaxScoutCount")	MaxScoutCount = stoi(fields[1]);
		if (fields[0] == "MaxABCIter")	MaxIter = stoi(fields[1]);
		if (fields[0] == "RewardImproveGlobal") RewardImproveGlobal = stof(fields[1]);
		if (fields[0] == "RewardImproveLocal") RewardImproveLocal = stof(fields[1]);
		if (fields[0] == "RewardWorse") RewardWorse = stof(fields[1]);
		if (fields[0] == "ReactionFactor") ReactionFactor = stof(fields[1]);
		if (fields[0] == "CompareScoreMethod")
		{
			if (fields[1] == "Ave") CompareScoreMethod = enum_CompareScoreMethod::Ave;
			else if (fields[1] == "Total") CompareScoreMethod = enum_CompareScoreMethod::Total;
			else {
				TRACE("CompareScoreMethod is not read properly");
			}

		}
		if (fields[0] == "SelectOperator")
		{
			if (fields[1]._Equal("ALNS"))
				SelectOp = SelectOperatorType::ALNS;
			if (fields[1]._Equal("Uni"))
				SelectOp = SelectOperatorType::Uniform;
		}
	}
	f_ABCpara.close();
	std::cout << "complete read ABC para" << endl;
	std::cout << "Start to read failure link data" << endl;
	while (getline(f_FailLinks, line))
	{
		splitf(fields, line, ",");
		if (fields.size() == 3)
		{
			int linkId = stoi(fields[0]);
			int length = stoi(fields[1]);
			int res = stoi(fields[2]);
			SetOfFailureLinks.push_back(linkId);
			(*Graph).Links.at(linkId).RecoverTime = length;
			(*Graph).Links.at(linkId).RequiredRes = res;
		}
		else if (fields.size() == 2)
		{
			if (fields[0]._Equal("res"))
			{
				SetOfResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
			}
			else
				std::cout << "ERR: Read failure links warning" << endl;
		}
		else
			std::cout << "ERR: Read failure links warning" << endl;

	}
	f_FailLinks.close();

	std::cout << "compete read failure link data" << endl;

	/// <summary>
	/// Print and check the model parameters input
	/// </summary>
	ofstream fout;
	fout.open("..//OutPut//InputPara.txt");
	fout << "numNodes" << "," << numNodes << endl;
	fout << "numOD" << "," << numOD << endl;
	fout << "numLinks" << "," << numLinks << endl;
	fout << "oneDimEps" << "," << oneDimEps << endl;
	fout << "maxIterUE" << "," << maxIterUE << endl;
	fout << "NumEmployBee" << "," << NumEmployedBee << endl;
	fout << "NumOnlookerBee" << "," << NumOnlookers << endl;
	fout << "MaxScoutCount" << "," << MaxScoutCount << endl;
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
		std::cout << "Warning: SelectOp does not properly" << endl;
	}
	fout.close();
}


void Algorithm::PrintFinal(int sd)
{
	ofstream sf;
	if (name._Equal("ABC"))
	{
		sf.open("..//OutPut//ABCPrintSols.txt", ios::app);
	}
	else if (name._Equal("HH"))
	{
		sf.open("..//OutPut//HHPrintSols.txt", ios::app);
	}
	else
	{
		TRACE("Name of the algorithm is not specified");
	}
	for (size_t t = 0; t < GlobalBest.LinkID.size(); t++)
	{
		//sf << sd << "," << GlobalBest.LinkID.at(t)->Id << "," << GlobalBest.StartTime.at(t) << "," <<
		sf << sd << "," << GlobalBest.LinkID.at(t) << "," << GlobalBest.StartTime.at(t) << "," <<
			GlobalBest.EndTime.at(t) << endl;
	}
	sf.close();
	// Print the value of best solution
	sf.open("..//OutPut//ABCPrintSeedBestSolVal.txt", ios::app);
	sf << sd << ",";
	sf <<setiosflags(ios::fixed)<<setprecision(2)<<GlobalBest.Fitness << endl;
	sf.close();
	// Print the best solution period
	sf.open("..//OutPut//PrintPeriod.txt", ios::app);
	for (size_t t = 0; t < GlobalBest.TravelTime.size(); t++)
	{
		sf << sd << "," << t << ",";
		sf<<setiosflags(ios::fixed)<<setprecision(2)<<GlobalBest.TravelTime.at(t) << ","<<GlobalBest.UNPM.at(t)<<endl;
	}
	sf.close();
}

OperatorClass::OperatorClass():Id(-1),TotalCounterGood(0), TotalCounterBad(0), TotalCounterSum(0), 
Score(1),Prob(1.0/NumOperators),Weight(1){}
//OperatorClass::~OperatorClass()
//{
//	Id = -1; TotalCounterSum = 0; TotalCounterBad = 0; TotalCounterGood = 0;
//	Score = 1; Prob = 1.0/NumOperators;
//	Weight = 1;
//}

/// <summary>
///  Print the summary of the operators
/// </summary>
void Algorithm::PrintOperator(int seedId, int iter)
{
	ofstream OutFile;
	if (name._Equal("HH"))
	{
		OutFile.open("..//OutPut//HHOperatorsMeasure.txt", ios::app);
	}
	else if (name._Equal("ABC"))
	{
		OutFile.open("..//OutPut//ABCOperatorsMeasure.txt", ios::app);
	}
	else
	{
		TRACE("Name of the algorithm is not specified");
	}
	//OutFile << "Seed,Iter,Id,Good,Bad,Sum,Gp,Bp,Prob,Score,Weight" << endl;
	for (int i = 0; i < NumOperators; ++i)
	{
		OutFile << seedId<< ",";
		OutFile << iter << ",";
		OutFile << i<< ",";
		OutFile << Operators.at(i).TotalCounterGood << ",";
		OutFile << Operators.at(i).TotalCounterBad << ",";
		OutFile << Operators.at(i).TotalCounterSum << ",";
		OutFile << double(Operators.at(i).TotalCounterGood)/max(1.0, double(Operators.at(i).TotalCounterSum)) << ",";
		OutFile << double(Operators.at(i).TotalCounterBad)/max(1.0,double(Operators.at(i).TotalCounterSum))<<",";
		OutFile << Operators.at(i).Prob <<",";
		OutFile << Operators.at(i).Score << ",";
		OutFile << Operators.at(i).Weight;
		OutFile << endl;
	}
}

void Algorithm::UpdateOperatorWeight()
{
	if (SelectOp == SelectOperatorType::ALNS)
	{
		UpdateOperatorWeight_ALNS();
	}
	else { std::cout << "C++ Warning: Update Operator weight type is not specified" << endl; }
}

/// <summary>
/// given a solution from the exe input file then evaluate the solution
/// </summary>
/// <param name="vec"></param>
void Algorithm::ReadSolAndEvaluate(vector<int> &vec,GraphClass &g)
{
	// step 1: set the solution vector 
	SCHCLASS sol;
	sol.EndTime.assign(SetOfFailureLinks.size(), -1);
	sol.StartTime.assign(SetOfFailureLinks.size(), -1);
	//for (int l = 0; l < vec.size(); l++) sol.LinkID.push_back(&Graph->Links.at(vec[l]));
	for (int l = 0; l < vec.size(); l++) sol.LinkID.push_back(vec[l]);
	if (SetOfFailureLinks.size() != vec.size()) 
		std::cout << "c++: warning: the failure link size does not equal vec size" << endl;
	sol.AlignStartTime(SetOfResourceCap,g);
	sol.Evaluate(*Graph);
	//Remark: this is the last line of the exe, so that the results can be read from python
	//        Do not Print the "endl" at the end.
	std::cout << sol.Fitness;
}

/// <summary>
/// Compute the efficiency index for each link
/// </summary>
void Algorithm::ComputeFailureLinkEI()
{
	for (int l = 0; l < SetOfFailureLinks.size(); l++)
	{
		Graph->Links.at(SetOfFailureLinks[l]).EI = Graph->CalRelSpChange(SetOfFailureLinks[l]);
	}
}

void Algorithm::printLinkEI()
{
	ofstream  OutFile;
	OutFile.open("..//OutPut/LinkEI.txt", ios::app);
	for (int l = 0; l < SetOfFailureLinks.size(); l++)
	{
		OutFile << SetOfFailureLinks[l] << "," << Graph->Links.at(SetOfFailureLinks[l]).EI << endl;
	}
	OutFile.close();
}

//based the score value update the patten prob
void PatternClass::updateProb()
{
	// step 1: update the absolute probablity value
	assert(AbsScore.size() > 0);
	assert(AbsProb.size() > 0);
	double sum = std::accumulate(AbsScore.begin(), AbsScore.end(), 0.0);
	for (int p = 0; p < AbsProb.size(); p++)
	{
		AbsProb[p] = AbsScore[p] / sum;
	}
	// step 2: update the average probability value
	assert(AveScore.size() > 0);
	assert(AveProb.size() > 0);
	sum = std::accumulate(AveScore.begin(), AveScore.end(), 0.0);
	for (int p = 0; p < AveProb.size(); p++)
	{
		AveScore[p] = AveScore[p] / sum;
	}
}

// return the location of the vector
size_t findPatternIndex_fun(int lid, const vector<PatternClass> &Pat)
{
	assert(lid >= 0);
	for (size_t i = 0; i < Pat.size(); i++)
	{
		if (Pat[i].LinkId == lid) return i;
	}
	cout << "C++ Warning: Can not find pattern index" << endl;
	return  -1;
}

// return the location of the vector
size_t Algorithm::findPatternIndex(int lid)
{
	return findPatternIndex_fun(lid, Pattern);
}

//Print pattern on the file
void Algorithm::printPattern(int seedId)
{
	ofstream OutFile;
	if (name._Equal("ABC"))
	{
		OutFile.open("..//OutPut//ABCPrintPatternScore.txt", ios::app);
	}
	else if (name._Equal("HH"))
	{
		OutFile.open("..//OutPut//HHPrintPatternScore.txt", ios::app);
	}
	//OutFile << "Seed,First,Second,Score,Prob" << endl;
	for (auto p : Pattern)
	{
		for (int i=0;i<p.Next.size();i++)
		{
			OutFile << seedId << ",";
			OutFile << p.LinkId << ",";
			OutFile << p.Next.at(i) << ",";
			OutFile << p.AbsScore.at(i) << ",";
			OutFile << p.AbsProb.at(i) << endl;
		}
	}
	OutFile.close();
}
//void SetAlgoType(std::string _name, AlgorithmType& alot)
//{
//	//TODO
//	//	enum AlgorithmType
//	//{
//	//	CSA = 0, GA = 1, HH = 2, ABC = 3, UnDefined = 4
//	//};
//
//}
std::string getAlgoTypeName(const AlgorithmType& alot)
{
	switch (alot)
	{
	case (AlgorithmType::HH):
		return "HH"; break;
	case (AlgorithmType::ABC):
		return "ABC"; break;
	default:
		TRACE("Algorithm type is wrong input");
		break;
	}
	return "Undefined name and need to debug";
}

// this is to check whether the solution needs to be evaluated
bool Algorithm::isNeedToEvaluateSol(const SCHCLASS &sol)
{
	return isAddNewToArchive(sol.key);
}

// TODO: check whether the link is a new to the archive 
bool Algorithm::isAddNewToArchive(const string &key)
{
// step 1. get string from so
	if (m_str_val_solArchive.count(key) == 0)
		return true;
	else
		return false;
}

string Algorithm::getMapStrFromSol(const SCHCLASS &sol) //get string for the map sol archive
{
	string str_val;
	for (size_t l=0; l < sol.LinkID.size(); l++)
	{
		if (l == 0)
		{
			if (sol.LinkID[l]==0) str_val = "0";
			else str_val = std::to_string(sol.LinkID[l]);
		}
		else
		{
			if (sol.LinkID[l] == 0) str_val.append(",0");
			else
			{
				str_val.append(",");str_val.append(std::to_string(sol.LinkID[l]));
			}
		}
	}
#ifdef _DEBUG
	for (auto l : sol.LinkID)
	{
		//cout << "wtf: link Id = " << l->Id << endl;
		cout << "wtf: link Id = " << l << endl;
	}
	cout << "converted str = " << str_val << endl;
#endif
	return str_val;
}

void Algorithm::IniSolArchive()
{	
	if (m_str_val_solArchive.size() > 0)
	{
		m_str_val_solArchive.clear();
	}
}

/// <summary>
/// cal the weight 
/// </summary>
void OperatorClass::CalWeight(double r)
{
	if (TotalCounterSum > 0)
	{
		Weight = (1 - r) * Weight + r * Score / TotalCounterSum;
	}
}

