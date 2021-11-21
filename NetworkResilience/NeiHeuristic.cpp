// cpp for the heuristic operators
#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>

int Select_One_Nei()
{
	return GenRandomInt(1, 4);
}

//main fun for generate a nei
void SCHCLASS::GenNei(GRAPH& g, const vector<int>& FailureLinkSet, const vector<double>& ResCap)
{
	//Step 1: Create a neighbor solutions
	SCHCLASS nei(*this);
	if (nei.findEarliestInFeasibleSt(ResCap) >= 0) cout << "ini solution is infeasible" << endl;
	//Step 2: Call Neighbour Operator
	int NeighourOperatorIndex = Select_One_Nei();
	switch (NeighourOperatorIndex)
	{
	case(1):Nei_Swap(nei);
		break;
	case(2):
		Nei_CrossOver_OnePoint(nei);
		break;
	case(3):
		Nei_Move_One_To_Left(nei);
		break;
	case(4):
		Nei_Move_One_To_Right(nei);
		break;
	default:
		cout << "Neighbor operator index is properly set" << endl;
		system("PAUSE");
		break;
	}
	nei.GenerateTimeFromOrder(ResCap);
	if (!nei.isFeasible(ResCap))
	{
		cout << "*************************" << endl;
		cout << "The solution is infeasible" << endl;
		cout << "need to write repair" << endl;
		cout << "*************************" << endl;
	}
	nei.Evaluate(g);
	
	cout << "************Compare Obje************" << endl;
	if (this->TotalCost != nei.TotalCost)
	{
		cout << this->TotalCost << "," << nei.TotalCost << endl;
		nei.print();
		this->print();
	}
	

}

//Swap the order of two links
void SCHCLASS::Nei_Swap(SCHCLASS& NewSol) {
	// step 1: randomly generate two locations
	cout << "------------Start Swap-----------" << endl;
	size_t locA = GenRandomInt(0, Links.size() - 1);
	size_t locB = GenRandomInt(0, Links.size() - 1);
	int whileCounter = 0;
	while (locA == locB)
	{
		locB = GenRandomInt(0, Links.size() - 1);
		++whileCounter;
		if (whileCounter>100)
		{
			cout << "ERR: find random swap has err in counter" << endl;
		}
	}
	cout << "Before Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Swap-----------" << endl;
}

//move one to the left 
void SCHCLASS::Nei_Move_One_To_Left(SCHCLASS& NewSol)
{

	cout << "------------Start Move One To Left-----------" << endl;
	size_t locA = GenRandomInt(1, Links.size() - 1);
	size_t locB = locA - 1;
	cout << "Before Nei_Order_to_left: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Move One To Left-----------" << endl;
}

//move one to the left 
void SCHCLASS::Nei_Move_One_To_Right(SCHCLASS& NewSol)
{
	cout << "------------Start Move One To Right-----------" << endl;
	size_t locA = GenRandomInt(0, Links.size() - 2);
	size_t locB = locA + 1;
	cout << "Before Nei_Order_to_right: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ",locB = " << NewSol.Links.at(locB)->ID << endl;
	NewSol.Links.at(locA) = Links.at(locB);
	NewSol.Links.at(locB) = Links.at(locA);
	cout << "After Swap: LocA = " << NewSol.Links.at(locA)->ID;
	cout << ", LocB = " << NewSol.Links.at(locB)->ID << endl;
	cout << "-----------End Move One To Right-----------" << endl;
}


void SCHCLASS::Nei_CrossOver_OnePoint(SCHCLASS& NewSol)
{
	cout << "-----------------Start One Point CrossOver---------" << endl;
	size_t locA = GenRandomInt(0, Links.size()-1);
	cout << "Cut Point = " << locA << endl;
	size_t RightCount = Links.size() - locA;
	size_t LeftCount = locA;
	
	for (size_t i = 0; i < RightCount; i++)
	{
		NewSol.Links.at(i) = this->Links.at(locA + i);
	}
	for (size_t i = 0; i < locA; i++)
	{
		NewSol.Links.at(RightCount + i) = this->Links.at(i);
	}

	cout << "-----before one point cross over" << endl;
	for (size_t i = 0; i<Links.size()-1;i++)
	{
		cout << this->Links.at(i)->ID << ",";
	}
	cout << this->Links.back()->ID << endl;

	cout << "-----after one point cross over" << endl;
	for (size_t i = 0; i<Links.size()-1;i++)
	{
		cout << NewSol.Links.at(i)->ID << ",";
	}
	cout <<NewSol.Links.back()->ID<< endl;

	cout << "------------End One Point Cross Over---------------------------" << endl;

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
