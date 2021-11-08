#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
#include "RandomFuncs.h"
using namespace std;
void GenerateCloneSet(const vector<CHROME> Chroms, const int NumPop, const int NumClone,
	vector<int> &CloneSet);
// the compare class is from http://www.cplusplus.com/reference/algorithm/sort/
struct CompareStruct {
	bool operator() (const CHROME &A, CHROME &B) { return (A.Fitness > B.Fitness); }
} SortDecent;
Algorithms::Algorithms(int NumPop, int NumClone, int NumRep)
{
	//Pop solution
	CHROME ts;
	int idCount = 0;
	for (int i = 0; i < NumPop; i++)
	{
		ts.ID = idCount;
		this->Chroms.push_back(ts);
		idCount++;
	}
	// clone
	for (int i = 0; i < NumPop / 2; i++)
	{
		for (int j = 0; j < NumClone; j++)
		{
			ts.ID = idCount;
			this->Chroms.push_back(ts);
			idCount++;
		}
	}
	// Rep
	for (int i = 0; i < NumRep; i++)
	{
		ts.ID = idCount;
		this->Chroms.push_back(ts);
		idCount++;
	}
	NodeVarSet.reserve(NumNodes);
	NodeDofVarSet.reserve(NumNodes);
	AlgorithmIndex = CSA;
};
Algorithms::Algorithms(int NumPop, int NumChild){

	//Pop solution
	CHROME ts;
	int idCount = 0;
	for (int i = 0; i < NumPop; i++)
	{
		ts.ID = idCount;
		this->Chroms.push_back(ts);
		idCount++;
	}
	// child
	for (int i = 0; i < NumChild; i++)
	{
		ts.ID = idCount;
		this->Chroms.push_back(ts);
		idCount++;
	}
	NodeVarSet.reserve(NumNodes);
	NodeDofVarSet.reserve(NumNodes);
	AlgorithmIndex = GA;
}
void RecordSolVa(double &SolFit, double &CurrentBest,int &NumCount,ofstream &fout){
	if (SolFit>CurrentBest)
	{
		CurrentBest = SolFit;
	}
	NumCount++;

	if (isWriteConverge)
	{
		// write best solution with respect to the number of solution generated
		fout << SeedRunCount<<","<<NumCount << "," << CurrentBest << endl;
	}
}


Algorithms::~Algorithms(){
	this->Chroms.clear();
	NodeVarSet.clear();
	NodeDofVarSet.clear();
};
void Algorithms::SortSol(unsigned int Num)
{
	//cout << (this->Chroms.begin() + Num)->ID << endl;
	std::sort(this->Chroms.begin(), this->Chroms.begin() + Num, SortDecent);
	MaxFitValue = Chroms.begin()->Fitness;
	MinFitValue = (Chroms.begin() + Num - 1)->Fitness;
	//assert(MinFitValue >= -10.0);
};
int Algorithms::GenerateSol(int ChromIndex)
{
	/******************Random generation procedure***************/
	//vector<int> CandiSet(NodeVarSet);

	//int pos = -1;
	///*this->Chroms.at(ChromIndex).Nodes.clear();
	//this->Chroms.at(ChromIndex).Dof.clear();*/
	//for (unsigned int i = 0; i < NodeVarSet.size(); i++)
	//{
	//	pos = GenRandomPos((unsigned int)CandiSet.size());

	//	//this->Chroms.at(ChromIndex).Nodes.push_back(NodeVarSet.at(pos));
	//	this->Chroms.at(ChromIndex).Nodes.push_back(CandiSet.at(pos));
	//	CandiSet.erase(CandiSet.begin() + pos);
	//	this->Chroms.at(ChromIndex).NodeDof.push_back(GenRandomdouble(NodeDofVarSet));
	//}
	/*******************************************/


	/******************Secquence***************/
	//vector<int> CandiSet(NodeVarSet);
	for (int i = 0; i < NodeVarSet.size(); i++)
	{
		this->Chroms.at(ChromIndex).Nodes.push_back(NodeVarSet.at(i));
		this->Chroms.at(ChromIndex).NodeDof.push_back(GenRandomFloat(NodeDofVarSet));
	}
	//int pos = -1;
	///*this->Chroms.at(ChromIndex).Nodes.clear();
	//this->Chroms.at(ChromIndex).Dof.clear();*/
	//for (unsigned int i = 0; i < NodeVarSet.size(); i++)
	//{
	//	pos = GenRandomPos((unsigned int)CandiSet.size());

	//	//this->Chroms.at(ChromIndex).Nodes.push_back(NodeVarSet.at(pos));
	//	this->Chroms.at(ChromIndex).Nodes.push_back(CandiSet.at(pos));
	//	CandiSet.erase(CandiSet.begin() + pos);
	//	this->Chroms.at(ChromIndex).NodeDof.push_back(GenRandomFloat(NodeDofVarSet));
	//}
	/*******************************************/


	return 1;
};


      //Oringal colone section
