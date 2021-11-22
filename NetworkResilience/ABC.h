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

class ABCAlgorithms // to begin with use ABC algorithm
{
public:
	//double BaseUNPM;
	double MaxFitValue;
	double MinFitValue;
	int NumEmployedBee;
	int NumOnlookers;
	int MaxIter;
	vector<int> ScountCounter;
	int MaxScountCount;
	//TODO: the resource cap for each period should be a large period than the project perido
	std::vector<double> ResourceCap;  // Capacity of the resources
	std::vector<int> FailureLinks;
	vector<double> Prob; // probability for the onlookers
	SCHCLASS GlobalBest;
	vector<double> ConvergeMeasure;
	ABCAlgorithms() {
		MaxFitValue = -99999999999999;
		MinFitValue = 999999999999999;
		ResourceCap.assign(MaxNumOfSchPeriod, ResourceForEachPeriod);
		//TODO: add set of failure links
		//TODO: adjust the number of algorithm parameters 
		NumEmployedBee = -1; NumOnlookers = -1; MaxScountCount = -1; MaxIter = -1;
		FailureLinks.push_back(3);
		FailureLinks.push_back(5);
		FailureLinks.push_back(6);
		FailureLinks.push_back(7);
		FailureLinks.push_back(8);
		//Prob.assign(NumEmployedBee, 0.0);
		//ScountCounter.assign(NumEmployedBee, 0);
	};
	~ABCAlgorithms() {};
	std::vector<SCHCLASS> Sols;
	void GenerateIni(GRAPH& Graph);
	void ABCMain(GRAPH& Graph);
	void EmployBeePhase(GRAPH& Graph);
	void OnlookerPhase(GRAPH& Graph);
	void ScoutPhase(GRAPH& Graph);

	void GetProb();
	void ReadData();
	size_t Select_Basedon_Prob();
	void PrintFinal(int sd);

};


#endif
