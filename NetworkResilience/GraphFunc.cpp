#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;


GRAPH::GRAPH(){
	this->OdPairs.reserve(NumOD + 1);
	this->OriginSet.reserve(NumNodes);
	this->Nodes.reserve(NumNodes + 1);
	this->Links.reserve(NumLinks + 1);
	//int** MinPathPredLink;
	if (NumNodes==0)
	{
		cerr << "Input node should be known before construct graph" << endl;
	}
	this->MinPathPredLink
		= Create2DAarray<int>(NumNodes + 1, NumNodes + 1);
	for (int i = 0; i < NumNodes + 1; i++)
	{
		for (int j = 0; j < NumNodes + 1; j++)
		{
			this->MinPathPredLink[i][j] = InvaildInt;
		}
	}

};
GRAPH::~GRAPH(){
	Free2DArrey<int>(this->MinPathPredLink, NumNodes + 1);
};

int GRAPH::FindMinCostRoutes(){

	try{
		int StatusMsg;
		std::vector<double> Lable;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			int Orig = o->Onode;
			StatusMsg = this->SP(o->Onode, Lable);// shortest path
			assert(StatusMsg);
			//printf("Find min tree for Onode = %d \n", Orig);
			//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
			for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
			{
				int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
				if (isnan(Lable[Dest])) DEBUG("Od Pair %d,mincost is NaN", *d);
#endif	
				this->OdPairs.at((*d)->ID).MinCost = Lable[Dest];

				//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
				if (this->OdPairs.at((*d)->ID).MinCost < InvalidMinCost)
				{
					this->OdPairs.at((*d)->ID).isConnected = true;
				}
				/*if (ODPairs.at(*d).MinCost.at(ModeType) < InvalidMinCost)
					ODPairs.at(*d).isConnected = true;*/
			}
		}
		return 1;
	}

	catch (exception& e)
	{
		TRACE("FindMinCostRoutes %s", e.what());
		return 0;
	}
}

void GRAPH::CreateOriginSet()
{
	OriginBasedOD Oset;
	vector<bool> isOrign(NumNodes, false);
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		//cout << od->Orign << endl;
		isOrign.at(od->Orign) = true;
	}
	for (int n = 0; n < NumNodes; n++) {
		if (isOrign.at(n))
		{
			Oset.Onode = n;
			this->OriginSet.push_back(Oset);
		}
	}
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		this->OriginSet.at(od->Orign).ODset.push_back(&*od);
	}

}

void GRAPH::CreateNodes(){
	assert(this->Links.size() > 0);
	NODE tnode;
	for (int i = 0; i < NumNodes; i++)
	{
		this->Nodes.push_back(tnode);
		this->Nodes.back().ID = i;
	}
	for (auto l = this->Links.begin(); l != this->Links.end(); l++)
	{
		this->Nodes.at(l->Tail).OutLinks.push_back(&*l);
		this->Nodes.at(l->Head).InLinks.push_back(&*l);
	}
}


int GRAPH::PrintLinks(std::ofstream &fout){

	try{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "ID" << ",";
		fout << left << setw(12) << "Tail" << ",";
		fout << left << setw(12) << "Head" << ",";
		fout << left << setw(12) << "T0" << ",";
		fout << left << setw(12) << "Ca0" << ",";
		fout << left << setw(12) << "CaRev" << ",";
		fout << left << setw(12) << "Cost" << ",";
		fout << left << setw(12) << "Flow" <<endl;
		//fout << endl;
		for (auto l = this->Links.begin(); l != this->Links.end(); l++)
		{
			fout << l->ID << ",";
			fout << l->Tail << ",";
			fout << l->Head << ",";
			fout << l->T0 << ",";
			fout << l->CaInput << ",";
			fout << l->CaRevise << ",";
			fout << l->Cost << ",";
			fout << l->Flow <<endl;
			//fout << endl;
		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}

}

int GRAPH::PrintOD(std::ofstream &fout)
{
	try
	{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "ID" << ",";
		fout << left << setw(12) << "Origin" << ",";
		fout << left << setw(12) << "Dest" << ",";
		fout << left << setw(12) << "Demand" << ",";
		fout << left << setw(12) << "MinCost" << endl;
		//fout << endl;

		for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
		{
			fout << od->ID << ",";
			fout << od->Orign << ",";
			fout << od->Dest << ",";
			fout << od->Demand << ",";
			fout << od->MinCost << endl;
			//fout << endl;

		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}

}

int GRAPH::PrintSp(int Orign, int Dest, std::ofstream &fout)
{
	try{

		vector<int> Path;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			for (auto od = o->ODset.begin(); od != o->ODset.end(); od++)
			{
				if ((*od)->Orign == Orign && Dest == (*od)->Dest)
				{
					Path.clear();
					int CurrentNode = (*od)->Dest;
					while (CurrentNode != (*od)->Orign)
					{
						int k = this->MinPathPredLink[(*od)->Orign][CurrentNode];
						Path.push_back(k);
						CurrentNode = this->Links.at(k).Tail;
					}
					for (auto i = Path.rbegin(); i != Path.rend(); i++)
					{
						fout << *i << ",";
					}
					fout << endl;
				}
			}
		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}
}


