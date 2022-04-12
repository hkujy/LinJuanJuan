// cpp for the heuristic operators
#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <map>
#include "GraphElements.h"
#include "Parameters.h"

using namespace std;

int FindValIndex(const vector<int>& vec, int key);
//main fun for generate a neighborhoods operator
void ScheduleClass::GenNei(ScheduleClass& nei, GraphClass& g, 
	int &operatorId,const vector<int>& failureLinkSet, const vector<double>& resCap,
	const vector<PatternClass> &pat, enum_CompareScoreMethod &compareMethod)
{
#ifdef _DEBUG
	cout << ".......selected neighbor index = " << operatorId <<"........."<<endl;
#endif // _DEBUG
	switch (operatorId)
	{
		case(0): Nei_Swap(nei); break;
		case(1): Nei_FlipOver_OnePoint(nei); break;
		case(2): Nei_Move_One_To_Left(nei); break;
		case(3): Nei_Move_One_To_Right(nei); break;
		case(4): Nei_Insert_One_Random_To_Left(nei); break;
		case(5): Nei_Insert_One_Random_To_Right(nei); break;
		case(6): Nei_Greedy_EI_Based(nei, g,"Max"); break;
		case(7): Nei_Greedy_EI_Based(nei, g,"Prob"); break;
		//case(8): Nei_New_BasedOn_Pattern(nei,g,failureLinkSet,resCap,pat); break;
		//case(8): Nei_Swap_BasedOn_Pattern(nei,g,failureLinkSet,resCap,pat); break;
		case(8): Nei_Swap_BasedOn_PatternRelation(nei, g, failureLinkSet, resCap, pat, compareMethod); break;
	default:
		cout << "Neighbor operator index is properly set" << endl;
		system("PAUSE");
		break;
	}
	//nei.computeKey();
	nei.GenerateTimeFromOrder(resCap,g);
	if (!nei.isFeasible(resCap))
	{
		cout << "*************************" << endl;
		cout << "The solution is infeasible" << endl;
		cout << "need to write repair" << endl;
		cout << "*************************" << endl;
	}
	//nei.Evaluate(g);
}

/// <summary>
/// randomly select two nodes and swap them
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_Swap(ScheduleClass& newSol) {
	//Step 1: randomly generate two locations
#ifdef _DEBUG
	cout << "------------Start Swap-----------" << endl;
#endif // _DEBUG
	int locA = GenRandomInt(0, int(LinkId.size() - 1));
	int locB = GenRandomInt(0, int(LinkId.size() - 1));
	int whileCounter = 0;
	while (locA == locB)
	{
		locB = GenRandomInt(0, int(LinkId.size() - 1));
		++whileCounter;
		if (whileCounter > 100)
		{
			cout << "ERR: find random swap has err in counter" << endl;
		}
	}
#ifdef _DEBUG
	//cout << "Before Swap: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "Before Swap: LocA = " << newSol.LinkId.at(locA);
	//cout << ",locB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ",locB = " << newSol.LinkId.at(locB) << endl;
#endif // _DEBUG
	newSol.LinkId.at(locA) = LinkId.at(locB);
	newSol.LinkId.at(locB) = LinkId.at(locA);

#ifdef _DEBUG
	//cout << "After Swap: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "After Swap: LocA = " << newSol.LinkId.at(locA);
	//cout << ", LocB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ", LocB = " << newSol.LinkId.at(locB) << endl;
	cout << "-----------End Swap-----------" << endl;
#endif
}

