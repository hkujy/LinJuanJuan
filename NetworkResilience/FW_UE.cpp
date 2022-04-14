#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;

double LinksSDLineSearch(vector<LinkClass>& XLink, vector<LinkClass>& YLink);
double Wobj(const vector<LinkClass>& Links)
{
	double Wobj = 0.0;
	for (auto l = Links.begin(); l != Links.end(); ++l)
	{
		//Wobj += l->Flow*l->Cost + (l->AlphaBpr) * l->Cost * l->CaRevise *(std::pow((l->Flow / l->CaRevise), l->BetaBBpr + 1.0f) / (double)(l->BetaBBpr+ 1.0f));
		//Wobj += l->Flow*l->Cost + 0.15 * l->Cost * l->CaRevise *(std::pow((l->Flow / l->CaRevise), 4 + 1.0f) / (double)(4+ 1.0f));
		Wobj += l->Flow * l->Cost + 0.15 * l->Cost * (pow(1 / l->CaRevise, 4)) * pow(l->Flow, 5) / 5;
		//WOBJ = WOBJ + V(i)*TOLL(I) / VOT :: toll is not considered in this paper
	}
	return Wobj;
}

double OneDim(vector<LinkClass> VALinks, std::vector<LinkClass> VBLinks,
	vector<LinkClass>& V1Links, vector<LinkClass>& V2Links)
{
	try {
		double Alfa = 1.0;
		double R1 = (3.0 - std::sqrt(5.0)) / 2.0;
		double R2 = (std::sqrt(5.0) - 1.0) / 2.0;
		double VA = 0.0, VB = 1.0;
		double V1 = 0.0, V2 = 1.0;
		//double F1 = Wobj(VALinks);
		//double F2 = Wobj(VBLinks);
		//double F1, F2;
		for (unsigned int i = 0; i < VALinks.size(); i++)
		{
			V1Links.at(i).Flow = VALinks.at(i).Flow + R1 * (VBLinks.at(i).Flow - VALinks.at(i).Flow);
			V2Links.at(i).Flow = VALinks.at(i).Flow + R2 * (VBLinks.at(i).Flow - VALinks.at(i).Flow);
		}
		/*VA = VA + R1*(VB - VA);
		VB = VA + R2*(VB - VA);*/
		V1 = VA + R1 * (VB - VA);
		V2 = VA + R2 * (VB - VA);
		double F1 = Wobj(V1Links);
		double F2 = Wobj(V2Links);

	Loop:
		if (F1 <= F2)
		{
			for (unsigned int i = 0; i < VBLinks.size(); i++)
			{
				VBLinks.at(i).Flow = V2Links.at(i).Flow;
				V2Links.at(i).Flow = V1Links.at(i).Flow;
				V1Links.at(i).Flow = VALinks.at(i).Flow + R1 * (VBLinks.at(i).Flow - VALinks.at(i).Flow);
			}
			VB = V2;
			V2 = V1;
			V1 = VA + R1 * (VB - VA);
			F2 = F1;
			F1 = Wobj(V1Links);
		}
		else
		{
			for (unsigned int i = 0; i < VALinks.size(); i++)
			{
				VALinks.at(i).Flow = V1Links.at(i).Flow;
				V1Links.at(i).Flow = V2Links.at(i).Flow;
				V2Links.at(i).Flow = VALinks.at(i).Flow + R2 * (VBLinks.at(i).Flow - VALinks.at(i).Flow);
			}
			VA = V1;
			V1 = V2;
			V2 = VA + R2 * (VB - VA);
			F1 = F2;
			F2 = Wobj(V2Links);
		}


		if (std::abs(VA - VB) > oneDimEps) goto Loop;

		Alfa = (VA + VB) / 2.0;

		return Alfa;
	}
	catch (exception& e)
	{
		TRACE("ONEDIM %s", e.what());
		return INVALID_FLOAT;
	}
}

