#include "CommonHeaders.h"
#include <cassert>
#include <thread>
using namespace std;

bool ReadLinkData(std::vector<LinkClass>& links,
	ifstream& fin) {
	LinkClass tl;
	int IdCount = 0;
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
		links.push_back(tl);
		links.back().Id = IdCount;
		links.back().Tail = std::stoi(fields[0]);
		links.back().Head = std::stoi(fields[1]);
		links.back().T0 =static_cast<double>(std::stof(fields[2]));
		links.back().CaInput = static_cast<double>(std::stof(fields[3]));
		links.back().CaRevise = static_cast<double>(std::stof(fields[3]));
		links.back().AlphaBpr = static_cast<double>(std::stof(fields[4]));
		links.back().BetaBBpr = static_cast<double>(std::stof(fields[5]));
		IdCount++;
	}
	return true;
}

GraphClass::GraphClass():UNPM(0.0), TotalSystemCost(0.0)
{
	this->OdPairs.reserve(static_cast<std::vector<ODClass, std::allocator<ODClass>>::size_type>(numOD) + 1);
	this->OriginSet.reserve(numNodes);
	this->Nodes.reserve(static_cast<std::vector<NodeClass, std::allocator<NodeClass>>::size_type>(numNodes) + 1);
	this->Links.reserve(static_cast<std::vector<LinkClass, std::allocator<LinkClass>>::size_type>(numLinks) + 1);
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
			this->MinPathPredLink[i][j] = INVALID_INT;
		}
	}

};
GraphClass::~GraphClass(){
	Free2DArray<int>(this->MinPathPredLink, numNodes + 1);
	OdPairs.clear(); Links.clear(); Nodes.clear(); OriginSet.clear();
};

