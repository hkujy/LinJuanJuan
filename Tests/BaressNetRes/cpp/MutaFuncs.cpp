#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
#include "RandomFuncs.h"
using namespace std;

void FirstProcedureFunc(CHROME &Chrom, vector<double> DofVarSet){
	for (unsigned int i = 0; i < Chrom.NodeDof.size(); i++)
	{
		Chrom.NodeDof.at(i) = GenRandomFloat(DofVarSet);
	}
}
void addNewNodeFunc(CHROME &Chrom, vector<double> DofVarSet)
{
	// case 1 if node == number of node change select a node with zeor and assing a possitive 
	if (Chrom.Nodes.size() == NumNodes)
	{
		vector<int> ZeroDofNodeSet;
		for (unsigned int i = 0; i < Chrom.NodeDof.size(); i++)
		{
			if (isEqual(Chrom.NodeDof.at(i), 0.0f))
			{
				ZeroDofNodeSet.push_back(Chrom.Nodes.at(i));
			}
		}
		if (ZeroDofNodeSet.size() == 0)
		{
			int Rnode = GenRandomInt(Chrom.Nodes);// random select a node
			double OldValue = Chrom.NodeDof.at(Rnode);
			do
			{
				Chrom.NodeDof.at(Rnode) = GenRandomFloat(DofVarSet);
			} while (isEqual(OldValue, Chrom.NodeDof.at(Rnode)));
		}
		else
		{
			int Rnode = GenRandomInt(ZeroDofNodeSet);// random select a node
			int pos = InvaildInt;
			for (unsigned int i = 0; i < Chrom.Nodes.size(); i++)
			{
				if (Chrom.Nodes.at(i) == Rnode)
				{
					do
					{
						Chrom.NodeDof.at(i) = GenRandomFloat(DofVarSet);
					} while (isEqual(Chrom.NodeDof.at(i), 0.0f));
				}
			}
		}
	}
	else
	{
		// case 1 if node < number 
		// then select a node and add it the vector
		// step find missing node
		vector<bool> NodeStatus(NumNodes, false);
		for (auto i = Chrom.Nodes.begin(); i != Chrom.Nodes.end(); i++)
		{
			NodeStatus.at(*i) = true;
		}
		vector<int> CandiSet;
		for (unsigned int i = 0; i < NodeStatus.size(); i++)
		{
			if (!NodeStatus.at(i))
			{
				CandiSet.push_back(i);
			}
		}
		Chrom.Nodes.push_back(GenRandomInt(CandiSet));
		double dof = 0.0f;
		do
		{
			dof = GenRandomFloat(DofVarSet);
		} while (isEqual(dof, 0.0f));
		Chrom.NodeDofProb.push_back(dof);
	}
}

void removeNodeDofFunc(CHROME &Chrom){
	try
	{
		int Pos = GenRandomPos((int)Chrom.Nodes.size());// 0 to size
		Chrom.NodeDof.at(Pos) = 0.0f;
	}
	catch (exception &e){
		TRACE("%s", e);
	}
}
void exchangeNodeDofFunc(CHROME &Chrom, const vector<double> &DofVarSet){
	vector<int> ZeroSet, PostiveSet;
	ZeroSet.reserve(NumNodes); PostiveSet.reserve(NumNodes);
	for (unsigned int i = 0; i < Chrom.NodeDof.size(); i++)
	{
		if (isEqual(Chrom.NodeDof.at(i), 0.0f)) ZeroSet.push_back(i);
		else PostiveSet.push_back(i);
	}
	int A, B;
	if (ZeroSet.size()==0)
	{
		B = GenRandomInt(PostiveSet);
		Chrom.NodeDofProb.at(B) = 0.0f;
	}
	if (PostiveSet.size()==0)
	{
		A = GenRandomInt(ZeroSet);
		Chrom.NodeDofProb.at(A) = GenRandomFloat(DofVarSet);
	}
	if (PostiveSet.size()!=0&&ZeroSet.size()!=0)
	{
		A = GenRandomInt(ZeroSet);
		B = GenRandomInt(PostiveSet);

		Chrom.NodeDofProb.at(A) = Chrom.NodeDofProb.at(B);
		Chrom.NodeDofProb.at(B) = 0.0f;
	}
	
}

void increasdof(double &NowDof, vector<double> DofVarSet){
	if (isEqual(NowDof, DofVarSet.back())) return;
	for (unsigned int i = 0; i < DofVarSet.size();i++)
	{
		if (isEqual(NowDof,DofVarSet.at(i)))
		{
			NowDof = DofVarSet.at(i + 1);
			return;
		}
	}
}
void decreaseDof(double &NowDof, vector<double> DofVarSet){
	if (isEqual(NowDof, *DofVarSet.begin())) return;
	for (unsigned int i = 0; i < DofVarSet.size();i++)
	{
		if (isEqual(NowDof,DofVarSet.at(i)))
		{
			NowDof = DofVarSet.at(i - 1);
		}
	}
}


void Algorithms::FirstProcedure(CHROME& Chrom) {
	FirstProcedureFunc(Chrom, this->NodeDofVarSet);
}

void Algorithms::addNewNode(CHROME& Chrom) {
	addNewNodeFunc(Chrom, this->NodeDofVarSet);
}

void Algorithms::removeNodeDof(CHROME& Chrom) {
	removeNodeDofFunc(Chrom);
}

void Algorithms::exchangeNodeDof(CHROME& Chrom) {
	exchangeNodeDofFunc(Chrom, this->NodeDofVarSet);
}

void Algorithms::SecondProcedure(CHROME& Chrom, double Ratio) {

	int NumOfRepeate = (int)((double)NumNodes * Ratio);
	std::vector<int> v = { 1, 2, 3 }; //a,b,c, three operators
	for (int i = 0; i < NumOfRepeate; i++)
	{
		int Index = GenRandomInt(v);
		//cout << "index = " << Index<<endl;
		switch (Index)
		{
			//add a new failed node and assign it dof value
		case 1: addNewNode(Chrom); break;
		case 2: removeNodeDof(Chrom); break;
		case 3: exchangeNodeDof(Chrom); break;
		default:
			TRACE("SecondProcedure generate wrong Index");
			system("PAUSE");
			break;
		}
	}

}
