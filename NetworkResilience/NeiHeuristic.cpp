// cpp for the heuristic operators
#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
//
//int Select_One_Nei()
//{
//	return GenRandomInt(0, NumOperators-1);
//}

//main fun for generate a neighborhoods operator
void SCHCLASS::GenNei(SCHCLASS& Nei, GRAPH& g, int &OpId,const vector<int>& FailureLinkSet, const vector<double>& ResCap)
{
	//OpId = Select_One_Nei();
	//NeighourOperatorIndex = 6;
#ifdef _DEBUG
	cout << ".......selected neighbor index = " << OpId <<"........."<<endl;
#endif // _DEBUG
	switch (OpId)
	{
		case(0): Nei_Swap(Nei); break;
		case(1): Nei_CrossOver_OnePoint(Nei); break;
		case(2): Nei_Move_One_To_Left(Nei); break;
		case(3): Nei_Move_One_To_Right(Nei); break;
		case(4): Nei_Insert_One_Random_To_Left(Nei); break;
		case(5): Nei_Insert_One_Random_To_Right(Nei); break;
		case(6): Nei_Greedy_EI_Based(Nei, g,"Max"); break;
		case(7): Nei_Greedy_EI_Based(Nei, g,"Prob"); break;
	default:
		cout << "Neighbor operator index is properly set" << endl;
		system("PAUSE");
		break;
	}
	Nei.GenerateTimeFromOrder(ResCap);
	if (!Nei.isFeasible(ResCap))
	{
		cout << "*************************" << endl;
		cout << "The solution is infeasible" << endl;
		cout << "need to write repair" << endl;
		cout << "*************************" << endl;
	}
	Nei.Evaluate(g);
}

//Swap the order of two links
void SCHCLASS::Nei_Swap(SCHCLASS& NewSol) {
	// step 1: randomly generate two locations
#ifdef _DEBUG
	cout << "------------Start Swap-----------" << endl;
#endif // _DEBUG
	int locA = GenRandomInt(0, int(Links.size() - 1));
	int locB = GenRandomInt(0, int(Links.size() - 1));
	int whileCounter = 0;
	while (locA == locB)
	{
		locB = GenRandomInt(0, int(Links.size() - 1));
		++whileCounter;
		if (whileCounter > 100)
		{
			cout << "ERR: find random swap has err in counter" << endl;
		}
	}
#ifdef _DEBUG
	cout << "Before Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
#endif // _DEBUG
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);

#ifdef _DEBUG
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Swap-----------" << endl;
#endif
}

//move one to the left 
void SCHCLASS::Nei_Move_One_To_Left(SCHCLASS& NewSol)
{
	int locA = GenRandomInt(1, int(Links.size() - 1));
	int locB = locA - 1;
#ifdef _DEBUG
	cout << "------------Start Move One To Left-----------" << endl;
	cout << "Before Nei_Order_to_left: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
#endif // _DEBUG
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);
#ifdef _DEBUG
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Move One To Left-----------" << endl;
#endif // _DEBUG
}

//move one to the left 
void SCHCLASS::Nei_Move_One_To_Right(SCHCLASS& NewSol)
{
	int locA = GenRandomInt(0, int(Links.size() - 2));
	int locB = locA + 1;
#ifdef _DEBUG
	cout << "------------Start Move One To Right-----------" << endl;
	cout << "Before Nei_Order_to_right: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
#endif // _DEBUG
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);
#ifdef _DEBUG
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Move One To Right-----------" << endl;
#endif // _DEBUG
}


void SCHCLASS::Nei_CrossOver_OnePoint(SCHCLASS& NewSol)
{
	int locA = GenRandomInt(0, int(Links.size() - 1));
#ifdef _DEBUG
	cout << "-----------------Start One Point CrossOver---------" << endl;
	cout << "Cut Point = " << locA << endl;
#endif // _DEBUG
	int RightCount = int(Links.size()) - locA;
	int LeftCount = locA;

	for (size_t i = 0; i < RightCount; i++)
	{
		NewSol.Links.at(i) = this->Links.at(locA + i);
	}
	for (size_t i = 0; i < locA; i++)
	{
		NewSol.Links.at(RightCount + i) = this->Links.at(i);
	}
#ifdef _DEBUG
	cout << "-----before one point cross over" << endl;
	for (size_t i = 0; i < Links.size() - 1; i++)
	{
		cout << this->Links.at(i)->ID << ",";
	}
	cout << this->Links.back()->ID << endl;

	cout << "-----after one point cross over" << endl;
	for (size_t i = 0; i < Links.size() - 1; i++)
	{
		cout << NewSol.Links.at(i)->ID << ",";
	}
	cout << NewSol.Links.back()->ID << endl;
	cout << "------------End One Point Cross Over---------------------------" << endl;
#endif // _DEBUG

}



void SCHCLASS::Repair_Delay() // repair the solution by postpone the repair
{
	//
	cout << "repair_delay operator is called" << endl;
}