void Algorithms::CSAmain(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile)
{
	//phase 1
	ofstream fsolconv;
	fsolconv.open("..//OutPut//CSAConvergeBasedOnSol.txt",ios::app);
	double CurrentBestFitness = -1.0f;
	int NumSolEvaluated = 0;
	int IdCount = NumPop;
	for (int i = 0; i < NumPop; i++)
	{
		//cout << "Evaluate solution " << i << endl;
		bool isRepeat = false;
		do
		{
			this->Chroms.at(i).clear();
			GenerateSol(i);
			isRepeat = false;
			for (int j = 0; j <= i; j++)
			{
				if (j == i) break;
				if (this->Chroms.at(i).isSame(this->Chroms.at(j)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);
		// test zoe network 

/*		for (int m = 0; m < 24; m++) this->Chroms.at(i).NodeDof[m] = 0.0;
		this->Chroms.at(i).NodeDof[9] = 0.6*/;

		this->Chroms.at(i).getSolProb(NodeProbMatrix, LinkProbMatrix);
		Chroms.at(i).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(i).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
	}

	int IterCounter = 0;
	this->SortSol((unsigned int)NumPop);
	vector<int> CloneSet(NumClone, -1);
Repeat:
	IterCounter++;
	IdCount = NumPop;

	GenerateCloneSet(this->Chroms, NumPop, NumClone, CloneSet);
	//phase 2
	// clone: for half solution each colon 
	for (int i = 0; i < CloneSet.size();i++)
	{
		Chroms.at(IdCount).Copy(Chroms.at(CloneSet.at(i)));
		bool isRepeat;
		do
		{
			isRepeat = false;
			this->HyperMutateMain(Chroms.at(IdCount));
			for (int kk = 0; kk <= IdCount; kk++)
			{
				if (kk == IdCount) break;
				if (this->Chroms.at(IdCount).isSame(this->Chroms.at(kk)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);
		Chroms.at(IdCount).getSolProb(NodeProbMatrix, LinkProbMatrix);
		Chroms.at(IdCount).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(IdCount).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
		if (StopCriteria == 1 && NumSolEvaluated >= MaxNumSolEval) return;
		IdCount++;
	}
		
	for (int i = 0; i < NumRep; i++)
	{
		bool isRepeat = false;
		do
		{
			this->Chroms.at(IdCount).clear();
			GenerateSol(IdCount);
			isRepeat = false;
			for (int j = 0; j <= IdCount; j++)
			{
				if (j == IdCount) break;
				if (this->Chroms.at(IdCount).isSame(this->Chroms.at(j)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);
		Chroms.at(IdCount).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		this->Chroms.at(IdCount).getSolProb(NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(IdCount).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
		if (StopCriteria == 1 && NumSolEvaluated >= MaxNumSolEval) return;
		IdCount++;
	}

	this->SortSol((unsigned int)Chroms.size());
	if (isWriteConverge)
	{
		ConvergeFile << SeedRunCount <<","<< IterCounter << "," << this->Chroms.at(0).Fitness << endl;
	}
#if DEBUG
	cout << IterCounter << "\t" << this->Chroms.at(0).Fitness << endl;
#endif
	if (StopCriteria == 0&&IterCounter < MaxCsaIter) goto Repeat;
	if (StopCriteria == 1 && NumSolEvaluated<MaxNumSolEval) goto Repeat;

	fsolconv.close();
}
void Algorithms::CSAmainVarA(GRAPH &Graph, int NumPop, int NumClone, int NumRep,
	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrix, ofstream &ConvergeFile)
{
	//phase 1
	ofstream fsolconv;
	fsolconv.open("..//OutPut//CSAConvergeBasedOnSol.txt", ios::app);
	double CurrentBestFitness = -1.0f;
	int NumSolEvaluated = 0;
	int IdCount = NumPop;
	for (int i = 0; i < NumPop; i++)
	{
		bool isRepeat = false;
		do
		{
			this->Chroms.at(i).clear();
			GenerateSol(i);
			isRepeat = false;
			for (int j = 0; j <= i; j++)
			{
				if (j == i) break;
				if (this->Chroms.at(i).isSame(this->Chroms.at(j)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);

		this->Chroms.at(i).getSolProb(NodeProbMatrix, LinkProbMatrix);



		Chroms.at(i).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(i).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
	}
	int IterCounter = 0;
	this->SortSol((unsigned int)NumPop);
	vector<double> CloneProb;
	double fitSum = 0.0;
	vector<int> CloneVec;
Repeat:
	IterCounter++;
	//cout << "Iteration = " << IterCounter << endl;
	//if (IterCounter == 3)
	//{
	//	cout << endl;
	//}
	//cout << "CSA BaseUNPM = " << this->BaseUNPM << endl;
	IdCount = NumPop;
	//phase 2
	// clone: for half solution each colon 

	//select clone based on the probablity 
	CloneProb.assign(NumPop+1, 0.0);
	fitSum = 0.0;
	for (int i = 0; i < NumPop;i++)
	{
		fitSum += this->Chroms.at(i).Fitness;
	}
	CloneProb.at(0) = 0.0f;
	for (int i = 0; i < NumPop; i++)
	{
		CloneProb.at(i + 1) = CloneProb.at(i) + this->Chroms.at(i).Fitness / fitSum;// cumulative prob
	}
	CloneVec.assign(NumClone, -1);
	for (int i = 0; i < NumClone;i++)
	{
		double f = GenRandomReal();
		for (int j = 0; j < CloneProb.size() - 1; j++)
		{
			if (f >= CloneProb.at(j) && f < CloneProb.at(j + 1))
			{
				CloneVec.at(i) = j;
				break;
			}
		}
	}

	for (int i = 0; i < CloneVec.size();i++)
	{
		Chroms.at(IdCount).Copy(Chroms.at(CloneVec.at(i)));
		bool isRepeat;
		do
		{
			isRepeat = false;
			this->HyperMutateMain(Chroms.at(IdCount));
			for (int kk = 0; kk <= IdCount; kk++)
			{
				if (kk == IdCount) break;
				if (this->Chroms.at(IdCount).isSame(this->Chroms.at(kk)))
				{
					isRepeat = true;
					break;
				}
			}
			//} while (this->Chroms.at(IdCount).isSame(Chroms.at(i)));
		} while (isRepeat);

		Chroms.at(IdCount).getSolProb(NodeProbMatrix, LinkProbMatrix);
		Chroms.at(IdCount).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(IdCount).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
		if (StopCriteria == 1 && NumSolEvaluated >= MaxNumSolEval) return;
		IdCount++;
	}

	for (int i = 0; i < NumRep; i++)
	{
		bool isRepeat = false;
		do
		{
			this->Chroms.at(IdCount).clear();
			GenerateSol(IdCount);

			isRepeat = false;
			for (int j = 0; j <= IdCount; j++)
			{
				if (j == IdCount) break;
				if (this->Chroms.at(IdCount).isSame(this->Chroms.at(j)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);


		Chroms.at(IdCount).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		this->Chroms.at(IdCount).getSolProb(NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(IdCount).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
		if (StopCriteria == 1 && NumSolEvaluated >= MaxNumSolEval) return;
		IdCount++;
	}

	this->SortSol((unsigned int)Chroms.size());
	if (isWriteConverge)
	{
		ConvergeFile << SeedRunCount<<","<<IterCounter << "," << this->Chroms.at(0).Fitness << endl;
	}

	if (IterCounter < MaxCsaIter) goto Repeat;
	fsolconv.close();
}

