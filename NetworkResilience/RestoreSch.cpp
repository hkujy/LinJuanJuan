#include <vector>
#include <algorithm>
#include <numeric>
#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include "GraphElements.h"
#include <assert.h>
using namespace std;
void SortStartTime(vector<int>& st)
{
	std::sort(st.begin(), st.end());
}

int FindValIndex(const vector<int>& vec, int key)
{
	std::vector<int>::const_iterator itr = std::find(vec.begin(), vec.end(), key);
	if (itr != vec.cend()) {
		return static_cast<int>(std::distance(vec.begin(), itr));
		//std::cout << "Element present at index " << std::distance(v.begin(), itr);
	}
	else {
		cerr << "wtf: can not find the index of a Key" << endl;
		return -1;
	}
}

//generate the time from order to time
void ScheduleClass::GenerateTimeFromOrder(const vector<double>& resCap,GraphClass &g)
{
	AlignStartTime(resCap,g);
}
void ScheduleClass::print() const
{
	cout << "*******Print and check one sol********" << endl;
	for (size_t i = 0; i < LinkId.size(); i++)
	{
		//cout << "Link = " << LinkId.at(i)->Id << ", Start Time = " << StartTime.at(i) << ", End Time = " << EndTime.at(i) << endl;
		cout << "Link = " << LinkId.at(i) << ", Start Time = " << StartTime.at(i) << ", End Time = " << EndTime.at(i) << endl;
	}
	cout << "********End*************************" << endl;
}

ScenarioClass ScheduleClass::ConvertToScenario() {
	ScenarioClass s;
	//for (auto l : LinkId) s.LinkIds.push_back(l->Id);
	for (auto l : LinkId) s.LinkIds.push_back(l);
	for (auto t : StartTime) s.tau.push_back(t);
	return s;
}

void ScheduleClass::getRes(GraphClass &g)
{
	UsedRes.assign(MAX_NUM_OF_SCH_PERIOD, 0);

	for (size_t l = 0; l < LinkId.size(); l++)
	{
		for (int t = StartTime.at(l); t < EndTime.at(l); t++)
		{
			//UsedRes.at(t) += LinkId.at(l)->RequiredRes;
			UsedRes.at(t) += g.Links.at(LinkId[l]).RequiredRes;
		}
	}
}
void ScheduleClass::updateEndTime(GraphClass &g)
{
	for (int l = 0; l < LinkId.size(); l++)
	{
		//EndTime.at(l) = StartTime.at(l) + LinkId.at(l)->RecoverTime;
		EndTime.at(l) = StartTime.at(l) + g.Links.at(LinkId[l]).RecoverTime;
	}
}

bool ScheduleClass::isFeasible(const vector<double>& res)
{
	for (size_t i = 0; i < res.size(); i++)
	{
		if (UsedRes.at(i) > res.at(i))
		{
			cout << "Resource Violation At Period " << i << ", used = " << UsedRes[i] << ", avi = " << res[i] << endl;
			return false;
		}
	}
	return true;
}

//update the resource used before the pos project
//does not include the et link, which is supposed to be the current link
void ScheduleClass::updatePrecedingRes(size_t st, size_t et,GraphClass &g)
{
	for (size_t l = st; l < et; l++)
	{
		for (size_t t = StartTime.at(l); t < EndTime.at(l); t++)
		{
			//UsedRes.at(t) += LinkId.at(l)->RequiredRes;
			UsedRes.at(t) += g.Links.at(LinkId[l]).RequiredRes;
		}
	}
}

void ScheduleClass::updateResFor(size_t pos, GraphClass &g)
{
	for (int t = StartTime.at(pos); t < EndTime.at(pos); t++)
	{
		UsedRes.at(t) += g.Links.at(LinkId[pos]).RequiredRes;
	}
}

//find the earliest a time with available resources
int ScheduleClass::findEarliestFeasibleSt(size_t l, const vector<double>& resCap,GraphClass &g) {

	for (int t = 0; t < MAX_NUM_OF_SCH_PERIOD; t++)
	{
		//if (UsedRes.at(t) + this->LinkId.at(l)->RequiredRes <= resCap.at(t))
		if (UsedRes.at(t) + g.Links.at(LinkId[l]).RequiredRes <= resCap.at(t))
		{
			return t;
		}
	}
	cout << "ERR: can not find the earliest start time for project " << l << endl;
	return -1;
}

int ScheduleClass::findEarliestInFeasibleSt(const vector<double>& resCap) {

	for (int t = 0; t < MAX_NUM_OF_SCH_PERIOD; t++)
	{
		if (UsedRes.at(t) > resCap.at(t))
		{
			return t;
		}
	}
	return -1;
}

