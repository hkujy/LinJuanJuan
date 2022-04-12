#include "CommonHeaders.h"
#include <assert.h>
using namespace std;

bool ReadLinkData(std::vector<LinkClass>& Links,
	ifstream& fin) {
	LinkClass tl;
	int IDcount = 0;
	//int tail, head;
	//float t0, ca0;
	//float BprAlph, BprBeta;
	vector<string> fields;
	string line;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0]) < 0)
			continue;
		/*	fscanf_s(fin, "%i %i %f %f %f %f",
				&tail, &head, &t0, &ca0, &BprAlph, &BprBeta);*/
		Links.push_back(tl);
		Links.back().Id = IDcount;
		Links.back().Tail = std::stoi(fields[0]);
		Links.back().Head = std::stoi(fields[1]);
		Links.back().T0 = std::stof(fields[2]);
		Links.back().CaInput = std::stof(fields[3]);
		Links.back().CaRevise = std::stof(fields[3]);
		Links.back().AlphaBpr = std::stof(fields[4]);
		Links.back().BetaBBpr = std::stof(fields[5]);
		IDcount++;
	}
	return true;
}

GraphClass::GraphClass():UNPM(0.0), TotalSystemCost(0.0)
{
	this->OdPairs.reserve(numOD + 1);
	this->OriginSet.reserve(numNodes);
	this->Nodes.reserve(numNodes + 1);
	this->Links.reserve(numLinks + 1);
	//int** MinPathPredLink;
	if (numNodes==0)
	{
		cerr << "Input node should be known before construct graph" << endl;
	}
	this->MinPathPredLink
		= Create2DArray<int>(numNodes + 1, numNodes + 1);
	for (int i = 0; i < numNodes + 1; i++)
	{
		for (int j = 0; j < numNodes + 1; j++)
		{
			this->MinPathPredLink[i][j] = InvaildInt;
		}
	}

};
GraphClass::~GraphClass(){
	Free2DArray<int>(this->MinPathPredLink, numNodes + 1);
	OdPairs.clear(); Links.clear(); Nodes.clear(); OriginSet.clear();
};

int GraphClass::FindMinCostRoutes(){

	try{
		int StatusMsg;
		std::vector<double> Lable;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			int Orig = o->Onode;
			if (Orig<0) continue;
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
				this->OdPairs.at((*d)->Id).MinCost = Lable[Dest];

				//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
				if (this->OdPairs.at((*d)->Id).MinCost < InvalidMinCost)
				{
					this->OdPairs.at((*d)->Id).isConnected = true;
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

void GraphClass::CreateOriginSet()

{
	OriginBasedOD Oset;
	vector<bool> isOrign(numNodes, false);
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		//cout << od->Origin << endl;
		isOrign.at(od->Origin) = true;
	}
	for (int n = 0; n < numNodes; n++) {

		this->OriginSet.push_back(Oset);
		if (isOrign.at(n))	this->OriginSet.back().Onode = n;
		//if (isOrign.at(n))
		//{
		//	Oset.Onode = n;
		//	this->OriginSet.push_back(Oset);
		//}
	}

	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		this->OriginSet.at(od->Origin).ODset.push_back(&*od);
	}

	cout << "finish create origin set" << endl;
}

void GraphClass::CreateNodes(){
	cout <<"Read Number of Nodes in the network is"<<this->Links.size() << endl;
	assert(this->Links.size() > 0);
	NodeClass tnode;
	for (int i = 0; i < numNodes; i++)
	{
		this->Nodes.push_back(tnode);
		this->Nodes.back().Id = i;
	}
	for (auto l = this->Links.begin(); l != this->Links.end(); l++)
	{
		this->Nodes.at(l->Tail).OutLinks.push_back(&*l);
		this->Nodes.at(l->Head).InLinks.push_back(&*l);
	}
}

bool ReadDemandData(vector<ODClass>& ODPairs,
	ifstream& fin) {

	int IDcount = 0;
	//int or , de;
	//float dd;
	vector<string> fields;
	string line;
	ODClass tod;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0]) < 0)
			continue;

		ODPairs.push_back(tod);
		ODPairs.back().Id = IDcount;
		ODPairs.back().Origin = std::stoi(fields[0]);
		ODPairs.back().Dest = std::stoi(fields[1]);
		ODPairs.back().Demand = std::stof(fields[2]);
		IDcount++;
	}

	return true;
}


