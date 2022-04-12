#pragma once

#ifndef RestoreClass
#define RestoreClass
#include <iostream>
#include <vector>
#include "GlobalVar.h"
#include "Debug.h"
#include "GraphElements.h"
using namespace std;

//map the relationship between two nodes
class RelationClass
{
public:
	int CompareLink;
	double BeforeScore_Ave;
	double BeforeScore_Total;
	double AfterScore_Ave;
	double AfterScore_Total;
	double SamePeriodScore_Ave;
	double SamePeriodScore_Total;
	int BeforeCount;
	int AfterCount;
	int SamePeriodCount;
	RelationClass(int _lid) {
		CompareLink = _lid; BeforeScore_Ave = 0; BeforeScore_Total = 0; AfterScore_Ave = 0; AfterScore_Total = 0;
		SamePeriodScore_Ave = 0; SamePeriodScore_Total = 0; BeforeCount = 0; AfterCount = 0; SamePeriodCount = 0;
	}
	RelationClass(const RelationClass& obj)
	{
		CompareLink = obj.CompareLink; BeforeScore_Ave = obj.BeforeScore_Ave; BeforeScore_Total = obj.BeforeScore_Total;
		BeforeCount = obj.BeforeCount; AfterScore_Ave = obj.AfterScore_Ave; AfterScore_Total = obj.AfterScore_Total;
		AfterCount = obj.AfterCount; SamePeriodCount = obj.SamePeriodCount;
		SamePeriodScore_Ave = obj.SamePeriodScore_Ave; SamePeriodScore_Total = obj.SamePeriodScore_Total;
	}
	void UpdateScore(const LinkSchRelations &r,double _score);
};

class PatternClass
{
public:
	int id;
	int LinkId;
	vector<double> AbsScore;  // absolute score value
	vector<double> AbsProb;   // absolute score value
	vector<int> Count;
	vector<double> AveScore;
	vector<double> AveProb;
	vector<int> Next;
	vector<RelationClass> Relation;
	size_t findRelationId(int ComLink) const
	{
		for (size_t t = 0; t < Relation.size(); t++)
		{
			if (Relation[t].CompareLink == ComLink)
			{
				return t;
			}
		}
		cerr << "can not find relation compared linked Id";
		TRACE("Debug find relationid fun");
		return -1;
	};
	PatternClass() { id = -1; LinkId = -1; AbsScore.reserve(100); AbsProb.reserve(100); Count.reserve(100); 
	AveProb.reserve(100); AveScore.reserve(100); }
	~PatternClass() { 
		AbsScore.clear(); AbsProb.clear(); id = -1; LinkId = -1; Next.clear(); 
		AveScore.clear(); AveProb.clear(); Count.clear(); 
	}
	void updateProb();
	PatternClass(const PatternClass& obj)
	{
		id = obj.id; LinkId = obj.LinkId;
		AbsScore.assign(obj.AbsScore.begin(), obj.AbsScore.end()); AbsProb.assign(obj.AbsProb.begin(), obj.AbsProb.end());
		Count.assign(obj.Count.begin(), obj.Count.end()); AveScore.assign(obj.AveScore.begin(), obj.AveScore.end());
		AveProb.assign(AveProb.begin(), AveProb.end()); Next.assign(obj.Next.begin(), obj.Next.end());
		Relation.assign(obj.Relation.begin(), obj.Relation.end());
	}
};

