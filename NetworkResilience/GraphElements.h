
#ifndef MyClasses
#define MyClasses
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
using namespace std;

template <typename T>  T** Create2DArray(const int dim1, const int dim2);
class LinkClass
{
	// Tail ---> Head 
public:
	int Id;
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
	int RecoverTime;  // recover time
	int RequiredRes; // required resources
	double CleanLinkFlow();
	double BPRCost();
	double IniCost();
	void IniCap();
	LinkClass():Id(InvaildInt), Tail(InvaildInt), Head(InvaildInt), T0(-1.0),
	CaInput(InvaildFloat), CaRevise(InvaildFloat),
	Cost(InvaildFloat), Flow(InvaildFloat), AlphaBpr(0.15), BetaBBpr(4.0),
	EI(0), RecoverTime(-1), RequiredRes(-1){}
	~LinkClass() = default;
	LinkClass(LinkClass&& other) = default;
	LinkClass(const LinkClass& otherLink);
	LinkClass& operator=(LinkClass&& other) = default;
	LinkClass& operator=(const LinkClass& rhs) = default;
};

class NodeClass
{
	// Head ---> Tail
public:
	int Id;
	std::vector<const LinkClass*> OutLinks;
	std::vector<const LinkClass*> InLinks;
	//std::vector<int> OutLinks;
	//std::vector<int> InLinks;
	NodeClass():Id(-1) { OutLinks.reserve(10); InLinks.reserve(10);
	}
	NodeClass(const NodeClass& rhs):Id(rhs.Id)
	{
		OutLinks.assign(rhs.OutLinks.begin(), rhs.OutLinks.end());
		InLinks.assign(rhs.InLinks.begin(), rhs.InLinks.end());
	}
	NodeClass& operator=(const NodeClass& rhs)
	{
		if (this != &rhs)
		{
			Id = rhs.Id;
			OutLinks.assign(rhs.OutLinks.begin(), rhs.OutLinks.end());
			InLinks.assign(rhs.InLinks.begin(), rhs.InLinks.end());
		}
		return *this;
	}
	NodeClass(NodeClass&& other) noexcept:Id(other.Id)
	{
		OutLinks.assign(other.OutLinks.begin(), other.OutLinks.end());
		InLinks.assign(other.InLinks.begin(), other.InLinks.end());
	}
	NodeClass& operator=(NodeClass&& other) noexcept
	{
		if (this != &other)
		{
			Id = other.Id;
			OutLinks.assign(other.OutLinks.begin(), other.OutLinks.end());
			InLinks.assign(other.InLinks.begin(), other.InLinks.end());
		}
		return *this;
	}
	~NodeClass(){
	/*	for (vector<const LinkClass*>::iterator itr = OutLinks.begin(); itr !=OutLinks.end(); ++itr) {
			delete* itr;
		}*/
		OutLinks.clear();
		//for (vector<const LinkClass*>::iterator itr = InLinks.begin(); itr !=InLinks.end(); ++itr) {
		//	delete* itr;
		//}
		InLinks.clear();
	}
};

class ODClass
{
public:
	int Id;
	int Origin;
	int Dest;
	double Demand;
	double MinCost;
	double BeforeRemoveSpDist;
	double AfterRemoveSpDist;
	bool isConnected;
	ODClass():Id(-1), Origin(InvaildInt), Dest(InvaildInt),
	Demand(InvaildFloat),
	MinCost(InvaildFloat),
	BeforeRemoveSpDist(InvaildFloat),
	AfterRemoveSpDist(InvaildFloat),
	isConnected(false){}
	~ODClass() = default;
	ODClass(const ODClass& rhs) = default;
	ODClass& operator=(const ODClass& rsh) = default;
	ODClass(ODClass&& other) = default;
	ODClass& operator=(ODClass&& other) = default;
};

class OriginBasedOD
{
public:
	int Onode;
	std::vector<const ODClass*>  ODset;
	//std::vector<ODClass> ODset;
	OriginBasedOD():Onode(InvaildInt){
		ODset.reserve(numNodes);
	}
	//~OriginBasedOD(){
	//	Onode = InvaildInt;
	//	ODset.clear();
	//}
};

