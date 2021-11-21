#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "ABC.h"
#include <assert.h>     /* assert */
using namespace std;

void ABCAlgorithms::ABCMain(GRAPH& Graph)
{
	//step 1: generate initial solutions
	GlobalBest.Fitness = 9999999;
	GenerateIni(Graph);
	//step 2: call employ bee phase
	for (int i = 0; i < MaxIter; i++)
	{
		EmployBeePhase(Graph);
		GetProb();
		OnlookerPhase(Graph);
		ScoutPhase(Graph);
	}
	cout << "*************************ABC completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
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
		if (Sols.back().Fitness < GlobalBest.Fitness)
		{
			GlobalBest = Sols.back();
		}
	}
	for (auto s : Sols) cout << s.ID << "," << s.Fitness << endl;
}



void ABCAlgorithms::EmployBeePhase(GRAPH& Graph)
{
	for (int i = 0; i < NumEmployedBee; i++)
	{
		SCHCLASS Nei(this->Sols.at(i));
		cout << "Eb = " << i << endl;
		this->Sols.at(i).GenNei(Nei,Graph, FailureLinks, ResourceCap);
		if (Nei.Fitness< this->Sols.at(i).Fitness)
		{
			cout << this->Sols.at(i).Fitness << "," << Nei.Fitness << endl;
			Nei.print();
			this->Sols.at(i).print();
			this->Sols.at(i) = Nei;
		}
		else
		{
			ScountCounter.at(i)++;
		}
		if (Nei.Fitness < GlobalBest.Fitness) GlobalBest = Nei;
	}
}

void ABCAlgorithms::OnlookerPhase(GRAPH& Graph)
{
	for (int i = 0; i < NumOnlookers; i++)
	{
		size_t Selected = Select_Basedon_Prob();
		SCHCLASS Nei(this->Sols.at(Selected));
		this->Sols.at(Selected).GenNei(Nei,Graph, FailureLinks, ResourceCap);

		if (Nei.Fitness < this->Sols.at(Selected).Fitness)
		{
			cout << this->Sols.at(Selected).Fitness << "," << Nei.Fitness << endl;
			Nei.print();
			this->Sols.at(Selected).print();
			this->Sols.at(Selected) = Nei;
		}
		else
		{
			ScountCounter.at(Selected)++;
		}
		if (Nei.Fitness < GlobalBest.Fitness) GlobalBest = Nei;
	}
}

void ABCAlgorithms::ScoutPhase(GRAPH& Graph)
{
	for (size_t t=0;t<NumEmployedBee;t++)
	{
		if (ScountCounter.at(t)> MaxScountCount)
		{
			this->Sols.at(t).GenerateIniSch(Graph,FailureLinks);
			this->Sols.at(t).AlignStartTime(ResourceCap);
			this->Sols.at(t).Evaluate(Graph);
			if (this->Sols.at(t).Fitness < GlobalBest.Fitness)
			{
				GlobalBest = this->Sols.at(t);
			}
			ScountCounter.at(t) = 0;
		}
	}
}

//get the fitness probability vector
void ABCAlgorithms::GetProb()  // compute probability based on the fitness values
{
	double sumFit = 0.0;
	for (auto s : Sols) sumFit += s.Fitness;
	assert(sumFit > 0);
	Prob.at(0) = 0.0;
	for (size_t i = 0; i < NumEmployedBee-1; i++)
	{
		Prob.at(i + 1) = Prob.at(i) + this->Sols.at(i).Fitness / sumFit;
	}
}

size_t ABCAlgorithms::Select_Basedon_Prob()
{
	size_t selected = -1;
	double f = GenRandomReal();
	for (size_t i = 0; i < Prob.size() - 1; i++)
	{
		if (f >= Prob.at(i) && f < Prob.at(i + 1))
		{
			selected = static_cast<int>(i);
			break;
		}
	}
	if (f >= Prob.back()) selected = Prob.size() - 1;
	assert(selected >= 0);
	return selected;
}

void ABCAlgorithms::ReadData()
{
	//read abc para
	//read failure link para


}