void Assign(const std::vector<OriginBasedOD>& Oset,
	int** PreLinks,
	std::vector<LinkClass>& Links) {
	//try {

	//for (auto l = Links.begin(); l != Links.end(); l++)
	for (auto& l : Links)
	{
		l.CleanLinkFlow();
		//l->Flow = l->CleanLinkFlow();
	}
	for (auto o = Oset.begin(); o != Oset.end(); o++)
	{
		for (auto od = o->ODset.begin(); od != o->ODset.end(); ++od)
		{
			//if (isEqual((*od)->Demand,0.0f)) continue;
			if ((*od)->Demand < 1.0) continue;
			assert((*od)->Demand > 0.0);
			int CurrentNode = (*od)->Dest;
			while (CurrentNode != (*od)->Origin) {
				int k = PreLinks[(*od)->Origin][CurrentNode];
				if (k < 0)
				{
					std::cout << "k=" << k << endl;
					std::cout << "origin = " << o->OirginNode << ", dest = " << (*od)->Dest << endl;
					std::cout << "This OD pair may not be connected" << endl;
				}

#ifdef __DEBUG__ 
				if (k == INVALID_INT) {
					DEBUG("A problem in mincostroutes.c (Assign): Invalid pred for node %d Orig %d \n\n", CurrentNode, Origin);
					break;
				}
#endif
				Links.at(k).Flow += (*od)->Demand;
				CurrentNode = Links.at(k).Tail;
			}

		}
	}
	//}
	//catch (exception& e)
	//{
	//	TRACE("Assign Flow %s", e.what());
	//}
}
int PrintConverge(int NumIter, double Err, ofstream& fout) {

	fout << "Iter," << NumIter << "," << Err << endl;
	return 1;
}
int GraphClass::FW_UE() {
	//try {
	int statusMsg = -1;
	// step -1 clean and create variable:
	int NumIter = -1;
	for (auto& l : Links)
	{
		l.CleanLinkFlow();
		l.SetIniCost();
	}
	//for (auto l = this->Links.begin(); l != this->Links.end(); l++)
	//{
	//	//l->Flow = l->CleanLinkFlow();
	//	l->Cost = l->GetIniCost();
	//}

	vector<LinkClass> YLinks;
	vector<LinkClass> OldLinks;
	vector<LinkClass> V1Links, V2Links;

	for (unsigned int i = 0; i < this->Links.size(); i++)
	{
		YLinks.push_back(this->Links.at(i));
		YLinks.back().AlphaBpr = this->Links.at(i).AlphaBpr;
		YLinks.back().BetaBBpr = this->Links.at(i).BetaBBpr;
		OldLinks.push_back(this->Links.at(i));
		OldLinks.back().AlphaBpr = this->Links.at(i).AlphaBpr;
		OldLinks.back().BetaBBpr = this->Links.at(i).BetaBBpr;

		V1Links.push_back(this->Links.at(i));
		V1Links.back().AlphaBpr = this->Links.at(i).AlphaBpr;
		V1Links.back().BetaBBpr = this->Links.at(i).BetaBBpr;

		V2Links.push_back(this->Links.at(i));
		V2Links.back().AlphaBpr = this->Links.at(i).AlphaBpr;
		V2Links.back().BetaBBpr = this->Links.at(i).BetaBBpr;
	}
	// STEP 0: INITIAL ASSIGNMENT

	statusMsg = this->FindMinCostRoutes();
	assert(statusMsg);
	Assign(this->OriginSet, this->MinPathPredLink, this->Links);

	NumIter = 1;

	//STEP 1: UPDATE LinkClass TRAVEL COST
UeLop:
	//cout << "update cost:-----" << endl;
	for (auto l = this->Links.begin(); l != this->Links.end(); l++)
	{
		l->Cost = l->BPRCost();
		//cout << l->Id << "," << l->Cost << endl;
	}
	for (unsigned int i = 0; i < this->Links.size(); i++)
	{
		OldLinks.at(i).Flow = this->Links.at(i).Flow;
	}
	// STEP 2: DIRECTION FINDING TO OBTAIN YFLOW(I)

	statusMsg = this->FindMinCostRoutes();
	assert(statusMsg);
	Assign(this->OriginSet, this->MinPathPredLink, YLinks);
	NumIter++;
	//STEP 3: LINE SEARCH TO OBTAIN ALFA

	//double Alfa = OneDim(this->Links, YLinks,V1Links,V2Links); 
	double Alfa = LinksSDLineSearch(this->Links, YLinks);
	assert(Alfa <= 1.0 && Alfa >= 0);
	//cout << "update Alfa:-----" << Alfa << endl;

	//STEP 4 : MOVE
	for (size_t i = 0; i < this->Links.size(); i++)
	{
		this->Links.at(i).Flow = this->Links.at(i).Flow + Alfa * (YLinks.at(i).Flow - this->Links.at(i).Flow);
		//this->Links.at(i).Flow = this->Links.at(i).Flow + Alfa*(V1Links.at(i).Flow - V2Links.at(i).Flow);
	}

	//STEP 5 : CONVERGENCE TEST

	double TotalFlow = 0.0, Err = 0.0;
	for (unsigned int i = 0; i < this->Links.size(); i++)
	{
		Err += (OldLinks.at(i).Flow - this->Links.at(i).Flow)
			* (OldLinks.at(i).Flow - this->Links.at(i).Flow);
		TotalFlow += this->Links.at(i).Flow;
	}

	Err = std::sqrt(Err);
	//cout << TotalFlow << endl;
	if (isEqual(TotalFlow, 0.0f))
	{
		cout << "Total flow=0.0" << endl;
		cout << "Reason 1: check the largest link cost, it could be larger than the LargeNum, thus no route is found in SP" << endl;
		system("Pause");
	}
	Err = Err / TotalFlow;

	if (Err > epsUE && NumIter < maxIterUE)
	{
#ifdef _DEBUG
		cout << NumIter << "\t" << Err << endl;
#endif // _DEBUG
		goto UeLop;
	}
	for (auto l = this->Links.begin(); l != this->Links.end(); ++l)
	{
		l->Cost = l->BPRCost();
	}
	statusMsg = this->FindMinCostRoutes();
#ifdef _DEBUG
	cout << "UE No. of Iter = " << NumIter << ",Converge Gap Value =" << Err << endl;
#endif // _DEBUG

	//assert(PrintConverge(NumIter, Err, AssertLog));
	return 1;
	//}
	//catch (exception& e)
	//{
	//	TRACE("%s", e);
	//	return 0;
	//}
}