// Contains the disruption scenario
class ScenarioClass
{
public:
	std::vector<int> LinkIds;  // affected link Id
	std::vector<int> tau;   // time period for the corresponding times
	ScenarioClass() = default;
	ScenarioClass& operator=(const ScenarioClass& rhs) = default;
	ScenarioClass(const ScenarioClass& rhs) = default;
	ScenarioClass(ScenarioClass&&) = default;
	ScenarioClass& operator=(ScenarioClass&& rhs) = default;
	~ScenarioClass() { LinkIds.clear(); tau.clear(); }
	void Print() const
	{
		for (size_t i = 0; i < LinkIds.size(); i++)
		{
			std::cout << "DisruptedLinkID=" << LinkIds.at(i) << ",TimePeriod=" << tau.at(i) << endl;
		}
	}
};


class GraphClass
{
public:
	double UNPM;
	double TotalSystemCost;
	int** MinPathPredLink;
	std::vector<ODClass> OdPairs;
	std::vector<LinkClass> Links;
	std::vector<NodeClass> Nodes;
	std::vector<OriginBasedOD> OriginSet;
	GraphClass();
	~GraphClass();
	GraphClass& operator=(const GraphClass& rhs)
	{
		if (this == &rhs) return *this;
		UNPM = rhs.UNPM; TotalSystemCost = rhs.TotalSystemCost;
		for (int i = 0; i < numNodes + 1; i++)
		{
			for (int j = 0; j < numNodes + 1; j++)
			{
				MinPathPredLink[i][j] = rhs.MinPathPredLink[i][j];
			}
		}
		OdPairs.assign(rhs.OdPairs.begin(), rhs.OdPairs.end());
		Links.assign(rhs.Links.begin(), rhs.Links.end());
		Nodes.assign(rhs.Nodes.begin(), rhs.Nodes.end());
		OriginSet.assign(rhs.OriginSet.begin(), rhs.OriginSet.end());
		return *this;
	}
	GraphClass(const GraphClass& rhs):UNPM(rhs.UNPM), TotalSystemCost(rhs.TotalSystemCost),
		MinPathPredLink(Create2DArray<int>(numNodes + 1, numNodes + 1))
	{
		for (int i = 0; i < numNodes + 1; i++)
		{
			for (int j = 0; j < numNodes + 1; j++)
			{
				MinPathPredLink[i][j] = rhs.MinPathPredLink[i][j];
			}
		}
		OdPairs.assign(rhs.OdPairs.begin(), rhs.OdPairs.end());
		Links.assign(rhs.Links.begin(), rhs.Links.end());
		Nodes.assign(rhs.Nodes.begin(), rhs.Nodes.end());
		OriginSet.assign(rhs.OriginSet.begin(), rhs.OriginSet.end());
	}
	GraphClass(GraphClass&& other) noexcept:UNPM(other.UNPM),TotalSystemCost(other.TotalSystemCost),
		MinPathPredLink(Create2DArray<int>(numNodes + 1, numNodes + 1))
	{
		for (int i = 0; i < numNodes + 1; i++)
		{
			for (int j = 0; j < numNodes + 1; j++)
			{
				MinPathPredLink[i][j] = other.MinPathPredLink[i][j];
			}
		}
		OdPairs.assign(other.OdPairs.begin(), other.OdPairs.end());
		Links.assign(other.Links.begin(), other.Links.end());
		Nodes.assign(other.Nodes.begin(), other.Nodes.end());
		OriginSet.assign(other.OriginSet.begin(), other.OriginSet.end());
	}
	GraphClass& operator=(GraphClass&& rhs) noexcept
	{
		if (this!=&rhs)
		{
			UNPM = rhs.UNPM; TotalSystemCost = rhs.TotalSystemCost;
			for (int i = 0; i < numNodes + 1; i++)
			{
				for (int j = 0; j < numNodes + 1; j++)
				{
					MinPathPredLink[i][j] = rhs.MinPathPredLink[i][j];
				}
			}
			OdPairs.assign(rhs.OdPairs.begin(), rhs.OdPairs.end());
			Links.assign(rhs.Links.begin(), rhs.Links.end());
			Nodes.assign(rhs.Nodes.begin(), rhs.Nodes.end());
			OriginSet.assign(rhs.OriginSet.begin(), rhs.OriginSet.end());
		}
		return *this;
	}
	int SP(const int origin, std::vector<double> &label);
	int FindMinCostRoutes();
	int FW_UE();
	void CreateOriginSet();
	void CreateNodes();
	int PrintLinks(std::ofstream &fout);
	int PrintLinks_onscreen();
	int PrintOD(std::ofstream &fout);
	int PrintSp(int Orign, int Dest, std::ofstream &fout);
	void EvaluteGraph();//ue total cost unpin
	void EvalutateFailureScenarios(const ScenarioClass &s);  // evaluate one failure scenario 
	void RevertFailureScenarios(const ScenarioClass &s);  // evaluate one failure scenario 
	//friend class CHROME;
	//friend class Algorithms;
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
public:
	int Id;
	TEST():Id(InvaildInt){}
};

