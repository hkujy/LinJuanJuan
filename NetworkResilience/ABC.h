#pragma once
#ifndef ABCclass
#define ABCclass
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
#include "RestoreSchClass.h"
using namespace std;

class OperatorClass
{
public:
	int id;
	int TotalCounterGood;  // number of calls improve
	int TotalCounterBad; // number of counters drops
	int TotalCounterSum; // Total number of counters 
	double Score;
	double Prob;
	double Weight;
	OperatorClass();
	~OperatorClass();
	void calWeight(double r);
};


class ABCAlgorithms // to begin with use ABC algorithm
{
public:
	//double BaseUNPM;
	GRAPH* Graph;
	double MaxFitValue;
	double MinFitValue;
	int NumEmployedBee;
	int NumOnlookers;
	int MaxIter;
	vector<int> ScountCounter; // record the number of scouts
	int MaxScountCount;
	std::vector<double> setResourceCap;  // Capacity of the resources
	std::vector<int> setOfFailureLinks;
	vector<double> CumProbForSelectOnlooker; // probability for the onlookers
	vector<double> CumProbForSelectNei; // probability for the onlookers
	SCHCLASS GlobalBest;
	vector<double> ConvergeMeasure;
	vector<OperatorClass> Operators;
	double RewardImproveGlobal;
	double RewardImproveLocal;
	double RewardWorse;
	double ReactionFactor;
	SelectOperatorType SelectOp;
	ABCAlgorithms() {
		MaxFitValue = -99999999999999; MinFitValue = 999999999999999;
		NumEmployedBee = -1; NumOnlookers = -1; MaxScountCount = -1; MaxIter = -1;
		ScountCounter.reserve(100); setResourceCap.reserve(100); setOfFailureLinks.reserve(100);
		CumProbForSelectOnlooker.reserve(100);
		CumProbForSelectNei.reserve(100);
		Graph = new GRAPH;
		RewardImproveGlobal = 0.0;
		RewardImproveLocal = 0.0;
		RewardWorse = 0.0;
		ReactionFactor = 0.0;
		SelectOp = SelectOperatorType::None;
		for (int i = 0; i < NumOperators; i++)
		{
			Operators.push_back(OperatorClass());
			Operators.back().id = i;
		}
	};
	~ABCAlgorithms() { Graph = nullptr; };
	std::vector<SCHCLASS> Sols;
	void ReadSolAndEvaluate(vector<int>& vec);
	void GenerateIni();
	void ABCMain();
	void EmployBeePhase();
	void OnlookerPhase();
	void ScoutPhase();
	void GetProb();
	void ReadData(GRAPH& Graph);
	size_t SelectOnLookerBasedonProb();
	void PrintFinal(int sd);
	void UpdateOperatorMeaures(int _id, bool isImproved);
	bool CompareTwoSolsAndReplace(SCHCLASS& lhs, SCHCLASS& rhs, int NeiOperatorId);
	void PrintOperator(int seedid);
	void UpdateOperatorProb();
	void IniOperatorProb();
	void UpdateOperatorWeight();
	int SelectOperIndex();
	void UpdateOperatorScore(int OpId, double ResultFit, double LocalFit, double GlobalFit);

	void UpdateOperatorProb_ALNS();
	void UpdateOperatorScore_ALNS(int OpId, double ResultFit, double LocalFit, double GlobalFit);
	void UpdateOperatorWeight_ALNS();
	int SelectOperator_ALNS();
	void IniOperatorProb_ANLS();


};


#endif
