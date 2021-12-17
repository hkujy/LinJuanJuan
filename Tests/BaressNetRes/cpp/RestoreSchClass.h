#pragma once

#ifndef RestoreClass
#define RestoreClass
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
using namespace std;

class PatternClass
{
public:
	int id;
	int LinkId;
	vector<double> Score;
	vector<double> Prob;
	vector<int> next;
	PatternClass() { id = -1; LinkId = -1; Score.reserve(100); Prob.reserve(100); }
	~PatternClass() { Score.clear(); Prob.clear(); id = -1; LinkId = -1; }
	void updateProb();
};

class SCHCLASS   // class for the schedule 
{
public:
	int ID;
	std::vector<LINK*> Links;
	std::vector<int> StartTime;
	std::vector<int> EndTime;
	std::vector<double> UsedRes; /// resource consumption 
	std::vector<double> TravelTime;
	std::vector<double> UNPM;
	double Fitness;
	SCHCLASS() {
		ID = -1; Fitness = -1; Links.reserve(100); StartTime.reserve(100); EndTime.reserve(100); UsedRes.reserve(100); 
		TravelTime.reserve(100); UNPM.reserve(100);
	};
	SCHCLASS(int _id) {
		ID = _id; Fitness = -1;
		Links.reserve(100); StartTime.reserve(100); EndTime.reserve(100); 
		UsedRes.reserve(100); TravelTime.reserve(100); UNPM.reserve(100);
	}
	/// <summary>
	/// It is important to write this copy constructor
	/// </summary>
	/// <param name="obj"></param>
	SCHCLASS(const SCHCLASS& obj) {
		for (int l = 0; l < obj.Links.size(); l++) this->Links.push_back(obj.Links.at(l));
		this->StartTime.assign(obj.StartTime.begin(), obj.StartTime.end());
		this->EndTime.assign(obj.EndTime.begin(), obj.EndTime.end());
		this->UsedRes.assign(obj.UsedRes.begin(), obj.UsedRes.end());
		this->TravelTime.assign(obj.TravelTime.begin(), obj.TravelTime.end());
		this->UNPM.assign(obj.UNPM.begin(), obj.UNPM.end());
		this->ID = obj.ID;
		this->Fitness = obj.Fitness;
	};  // copy constructor
	~SCHCLASS()
	{
		if (Links.size() > 0)
		{
			Links.clear();
		}
		StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear();
	}
	// functions
	Scenario ConvertToScenario(); // convert sch class to scenario and return 
	void AlignStartTime(const vector<double> &ResCap); // improve the solution itself
	bool isFeasible(const vector<double> &res);  // check the solution is feasible or not
	void GenerateIniSch(GRAPH& g, const vector<int>& FailureLinkSet);
	void GenerateIniBasedOnPattern(GRAPH& g, const vector<int>& FailureLinkSet,
		const vector<PatternClass> &pat);
	void print() const;
	void getRes();
	void updatePrecedingRes(size_t st,size_t et);
	void updateResFor(size_t Pos);
	int findEarliestFeasibleSt(size_t l, const vector<double>& ResCap);
	int findEarliestInFeasibleSt(const vector<double>& ResCap);
	void updateEndTime();
	void Evaluate(GRAPH& g);
	int GetLastPeriod()
	{
		return *max_element(EndTime.begin(), EndTime.end()); ;
	}
	vector<size_t> getNewReadyLinks(int tau);
	// write a few neighbor operators
	void GenNei(SCHCLASS& Nei, GRAPH& g, int& OpId,
		const vector<int>& FailureLinkSet, const vector<double>& ResCap, const vector<PatternClass>& Pat);
	void Nei_Swap(SCHCLASS &NewSol);
	void Nei_New(SCHCLASS& NewSol, GRAPH& g, const vector<int>& FailureLinkSet, const vector<double>& ResCap);
	void Nei_Move_One_To_Right(SCHCLASS& NewSol);
	void Nei_Move_One_To_Left(SCHCLASS& NewSol);
	void Nei_Insert_One_Random_To_Right(SCHCLASS& NewSol);
	void Nei_Insert_One_Random_To_Left(SCHCLASS& NewSol);
	void Nei_CrossOver_OnePoint(SCHCLASS& NewSol);
	void Nei_Greedy_EI_Based(SCHCLASS& NewSol, GRAPH& g, string sType);
	void Nei_New_Basedon_Pattern(SCHCLASS& NewSol, GRAPH& g, const vector<int>& FailureLinkSet,
		const vector<double>& ResCap, const vector<PatternClass> &pat);

	void GenerateTimeFromOrder(const vector<double>& ResCap);
	void Repair_Delay();

	SCHCLASS& operator=(const SCHCLASS& rhs) {
		ID = rhs.ID; 
		StartTime.assign(rhs.StartTime.begin(), rhs.StartTime.end());
		EndTime.assign(rhs.EndTime.begin(), rhs.EndTime.end());
		UsedRes.assign(rhs.UsedRes.begin(), rhs.UsedRes.end());
		Fitness = rhs.Fitness;
		Links.assign(rhs.Links.begin(), rhs.Links.end());
		TravelTime.assign(rhs.TravelTime.begin(), rhs.TravelTime.end());
		UNPM.assign(rhs.UNPM.begin(), rhs.UNPM.end());
		return* this;
	}
};

#endif
