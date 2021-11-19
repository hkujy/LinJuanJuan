#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>

void SortStartTime(vector<int>& st)
{
	std::sort(st.begin(), st.end());
}

int FindValIndex(const vector<int>& vec, int key)
{
	std::vector<int>::const_iterator itr = std::find(vec.begin(), vec.end(), key);
	if (itr != vec.cend()) {
		return std::distance(vec.begin(), itr);
		//std::cout << "Element present at index " << std::distance(v.begin(), itr);
	}
	else {
		cerr << "wtf: can not find the index of a key" << endl;
		return -1;
	}
}

void SCHCLASS::print() const
{
	cout << "*******Print and check one sol********" << endl;
	for (size_t i = 0; i < Links.size(); i++)
	{
		cout << "Link = " << Links.at(i)->ID << ", Start Time = " << StartTime.at(i) <<", End Time = "<<EndTime.at(i)<< endl;
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
		for (int t = StartTime.at(l); t <= EndTime.at(0); t++)
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
void SCHCLASS::updatePrecedingRes(int st,int et)
{
	for (int l = st; l < et; l++)
	{
		for (int t = StartTime.at(l); t < EndTime.at(l); t++)
		{
			UsedRes.at(t) += Links.at(l)->RequiredRes;
		}
	}
}

void SCHCLASS::updateResFor(int Pos)
{
	for (int t = StartTime.at(Pos); t < EndTime.at(Pos); t++)
	{
		UsedRes.at(t) += Links.at(Pos)->RequiredRes;
	}
}

//find the earliest a time with available resources
int SCHCLASS::findEarliestSt(int l, const vector<double> &ResCap) {
	
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

void SCHCLASS::AlignStartTime(const vector<double> &ResCap) {
	// shift the project to the earliest feasible time
	this->UsedRes.assign(MaxNumOfSchPeriod,0.0);
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
			StartTime.at(l) = findEarliestSt(l,ResCap);
			EndTime.at(l) = StartTime.at(l) + Links.at(l)->RecoverTime;
		}
	}
	updateResFor(this->Links.size() - 1);
}

void SCHCLASS::GenerateIniSch(GRAPH& g, const vector<int> &FailureLinks)
{
	cout << "wtf: set the ini recover time for each link to be 2" << endl;
	// step 1 generate ini number of links
	int NumOfFailureLinks = (int)FailureLinks.size();
	vector<bool> isSelected(FailureLinks.size(), false);
	do 
	{
		int linkNum = GenRandomInt(FailureLinks);
		int pos = FindValIndex(FailureLinks, linkNum);
		this->Links.push_back(new LINK());
		Links.back() = &g.Links.at(linkNum);
	} while (std::find(isSelected.begin(), isSelected.end(), false)!=isSelected.end());
	// step 1 generate ini number of links
	for (int i = 0; i < MaxNumOfSchPeriod; i++)
	{
		this->StartTime.push_back(GenRandomInt(0, MaxNumOfSchPeriod));
	}
	SortStartTime(StartTime);
	updateEndTime();
	this->print();
}

void ABCAlgorithms:: GenerateIni(GRAPH& Graph)
{	
	// improve the solutions
	for (int i = 0; i < NumEmployedBee; i++)
	{
		Sols.push_back(SCHCLASS());
		Sols.back().GenerateIniSch(Graph,FailureLinks);
	}
}