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
	int NumScouts;
	//TODO: the resource cap for each period should be a large period than the project perido
	std::vector<double> ResourceCap;  // Capacity of the resources
	std::vector<int> FailureLinks;
	ABCAlgorithms() {
		MaxFitValue = -99999999999999;
		MinFitValue = 999999999999999;
		ResourceCap.assign(MaxNumOfSchPeriod, ResourceForEachPeriod);
		//TODO: add set of failure links
		//TODO: adjust the number of algorithm parameters 
		NumEmployedBee = 10;
		NumOnlookers = 1;
		NumScouts = 1;
		FailureLinks.push_back(5);
		FailureLinks.push_back(6);
		FailureLinks.push_back(7);
		FailureLinks.push_back(8);
	};
	~ABCAlgorithms() {};
	std::vector<SCHCLASS> Sols;
	void GenerateIni(GRAPH& Graph);
	void ABCMain(GRAPH& Graph);
	void EmployBee(GRAPH& Graph);
	void Onlooker();
	void Scout();
};


#endif