/// <summary>
/// advance one node, swap it with its left
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_Move_One_To_Left(ScheduleClass& newSol)
{
	int locA = GenRandomInt(1, int(LinkId.size() - 1));
	int locB = locA - 1;
#ifdef _DEBUG
	cout << "------------Start Move One To Left-----------" << endl;
	//cout << "Before Nei_Order_to_left: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "Before Nei_Order_to_left: LocA = " << newSol.LinkId.at(locA);
	//cout << ",locB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ",locB = " << newSol.LinkId.at(locB) << endl;
#endif // _DEBUG
	newSol.LinkId.at(locA) = LinkId.at(locB);
	newSol.LinkId.at(locB) = LinkId.at(locA);
#ifdef _DEBUG
	//cout << "After Swap: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "After Swap: LocA = " << newSol.LinkId.at(locA);
	//cout << ", LocB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ", LocB = " << newSol.LinkId.at(locB) << endl;
	cout << "-----------End Move One To Left-----------" << endl;
#endif // _DEBUG
}
/// <summary>
/// delay one by moving it to its right
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_Move_One_To_Right(ScheduleClass& newSol)
{
	int locA = GenRandomInt(0, int(LinkId.size() - 2));
	int locB = locA + 1;
#ifdef _DEBUG
	cout << "------------Start Move One To Right-----------" << endl;
	//cout << "Before Nei_Order_to_right: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "Before Nei_Order_to_right: LocA = " << newSol.LinkId.at(locA);
	//cout << ",locB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ",locB = " << newSol.LinkId.at(locB) << endl;
#endif // _DEBUG
	newSol.LinkId.at(locA) = LinkId.at(locB);
	newSol.LinkId.at(locB) = LinkId.at(locA);
#ifdef _DEBUG
	//cout << "After Swap: LocA = " << newSol.LinkId.at(locA)->Id;
	cout << "After Swap: LocA = " << newSol.LinkId.at(locA);
	//cout << ", LocB = " << newSol.LinkId.at(locB)->Id << endl;
	cout << ", LocB = " << newSol.LinkId.at(locB) << endl;
	cout << "-----------End Move One To Right-----------" << endl;
#endif // _DEBUG
}

/// <summary>
/// flip the solution from one positions
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_FlipOver_OnePoint(ScheduleClass& newSol)
{
	int locA = GenRandomInt(0, int(LinkId.size() - 1));
#ifdef _DEBUG
	cout << "-----------------Start One Point CrossOver---------" << endl;
	cout << "Cut Point = " << locA << endl;
#endif // _DEBUG
	int RightCount = int(LinkId.size()) - locA;
	int LeftCount = locA;

	for (size_t i = 0; i < RightCount; i++)
	{
		newSol.LinkId.at(i) = this->LinkId.at(locA + i);
	}
	for (size_t i = 0; i < locA; i++)
	{
		newSol.LinkId.at(RightCount + i) = this->LinkId.at(i);
	}
#ifdef _DEBUG
	cout << "-----before one point cross over" << endl;
	for (size_t i = 0; i < LinkId.size() - 1; i++)
	{
		//cout << this->LinkId.at(i)->Id << ",";
		cout << this->LinkId.at(i) << ",";
	}
	//cout << this->LinkId.back()->Id << endl;
	cout << this->LinkId.back() << endl;

	cout << "-----after one point cross over" << endl;
	for (size_t i = 0; i < LinkId.size() - 1; i++)
	{
		//cout << newSol.LinkId.at(i)->Id << ",";
		cout << newSol.LinkId.at(i) << ",";
	}
	//cout << newSol.LinkId.back()->Id << endl;
	cout << newSol.LinkId.back() << endl;
	cout << "------------End One Point Cross Over---------------------------" << endl;
#endif // _DEBUG

}

void ScheduleClass::Repair_Delay() // repair the solution by postpone the repair
{
	//
	cout << "repair_delay operator is called" << endl;
}

