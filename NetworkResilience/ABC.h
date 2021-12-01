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
	int CounterGood;  // number of calls improve
	int CounterBad; // number of counters drops
	int CounterSum; // Total number of counters 
	OperatorClass();
	~OperatorClass();
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
	vector<int> ScountCounter;
	int MaxScountCount;
	std::vector<double> ResourceCap;  // Capacity of the resources
	std::vector<int> FailureLinks;
	vector<double> Prob; // probability for the onlookers
	SCHCLASS GlobalBest;
	vector<double> ConvergeMeasure;
	vector<OperatorClass> Operators;
	ABCAlgorithms() {
		MaxFitValue = -99999999999999; MinFitValue = 999999999999999;
		NumEmployedBee = -1; NumOnlookers = -1; MaxScountCount = -1; MaxIter = -1;
		ScountCounter.reserve(100); ResourceCap.reserve(100); FailureLinks.reserve(100); Prob.reserve(100);
		Graph = new GRAPH;
		for (int i=0;i<NumOperators;i++)
		{
			Operators.push_back(OperatorClass());
			Operators.back().id = i;
			Operators.back().CounterBad = 0;
			Operators.back().CounterGood = 0;
			Operators.back().CounterSum = 0;
		}
	};
	~ABCAlgorithms() { Graph = nullptr; };
	std::vector<SCHCLASS> Sols;
	void GenerateIni();
	void ABCMain();
	void EmployBeePhase();
	void OnlookerPhase();
	void ScoutPhase();
	void GetProb();
	void ReadData(GRAPH& Graph);
	size_t Select_Basedon_Prob();
	void PrintFinal(int sd);
	void UpdateOperatorMeaures(int _id, bool isImproved);
	bool CompareTwoSolsAndReplace(SCHCLASS& lhs, SCHCLASS& rhs, int NeiOperatorId);
	void PrintOperator(int seedid);

};


#endif
