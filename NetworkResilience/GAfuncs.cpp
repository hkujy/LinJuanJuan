#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
#include "RandomFuncs.h"
using namespace std;
void increasdof(double& NowDof, vector<double> DofVarSet);
void decreaseDof(double& NowDof, vector<double> DofVarSet);
struct CompareStruct {
	bool operator() (const CHROME& A, CHROME& B) { return (A.Fitness > B.Fitness); }
} SortDecent;
void Algorithms::SortSol(unsigned int Num)
{
	//cout << (this->Chroms.begin() + Num)->ID << endl;
	std::sort(this->Chroms.begin(), this->Chroms.begin() + Num, SortDecent);
	MaxFitValue = Chroms.begin()->Fitness;
	MinFitValue = (Chroms.begin() + Num - 1)->Fitness;
	//assert(MinFitValue >= -10.0);
};
Algorithms::~Algorithms() {
	this->Chroms.clear();
	NodeVarSet.clear();
	NodeDofVarSet.clear();
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
	for (size_t i = 0; i < NodeVarSet.size(); i++)
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
void Algorithms::HyperMutateMain(CHROME& Chrom) {

	int MutatorIndex;  // from 1 to 5
	double f;
	switch (AlgorithmIndex)
	{
	case CSA:
		f = GenRandomReal();
		if (f < double(1.0f / 6.0f))
		{
			MutatorIndex = 0;	break;
		}
		if (f < double(2.0f / 6.0f) && f >= double(1.0f / 6.0f))
		{
			MutatorIndex = 1;	break;
		}
		if (f < double(3.0f / 6.0f) && f >= double(2.0f / 6.0f))
		{
			MutatorIndex = 2;	break;
		}
		if (f < double(4.0f / 6.0f) && f >= double(3.0f / 6.0f))
		{
			MutatorIndex = 3;	break;
		}
		if (f < double(5.0f / 6.0f) && f >= double(4.0f / 6.0f))
		{
			MutatorIndex = 4;	break;
		}
		if (f <= double(6.0f / 6.0f) && f >= double(5.0f / 6.0f))
		{
			MutatorIndex = 5;
			break;
		}
		//MutatorIndex = CaseIndex(Chrom.Fitness, MaxFitValue, MinFitValue); break;
	case GA:
		f = GenRandomReal();
		if (f < double(1.0f / 6.0f))
		{
			MutatorIndex = 0;	break;
		}
		if (f < double(2.0f / 6.0f) && f >= double(1.0f / 6.0f))
		{
			MutatorIndex = 1;	break;
		}
		if (f < double(3.0f / 6.0f) && f >= double(2.0f / 6.0f))
		{
			MutatorIndex = 2;	break;
		}
		if (f < double(4.0f / 6.0f) && f >= double(3.0f / 6.0f))
		{
			MutatorIndex = 3;	break;
		}
		if (f < double(5.0f / 6.0f) && f >= double(4.0f / 6.0f))
		{
			MutatorIndex = 4;	break;
		}
		if (f <= double(6.0f / 6.0f) && f >= double(5.0f / 6.0f))
		{
			MutatorIndex = 5;
			break;
		}

	default:
		cerr << "Algorithm index is not proper defined" << endl;
		system("PAUSE");
		break;
	}

	switch (MutatorIndex)
	{
	case 0:
		for (unsigned int i = 0; i < Chrom.NodeDof.size(); i++)
		{
			if (GenRandomReal() >= 0.5f) increasdof(Chrom.NodeDof.at(i), NodeDofVarSet);
			else decreaseDof(Chrom.NodeDof.at(i), NodeDofVarSet);
		}
		break;
	case 1:
		if (GenRandomReal() <= 0.1f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.2f);
		break;
	case 2:
		if (GenRandomReal() <= 0.2f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.4f);
		break;
	case 3:
		if (GenRandomReal() <= 0.3f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.6f);
		break;
	case 4:
		if (GenRandomReal() <= 0.4f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 0.8f);
		break;
	case 5:
		if (GenRandomReal() <= 0.5f) this->FirstProcedure(Chrom);
		else this->SecondProcedure(Chrom, 1.0f);
		break;
	default:
		TRACE("CSA Hypermutation main generates wrong Index");
		system("PAUSE");
		break;
	}

};
void Algorithms::GAselectParent(int& Father, int& Mother, const int NumPop) {

	vector<double> Prob(NumPop + 1, 0.0f);
	double sumFit = 0.0;
	for (auto c = this->Chroms.begin(); c < this->Chroms.begin() + NumPop; c++)
	{
		sumFit += c->Fitness;
	}
	assert(sumFit > 0);
	Prob.at(0) = 0.0f;
	for (int i = 0; i < NumPop; i++)
	{
		Prob.at(i + 1) = Prob.at(i) + this->Chroms.at(i).Fitness / sumFit;// cumulative prob
	}
	double f = GenRandomReal();
	for (size_t i = 0; i < Prob.size() - 1; i++)
	{
		if (f >= Prob.at(i) && f < Prob.at(i + 1))
		{
			Father = static_cast<int>(i);
			break;
		}
	}
	do
	{
		double m = GenRandomReal();
		for (size_t i = 0; i < Prob.size() - 1; i++)
		{
			if (m >= Prob.at(i) && m < Prob.at(i + 1))
			{
				Mother =static_cast<int>(i);
				break;
			}
		}
	} while (Father == Mother);

}

void Algorithms::GACrossOver(CHROME& Father, CHROME& Mother, CHROME& BigBro, CHROME& CuteSis)
{
	//step: 1 sect a location
	int pos;
	do
	{
		pos = GenRandomPos((unsigned int)this->NodeVarSet.size() - 1);
	} while (pos == 0);
	if (BigBro.Nodes.size() == 0 || CuteSis.Nodes.size() == 0)
	{
		BigBro.Nodes.assign(Father.Nodes.size(), 0);
		BigBro.NodeDof.assign(Father.Nodes.size(), 0.0f);
		BigBro.NodeDofProb.assign(Father.Nodes.size(), 0.0f);
		CuteSis.Nodes.assign(Father.Nodes.size(), 0);
		CuteSis.NodeDof.assign(Father.Nodes.size(), 0.0f);
		CuteSis.NodeDofProb.assign(Father.Nodes.size(), 0.0f);;
	}
	for (int i = 0; i < pos; i++)
	{
		BigBro.Nodes.at(i) = Father.Nodes.at(i);
		BigBro.NodeDof.at(i) = Father.NodeDof.at(i);
		BigBro.NodeDofProb.at(i) = Father.NodeDofProb.at(i);
		CuteSis.Nodes.at(i) = Mother.Nodes.at(i);
		CuteSis.NodeDof.at(i) = Mother.NodeDof.at(i);
		CuteSis.NodeDofProb.at(i) = Mother.NodeDofProb.at(i);
	}
	for (size_t i = pos; i < this->NodeVarSet.size(); i++)
	{
		BigBro.Nodes.at(i) = Mother.Nodes.at(i);
		BigBro.NodeDof.at(i) = Mother.NodeDof.at(i);
		BigBro.NodeDofProb.at(i) = Mother.NodeDofProb.at(i);
		CuteSis.Nodes.at(i) = Father.Nodes.at(i);
		CuteSis.NodeDof.at(i) = Father.NodeDof.at(i);
		CuteSis.NodeDofProb.at(i) = Father.NodeDofProb.at(i);
	}


}
void RecordSolVa(double& SolFit, double& CurrentBest, int& NumCount, ofstream& fout) {
	if (SolFit > CurrentBest)
	{
		CurrentBest = SolFit;
	}
	NumCount++;

	if (isWriteConverge)
	{
		// write best solution with respect to the number of solution generated
		fout << SeedRunCount << "," << NumCount << "," << CurrentBest << endl;
	}
}

void Algorithms::GAmain(GRAPH& Graph, const int NumPop, const int NumChild,
	const NODEPROMATRIX& NodeProbMatrix, const LINKPROMATRIX& LinkProbMatrix, ofstream& ConvergeFile)
{
	//phase 1 Generate Initial Solution
	ofstream fsolconv;
	fsolconv.open("..//OutPut//GAConvergeBasedOnSol.txt", ios::app);
	double CurrentBestFitness = -1.0f;
	int NumSolEvaluated = 0;

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
Repeat:
	IterCounter++;
	//cout << "Iteration = " << IterCounter << endl;
	// step 1 crossover and generate offspring 
	int Father, Mother;
	for (int i = 0; i < NumChild; i = i + 2)
	{
		this->GAselectParent(Father, Mother, NumPop);
		int BigBro = NumPop + i;
		int CuteSis = NumPop + i + 1;
		// assume crossover rate is 1.0
		this->GACrossOver(this->Chroms.at(Father), this->Chroms.at(Mother),
			this->Chroms.at(BigBro), this->Chroms.at(CuteSis));
	}
	// step 2 mutation on the offspring
	int NumMutation = static_cast<int>(GaMutationRate * NumPop);

	std::vector<bool> Mutated(NumChild, false);

	for (int i = 0; i < NumMutation; i++)
	{
		int MutationIndex = static_cast<int>(NumChild * GenRandomReal());
		int LoopCount = 0;
		while (Mutated.at(MutationIndex) && LoopCount < 100)
		{
			MutationIndex = static_cast<int>(NumChild * GenRandomReal());
			LoopCount++;
		}
		Mutated.at(MutationIndex) = true;
		MutationIndex += NumPop;
		bool isRepeat;
		do
		{
			isRepeat = false;
			this->HyperMutateMain(this->Chroms.at(MutationIndex));
			for (size_t kk = 0; kk < Chroms.size(); kk++)
			{
				if (kk == MutationIndex) continue;
				if (this->Chroms.at(MutationIndex).isSame(this->Chroms.at(kk)))
				{
					isRepeat = true;
					break;
				}
			}
		} while (isRepeat);
	}

	// step 3 evaluate offspring

	for (int i = 0; i < NumChild; i++)
	{
		int ChildIndex = i + NumPop;
		this->Chroms.at(ChildIndex).getSolProb(NodeProbMatrix, LinkProbMatrix);
		Chroms.at(ChildIndex).EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkProbMatrix);
		RecordSolVa(Chroms.at(ChildIndex).Fitness, CurrentBestFitness, NumSolEvaluated, fsolconv);
		if (StopCriteria == 1 && NumSolEvaluated >= MaxNumSolEval) return;
	}

	// step 4 sort and return next generation

	this->SortSol((unsigned int)Chroms.size());
	if (isWriteConverge)
	{
		ConvergeFile << SeedRunCount << "," << IterCounter << "," << this->Chroms.at(0).Fitness << endl;
		//cout << IterCounter << "," << this->Chroms.at(0).Fitness << endl;
	}

	Mutated.clear();
	if (StopCriteria == 0 && IterCounter < MaxGAIter) goto Repeat;
	if (StopCriteria == 1 && NumSolEvaluated < MaxNumSolEval) goto Repeat;

	fsolconv.close();
}