void ScheduleClass::Nei_New(ScheduleClass& newSol, GraphClass& g,
	const vector<int>& failureLinkSet, const vector<double>& resCap)
{
	newSol.GenerateIniSch(g, failureLinkSet);
	newSol.AlignStartTime(resCap,g);
	newSol.print();
}
/// <summary>
/// randomly move one to its right 
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_Insert_One_Random_To_Right(ScheduleClass& newSol) // randomly insert to a position
{
#ifdef _DEBUG
	cout << "-----------------Nei Insert One Random To Right is called------------" << endl;
#endif // _DEBUG
	int cp = GenRandomInt(0, int(LinkId.size() - 2));
	int dest = GenRandomInt(cp,int(LinkId.size() - 1));
	int whileCounter = 0;
	while (dest == cp)
	{
		dest = GenRandomInt(cp, int(LinkId.size() - 1));
		++whileCounter;
		if (whileCounter > 100)
		{
			cout << "ERR: find random insert to the right " << endl;
		}
	}
	// step 1: for the element before the position 
	for (size_t t = 0; t < cp; t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t);
	}
	// step 2: between the cp location and destination location
	for (size_t t = cp; t < dest; t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t + 1);
	}
	newSol.LinkId.at(dest) = LinkId.at(cp);
	for (size_t t = dest + 1; t < LinkId.size(); t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t);
	}

#ifdef _DEBUG
	cout << "***before***" << endl;
	//for (auto l : LinkId) cout << l->Id << endl;
	for (auto l : LinkId) cout << l << endl;
	cout << "Selected CP = " << cp << ", Dest = " << dest << endl;
	cout << "***after***" << endl;
	//for (auto l : newSol.LinkId) cout << l->Id << endl;
	for (auto l : newSol.LinkId) cout << l << endl;
	cout << "-----------------Complete random To Right------------" << endl;
#endif // _DEBUG
}
/// <summary>
/// randomly insert it to its left 
/// </summary>
/// <param name="newSol"></param>
void ScheduleClass::Nei_Insert_One_Random_To_Left(ScheduleClass& newSol)
{
#ifdef _DEBUG
	cout << "-----------------Nei Insert One Random To Left is called------------" << endl;
#endif // _DEBUG
	int cp = GenRandomInt(1, int(LinkId.size() - 1)); // selected candidate not position
	int dest = GenRandomInt(0, cp);
	int whileCounter = 0;
	while (dest == cp)
	{
		dest = GenRandomInt(0, cp);
		++whileCounter;
		if (whileCounter > 100)
		{
			cout << "ERR: find random insert to the left " << endl;
		}
	}
	for (size_t t = 0; t < dest; t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t);
	}
	newSol.LinkId.at(dest) = LinkId.at(cp);
	for (size_t t = dest+1;t<=cp;t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t - 1);
	}
	for (size_t t = cp+1;t<LinkId.size();t++)
	{
		newSol.LinkId.at(t) = LinkId.at(t);
	}
#ifdef _DEBUG
	cout << "***before***" << endl;
	//for (auto l : LinkId) cout << l->Id << endl;
	for (auto l : LinkId) cout << l << endl;
	cout << "Selected CP = " << cp << ", Dest = " << dest << endl;
	cout << "***after***" << endl;
	//for (auto l : newSol.LinkId) cout << l->Id << endl;
	for (auto l : newSol.LinkId) cout << l << endl;
	cout << "-----------------Complete Random To Left------------" << endl;
#endif // _DEBUG
}

template<typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> get_max(const std::map<KeyType, ValueType>& x) {
	using pairtype = std::pair<KeyType, ValueType>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype& p1, const pairtype& p2) {
		return p1.second < p2.second;
		});
}

