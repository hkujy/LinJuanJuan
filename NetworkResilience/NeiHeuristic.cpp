// cpp for the heuristic operators
#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>

int Select_One_Nei()
{
	return GenRandomInt(0, NumOperators-1);
}

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
