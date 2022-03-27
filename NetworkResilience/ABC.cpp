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


int Algorithm::SelectOperIndex()
{
	//return 8;
	if (isTestSingleOperator) return TestSingleOpIndex;
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

	ofstream fout;
	fout.open("..//OutPut//CheckSeed.txt");
	for (int i = 0; i < SeedVec.size(); i++)
	{
		fout << SeedVec.at(i) << endl;
	}

	fout.close();

	return true;
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
	clock_t St, Et;
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		St = clock();
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
		Et = clock();
		CpuTimes.push_back((double)((Et - St)/CLOCKS_PER_SEC)); // time unit is second
		PrintFinal(s); 
		printPattern(s);
	}
	ofstream CpuTimeFile;
	CpuTimeFile.open("..//OutPut//CpuTime.txt", ios::trunc);
	CpuTimeFile << "Seed,Time" << endl;
	for (int s= 0;s< SeedVecVal.size();s++)
	{
		CpuTimeFile << s << "," << fixed << setprecision(2) <<CpuTimes[s]<< endl;
	}
	CpuTimeFile.close();

#ifdef _DEBUG
	cout << "*************************ABC completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
#endif // _DEBUG
}

void Algorithm::GenerateIni()
{
	// improve the solutions
	for (int i = 0; i < NumEmployedBee; i++)
	{
#ifdef _DEBUG
		cout << "-----------------------Generate Ini Sol= " << i << "-------------------" << endl;
#endif // _DEBUG
		Sols.push_back(SCHCLASS(i));
		Sols.back().GenerateIniSch(*Graph, setOfFailureLinks);
		Sols.back().AlignStartTime(setResourceCap);
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

void Algorithm::UpdateOperatorMeaures(int _id, bool isImproved)
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
			GlobalBest = rhs;
			updatePatternScore(GlobalBest, true);
		}
		else
		{
			updatePatternScore(rhs, false);
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
		int OpId = SelectOperIndex();
		this->Sols.at(i).GenNei(Nei, *Graph, OpId, setOfFailureLinks, setResourceCap,Pattern);
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, this->Sols.at(i).Fitness, GlobalBest.Fitness);
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(i), Nei, OpId);
		if (isImproved) ScountCounter.at(i) = 0;
		else ScountCounter.at(i)++;
		UpdateOperatorMeaures(OpId, isImproved);
	}
}

void Algorithm::OnlookerPhase()
{
	for (int i = 0; i < NumOnlookers; i++)
	{
		size_t Selected = SelectOnLookerBasedonProb();
#ifdef _DEBUG
		cout << "******Onlooker Bee = " << i << "**************" << endl;
		cout << "******Selected Bee = " << Selected << "**************" << endl;
#endif // _DEBUG
		SCHCLASS Nei(this->Sols.at(Selected));
		int OpId = SelectOperIndex();
		this->Sols.at(Selected).GenNei(Nei, *Graph, OpId, setOfFailureLinks, setResourceCap,Pattern);
		bool isImproved = false;
		if (SelectOp!=SelectOperatorType::Uniform) UpdateOperatorScore(OpId, Nei.Fitness, this->Sols.at(SelectOp).Fitness, GlobalBest.Fitness);
		isImproved = CompareTwoSolsAndReplace(this->Sols.at(Selected), Nei, OpId);
		if (isImproved) ScountCounter.at(Selected) = 0;
		else ScountCounter.at(Selected)++;
		UpdateOperatorMeaures(OpId, isImproved);
	}
}

void Algorithm::ScoutPhase()
{
	for (size_t t = 0; t < NumEmployedBee; t++)
	{
		if (ScountCounter.at(t) > MaxScountCount)
		{
#ifdef _DEBUG
			cout << "******Scout selected employed bee = " << t << "**************" << endl;
#endif // _DEBUG
			this->Sols.at(t).GenerateIniSch(*Graph, setOfFailureLinks);
			this->Sols.at(t).AlignStartTime(setResourceCap);
			this->Sols.at(t).Evaluate(*Graph);
			if (this->Sols.at(t).Fitness < GlobalBest.Fitness)
			{
				GlobalBest = this->Sols.at(t);
				updatePatternScore(GlobalBest, true);
			}
			ScountCounter.at(t) = 0;
		}
	}
}

//get the fitness probability vector
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

void Algorithm::UpdateOperatorScore(int OpId, double ResultFit, double LocalFit, double GlobalFit)
{
	UpdateOperatorScore_ALNS(OpId, ResultFit, LocalFit, GlobalFit);

}