void SCHCLASS::Nei_New(SCHCLASS& NewSol, GRAPH& g,
	const vector<int>& FailureLinkSet, const vector<double>& ResCap)
{
	NewSol.GenerateIniSch(g, FailureLinkSet);
	NewSol.AlignStartTime(ResCap);
	NewSol.print();

}
void SCHCLASS::Nei_Insert_One_Random_To_Right(SCHCLASS& NewSol) // randomly insert to a position
{
#ifdef _DEBUG
	cout << "-----------------Nei Insert One Random To Right is called------------" << endl;
#endif // _DEBUG
	int cp = GenRandomInt(0, int(Links.size() - 2));
	int dest = GenRandomInt(cp,int(Links.size() - 1));
	int whileCounter = 0;
	while (dest == cp)
	{
		dest = GenRandomInt(cp, int(Links.size() - 1));
		++whileCounter;
		if (whileCounter > 100)
		{
			cout << "ERR: find random insert to the right " << endl;
		}
	}
	// step 1: for the element before the position 
	for (size_t t = 0; t < cp; t++)
	{
		NewSol.Links.at(t) = Links.at(t);
	}
	// step 2: between the cp location and destination location
	for (size_t t = cp; t < dest; t++)
	{
		NewSol.Links.at(t) = Links.at(t + 1);
	}
	NewSol.Links.at(dest) = Links.at(cp);
	for (size_t t = dest + 1; t < Links.size(); t++)
	{
		NewSol.Links.at(t) = Links.at(t);
	}

#ifdef _DEBUG
	cout << "***before***" << endl;
	for (auto l : Links) cout << l->ID << endl;
	cout << "Selected CP = " << cp << ", Dest = " << dest << endl;
	cout << "***after***" << endl;
	for (auto l : NewSol.Links) cout << l->ID << endl;
	cout << "-----------------Complete random To Right------------" << endl;
#endif // _DEBUG
}

void SCHCLASS::Nei_Insert_One_Random_To_Left(SCHCLASS& NewSol)
{
#ifdef _DEBUG
	cout << "-----------------Nei Insert One Random To Left is called------------" << endl;
#endif // _DEBUG
	int cp = GenRandomInt(1, int(Links.size() - 1)); // selected candidate not position
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
		NewSol.Links.at(t) = Links.at(t);
	}
	NewSol.Links.at(dest) = Links.at(cp);
	for (size_t t = dest+1;t<=cp;t++)
	{
		NewSol.Links.at(t) = Links.at(t - 1);
	}
	for (size_t t = cp+1;t<Links.size();t++)
	{
		NewSol.Links.at(t) = Links.at(t);
	}
#ifdef _DEBUG
	cout << "***before***" << endl;
	for (auto l : Links) cout << l->ID << endl;
	cout << "Selected CP = " << cp << ", Dest = " << dest << endl;
	cout << "***after***" << endl;
	for (auto l : NewSol.Links) cout << l->ID << endl;
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

void SCHCLASS::Nei_Greedy_EI_Based(SCHCLASS& NewSol, GRAPH& g, string sType)
{
	// step: random select a location
	int stp= GenRandomInt(0, int(Links.size() - 1));
#ifdef _DEBUG
	cout << "-----------Nei_Greedy_MaxEI is called--------" << endl;
	cout << "selected random start location value is " << stp << endl;
#endif // _DEBUG
	// copy the first stp element 
	vector<bool> InsertedFlag; InsertedFlag.assign(Links.size(), false);
	for (int i = 0; i < stp; i++)
	{
		NewSol.Links.at(i) = Links.at(i);
		InsertedFlag[i] = true;
	}
	// step 2: set the conditional graph 
	for (int i = stp; i < Links.size(); i++)
	{
		(*Links.at(i)).Cost = RemoveLinkCost;
	}
	for (int i=stp;i<Links.size();i++)
	{
		std::map<int, double> LinkEIs;
		// step 1 create a candidate set
		for (int j = stp; j < Links.size(); j++)
		{
			if (false == InsertedFlag[j])
			{
				(*Links.at(j)).Cost = (*Links.at(j)).IniCost();
				double ev = g.CalRelSpChange((*Links[j]).ID);
				LinkEIs.insert(make_pair((*Links[j]).ID, ev));
				(*Links.at(j)).Cost = RemoveLinkCost;
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
		//NewSol.Links.at(i) = Links.at(lid);
		NewSol.Links.at(i) = &g.Links.at(lid);
		for (int j = stp; j < Links.size(); j++)
		{
			if (Links.at(j)->ID == lid) 
			{
				InsertedFlag.at(j) = true;
				(*Links.at(j)).Cost = (*Links.at(j)).IniCost();
			}
		}
	}

#ifdef _DEBUG
	cout << "***before***" << endl;
	for (auto l : Links) cout << l->ID << endl;
	cout << "***after***" << endl;
	for (auto l : NewSol.Links) cout << l->ID << endl;
	cout << "-----------------Complete Greedy MaxEI------------" << endl;
#endif // _DEBUG

}
