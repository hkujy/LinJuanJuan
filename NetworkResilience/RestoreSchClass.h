#pragma once

#ifndef RestoreClass
#define RestoreClass
#include <iostream>
#include <vector>
#include "GlobalVar.h"
#include "Debug.h"
#include "GraphElements.h"

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
	explicit RelationClass(const int lid):
	CompareLink(lid),BeforeScore_Ave(0),
	BeforeScore_Total(0), AfterScore_Ave(0),AfterScore_Total(0),
	SamePeriodScore_Ave(0), SamePeriodScore_Total(0), BeforeCount(0),
	AfterCount(0), SamePeriodCount(0){}
	RelationClass(const RelationClass& obj) = default;
	~RelationClass() = default;
	RelationClass& operator=(const RelationClass& obj) = default;
	RelationClass(RelationClass&& rhs) = default;
	RelationClass& operator=(RelationClass&& rhs) = default;
	void UpdateScore(const LinkSchRelations &r,double score);
};

class PatternClass
{
public:
	int Id;
	int LinkId;
	std::vector<double> AbsScore;  // absolute score value
	std::vector<double> AbsProb;   // absolute score value
	std::vector<int> Count;
	std::vector<double> AveScore;
	std::vector<double> AveProb;
	std::vector<int> Next;
	std::vector<RelationClass> Relation;
	size_t findRelationId(const int comparedLink) const
	{
		for (size_t t = 0; t < Relation.size(); t++)
		{
			if (Relation[t].CompareLink == comparedLink)
			{
				return t;
			}
		}
		std::cerr << "can not find relation compared linked Id";
		TRACE("Debug find relationid fun");
		return -1;
	}
	PatternClass():Id(-1),LinkId(-1)
	{AbsScore.reserve(100); AbsProb.reserve(100); Count.reserve(100); 
	AveProb.reserve(100); AveScore.reserve(100); }
	~PatternClass() { 
		AbsScore.clear(); AbsProb.clear(); Id = -1; LinkId = -1; Next.clear(); 
		AveScore.clear(); AveProb.clear(); Count.clear(); 
	}
	void updateProb();
	PatternClass(const PatternClass& obj) = default;
	PatternClass& operator=(const PatternClass& rhs) = default;
	PatternClass(PatternClass&& other) = default;
	PatternClass& operator=(PatternClass&& rhs) = default;
};