void GraphClass::EvaluteGraph() {

	const int statusMsg = this->FW_UE();
	assert(statusMsg);
	TotalSystemCost = 0.0;
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); ++od)
	{
		TotalSystemCost += od->Demand * od->MinCost;
	}
	UNPM = 0.0;
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); ++od)
	{
		assert(od->MinCost >= 0.0);
		if (od->MinCost < zero)
		{
			TRACE("od->MinCost <Zeor\n");
		}
		UNPM += od->Demand / od->MinCost;
	}
	UNPM = UNPM / static_cast<double>(OdPairs.size());
#if _DEBUG
	std::cout << "Print Detail solution" << std::endl;
	std::ofstream baseSolutionOutput;
	baseSolutionOutput.open("..//OutPut//BaseSolLink.txt", ios::trunc);
	baseSolutionOutput << "LinkId,Tail,Head,t0,Cap,Flow,Cost" << endl;
	for (auto l = this->Links.begin(); l != Links.end(); ++l)
	{
		baseSolutionOutput << l->Id << ",";
		baseSolutionOutput << l->Tail << ",";
		baseSolutionOutput << l->Head << ",";
		baseSolutionOutput << l->T0 << ",";
		baseSolutionOutput << l->CaRevise << ",";
		baseSolutionOutput << l->Flow << ",";
		baseSolutionOutput << l->Cost << endl;
	}

	baseSolutionOutput.close();
	baseSolutionOutput.open("..//OutPut//BaseSolOD.txt", ios::trunc);
	baseSolutionOutput << "ODId,Origin,Dest,Demand,minCost" << endl;
	for (auto l = this->OdPairs.begin(); l != OdPairs.end(); ++l)
	{
		baseSolutionOutput << l->Id << ",";
		baseSolutionOutput << l->Origin << ",";
		baseSolutionOutput << l->Dest << ",";
		baseSolutionOutput << l->Demand << ",";
		baseSolutionOutput << l->MinCost << endl;

	}
	baseSolutionOutput.close();
#endif



}