#pragma region NotUsedInResotration
//class CHROME // ==  scenario
//{
//public:
//	int ID;
//	vector<int> Nodes;
//	vector<double> NodeDof;
//	vector<double> NodeDofProb;
//	vector<int> Links;
//	vector<double> LinkDof;
//	vector<double> LinkDofProb;
//	double SolProb;
//	double TotalCost;
//	double UNPM;
//	double ImpactValue;
//	double Fitness;
//	void clear();
//	//void EvaluateSol(GraphClass &Graph, const double BaseUNPM, const NODEPROMATRIX &Pmatrix,
//	//	const LINKPROMATRIX &LinkPmatrix);
//	void ReviseCap(GraphClass &Graph);
//	void IniCap(GraphClass &Graph);
//	//double getSolProb(const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix);
//	int PrintSol(ofstream &fout);
//	//void PrintGrahpLink(ofstream &fout);
//	//void PrintGrahpOD(ofstream &fout);
//	void Copy(const CHROME& FromSource); // does not copy solution Id
//	bool isSame(const CHROME &CompareSource);
//	CHROME();
//	~CHROME();
//	friend class Algorithms;
//};

#pragma endregion

#pragma region NotUsedInRestoration
//class Algorithms // ==  scenario
//{
//public:
//	double BaseUNPM;
//	double MaxFitValue;
//	double MinFitValue;
//	AlgorithmType AlgorithmIndex;  // 1: CSA, 2:GA
//	vector<int> NodeVarSet;
//	vector<double> NodeDofVarSet;// set of degree of reduction
//	vector<CHROME> Chroms;
//	int  GenerateSol(int ChromIndex);
//	void SortSol(unsigned int Num);
//	void HyperMutateMain(CHROME &Chrom);
//	void FirstProcedure(CHROME &Chrom);
//	void SecondProcedure(CHROME &Chrom, double Ratio);
//	void addNewNode(CHROME &Chrom);
//	void removeNodeDof(CHROME &Chrom);
//	void exchangeNodeDof(CHROME &Chrom);
//	void GAselectParent(int &Father, int &Mother, const int NumPop);
//	void GACrossOver(CHROME &Father, CHROME &Mother, CHROME &BigBro, CHROME &CuteSis);
//	//void GAmain(GraphClass &Graph, const int NumPop, const int NumChild, const NODEPROMATRIX &ProbMatrix,
//		//const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);*/
//	//Algorithms(int NumPop, int NumClone, int NumRep);//CSA
//	Algorithms(int NumPop, int NumChild);//GA
//	~Algorithms();
//		
//	//CSA
//	//void CSAmain(GraphClass &Graph, int NumPop, int NumClone, int NumRep,
//	//	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
//		//void CSAmainVarA(GraphClass &Graph, int NumPop, int NumClone, int NumRep,
//	//	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
//};
#pragma endregion
#endif