size_t Algorithm::SelectOnLookerBasedonProb()
{
	return RouletteSelect(CumProbForSelectOnlooker);
}


void Algorithm::Ini(GRAPH& g)
{
	ReadData(g);
	for (int l = 0; l < setOfFailureLinks.size(); l++)
	{
		Pattern.push_back(PatternClass());
		Pattern.back().id = l;
		Pattern.back().LinkId = setOfFailureLinks.at(l);
		for (int k =0;k<setOfFailureLinks.size();k++)
		{
			Pattern.back().next.push_back(setOfFailureLinks.at(k));
		}
		Pattern.back().Prob.assign(setOfFailureLinks.size(), 0.0);
		Pattern.back().Score.assign(setOfFailureLinks.size(), 1.0);
			
		// TODO. update the score to set the diagonal vector value =1
		for (int i = 0; i < setOfFailureLinks.size(); i++)
		{
			Pattern.back().Score.at(l) = 0;
		}
	}
	for (auto& p : Pattern)
	{
		p.updateProb();
	}
}

void Algorithm::ReadData(GRAPH& g)
{
	this->Graph = &g;
	ifstream f_ABCpara, f_FailLinks;
	FILE* fseedin;
	if (NetworkIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//MediumNetwork//Para.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (NetworkIndex == 2)
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
		//fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (NetworkIndex == 3)
	{
		//cout << "Model Index is not specified" << endl;
		//system("Pause");
		f_ABCpara.open("..//InPut//SiouxFallsNetwork//ABCPara.txt");
		f_FailLinks.open("..//InPut//SiouxFallsNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else if (NetworkIndex == 4)
	{
		f_ABCpara.open("..//InPut//ParadoxNet//ABCPara.txt");
		f_FailLinks.open("..//InPut//ParadoxNet//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	}
	else if (NetworkIndex == 5)
	{
		f_ABCpara.open("..//InPut//WangNetwork//ABCPara.txt");
		f_FailLinks.open("..//InPut//WangNetwork//FailureLinks.txt");
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
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
			setOfFailureLinks.push_back(linkId);
			(*Graph).Links.at(linkId).RecoverTime = length;
			(*Graph).Links.at(linkId).RequiredRes = res;
		}
		else if (fields.size() == 2)
		{
			if (fields[0]._Equal("res"))
			{
				setResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
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
	/// print and check the model parameters input
	/// </summary>
	ofstream fout;
	fout.open("..//OutPut//InputPara.txt");
	fout << "NumNodes" << "," << NumNodes << endl;
	fout << "NumOD" << "," << NumOD << endl;
	fout << "NumLinks" << "," << NumLinks << endl;
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
	for (size_t t = 0; t < GlobalBest.Links.size(); t++)
	{
		sf << sd << "," << GlobalBest.Links.at(t)->ID << "," << GlobalBest.StartTime.at(t) << "," <<
			GlobalBest.EndTime.at(t) << endl;
	}
	sf.close();

	sf.open("..//OutPut//PrintPeriod.txt", ios::app);

	for (size_t t = 0; t < GlobalBest.TravelTime.size(); t++)
	{
		sf << sd << "," << t << "," << GlobalBest.TravelTime.at(t) << ","<<GlobalBest.UNPM.at(t)<<endl;;
	}
	sf.close();
}
/// <summary>
/// update the score of an operator
/// </summary>
/// <param name="OpId">Id of the operator</param>
/// <param name="ImprovedFit"> if negative, then it is an improvement</param>
void Algorithm::UpdateOperatorScore_ALNS(int OpId, double ResultFit, double LocalFit, double GlobalFit)
{
	//TODO: write function to read reward 
	//      change the prob for selecting nei
	//      write python to read parameter 
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
void Algorithm::UpdateOperatorWeight_ALNS()
{
	for (int i = 0; i < Operators.size(); i++)
	{
		Operators.at(i).calWeight(ReactionFactor);
	}
}
/// <summary>
///  print the summary of the operators
/// </summary>
void Algorithm::PrintOperator(int seedid, int _iter)
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
		OutFile << seedid<< ",";
		OutFile << _iter << ",";
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
void Algorithm::ReadSolAndEvaluate(vector<int> &vec)
{
	// step 1: set the solution vector 
	SCHCLASS sol;
	sol.EndTime.assign(setOfFailureLinks.size(), -1);
	sol.StartTime.assign(setOfFailureLinks.size(), -1);
	for (int l = 0; l < vec.size(); l++) sol.Links.push_back(&Graph->Links.at(vec[l]));
	if (setOfFailureLinks.size() != vec.size()) 
		std::cout << "c++: warning: the failure link size does not equal vec size" << endl;
	sol.AlignStartTime(setResourceCap);
	sol.Evaluate(*Graph);
	//Remark: this is the last line of the exe, so that the results can be read from python
	//        Do not print the "endl" at the end.
	std::cout << sol.Fitness;
}

/// <summary>
/// Compute the efficiency index for each link
/// </summary>
void Algorithm::ComputeFailureLinkEI()
{
	for (int l = 0; l < setOfFailureLinks.size(); l++)
	{
		Graph->Links.at(setOfFailureLinks[l]).EI = Graph->CalRelSpChange(setOfFailureLinks[l]);
	}
}

void Algorithm::printLinkEI()
{
	ofstream  OutFile;
	OutFile.open("..//OutPut/LinkEI.txt", ios::app);
	for (int l = 0; l < setOfFailureLinks.size(); l++)
	{
		OutFile << setOfFailureLinks[l] << "," << Graph->Links.at(setOfFailureLinks[l]).EI << endl;
	}
	OutFile.close();
}

//based the score value update the patten prob
void PatternClass::updateProb()
{
	assert(Score.size() > 0);
	assert(Prob.size() > 0);
	double sum = std::accumulate(Score.begin(), Score.end(), 0.0);
	for (int p = 0; p < Prob.size(); p++)
	{
		Prob[p] = Score[p] / sum;
	}
}


// return the location of the vector
size_t Algorithm::findPatternIndex(int lid)
{
	assert(lid >= 0);
	for (size_t i = 0;i<Pattern.size();i++)
	{
		if (Pattern[i].LinkId == lid) return i;
	}
	cout << "C++ Warning: Can not find pattern index" << endl;
	return  -1;

}


int FindValIndex(const vector<int>& vec, int key);
// update the score based on the input solution 
// only call this when it is improved
void Algorithm::updatePatternScore(const SCHCLASS &sol,bool isGloablImprove)
{
	//TOOD: update the pattern of the improve solution
	for (int i = 0; i < sol.Links.size()-1; i++)
	{
		int first = (*sol.Links.at(i)).ID;
		int next = (*sol.Links.at(i+1)).ID;
		
		size_t PtLoc = findPatternIndex(first);
		size_t VecLoc = static_cast<size_t> (FindValIndex(setOfFailureLinks, next));
		if (isGloablImprove)
		{
			Pattern.at(PtLoc).Score.at(VecLoc) += PatternGlobalImproveScore;
		}
		else
		{
			Pattern.at(PtLoc).Score.at(VecLoc) += PatternLocalImproveScore;
		}
	}

	//todo : need to check whether this score is improved
	for (auto &p:Pattern)
	{
		p.updateProb();
	}
}
//print pattern on the file
void Algorithm::printPattern(int seedid)
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
		for (int i=0;i<p.next.size();i++)
		{
			OutFile << seedid << ",";
			OutFile << p.LinkId << ",";
			OutFile << p.next.at(i) << ",";
			OutFile << p.Score.at(i) << ",";
			OutFile << p.Prob.at(i) << endl;
		}
	}
	OutFile.close();

}



void SetAlgoType(std::string _name, AlgorithmType& alot)
{
	//TODO
	//	enum AlgorithmType
	//{
	//	CSA = 0, GA = 1, HH = 2, ABC = 3, UnDefined = 4
	//};

}
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

}

// this is to check whether the solution needs to be evaluated
bool Algorithm::isNeedToEvaluateSol(const SCHCLASS& Sol)
{
	// step 1: get key 
	string this_key = getMapStrFromSol(Sol);
	return isAddNewToArchive(this_key);
}


// TODO: check whether the link is a new to the archive 
bool Algorithm::isAddNewToArchive(const string &_key)
{
// step 1. get string from so
	if (m_str_val_solArchive.count(_key) == 0)
		return true;
	else
		return false;
}


// TODO: need to test this 
string Algorithm::getMapStrFromSol(const SCHCLASS &Sol) //get string for the map sol archive
{
	string val;
	for (auto l: Sol.Links)
	{
		val = std::to_string(l->ID)+val;
	}
	
	for (auto l : Sol.Links)
	{
		cout << "wtf: link id = " << l->ID << endl;
	}
	cout << "converted str = " << val << endl;
}
