
#ifndef MyClasses
#define MyClasses
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

	double CleanLinkFlow();
	double BPRCost();
	double IniCost();
	void IniCap();
	LINK();
	LINK(const LINK& OtherLink);
	~LINK();

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
	bool isConnected;
	OD(){
		Orign = InvaildInt;
		Dest = InvaildInt;
		isConnected = false;
		Demand = InvaildFloat;
		MinCost = InvaildFloat;
	}
	~OD(){
		Orign = InvaildInt;
		Dest = InvaildInt;
		isConnected = false;
		Demand = InvaildFloat;
		MinCost = InvaildFloat;
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

class GRAPH
{
public:
	std::vector<OD> OdPairs;
	std::vector<LINK> Links;
	std::vector<NODE> Nodes;
	std::vector<OriginBasedOD> OriginSet;
	double UNPM;
	double TotalSystemCost;
	//std::vector<LINK> Links;
	//std::vector<NODE> Nodes;
	//std::vector<OD> ODpairs;
	//std::vector<OriginBasedOD> OriginSet;
	int **MinPathPredLink;
	GRAPH();
	~GRAPH();
	int SP(const int Orig, std::vector<double> &Lable);
	int FindMinCostRoutes();
	int FW_UE();
	void CreateOriginSet();
	void CreateNodes();
	int PrintLinks(std::ofstream &fout);
	int PrintOD(std::ofstream &fout);
	int PrintSp(int Orign, int Dest, std::ofstream &fout);
	void EvaluteGraph();//ue total cost unpn
	friend class CHROME;
	friend class Algorithms;
};
class TEST
{
	int ID;
	TEST(){
		ID = InvaildInt;
	}

};

class NODEPROMATRIX{
public:
	vector<double> Dof; // degree of failure
	double **Matrix;// matrix[degree of failure index][node number]
	NODEPROMATRIX();
	~NODEPROMATRIX();
	int PrintMatrix(ofstream &fout);
};


class LINKPROMATRIX{
public:
	vector<double> Dof; // degree of failure
	double **Matrix;// matrix[degree of failure index][node number]
	LINKPROMATRIX();
	~LINKPROMATRIX();
	int PrintMatrix(ofstream &fout);
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
	void EvaluateSol(GRAPH &Graph, const double BaseUNPM, const NODEPROMATRIX &Pmatrix,
		const LINKPROMATRIX &LinkPmatrix);
	void ReviseCap(GRAPH &Graph);
	void IniCap(GRAPH &Graph);
	double getSolProb(const NODEPROMATRIX &Pmatrix, const LINKPROMATRIX &LinkPmatrix);
	int PrintSol(ofstream &fout);
	void PrintGrahpLink(ofstream &fout);
	void PrintGrahpOD(ofstream &fout);

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
	void CSAmain(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
		const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
	void GAmain(GRAPH &Graph, const int NumPop, const int NumChild, const NODEPROMATRIX &ProbMatrix,
		const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
	void CSAmainVarA(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
		const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile);
	Algorithms(int NumPop, int NumClone, int NumRep);//CSA
	Algorithms(int NumPop, int NumChild);//GA
	~Algorithms();

};
#endif