int RouletteSelect(const vector<double> &cumProb);
/// <summary>
/// return the first element of selected map value
/// </summary>
/// <param name="m_Lid2EI"></param>
/// <returns></returns>
int SectOneFromMap(const std::map<int, double> &m_Lid2EI)
{
	if (m_Lid2EI.size() == 1) // if it equals one
	{
		return m_Lid2EI.begin()->first;
	}
	vector<double> pro(m_Lid2EI.size(), 0.0);
	vector<double> cumProb(m_Lid2EI.size(), 0.0);// ini the cumulate pro
	double sum = 0;
	for (auto it : m_Lid2EI) {
		sum += it.second;
	}
	int counter = 0;
	for (auto it : m_Lid2EI) {
		pro.at(counter) = it.second / sum;
		counter++;
	}
	cumProb.at(0) = 0.0;
    std::map<int, double>::iterator itr;
	counter = 0;
	for (auto itr:m_Lid2EI)
	{
		cumProb[counter + 1] = cumProb[counter] + pro[counter];
		counter++;
		if (counter == m_Lid2EI.size() - 1) break;
	}
	int SelectedNum = -1;
	SelectedNum = RouletteSelect(cumProb);
	counter = 0;

	for (auto itr : m_Lid2EI)
	{
		if (counter == SelectedNum)
		{
			return itr.first;
		}
		counter++;
	}
	cout << "C++ Warning: can not find a number from SectOneFromMap" << endl;
	return -1;
}

/// <summary>
/// greedy generate the following nodes based on the efficiency index
/// </summary>
/// <param name="newSol"></param>
/// <param name="g"></param>
/// <param name="sType">
/// stype is "max" or "prob", where max is based on the maximu prob and prob is rooleet method 
/// </param>
void ScheduleClass::Nei_Greedy_EI_Based(ScheduleClass& newSol, GraphClass& g, string sType)
{
	// step: random select a location
	int stp= GenRandomInt(0, int(LinkId.size() - 1));
#ifdef _DEBUG
	cout << "-----------Nei_Greedy_MaxEI is called--------" << endl;
	cout << "selected random start location value is " << stp << endl;
#endif // _DEBUG
	// copy the first stp element 
	vector<bool> InsertedFlag; InsertedFlag.assign(LinkId.size(), false);
	for (int i = 0; i < stp; i++)
	{
		newSol.LinkId.at(i) = LinkId.at(i);
		InsertedFlag[i] = true;
	}
	// step 2: set the conditional graph 
	for (int i = stp; i < LinkId.size(); i++)
	{
		g.Links.at(LinkId[i]).Cost = REMOVE_LINK_COST;
		//(*LinkId.at(i)).Cost = REMOVE_LINK_COST;
	}
	for (int i=stp;i<LinkId.size();i++)
	{
		std::map<int, double> LinkEIs;
		// step 1 create a candidate set
		for (int j = stp; j < LinkId.size(); j++)
		{
			if (false == InsertedFlag[j])
			{
				g.Links.at(LinkId[j]).Cost = g.Links.at(LinkId[j]).GetIniCost();
				//(*LinkId.at(j)).Cost = (*LinkId.at(j)).GetIniCost();
				//double ev = g.CalRelSpChange((*LinkId[j]).Id);
				double ev = g.CalRelSpChange(LinkId[j]);
				//LinkEIs.insert(make_pair((*LinkId[j]).Id, ev));
				LinkEIs.insert(make_pair(LinkId[j], ev));
				//(*LinkId.at(j)).Cost = REMOVE_LINK_COST;
				g.Links.at(LinkId[j]).Cost = REMOVE_LINK_COST;
				//(*LinkId.at(j)).Cost = REMOVE_LINK_COST;
			}
		}
		int lid = -1;
		if (sType._Equal("Max"))
		{
			lid = get_max(LinkEIs).first;
#ifdef _DEBUG
			cout << "max ei element is " << get_max(LinkEIs).first << endl;
#endif // _DEBUG
		}
		else if (sType._Equal("Prob"))
		{
			lid = SectOneFromMap(LinkEIs);
#ifdef _DEBUG
			cout << "selected prob ei element is " << lid << endl;
#endif // _DEBUG
		}
		else
			cout << "need to specific the input of sType for EI based operator" << endl;
		//newSol.Links.at(i) = Links.at(lid);
		newSol.LinkId.at(i) = lid;
		//newSol.LinkId.at(i) = &g.Links.at(lid);
		for (int j = stp; j < LinkId.size(); j++)
		{
			//if (LinkId.at(j)->Id == lid) 
			if (LinkId.at(j) == lid) 
			{
				InsertedFlag.at(j) = true;
				//(*LinkId.at(j)).Cost = (*LinkId.at(j)).GetIniCost();
				g.Links.at(LinkId[j]).Cost = g.Links.at(LinkId[j]).GetIniCost();
				//(*LinkId.at(j)).Cost = (*LinkId.at(j)).GetIniCost();
			}
		}
	}

#ifdef _DEBUG
	cout << "***before***" << endl;
	//for (auto l : LinkId) cout << l->Id << endl;
	for (auto l : LinkId) cout << l << endl;
	cout << "***after***" << endl;
	//for (auto l : newSol.LinkId) cout << l->Id << endl;
	for (auto l : newSol.LinkId) cout << l << endl;
	cout << "-----------------Complete Greedy MaxEI------------" << endl;
#endif // _DEBUG
}