int GraphClass::FindMinCostRoutes(){

	try{
		int statusMsg = -1;
		std::vector<double> label;
		//for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); ++o)
		for (auto o:OriginSet)
		{
			const int orig = o.OirginNode;
			if (orig<0) continue;
			statusMsg = this->SP(o.OirginNode, label);// shortest path
			assert(statusMsg);
			//printf("Find min tree for OirginNode = %d \n", Orig);
			//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
			//for (auto d = o.ODset.begin(); d != o.ODset.end(); ++d)
			for (auto d:o.ODset)
			{
				const int dest = (*d).Dest;
#ifdef __DEBUG__ 
				if (isnan(Lable[Dest])) DEBUG("Od Pair %d,mincost is NaN", *d);
#endif	
				this->OdPairs.at((*d).Id).MinCost = label[dest];

				if (this->OdPairs.at((*d).Id).MinCost < INVALID_MIN_COST)
				{
					this->OdPairs.at((*d).Id).isConnected = true;
				}
				/*if (ODPairs.at(*d).MinCost.at(ModeType) < INVALID_MIN_COST)
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
	vector<bool> isOrigin(numNodes, false);
	//for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	for (auto const &od:OdPairs)
	{
		isOrigin.at(od.Origin) = true;
	}
	for (int n = 0; n < numNodes; ++n) {

		this->OriginSet.push_back(Oset);
		if (isOrigin.at(n))	this->OriginSet.back().OirginNode = n;
	}
	//for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); ++od)
	////for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); ++od)
	//{
	//	this->OriginSet.at(od->Origin).ODset.push_back(&*od);
	//}

	for (auto &od: OdPairs)
		//for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); ++od)
	{
		this->OriginSet.at(od.Origin).ODset.push_back(&od);
	}

	cout << "finish create origin set" << endl;
}

void GraphClass::CreateNodes(){
	cout <<"Read Number of Nodes in the network is"<<this->Links.size() << endl;
	assert(!Links.empty());
	NodeClass tnode;
	for (int i = 0; i < numNodes; i++)
	{
		this->Nodes.push_back(tnode);
		this->Nodes.back().Id = i;
	}
	//for (auto l = this->Links.begin(); l != this->Links.end(); ++l)
	//{
	//	this->Nodes.at(l->Tail).OutLinks.push_back(&*l);
	//	this->Nodes.at(l->Head).InLinks.push_back(&*l);
	//}

	for (auto &l:this->Links)
	{
		this->Nodes.at(l.Tail).OutLinks.push_back(&l);
		this->Nodes.at(l.Head).InLinks.push_back(&l);
	}
}

bool ReadDemandData(vector<ODClass>& ODPairs,
	ifstream& fin) {

	int idCount = 0;
	vector<string> fields;
	string line;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0]) < 0)
			continue;
		//ODClass tempOd;
		//ODPairs.push_back(tempOd);
		ODPairs.emplace_back(ODClass());
		ODPairs.back().Id = idCount;
		ODPairs.back().Origin = std::stoi(fields[0]);
		ODPairs.back().Dest = std::stoi(fields[1]);
		ODPairs.back().Demand = static_cast<double>(std::stof(fields[2]));
		idCount++;
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
		fin.open("..//Input//MediumNetwork//DeamdData.txt");
	}
	else if (networkIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//DeamdData.txt");
	}
	else if (networkIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//DeamdData.txt");
	}
	else if (networkIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//DeamdData.txt");
	}
	else if (networkIndex ==5)
	{
		fin.open("..//Input//WangNetwork//DeamdData.txt");
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
	{
		cout << "ERR:link data file is not open" << endl;
		system("PAUSE");
	}

	if (!ReadLinkData(Links, fin))
	{
		cerr << "Err:read link data fails" << endl;
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
	catch (std::exception &e)
	{
		TRACE("%s", e.what());
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
	catch (std::exception& e)
	{
		TRACE("%s", e.what());
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

int GraphClass::PrintSp(int origin, int dest, std::ofstream &fout)
{
	try{
		vector<int> path;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); ++o)
		{
			for (auto od = o->ODset.begin(); od != o->ODset.end(); ++od)
			{
				if ((*od)->Origin == origin && dest == (*od)->Dest)
				{
					path.clear();
					int CurrentNode = (*od)->Dest;
					while (CurrentNode != (*od)->Origin)
					{
						int k = this->MinPathPredLink[(*od)->Origin][CurrentNode];
						path.push_back(k);
						CurrentNode = this->Links.at(k).Tail;
					}
					for (auto i = path.rbegin(); i != path.rend(); ++i)
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
		TRACE("%s", e.what());
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
	for (int i = 0; i < MAX_NUM_OF_SCH_PERIOD; i++)  // loop all the period
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

// calculate relative changes in the shortest path
double GraphClass::CalRelSpChange(int linkId)
{
	// Step 0: Compute the Sp labels before remove the links
	int statusMsg=-2;
	//for (int i = 0; i < Links.size(); i++)
	//{
	//	Links.at(i).Cost = Links.at(i).GetIniCost();
	//}
	for (auto &l:Links)
	{
		l.SetIniCost();
	}
		//Links.at(i).Cost = Links.at(i).GetIniCost();

	std::vector<double> label;
	//for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	for (auto &o:OriginSet)
	{
		const int origin = o.OirginNode;
		if (origin < 0) continue;
		statusMsg = this->SP(o.OirginNode, label);// shortest path
		assert(statusMsg);
		//printf("Find min tree for OirginNode = %d \n", Orig);
		//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
		//for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		for (auto const &d: o.ODset)
		{
			const int dest = (*d).Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d).Id).BeforeRemoveSpDist = label[dest];
			//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
			if (this->OdPairs.at((*d).Id).BeforeRemoveSpDist < INVALID_MIN_COST)
			{
				this->OdPairs.at((*d).Id).BeforeRemoveSpDist = true;
			}
			/*if (ODPairs.at(*d).MinCost.at(ModeType) < INVALID_MIN_COST)
				ODPairs.at(*d).isConnected = true;*/
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Step 1: Remove the link in the current network
	this->Links.at(linkId).Cost = REMOVE_LINK_COST;
	//////////////////////////////////////////////////////////////////////////
	//for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	for (auto &o: OriginSet)
	{
		const int origin = o.OirginNode;
		if (origin < 0) continue;
		statusMsg = this->SP(o.OirginNode, label);// shortest path
		assert(statusMsg);
		//printf("Find min tree for OirginNode = %d \n", Orig);
		//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
		//for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		for (auto const &d:o.ODset)
		{
			const int dest = (*d).Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d).Id).AfterRemoveSpDist = label[dest];
			//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
			if (this->OdPairs.at((*d).Id).AfterRemoveSpDist < INVALID_MIN_COST)
			{
				this->OdPairs.at((*d).Id).isConnected = true;
			}
			/*if (ODPairs.at(*d).MinCost.at(ModeType) < INVALID_MIN_COST)
				ODPairs.at(*d).isConnected = true;*/
		}
	}
	//TODO: need to update the denominator	
	double evaluateVal = 0.0;
	for (auto const &o:OdPairs)
	{
#ifdef _DEBUG
		assert(o.AfterRemoveSpDist > 0);
		assert(o.BeforeRemoveSpDist > 0);
#endif // _DEBUG
		evaluateVal += (1/ o.BeforeRemoveSpDist- 1 / o.AfterRemoveSpDist);
	}

	//Final step add back the link
	//this->Links.at(linkId).Cost = this->Links.at(linkId).GetIniCost();
	this->Links.at(linkId).SetIniCost();

	return evaluateVal;
}
