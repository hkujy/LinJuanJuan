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
	std::vector<int> EndTime;
	std::vector<double> UsedRes; /// resource consumption 
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
	void AlignStartTime(const vector<double> &ResCap); // improve the solution itself
	bool isFeasible(const vector<double> &res);  // check the solution is feasible or not
	void GenerateIniSch(GRAPH& g, const vector<int>& FailureLinkSet);
	void print() const;
	void getRes();
	void updatePrecedingRes(int st,int et);
	void updateResFor(int Pos);
	int findEarliestSt(int l, const vector<double>& ResCap);
	void updateEndTime();
};


class ABCAlgorithms // to begin with use ABC algorithm
{
public:
	double BaseUNPM;
	double MaxFitValue;
	double MinFitValue;
	int NumEmployedBee;
	int NumOnlookers;
	int NumScouts;
	//TODO: the resource cap for each period should be a large period than the project perido
	std::vector<double> ResourceCap;  // Capacity of the resources
	std::vector<int> FailureLinks;
	ABCAlgorithms() {
		ResourceCap.assign(MaxNumOfSchPeriod, ResourceForEachPeriod);
		//TODO: add set of failure links
		
		//TODO: adjust the number of algorithm parameters 
		NumEmployedBee = 10;
		NumOnlookers = 10;
		NumScouts = 10;
	};
	~ABCAlgorithms();
	std::vector<SCHCLASS> Sols;
	void GenerateIni(GRAPH& Graph);
	void ABCMain(GRAPH& Graph);
	void EmployBee();
	void Onlooker();
	void Scout();
};


#endif
