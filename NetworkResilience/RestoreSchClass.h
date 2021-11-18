#pragma once

#ifndef RestoreClass
#define RestoreClass
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
using namespace std;

class SCHCLASS   // class for the schedule 
{
public:
	std::vector<LINK*> Links;
	std::vector<int> StartTime;
	SCHCLASS() {};
	~SCHCLASS()
	{
		for (vector<LINK*>::iterator it = Links.begin(); it != Links.end(); ++it)
		{
			delete* it;
		}
	}
	// functions
	Scenario ConvertToScenario(); // convert sch class to scenario and return 
	void improve(); // improve the solution itself
	bool isFeasible();  // check the solution is feasible or not
};


class ABCAlgorithms // to begin with use ABC algorithm
{
public:
	double BaseUNPM;
	double MaxFitValue;
	double MinFitValue;
	std::vector<double> ResourceCap;  // Capacity of the resources
	ABCAlgorithms() {
		ResourceCap.assign(NumOfSchPeriod, ResourceForEachPeriod);
	};
	~ABCAlgorithms();
	std::vector<SCHCLASS> sol;
	int GenerateIni();
	void ABCMain(GRAPH& Graph);
	void EmployBee();
	void Onlooker();
	void Scout();
};





#endif