void ScheduleClass::AlignStartTime(const vector<double>& resCap, GraphClass &g) {
	// shift the project to the earliest feasible time
	EndTime.assign(LinkId.size(), -1);
	StartTime.assign(LinkId.size(), -1);
	UsedRes.assign(MAX_NUM_OF_SCH_PERIOD, 0.0);
	for (size_t l = 0; l < LinkId.size(); l++)
	{
		if (l == 0)
		{
			StartTime.at(l) = 0;
			EndTime.at(l) = StartTime.at(l) +g.Links.at(LinkId.at(l)).RecoverTime;
		}
		else
		{
			updatePrecedingRes(l - 1, l,g);
			StartTime.at(l) = findEarliestFeasibleSt(l, resCap,g);
			EndTime.at(l) = StartTime.at(l) + g.Links.at(LinkId.at(l)).RecoverTime;
		}
	}
	updateResFor(LinkId.size() - 1,g);
}
void ScheduleClass::GenerateIniSch(GraphClass& g, const vector<int>& FailureLinks)
{
	assert(!FailureLinks.empty());
	if (!LinkId.empty())
	{
		LinkId.clear(); StartTime.clear(); EndTime.clear();
	}
	// step 1 generate ini number of links
	vector<bool> isSelected(FailureLinks.size(), false);
	int countDoWhile = 0;
	do
	{
		int linkNum = GenRandomInt(FailureLinks);
		int pos = FindValIndex(FailureLinks, linkNum);
		if (!isSelected.at(pos))
		{
			LinkId.emplace_back(linkNum);
			isSelected.at(pos) = true;
		}
		countDoWhile++;
		if (countDoWhile > 1000)
		{
			cout << "ERR: Do while count in GenerateIniSch is larger than 1000" << endl;
			system("PAUSE");
		}
	} while (std::find(isSelected.begin(), isSelected.end(), false) != isSelected.end());
}
vector<size_t> ScheduleClass::getNewReadyLinks(int tau) const
{
	vector<size_t> results;
	if (tau == 0)
	{
		return results;
	}
	else
	{
		for (size_t l = 0; l < LinkId.size(); l++)
		{
			if (EndTime.at(l) == tau)
			{
				results.push_back(l);
			}
		}
	}

	return results;
}

void ScheduleClass::Evaluate(GraphClass& g)
{

#ifdef _DEBUG
	for (auto const &l:g.Links)
	{
		if (l.CaRevise < l.CaInput - 1)
		{
			std::cout << "Warning: the capacity is not set to initial value before the evalute procedure" << endl;
			system("PAUSE");
		}
	}
#endif
	//Fitness = 0.0;
	vector<size_t> cumulativeReadyLinks;
	TravelTime.assign(GetLastPeriod(), 0.0);
	UNPM.assign(GetLastPeriod(), 0.0);
	// step 1: set all the capacity of failure links to be 0.0 and evaluate the total cost in tau = 0
	for (size_t l = 0; l < LinkId.size(); l++)
	{
		g.Links.at(LinkId[l]).CaRevise = zero;
	}
	g.EvaluteGraph();

	// step 2: compute the time in each period
	for (int t = 0; t < GetLastPeriod() + 1; t++)
	{
		vector<size_t> NewReady = getNewReadyLinks(t);
		//for (auto v : NewReady) cout << v << ",";
		//cout << endl;
		// case 1: at the begin, there is not other links
		if (NewReady.size() == 0)
		{
			if (t == 0)
			{
				TravelTime.at(t) = g.TotalSystemCost;
				UNPM.at(t) = g.UNPM;
			}
			else
			{
				TravelTime.at(t) = TravelTime.at(t - 1);
				UNPM.at(t) = UNPM.at(t - 1);
			}
#ifdef _DEBUG
			std::cout << "---Period = " << t << ", no link is added" << endl;
#endif // _DEBUG
		}
		else
		{
			for (auto l : NewReady)
			{
				g.Links.at(LinkId[l]).IniCap();
				//LinkId.at(l)->IniCap();
			}

			g.EvaluteGraph();
#ifdef _DEBUG
			std::cout << "---Period = " << t << "," << NewReady.size() << " link is added" << endl;
			cout << "---Period = " << t << "," << "min cost = " << g.OdPairs.at(0).MinCost << "unpm=" << g.UNPM << endl;;
#endif // _DEBUG
			cumulativeReadyLinks.insert(cumulativeReadyLinks.end(), NewReady.begin(), NewReady.end());
			if (t == GetLastPeriod())
			{
				//TravelTime.push_back(0);
			}
			else
			{
				TravelTime.at(t) = g.TotalSystemCost;
				UNPM.at(t) = g.UNPM;
			}
		}
	}

	this->Fitness = std::accumulate(TravelTime.begin(), TravelTime.end(), 0.0);
#ifdef _DEBUG
	cout << "-------summary of travel time----------" << endl;
	cout << "Last end time = " << GetLastPeriod() << endl;
	for (int t = 0; t < TravelTime.size(); t++)
	{
		cout << "t = " << t << ", TravelTime = " << TravelTime.at(t) << ",";
		cout << "UNPM = " << UNPM.at(t) << endl;
	}
	cout << "total cost = " << this->Fitness << endl;
#endif // _DEBUG
}


