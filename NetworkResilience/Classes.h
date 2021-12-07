
#ifndef MyClasses
#define MyClasses
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"

using namespace std;

class LINK
{
	// Tail ---> Head 
public:
	int ID;
	int Tail;
	int Head;
	double T0;
	double CaInput;
	double CaRevise;
	double Cost;
	double Flow;
	double AlphaBpr; // parameter in the BPR  // 0.15
	double BetaBBpr; // parameter in the BPR  // 4.0
	double EI; // Efficiency Index
	double CleanLinkFlow();
	double BPRCost();
	double IniCost();
	void IniCap();
	int RecoverTime;  // recover time
	int RequiredRes; // required resources
	LINK();
	LINK(const LINK& OtherLink);
	~LINK();
	LINK& operator=(const LINK& rhs) {
		ID = rhs.ID; Tail = rhs.Tail; Head = rhs.Head;
		T0 = rhs.T0; CaInput = rhs.CaInput;
		CaRevise = rhs.CaRevise;
		Cost = rhs.Cost; Flow = rhs.Flow;
		AlphaBpr = rhs.AlphaBpr;
		BetaBBpr = rhs.BetaBBpr;
		RecoverTime = rhs.RecoverTime;  // recover time
		RequiredRes=rhs.RequiredRes; // required resources
		return*this;
	}

};

class NODE
{
	// Head ---> Tail
public:
	int ID;
	std::vector<const LINK*> OutLinks;
	std::vector<const LINK*> InLinks;
	//std::vector<int> OutLinks;
	//std::vector<int> InLinks;
	NODE(){
		ID = -1;
		OutLinks.reserve(10);
		InLinks.reserve(10);
	}
	~NODE(){
		OutLinks.clear();
		InLinks.clear();
	}
};

class OD
{
public:
	int ID;
	int Orign;
	int Dest;
	double Demand;
	double MinCost;
	double BeforeRemoveSpDist;
	double AfterRemoveSpDist;
	bool isConnected;
	OD(){
		ID = -1;
		Orign = InvaildInt;
		Dest = InvaildInt;
		isConnected = false;
		Demand = InvaildFloat;
		MinCost = InvaildFloat;
		BeforeRemoveSpDist = InvaildFloat;
		AfterRemoveSpDist = InvaildFloat;
	}
	~OD(){
		Orign = InvaildInt;
		Dest = InvaildInt;
		isConnected = false;
		Demand = InvaildFloat;
		MinCost = InvaildFloat;
		BeforeRemoveSpDist = InvaildFloat;
		AfterRemoveSpDist = InvaildFloat;
	}
};

class OriginBasedOD
{
public:
	int Onode;
	std::vector<const OD*>  ODset;
	//std::vector<OD> ODset;
	OriginBasedOD(){
		Onode = InvaildInt;
		ODset.reserve(NumNodes);
	};
	~OriginBasedOD(){
		Onode = InvaildInt;
		ODset.clear();
	}
};

// Contains the disruption scenario
class Scenario
{
public:
	std::vector<int> LinkIds;  // affected link id
	std::vector<int> tau;   // time period for the corresponding times
	Scenario() {
		LinkIds.clear(); tau.clear();
	}
	~Scenario() { LinkIds.clear(); tau.clear(); }
	void print()
	{
		for (size_t i = 0; i < LinkIds.size(); i++)
		{
			std::cout << "DisruptedLinkID=" << LinkIds.at(i) << ",TimePeriod=" << tau.at(i) << endl;
		}
	}
};


class GRAPH
{
public:
	std::vector<OD> OdPairs;
	std::vector<LINK> Links;
	std::vector<NODE> Nodes;
	std::vector<OriginBasedOD> OriginSet;
	//double UNPM;
	double TotalSystemCost;
	int **MinPathPredLink;
	GRAPH();
	~GRAPH();
	int SP(const int Orig, std::vector<double> &Lable);
	int FindMinCostRoutes();
	int FW_UE();
	void CreateOriginSet();
	void CreateNodes();
	int PrintLinks(std::ofstream &fout);
	int PrintLinks_onscreen();
	int PrintOD(std::ofstream &fout);
	int PrintSp(int Orign, int Dest, std::ofstream &fout);
	void EvaluteGraph();//ue total cost unpin
	void EvalutateFailureScenarios(const Scenario &s);  // evaluate one failure scenario 
	void RevertFailureScenarios(const Scenario &s);  // evaluate one failure scenario 
	friend class CHROME;
	friend class Algorithms;
	void ReadGraphData();
	void ReadDataMain() {
		ofstream OutPutFile;
		ReadGraphData();
		OutPutFile.open("..//OutPut//LinkData.txt");
		PrintLinks(OutPutFile);
		OutPutFile.close();
		OutPutFile.open("..//OutPut//DemandData.txt");
		PrintOD(OutPutFile);
		OutPutFile.close();
	}
	double CalRelSpChange(int LinkID);
};


class TEST
{
	int ID;
	TEST(){
		ID = InvaildInt;
	}
};


class CHROME // ==  scenario
{
public:
	int ID;
	vector<int> Nodes;
	vector<double> NodeDof;
	vector<double> NodeDofProb;
	vector<int> Links;
	vector<double> LinkDof;
	vector<double> LinkDofProb;
	double SolProb;
	double TotalCost;
	double UNPM;
	double ImpactValue;
	double Fitness;
	void clear();
	//void EvaluateSol(GRAPH &Graph, const double BaseUNPM, const NODEPROMATRIX &Pmatrix,
	//	const LINKPROMATRIX &LinkPmatrix);
	void ReviseCap(GRAPH &Graph);
	void IniCap(GRAPH &Graph);
	//double getSolProb(const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix);
	int PrintSol(ofstream &fout);
	//void PrintGrahpLink(ofstream &fout);
	//void PrintGrahpOD(ofstream &fout);
	void Copy(const CHROME& FromSource); // does not copy solution ID
	bool isSame(const CHROME &CompareSource);
	CHROME();
	~CHROME();
	friend class Algorithms;
};

class Algorithms // ==  scenario
{
public:
	double BaseUNPM;
	double MaxFitValue;
	double MinFitValue;
	AlgorithmType AlgorithmIndex;  // 1: CSA, 2:GA
	vector<int> NodeVarSet;
	vector<double> NodeDofVarSet;// set of degree of reduction
	vector<CHROME> Chroms;
	int  GenerateSol(int ChromIndex);
	void SortSol(unsigned int Num);
	void HyperMutateMain(CHROME &Chrom);
	void FirstProcedure(CHROME &Chrom);
	void SecondProcedure(CHROME &Chrom, double Ratio);
	void addNewNode(CHROME &Chrom);
	void removeNodeDof(CHROME &Chrom);
	void exchangeNodeDof(CHROME &Chrom);
	void GAselectParent(int &Father, int &Mother, const int NumPop);
	void GACrossOver(CHROME &Father, CHROME &Mother, CHROME &BigBro, CHROME &CuteSis);
	//void GAmain(GRAPH &Graph, const int NumPop, const int NumChild, const NODEPROMATRIX &ProbMatrix,
		//const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);*/
	//Algorithms(int NumPop, int NumClone, int NumRep);//CSA
	Algorithms(int NumPop, int NumChild);//GA
	~Algorithms();
		
	//CSA
	//void CSAmain(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
	//	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
		//void CSAmainVarA(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
	//	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
};
#endif