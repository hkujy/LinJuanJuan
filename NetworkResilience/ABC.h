#pragma once
#ifndef ABCclass
#define ABCclass
#include <iostream>
#include <vector>
#include <unordered_map>
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


class Algorithm // to begin with use ABC algorithm
{
public:
	double BaseUNPM;
	GRAPH* Graph;
	double MaxFitValue;
	double MinFitValue;
	int NumEmployedBee;
	int NumOnlookers;
	int MaxIter;
	string name;
	vector<int> ScountCounter; // record the number of scouts
	int MaxScountCount;
	std::vector<double> setResourceCap;  // Capacity of the resources
	std::vector<int> setOfFailureLinks;
	std::unordered_map<std::string, double> m_str_val_solArchive;
	vector<double> CumProbForSelectOnlooker; // probability for the onlookers
	vector<double> CumProbForSelectNei; // probability for the onlookers
	SCHCLASS GlobalBest;
	vector<double> ConvergeMeasure;
	vector<OperatorClass> Operators;
	vector<PatternClass> Pattern;
	double RewardImproveGlobal;
	double RewardImproveLocal;
	double RewardWorse;
	double ReactionFactor;
	SelectOperatorType SelectOp;
	string getMapStrFromSol(const SCHCLASS &Sol); //get string for the map sol archive
	bool isAddNewToArchive(const string &_key);
	bool isNeedToEvaluateSol(const SCHCLASS &Sol);
	void EvaluteOneSol(SCHCLASS &Sch, GRAPH& g);
	void Ini(GRAPH& g);
	void IniPattern();
	Algorithm() {
		name = "";
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
		Pattern.reserve(100);
		SelectOp = SelectOperatorType::None;
		for (int i = 0; i < NumOperators; i++)
		{
			Operators.push_back(OperatorClass());
			Operators.back().id = i;
		}
	};

	~Algorithm() { Graph = nullptr; };
	std::vector<SCHCLASS> Sols;
	void clearSols() { if (Sols.size()>0) Sols.clear(); }
	void IniSolArchive();
	void ComputeFailureLinkEI();
	void printLinkEI();
	void ReadSolAndEvaluate(vector<int>& vec,GRAPH &g);
	void GenerateIniSol();
	void ABCMain();
	void HHMain();
	void HHGenNewNei();
	void HHGenIni();
	void EmployBeePhase();
	void OnlookerPhase();
	void ScoutPhase();
	void GetProb();
	void ReadData(GRAPH& Graph);
	size_t SelectOnLookerBasedonProb();
	void PrintFinal(int sd);
	void UpdateOperatorMeaures(int _id, bool isImproved);
	bool CompareTwoSolsAndReplace(SCHCLASS& lhs, SCHCLASS& rhs, int NeiOperatorId);
	void updatePatternScore(const SCHCLASS& sol, bool isGloablImprove);
	size_t findPatternIndex(int lid);
	void PrintOperator(int seedid,int _iter);
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
	void printPattern(int seedid);
};


#endif