int SelectOneIndexFrom(const vector<int>& candy, const vector<double>& prob);
//GenerateIniSolutionBasedonthe pattern 
void ScheduleClass::GenerateIniBasedOnPattern(GraphClass& g, const vector<int>& FailureLinks, 
	const vector<PatternClass>& pat)
{
#ifdef _DEBUG
	cout << "---------------Generate Ini Based on Pattern is called" << endl;
#endif // _DEBUG
	assert(FailureLinks.size() > 0);
	if (this->LinkId.size() > 0) { LinkId.clear(); StartTime.clear(); EndTime.clear(); }
	// step 1 generate ini number of links
	vector<bool> isSelected(FailureLinks.size(), false);
	int CountDoWhile = 0;
	// step 1: select the first link 
	int LinkNum = GenRandomInt(FailureLinks);
	int LocOfFailLinks = FindValIndex(FailureLinks, LinkNum);
	this->LinkId.push_back(LinkNum);
	//this->LinkId.push_back(new LinkClass());
	//LinkId.back() = &g.Links.at(LinkNum);
	isSelected.at(LocOfFailLinks) = true;
	vector<int> candy;
	vector<double> prob;
	int loopCounter = 0;
	candy.reserve(FailureLinks.size()); prob.reserve(FailureLinks.size());
	do							 
	{
		loopCounter++;
		if (loopCounter > FailureLinks.size() + 1)
			cout << "C++ Warning: Generate Ini based on pattern loop" << endl;
		candy.clear(); prob.clear();
		// step 1: create a list selected 
		for (int s = 0;s<isSelected.size();s++)
		{
			if (!isSelected.at(s))
			{
				candy.push_back(s);
				prob.push_back(pat[LocOfFailLinks].AbsProb.at(s));
			}
		}
		int selectedLocOfCandy = SelectOneIndexFrom(candy, prob);
		LinkNum = FailureLinks.at(candy.at(selectedLocOfCandy));
		LocOfFailLinks = FindValIndex(FailureLinks, LinkNum);
		LinkId.push_back(LinkNum);
		//LinkId.push_back(new LinkClass());
		//LinkId.back() = &g.Links.at(LinkNum);
		isSelected.at(LocOfFailLinks) = true;
	} while (std::find(isSelected.begin(), isSelected.end(), false) != isSelected.end());

#ifdef _DEBUG
	cout << "------Complete Generate Ini Based on Pattern is called" << endl;
#endif // _DEBUG


}

// compute the Key associated the solution
void ScheduleClass::computeKey()
{
	string val;
	for (int i=0;i<LinkId.size();i++)
	{
		val = std::to_string(LinkId[i]) + val;
	}
	//for (auto l : this->LinkId)
	//{
	//	//val = std::to_string(l->Id) + val;
	//	val = std::to_string(l) + val;
	//}
	for (auto l : this->LinkId)
	{
		//cout << "wtf: link Id = " << l->Id << endl;
		cout << "wtf: link Id = " << l << endl;
	}
	cout << "converted str = " << val << endl;
}


void RelationClass::UpdateScore(const LinkSchRelations& r,double score)
{
	switch (r)
	{
	case LinkSchRelations::Before:
		BeforeCount++;
		BeforeScore_Total += score;
		BeforeScore_Ave = BeforeScore_Total / BeforeCount;
		break;
	case LinkSchRelations::After:
		AfterCount++;
		AfterScore_Total += score;
		AfterScore_Ave = AfterScore_Total / AfterCount;
		break;
	case LinkSchRelations::Same:
		SamePeriodCount++;
		SamePeriodScore_Total += score;
		SamePeriodScore_Ave = SamePeriodScore_Total / SamePeriodCount;
		break;
	case LinkSchRelations::None:
		cout << "Warning: Relationship is not specified" << endl;
		TRACE("Debug Relationship Update");
		break;
	default:
		break;
	}
}

//get the relationship between two links based on their starting time
LinkSchRelations ScheduleClass::getRelation(int aLink, int comparedLink) const
{
	const int ALinkIndex = FindValIndex(LinkId, aLink);
	const int compareLinkIndex = FindValIndex(LinkId, comparedLink);

	if (StartTime[ALinkIndex] > StartTime[compareLinkIndex])
	{
		return LinkSchRelations::After;
	}
	else if (StartTime[ALinkIndex] < StartTime[compareLinkIndex])
	{
		return LinkSchRelations::Before;
	}
	else if (StartTime[ALinkIndex] == StartTime[compareLinkIndex])
	{
		return LinkSchRelations::Same;
	}
	else
	{
		TRACE("Debug Get Relation of two links");
		return LinkSchRelations::None;
	}

}

