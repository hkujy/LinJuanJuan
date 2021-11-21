#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "ABC.h"
using namespace std;

void ABCAlgorithms::ABCMain(GRAPH& Graph)
{
	//step 1: generate initial solutions
	GenerateIni(Graph);
	//step 2: call employ bee phase
	EmployBee(Graph);
}


void ABCAlgorithms::GenerateIni(GRAPH& Graph)
{
	// improve the solutions
	for (int i = 0; i < NumEmployedBee; i++)
	{
		cout << "-----------------------EB = " << i << "-------------------" << endl;
		//SCHCLASS news;
		Sols.push_back(SCHCLASS(i));
		Sols.back().GenerateIniSch(Graph, FailureLinks);
		Sols.back().AlignStartTime(ResourceCap);
		cout << "----------Print solution after solution alignment--------" << endl;
		Sols.back().print();
		cout << "----------End print solution after solution alignment--------" << endl;
		Sols.back().Evaluate(Graph);
	}
	for (auto s : Sols) cout << s.ID << "," << s.TotalCost << endl;
}



void ABCAlgorithms::EmployBee(GRAPH& Graph)
{
	for (int i = 0; i < NumEmployedBee; i++)
	{
		this->Sols.at(i).GenNei(Graph, FailureLinks, ResourceCap);
	}

}