void GraphClass::ReadGraphData()
{

	//FILE * fin;
	std::ifstream fin;
	// Read Demand data
	if (networkIndex == 1) // Scan
	{
		//fopen_s(&fin, "..//Input//MediumNetwork//DeamdData.txt", "r");
		fin.open("..//Input//MediumNetwork//DeamdData.txt");
		//fopen_s(&fin, "..//Input//MediumNetwork//DeamdData.txt", "r");
	}
	else if (networkIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else if (networkIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//DeamdData.txt");
	}
	else if (networkIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else if (networkIndex ==5)
	{
		fin.open("..//Input//WangNetwork//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else
	{
		cout << "Mode Index is not specified" << endl;
		system("PAUSE");
		fin.open("..//Input//DeamdData.txt");
		//fopen_s(&fin, "..//Input//DeamdData.txt", "r");
	}

	if (!ReadDemandData(OdPairs, fin))
	{
		cerr << "read demand data fails" << endl;
		system("PAUSE");
	}

	fin.close();

	// Read link data
	if (networkIndex == 1) // Scan
	{
		fin.open("..//Input//MediumNetwork//LinkData.txt");
		//fopen_s(&fin, "..//Input//MediumNetwork//LinkData.txt", "r");
	}
	else if (networkIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//LinkData.txt");
		//fin.open("C://GitCodes//NRI//InPut//Nagureny2009Network//LinkData.txt");
			//LinkData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//LinkData.txt", "r");
	}
	else if (networkIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else if (networkIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else if (networkIndex == 5)
	{
		fin.open("..//Input//WangNetwork//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else
	{
		fin.open("..//Input//LinkData.txt");
		//fopen_s(&fin, "..//Input//LinkData.txt", "r");
	}

	if (!fin.is_open())
		cout << "link data file is not open" << endl;

	if (!ReadLinkData(Links, fin))
	{
		cerr << "read link data fails" << endl;
		system("PAUSE");
	}
	fin.close();
	CreateOriginSet();
	CreateNodes();
}


int GraphClass::PrintLinks(std::ofstream &fout){

	try{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "Id" << ",";
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
			fout << l->Id << ",";
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

int GraphClass::PrintLinks_onscreen() {
	try {
		cout.setf(ios::fixed);
		cout << left << setw(6) << "Id" << ",";
		cout << left << setw(12) << "Tail" << ",";
		cout << left << setw(12) << "Head" << ",";
		cout << left << setw(12) << "T0" << ",";
		cout << left << setw(12) << "Ca0" << ",";
		cout << left << setw(12) << "CaRev" << ",";
		cout << left << setw(12) << "Cost" << ",";
		cout << left << setw(12) << "Flow" << endl;
		//fout << endl;
		for (auto l = this->Links.begin(); l != this->Links.end(); l++)
		{
			cout << l->Id << ",";
			cout << l->Tail << ",";
			cout << l->Head << ",";
			cout << l->T0 << ",";
			cout << l->CaInput << ",";
			cout << l->CaRevise << ",";
			cout << l->Cost << ",";
			cout << l->Flow << endl;
			//fout << endl;
		}
		cout << " Total Cost = " << TotalSystemCost << endl;
		return 1;
	}
	catch (exception& e)
	{
		TRACE("%s", e);
		return 0;
	}

}

int GraphClass::PrintOD(std::ofstream &fout)
{
	try
	{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "Id" << ",";
		fout << left << setw(12) << "Origin" << ",";
		fout << left << setw(12) << "Dest" << ",";
		fout << left << setw(12) << "Demand" << ",";
		fout << left << setw(12) << "MinCost" << endl;
		//fout << endl;

		for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
		{
			fout << od->Id << ",";
			fout << od->Origin << ",";
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

int GraphClass::PrintSp(int Orign, int Dest, std::ofstream &fout)
{
	try{

		vector<int> Path;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			for (auto od = o->ODset.begin(); od != o->ODset.end(); od++)
			{
				if ((*od)->Origin == Orign && Dest == (*od)->Dest)
				{
					Path.clear();
					int CurrentNode = (*od)->Dest;
					while (CurrentNode != (*od)->Origin)
					{
						int k = this->MinPathPredLink[(*od)->Origin][CurrentNode];
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

void GraphClass::EvalutateFailureScenarios(const ScenarioClass &s)
{
	if (s.tau.size() != s.LinkIds.size())
	{
		cout << "err: the size of tau and linked should be equal" << endl;
		system("pause");
	}

	cout << "------Eva Link Cap Fail-----------------------" << endl;
	for (int i = 0; i < 1; i++)  // loop all the period
	{
		cout << "Remark: Only Evaluate one period for test case" << endl;
		for (size_t j = 0; j < s.tau.size(); j++)
		{
			if (i == s.tau.at(j))
			{
				this->Links.at(s.LinkIds.at(j)).CaRevise = zero;
			}
		}
		this->EvaluteGraph();
		cout << "------Summary for period = " << i << "-------" << endl;
		this->PrintLinks_onscreen();
		cout << "------End of summary for period = " << i << "-------" << endl;
	}
	cout << "------End of Eva Link Cap Fail-----------------------" << endl;
}

void GraphClass::RevertFailureScenarios(const ScenarioClass& s)
{
	if (s.tau.size() != s.LinkIds.size())
	{
		cout << "err: the size of tau and linked should be equal" << endl;
		system("pause");
	}

	cout << "------Restore Link Cap-----------------------" << endl;
	for (int i = 0; i < MaxNumOfSchPeriod; i++)  // loop all the period
	{
		for (size_t j = 0; j < s.tau.size(); j++)
		{
			if (i == s.tau.at(j))
			{
				this->Links.at(s.LinkIds.at(j)).IniCap();
			}
		}
		this->EvaluteGraph();
		cout << "------Summary for period = " << i << "-------" << endl;
		this->PrintLinks_onscreen();
		cout << "------End of summary for period = " << i << "-------" << endl;
	}
	cout << "------End Restore Link Cap-----------------------" << endl;
}

/// <summary>
/// compute the relative changes for the graph with and without the link
/// </summary>
/// <param name="link"></param>
double GraphClass::CalRelSpChange(int LinkID)
{
	// Step 0: Compute the Sp labels before remove the links
	int StatusMsg;
	for (int i = 0; i < Links.size(); i++)
	{
		Links.at(i).Cost= Links.at(i).IniCost();
	}
	std::vector<double> Lable;
	for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	{
		int Orig = o->Onode;
		if (Orig < 0) continue;
		StatusMsg = this->SP(o->Onode, Lable);// shortest path
		assert(StatusMsg);
		//printf("Find min tree for Onode = %d \n", Orig);
		//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
		for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		{
			int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d)->Id).BeforeRemoveSpDist = Lable[Dest];
			//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
			if (this->OdPairs.at((*d)->Id).BeforeRemoveSpDist < InvalidMinCost)
			{
				this->OdPairs.at((*d)->Id).BeforeRemoveSpDist = true;
			}
			/*if (ODPairs.at(*d).MinCost.at(ModeType) < InvalidMinCost)
				ODPairs.at(*d).isConnected = true;*/
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Step 1: Remove the link in the current network
	this->Links.at(LinkID).Cost = RemoveLinkCost;
	//////////////////////////////////////////////////////////////////////////
	for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	{
		int Orig = o->Onode;
		if (Orig < 0) continue;
		StatusMsg = this->SP(o->Onode, Lable);// shortest path
		assert(StatusMsg);
		//printf("Find min tree for Onode = %d \n", Orig);
		//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
		for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		{
			int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d)->Id).AfterRemoveSpDist = Lable[Dest];
			//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
			if (this->OdPairs.at((*d)->Id).AfterRemoveSpDist < InvalidMinCost)
			{
				this->OdPairs.at((*d)->Id).isConnected = true;
			}
			/*if (ODPairs.at(*d).MinCost.at(ModeType) < InvalidMinCost)
				ODPairs.at(*d).isConnected = true;*/
		}
	}
	//TODO: need to update the denominator	
	double EIvalue = 0.0;
	for (auto o:OdPairs)
	{
#ifdef _DEBUG
		assert(o.AfterRemoveSpDist > 0);
		assert(o.BeforeRemoveSpDist > 0);
#endif // _DEBUG
		EIvalue += (1/ o.BeforeRemoveSpDist- 1 / o.AfterRemoveSpDist);
	}

	//Final step add back the link
	this->Links.at(LinkID).Cost = this->Links.at(LinkID).IniCost();

	return EIvalue;
}