/// <summary>
/// Generate new solution based on the solution pattern 
/// </summary>
/// <param name="newSol"></param>
/// <param name="g"></param>
/// <param name="failureLinkSet"></param>
/// <param name="resCap"></param>
/// <param name="pat"></param>
void ScheduleClass::Nei_New_BasedOn_Pattern(ScheduleClass& newSol, GraphClass& g,
	const vector<int>& failureLinkSet, const vector<double>& resCap,
	const vector<PatternClass>& pat)
{
	newSol.GenerateIniBasedOnPattern(g,failureLinkSet,pat);
	newSol.AlignStartTime(resCap,g);
#ifdef _DEBUG
	cout << "-------Generate new solution based on pattern finishes and Print the sol" << endl;
	newSol.print();
#endif // _DEBUG
}
// input the vector pat and link Id 
// return the location in the pat vector for the corresponding linkid
int getPatLoc(const vector<PatternClass> &pat, const int lid)
{
	for (int l = 0; l < pat.size(); l++)
	{
		if (pat[l].LinkId == lid) return l;
	}
	return 0;
}


//TODO
//Nei:hybrid with swap and pattern
//only swap the pattern with higher probability
void ScheduleClass::Nei_Swap_BasedOn_Pattern(ScheduleClass& newSol, GraphClass& g, const vector<int>& failureLinkSet,
	const vector<double>& resCap, const vector<PatternClass>& pat)
{
		//Step 1: randomly generate two locations
#ifdef _DEBUG
		cout << "------------Start Swap-----------" << endl;
#endif // _DEBUG
		bool isSwap = false;
		int swapWhileCounter = 0;
		int locA = -1;
		int locB = -1;
		while (isSwap==false)
		{
			locA = GenRandomInt(0, int(LinkId.size() - 1));
			locB = GenRandomInt(0, int(LinkId.size() - 1));
			int whileCounter = 0;
			while (locA == locB)
			{
				locB = GenRandomInt(0, int(LinkId.size() - 1));
				++whileCounter;
				if (whileCounter > 100)
				{
					std::cout << "ERR: find random swap has err in counter" << endl;
				}
			}
			int LinkAId = LinkId[locA];
			int LinkBId = LinkId[locB];

			int patAloc = getPatLoc(pat, LinkAId);
			int patBloc = getPatLoc(pat, LinkBId);

			double ScoreA2B = pat[patAloc].AbsScore[patBloc];
			double ScoreB2A = pat[patBloc].AbsScore[patAloc];

			if (locA < locB)   /// A is the first node
			{
				if (ScoreA2B <= ScoreB2A) isSwap = true;
			}
			else   // A is the second node after B
			{
				if (ScoreA2B >= ScoreB2A) isSwap = true;
			}
			swapWhileCounter++;
			if (swapWhileCounter > 100)
			{
				std::cout << "ERR: find random swap based on the pattern value, and there is an err in counter" << endl;
			}
		}
#ifdef _DEBUG
		//cout << "Before Swap: LocA = " << newSol.LinkId.at(locA)->Id;
		cout << "Before Swap: LocA = " << newSol.LinkId.at(locA);
		//cout << ",locB = " << newSol.LinkId.at(locB)->Id << endl;
		cout << ",locB = " << newSol.LinkId.at(locB) << endl;
#endif // _DEBUG
		newSol.LinkId.at(locA) = LinkId.at(locB);
		newSol.LinkId.at(locB) = LinkId.at(locA);

#ifdef _DEBUG
		//cout << "After Swap: LocA = " << newSol.LinkId.at(locA)->Id;
		cout << "After Swap: LocA = " << newSol.LinkId.at(locA);
		//cout << ", LocB = " << newSol.LinkId.at(locB)->Id << endl;
		cout << ", LocB = " << newSol.LinkId.at(locB) << endl;
		cout << "-----------End Swap-----------" << endl;
#endif
	}

