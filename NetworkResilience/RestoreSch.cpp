#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>

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
		cerr << "wtf: can not find the index of a key" << endl;
		return -1;
	}
}

//generate the time from order to time
void SCHCLASS::GenerateTimeFromOrder(const vector<double>& ResCap)
{
	AlignStartTime(ResCap);
}
void SCHCLASS::print() const
{
	cout << "*******Print and check one sol********" << endl;
	for (size_t i = 0; i < Links.size(); i++)
	{
		cout << "Link = " << Links.at(i)->ID << ", Start Time = " << StartTime.at(i) << ", End Time = " << EndTime.at(i) << endl;
	}
	cout << "********End*************************" << endl;
}

Scenario SCHCLASS::ConvertToScenario() {
	Scenario s;
	for (auto l : Links) s.LinkIds.push_back(l->ID);
	for (auto t : StartTime) s.tau.push_back(t);
	return s;
}

void SCHCLASS::getRes()
{
	UsedRes.assign(MaxNumOfSchPeriod, 0);

	for (size_t l = 0; l < Links.size(); l++)
	{
		for (int t = StartTime.at(l); t < EndTime.at(l); t++)
		{
			UsedRes.at(t) += Links.at(l)->RequiredRes;
		}
	}
}
void SCHCLASS::updateEndTime()
{
	for (int l = 0; l < Links.size(); l++)
	{
		EndTime.at(l) = StartTime.at(l) + Links.at(l)->RecoverTime;
	}
}

bool SCHCLASS::isFeasible(const vector<double>& res)
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
void SCHCLASS::updatePrecedingRes(size_t st, size_t et)
{
	for (size_t l = st; l < et; l++)
	{
		for (size_t t = StartTime.at(l); t < EndTime.at(l); t++)
		{
			UsedRes.at(t) += Links.at(l)->RequiredRes;
		}
	}
}

void SCHCLASS::updateResFor(size_t Pos)
{
	for (int t = StartTime.at(Pos); t < EndTime.at(Pos); t++)
	{
		UsedRes.at(t) += Links.at(Pos)->RequiredRes;
	}
}

//find the earliest a time with available resources
int SCHCLASS::findEarliestFeasibleSt(size_t l, const vector<double>& ResCap) {

	for (int t = 0; t < MaxNumOfSchPeriod; t++)
	{
		if (UsedRes.at(t) + this->Links.at(l)->RequiredRes <= ResCap.at(t))
		{
			return t;
		}
	}
	cout << "ERR: can not find the earliest start time for project " << l << endl;
	return -1;
}

int SCHCLASS::findEarliestInFeasibleSt(const vector<double>& ResCap) {

	for (int t = 0; t < MaxNumOfSchPeriod; t++)
	{
		if (UsedRes.at(t) > ResCap.at(t))
		{
			return t;
		}
	}
	return -1;
}

void SCHCLASS::AlignStartTime(const vector<double>& ResCap) {
	// shift the project to the earliest feasible time
	this->UsedRes.assign(MaxNumOfSchPeriod, 0.0);
	for (size_t l = 0; l < Links.size(); l++)
	{
		if (l == 0)
		{
			StartTime.at(l) = 0;
			EndTime.at(l) = StartTime.at(l) + Links.at(l)->RecoverTime;
		}
		else
		{
			updatePrecedingRes(l - 1, l);
			StartTime.at(l) = findEarliestFeasibleSt(l, ResCap);
			EndTime.at(l) = StartTime.at(l) + Links.at(l)->RecoverTime;
		}
	}
	updateResFor(this->Links.size() - 1);
}

void SCHCLASS::GenerateIniSch(GRAPH& g, const vector<int>& FailureLinks)
{
	if (this->Links.size() > 0)
	{
		Links.clear(); StartTime.clear(); EndTime.clear();
	}
	// step 1 generate ini number of links
	int NumOfFailureLinks = (int)FailureLinks.size();
	vector<bool> isSelected(FailureLinks.size(), false);
	int CountDoWhile = 0;
	do
	{
		int linkNum = GenRandomInt(FailureLinks);
		int pos = FindValIndex(FailureLinks, linkNum);
		cout << "Link = " << linkNum << ", pos=" << pos << endl;
		if (!isSelected.at(pos))
		{
			this->Links.push_back(new LINK());
			Links.back() = &g.Links.at(linkNum);
			isSelected.at(pos) = true;
		}
		CountDoWhile++;
		if (CountDoWhile > 1000)
		{
			cout << "ERR: Do while count is larger than 1000" << endl;
			system("PAUSE");
		}
	} while (std::find(isSelected.begin(), isSelected.end(), false) != isSelected.end());
	// step 1 generate ini number of links
	EndTime.assign(FailureLinks.size(), -1);
	StartTime.assign(FailureLinks.size(), -1);
	for (int i = 0; i < FailureLinks.size(); i++)
	{
		this->StartTime.at(i) = GenRandomInt(0, MaxNumOfSchPeriod);
	}
	SortStartTime(StartTime);
	updateEndTime();
	this->print();
}

vector<size_t> SCHCLASS::getNewReadyLinks(int tau)
{

	vector<size_t> results;
	if (tau == 0) return results;
	else
	{
		for (size_t l = 0; l < Links.size(); l++)
		{
			if (EndTime.at(l) == tau)
			{
				results.push_back(l);
			}
		}
	}

	return results;
}

void SCHCLASS::Evaluate(GRAPH& g)
{
	Fitness = 0.0;
	vector<double> TravelTime;  // travel time for each period 
	vector<size_t> CumulativeReadyLinks;
	TravelTime.assign(GetLastPeriod(), 0);
	// step 1: set all the capacity of failure links to be 0.0 and evalute the total cost in tau = 0
	for (size_t l = 0; l < Links.size(); l++)
	{
		Links.at(l)->CaRevise = Zero;
	}
	g.EvaluteGraph();

	// step 2: compute the time in each period
	for (int t = 0; t < GetLastPeriod() + 1; t++)
	{
		cout << "time = " << t << endl;
		vector<size_t> NewReady = getNewReadyLinks(t);
		for (auto v : NewReady) cout << v << ",";
		cout << endl;
		// case 1: at the begin, there is not other links
		if (NewReady.size() == 0)
		{
			if (t == 0) TravelTime.at(t) = g.TotalSystemCost;
			else
			{
				TravelTime.at(t) = TravelTime.at(t - 1);
			}
			cout << "---Period = " << t << ", no link is added" << endl;
		}
		else
		{
			for (auto l : NewReady) Links.at(l)->IniCap();
			cout << "---Period = " << t << "," << NewReady.size() << " link is added" << endl;
			g.EvaluteGraph();
			CumulativeReadyLinks.insert(CumulativeReadyLinks.end(), NewReady.begin(), NewReady.end());
			if (t == GetLastPeriod()) TravelTime.push_back(g.TotalSystemCost);
			else TravelTime.at(t) = g.TotalSystemCost;
		}
	}
	cout << "-------summary of travel time----------" << endl;
	cout << "Last end time = " << GetLastPeriod() << endl;
	for (int t = 0; t < TravelTime.size(); t++)
	{
		cout << "t = " << t << ", TravelTime = " << TravelTime.at(t) << endl;
	}
	this->Fitness = std::accumulate(TravelTime.begin(), TravelTime.end(), 0.0);
	cout << "total cost = " << this->Fitness << endl;
}

