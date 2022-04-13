#pragma once
#ifndef ABC_ALGORITHM_CLASS
#define ABC_ALGORITHM_CLASS
#include <iostream>
#include <vector>
#include <unordered_map>
#include "RestoreSchClass.h"
using namespace std;

class OperatorClass
{
public:
	int Id;
	int TotalCounterGood;  // number of calls improve
	int TotalCounterBad; // number of counters drops
	int TotalCounterSum; // Total number of counters 
	double Score;
	double Prob;
	double Weight;
	OperatorClass();
	//~OperatorClass();
	void CalWeight(double r);
};

class Algorithm // to begin with use ABC algorithm
{
public:
	string name;
	double BaseUNPM;
	GraphClass* Graph;
	double MaxFitValue;
	double MinFitValue;
	int NumEmployedBee;
	int NumOnlookers;
	int MaxScoutCount;
	int MaxIter;
	vector<int> ScoutCounter; // record the number of scouts
	std::vector<double> SetOfResourceCap;  // Capacity of the resources
	std::vector<int> SetOfFailureLinks;
	std::unordered_map<std::string, double> m_str_val_solArchive;
	vector<double> CumProbForSelectOnlooker; // probability for the onlookers
	vector<double> CumProbForSelectNei; // probability for the onlookers
	ScheduleClass GlobalBest;
	vector<double> ConvergeMeasure;
	vector<OperatorClass> Operators;
	vector<PatternClass> Pattern;
	double RewardImproveGlobal;
	double RewardImproveLocal;
	double RewardWorse;
	double ReactionFactor;
	SelectOperatorType SelectOp;
	enum_CompareScoreMethod CompareScoreMethod;
	string getMapStrFromSol(const ScheduleClass &sol) const; //get string for the map sol archive
	bool isAddNewToArchive(const string &key);
	bool isNeedToEvaluateSol(const ScheduleClass &sol);
	void evaluateOneSol(ScheduleClass &sol, GraphClass& g);
	void Ini(GraphClass& g);
	void IniPattern();
	LinkSchRelations findDominantRelation(int ALink, int BLink);// find the dominate relationship between two links

	Algorithm(): name("Unspecified"),BaseUNPM(-1.0),
	Graph(new GraphClass), MaxFitValue(-9.9E10), MinFitValue(9.9E20),
	NumEmployedBee(-1),NumOnlookers(-1),MaxScoutCount(-1),MaxIter(-1),
	RewardImproveGlobal(0.0),RewardImproveLocal(0.0),RewardWorse(0.0),
	ReactionFactor(0.0), SelectOp(SelectOperatorType::None),
	CompareScoreMethod(enum_CompareScoreMethod::None)
	{
		ScoutCounter.reserve(100); SetOfResourceCap.reserve(100); SetOfFailureLinks.reserve(100);
		m_str_val_solArchive.reserve(5000);
		CumProbForSelectOnlooker.reserve(100);
		CumProbForSelectNei.reserve(100); ConvergeMeasure.reserve(1000);
		Pattern.reserve(100);
		for (int i = 0; i < NUM_OPERATORS; i++)
		{
			Operators.emplace_back(OperatorClass());
			Operators.back().Id = i;
		}
	}

	Algorithm(const Algorithm& rhs) = default;
	Algorithm& operator=(const Algorithm& other) = default;
	Algorithm(Algorithm&& other) = default;
	Algorithm& operator=(Algorithm&& other) = default;
	~Algorithm() { Graph = nullptr; }
	std::vector<ScheduleClass> Sols;
	void clearSols() { if (!Sols.empty()) Sols.clear(); }
	void IniSolArchive();
	void ComputeFailureLinkEI();
	void printLinkEI();
	void ReadSolAndEvaluate(vector<int>& vec,GraphClass &g);
	void GenerateIniSol();
	void ABCMain();
	void HHMain();
	void HHGenNewNei();
	void HHGenIni();
	void EmployBeePhase();
	void OnlookerPhase();
	void ScoutPhase();
	void GetProb();
	void ReadData(GraphClass& graph);
	size_t SelectOnLookerBasedOnProb() const;
	void PrintFinal(int sd);
	void UpdateOperatorMeasure(int id, bool isImproved);
	bool CompareTwoSolsAndReplace(ScheduleClass& lhs, const ScheduleClass& rhs, int neiOperatorId);
	void LearnPattern_Score(const ScheduleClass& sol, bool isGlobalImprove);
	void LearnPatternRelation_Score(const ScheduleClass& sol, bool isGlobalImprove);
	size_t findPatternIndex(int lid);
	void PrintOperator(int seedId,int iter);
	void UpdateOperatorProb();
	void IniOperatorProb();
	void UpdateOperatorWeight();
	int SelectOperatorIndex();
	void UpdateOperatorScore(int opId, double resultFit, double localFit, double globalFit);
	void UpdateOperatorProb_ALNS();
	void UpdateOperatorScore_ALNS(int opId, double resultFit, double localFit, double GlobalFit);
	void UpdateOperatorWeight_ALNS();
	int SelectOperator_ALNS();
	void IniOperatorProb_ALNS();
	void printPattern(int seedId);
	void printDomRelation(int seed) const;
};


#endif