class ScheduleClass   // class for the schedule 
{
public:
	int Id;
	std::vector<int> LinkId;
	std::vector<int> StartTime;
	std::vector<int> EndTime;
	std::vector<double> UsedRes; /// resource consumption 
	std::vector<double> TravelTime;
	std::vector<double> UNPM;
	double Fitness;
	std::string Key; // map to the string Key
	ScheduleClass():Id(-1),Fitness(-1),Key("-")
	{
		LinkId.reserve(100); StartTime.reserve(100); EndTime.reserve(100);
		UsedRes.reserve(100); 
		TravelTime.reserve(100);
		UNPM.reserve(100);
	}
	explicit  ScheduleClass(int id):Id(-1), Fitness(-1), Key("-")
	{
		LinkId.reserve(100); StartTime.reserve(100); EndTime.reserve(100); 
		UsedRes.reserve(100); TravelTime.reserve(100); UNPM.reserve(100);
	}
	~ScheduleClass() = default;
	ScheduleClass& operator=(const ScheduleClass& rhs) = default;
	ScheduleClass(const ScheduleClass& obj) = default;
	ScheduleClass(ScheduleClass&& other) = default;
	ScheduleClass& operator=(ScheduleClass&& other) = default;
#pragma region NotUsed
	//ScheduleClass& operator=(const ScheduleClass& rhs) {
	//	Id = rhs.Id;
	//	StartTime.assign(rhs.StartTime.begin(), rhs.StartTime.end());
	//	EndTime.assign(rhs.EndTime.begin(), rhs.EndTime.end());
	//	UsedRes.assign(rhs.UsedRes.begin(), rhs.UsedRes.end());
	//	Fitness = rhs.Fitness;
	//	LinkId.assign(rhs.LinkId.begin(), rhs.LinkId.end());
	//	TravelTime.assign(rhs.TravelTime.begin(), rhs.TravelTime.end());
	//	UNPM.assign(rhs.UNPM.begin(), rhs.UNPM.end());
	//	Key = rhs.Key;
	//	return*this;
	//}
	//ScheduleClass(const ScheduleClass& obj) {
	//	this->LinkId.assign(obj.LinkId.begin(), obj.LinkId.end());
	//	//for (int l = 0; l < obj.LinkId.size(); l++) this->LinkId.push_back(obj.LinkId.at(l));
	//	this->StartTime.assign(obj.StartTime.begin(), obj.StartTime.end());
	//	this->EndTime.assign(obj.EndTime.begin(), obj.EndTime.end());
	//	this->UsedRes.assign(obj.UsedRes.begin(), obj.UsedRes.end());
	//	this->TravelTime.assign(obj.TravelTime.begin(), obj.TravelTime.end());
	//	this->UNPM.assign(obj.UNPM.begin(), obj.UNPM.end());
	//	this->Id = obj.Id;
	//	this->Fitness = obj.Fitness;
	//	this->Key = obj.Key;
	//};  // copy constructor
	//void clear() {
	//	if (!LinkId.empty() > 0) LinkId.clear();
	//	StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear(); Key.clear();
	//}
	//~ScheduleClass()
	//{
	//	//if (Links.size() > 0) Links.clear();
	//	//for (vector<LinkClass*>::iterator itr = Links.begin(); itr != Links.end(); ++itr) {
	//	//	LinkClass* l = *itr;
	//	//	delete &l;
	//	//	l = NULL;
	//	//	//delete* itr;
	//	//}
	//	LinkId.clear();
	//	StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear(); Key.clear();
	//}
	// functions
#pragma endregion
	ScenarioClass ConvertToScenario(); // convert sch class to scenario and return 
	void AlignStartTime(const std::vector<double> &resCap, GraphClass &g); // improve the solution itself
	bool isFeasible(const std::vector<double> &res);  // check the solution is feasible or not
	void GenerateIniSch(GraphClass& g, const std::vector<int>& failureLinkSet);
	void GenerateIniBasedOnPattern(GraphClass& g, const std::vector<int>& failureLinkSet,
		const std::vector<PatternClass> &pat);
	void print() const;
	void getRes(GraphClass &g);
	void updatePrecedingRes(size_t st,size_t et,GraphClass &g);
	void updateResFor(size_t pos,GraphClass &g);
	int findEarliestFeasibleSt(size_t l, const std::vector<double>& resCap,GraphClass &g);
	int findEarliestInFeasibleSt(const std::vector<double>& resCap);
	void updateEndTime(GraphClass &g);
	void Evaluate(GraphClass& g);
	void computeKey();
	LinkSchRelations findDominantRelation(int aLink, int bLink, const std::vector<PatternClass> &pat,
		enum_CompareScoreMethod &compareMethod) const;
	int GetLastPeriod() { return *max_element(EndTime.begin(), EndTime.end());}
	std::vector<size_t> getNewReadyLinks(int tau) const;
	// write a few neighbor operators
	void GenNei(ScheduleClass& nei, GraphClass& g, const int& operatorId,
		const std::vector<int>& failureLinkSet, const std::vector<double>& resCap, const std::vector<PatternClass>& pat,
		enum_CompareScoreMethod &compareMethod);
	void Nei_Swap(ScheduleClass &newSol) const;
	void Nei_New(ScheduleClass& newSol, GraphClass& g, const std::vector<int>& failureLinkSet, const std::vector<double>& resCap);
	void Nei_Move_One_To_Right(ScheduleClass& newSol);
	void Nei_Move_One_To_Left(ScheduleClass& newSol) const;
	void Nei_Insert_One_Random_To_Right(ScheduleClass& newSol);
	void Nei_Insert_One_Random_To_Left(ScheduleClass& newSol);
	void Nei_FlipOver_OnePoint(ScheduleClass& newSol);
	void Nei_Greedy_EI_Based(ScheduleClass& newSol, GraphClass& g, std::string sType);
	void Nei_New_BasedOn_Pattern(ScheduleClass& newSol, GraphClass& g, const std::vector<int>& failureLinkSet,
		const std::vector<double>& resCap, const std::vector<PatternClass> &pat);
	void Nei_Swap_BasedOn_Pattern(ScheduleClass& newSol, GraphClass& g, const std::vector<int>& failureLinkSet,
		const std::vector<double>& resCap, const std::vector<PatternClass> &pat);
	void Nei_Swap_BasedOn_PatternRelation(ScheduleClass& newSol, GraphClass& g, const std::vector<int>& failureLinkSet,
		const std::vector<double>& resCap, const std::vector<PatternClass>& pat,
		enum_CompareScoreMethod &compareMethod) const;
	void GenerateTimeFromOrder(const std::vector<double>& resCap,GraphClass &g);
	void Repair_Delay();
	LinkSchRelations getRelation(int aLink,int comparedLink) const;//get the relationship of the two links
};

#endif