class SCHCLASS   // class for the schedule 
{
public:
	int ID;
	std::vector<int> LinkID;
	std::vector<int> StartTime;
	std::vector<int> EndTime;
	std::vector<double> UsedRes; /// resource consumption 
	std::vector<double> TravelTime;
	std::vector<double> UNPM;
	double Fitness;
	std::string key; // map to the string key
	SCHCLASS() {
		ID = -1; Fitness = -1; LinkID.reserve(100); StartTime.reserve(100); EndTime.reserve(100); UsedRes.reserve(100); 
		TravelTime.reserve(100); UNPM.reserve(100);
		key = "";
	};
	SCHCLASS(int _id) {
		ID = _id; Fitness = -1; key = "";
		LinkID.reserve(100); StartTime.reserve(100); EndTime.reserve(100); 
		UsedRes.reserve(100); TravelTime.reserve(100); UNPM.reserve(100);
	}
	/// <summary>
	/// It is important to write this copy constructor
	/// </summary>
	/// <param name="obj"></param>
	SCHCLASS(const SCHCLASS& obj) {
		this->LinkID.assign(obj.LinkID.begin(), obj.LinkID.end());
		//for (int l = 0; l < obj.LinkID.size(); l++) this->LinkID.push_back(obj.LinkID.at(l));
		this->StartTime.assign(obj.StartTime.begin(), obj.StartTime.end());
		this->EndTime.assign(obj.EndTime.begin(), obj.EndTime.end());
		this->UsedRes.assign(obj.UsedRes.begin(), obj.UsedRes.end());
		this->TravelTime.assign(obj.TravelTime.begin(), obj.TravelTime.end());
		this->UNPM.assign(obj.UNPM.begin(), obj.UNPM.end());
		this->ID = obj.ID;
		this->Fitness = obj.Fitness;
		this->key = obj.key;
	};  // copy constructor
	void clear() {
		if (LinkID.size() > 0) LinkID.clear();
		StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear(); key.clear();
	}
	~SCHCLASS()
	{
		//if (Links.size() > 0) Links.clear();
		//for (vector<LinkClass*>::iterator itr = Links.begin(); itr != Links.end(); ++itr) {
		//	LinkClass* l = *itr;
		//	delete &l;
		//	l = NULL;
		//	//delete* itr;
		//}
		LinkID.clear();
		StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear(); key.clear();
	}
	// functions
	ScenarioClass ConvertToScenario(); // convert sch class to scenario and return 
	void AlignStartTime(const vector<double> &ResCap, GraphClass &g); // improve the solution itself
	bool isFeasible(const vector<double> &res);  // check the solution is feasible or not
	void GenerateIniSch(GraphClass& g, const vector<int>& FailureLinkSet);
	void GenerateIniBasedOnPattern(GraphClass& g, const vector<int>& FailureLinkSet,
		const vector<PatternClass> &pat);
	void print() const;
	void getRes(GraphClass &g);
	void updatePrecedingRes(size_t st,size_t et,GraphClass &g);
	void updateResFor(size_t Pos,GraphClass &g);
	int findEarliestFeasibleSt(size_t l, const vector<double>& ResCap,GraphClass &g);
	int findEarliestInFeasibleSt(const vector<double>& ResCap);
	void updateEndTime(GraphClass &g);
	void Evaluate(GraphClass& g);
	void computeKey();
	LinkSchRelations findDominantRelation(int Alink, int Blink, const vector<PatternClass> &Pattern,
		enum_CompareScoreMethod &CompareMethod);
	int GetLastPeriod()
	{
		return *max_element(EndTime.begin(), EndTime.end()); ;
	}
	vector<size_t> getNewReadyLinks(int tau);
	// write a few neighbor operators
	void GenNei(SCHCLASS& Nei, GraphClass& g, int& OpId,
		const vector<int>& FailureLinkSet, const vector<double>& ResCap, const vector<PatternClass>& Pat,
		enum_CompareScoreMethod &CompareMethod);
	void Nei_Swap(SCHCLASS &NewSol);
	void Nei_New(SCHCLASS& NewSol, GraphClass& g, const vector<int>& FailureLinkSet, const vector<double>& ResCap);
	void Nei_Move_One_To_Right(SCHCLASS& NewSol);
	void Nei_Move_One_To_Left(SCHCLASS& NewSol);
	void Nei_Insert_One_Random_To_Right(SCHCLASS& NewSol);
	void Nei_Insert_One_Random_To_Left(SCHCLASS& NewSol);
	void Nei_FlipOver_OnePoint(SCHCLASS& NewSol);
	void Nei_Greedy_EI_Based(SCHCLASS& NewSol, GraphClass& g, string sType);
	void Nei_New_BasedOn_Pattern(SCHCLASS& NewSol, GraphClass& g, const vector<int>& FailureLinkSet,
		const vector<double>& ResCap, const vector<PatternClass> &pat);
	void Nei_Swap_BasedOn_Pattern(SCHCLASS& NewSol, GraphClass& g, const vector<int>& FailureLinkSet,
		const vector<double>& ResCap, const vector<PatternClass> &pat);
	void Nei_Swap_BasedOn_PatternRelation(SCHCLASS& NewSol, GraphClass& g, const vector<int>& FailureLinkSet,
		const vector<double>& ResCap, const vector<PatternClass>& pat,
		enum_CompareScoreMethod &CompareMethod);
	void GenerateTimeFromOrder(const vector<double>& ResCap,GraphClass &g);
	void Repair_Delay();

	SCHCLASS& operator=(const SCHCLASS& rhs) {
		ID = rhs.ID; 
		StartTime.assign(rhs.StartTime.begin(), rhs.StartTime.end());
		EndTime.assign(rhs.EndTime.begin(), rhs.EndTime.end());
		UsedRes.assign(rhs.UsedRes.begin(), rhs.UsedRes.end());
		Fitness = rhs.Fitness;
		LinkID.assign(rhs.LinkID.begin(), rhs.LinkID.end());
		TravelTime.assign(rhs.TravelTime.begin(), rhs.TravelTime.end());
		UNPM.assign(rhs.UNPM.begin(), rhs.UNPM.end());
		key = rhs.key;
		return* this;
	}
	LinkSchRelations getRelation(int ALink,int ComparedLink) const;//get the relationship of the two links
};

#endif