bool isEqual(LinkSchRelations& rhs, LinkSchRelations& lhs)
{
	if (rhs == lhs) return true;
	else return false;
}
// update the relationship based on the patten relationship 
void ScheduleClass::Nei_Swap_BasedOn_PatternRelation(ScheduleClass& newSol, GraphClass& g, const vector<int>& failureLinkSet,
	const vector<double>& resCap, const vector<PatternClass>& pat,enum_CompareScoreMethod &compareMethod)
{
	//Step 1: randomly generate two locations
#ifdef _DEBUG
	cout << "------------Start Swap-----------" << endl;
#endif // _DEBUG
	bool isSwap = false;
	int swapWhileCounter = 0;
	int locA = -1;
	int locB = -1;
	while (isSwap == false)
	{
		// step 1 generate two different nodes
		locA = GenRandomInt(0, int(LinkId.size() - 1));
		locB = GenRandomInt(0, int(LinkId.size() - 1));
		int whileCounter = 0;
		while (locA == locB)
		{
			locB = GenRandomInt(0, int(LinkId.size() - 1));
			++whileCounter;
			if (whileCounter > 100)
			{
				std::cout << "ERR: find random swap has err in counter" << endl;
			}
		}
		int LinkA = LinkId[locA];
		int LinkB = LinkId[locB];

		// step 2: check their current relationship
		LinkSchRelations r = this->getRelation(LinkA, LinkB);
		// step 3: check their dominate relationship 
		LinkSchRelations domr = this->findDominantRelation(LinkA, LinkB, pat, compareMethod);
	
		// in the following, swap the two only if their dominated relationship is different 
		if (isEqual(r, domr))
		{
			if (r == LinkSchRelations::Same)
			{
				isSwap = true;
			}
			else isSwap = false;
		}
		else
		{
			if (domr==LinkSchRelations::After)
			{
				if (locA < locB) isSwap = true;
				else isSwap = false;
			}
			else if (domr == LinkSchRelations::Before)
			{
				if (locA > locB) isSwap = true;
				else isSwap = false;
			}
			else if (domr == LinkSchRelations::noDominated)
			{
				isSwap = true;
			}
			else
			{
				TRACE("UnDetectedCase");
			}
		}

		swapWhileCounter++;
		if (swapWhileCounter > 100)
		{
			std::cout << "ERR: find random swap based on the pattern value, and there is an err in counter" << endl;
		}
	}
#ifdef _DEBUG
	cout << "Before Swap: LocALink = " << newSol.LinkId.at(locA);
	cout << ", LocBLink = " << newSol.LinkId.at(locB) << endl;
#endif // _DEBUG
	newSol.LinkId.at(locA) = LinkId.at(locB);
	newSol.LinkId.at(locB) = LinkId.at(locA);

#ifdef _DEBUG
	cout << "After Swap: LocALink = " << newSol.LinkId.at(locA);
	cout << ", LocBLink = " << newSol.LinkId.at(locB) << endl;
	cout << "-----------End Swap-----------" << endl;
#endif
}
