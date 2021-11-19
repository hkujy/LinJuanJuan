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
	int ID;
	std::vector<LINK*> Links;
	std::vector<int> StartTime;
	std::vector<int> EndTime;
	std::vector<double> UsedRes; /// resource consumption 
	double TotalCost;
	SCHCLASS() {}
	SCHCLASS(int _id) { ID = _id; }
	/// <summary>
	/// It is important to write this copy constructor
	/// </summary>
	/// <param name="obj"></param>
	SCHCLASS(const SCHCLASS& obj) {
		for (int l = 0; l < obj.Links.size(); l++) this->Links.push_back(obj.Links.at(l));
		this->StartTime.assign(obj.StartTime.begin(), obj.StartTime.end());
		this->EndTime.assign(obj.EndTime.begin(), obj.EndTime.end());
		this->UsedRes.assign(obj.UsedRes.begin(), obj.UsedRes.end());
		this->ID = obj.ID;
		this->TotalCost = obj.TotalCost;
	};  // copy constructor
	~SCHCLASS()
	{
		if (Links.size() > 0)
		{
			Links.clear();
			//for (int l = 0; l < Links.size(); l++)
			//{
				//delete Links.at(l);
				//Links.at(l) = nullptr;
			//}
			//for (vector<LINK*>::iterator it = Links.begin(); it != Links.end(); ++it)
			//{
		}
	}
	// functions
	Scenario ConvertToScenario(); // convert sch class to scenario and return 
	void AlignStartTime(const vector<double> &ResCap); // improve the solution itself
	bool isFeasible(const vector<double> &res);  // check the solution is feasible or not
	void GenerateIniSch(GRAPH& g, const vector<int>& FailureLinkSet);
	void print() const;
	void getRes();
	void updatePrecedingRes(size_t st,size_t et);
	void updateResFor(size_t Pos);
	int findEarliestSt(size_t l, const vector<double>& ResCap);
	void updateEndTime();
	void Evaluate(GRAPH& g);
	int GetLastPeriod()
	{
		return EndTime.back();
	}
	vector<size_t> getNewReadyLinks(int tau);
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
		FailureLinks.push_back(6);
		FailureLinks.push_back(7);
		FailureLinks.push_back(8);
	};
	~ABCAlgorithms() {};
	std::vector<SCHCLASS> Sols;
	void GenerateIni(GRAPH& Graph);
	void ABCMain(GRAPH& Graph);
	void EmployBee();
	void Onlooker();
	void Scout();
};


#endif
