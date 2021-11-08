#include "CommonHeaders.h"
using namespace std;

void EnumerateMedium(vector<CHROME> &Sol, const NODEPROMATRIX &Prob);

int ScanMedium(GRAPH &BaseGraph, vector<CHROME> &BestSol,
	const NODEPROMATRIX &NodeProbMatrix, const LINKPROMATRIX &LinkProbMatrx)
{
	try
	{

		//Step 0: compute basic value 
		BaseGraph.EvaluteGraph();
		// step 1 enumerate all possibilities 
		vector<CHROME> Sol;
		EnumerateMedium(Sol, NodeProbMatrix);
		double BaseUNPM = BaseGraph.UNPM;

		CHROME tmc;
		Sol.push_back(tmc);
		BestSol.push_back(tmc);
		BestSol.back().ImpactValue = 0.0;
		BestSol.back().Fitness = 0.0;

		for (int i = 0; i < NumNodes; i++)
		{
			Sol.back().Nodes.push_back(i);
		}
		/*0	0	0.8	0	0	0	0	0.6*/
	/*	Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.8f);
		Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.0f);
		Sol.back().Dof.push_back(0.6f);
		Sol.back().getSolProb(Pmatrix);*/

		for (auto s = Sol.begin(); s != Sol.end(); s++ )
		{
			s->EvaluateSol(BaseGraph, BaseUNPM, NodeProbMatrix,LinkProbMatrx);
			//cout << "S->Fit = " << s->Fitness << endl;
			//cout << "Best->Fit = " << BestSol.back().Fitness << endl;
			if (s->Fitness > BestSol.back().Fitness)
			{
				BestSol.back().Copy(*s);
			}
		}
#if DEBUG
		std::ofstream AllScanSol;
		AllScanSol.open("..//OutPut//ScanSol.txt", ios::trunc);
		for (auto s = Sol.begin(); s != Sol.end(); s++)
		{
			for (auto r = s->NodeDof.begin(); r != s->NodeDof.end(); r++)
			{
				AllScanSol << *r << ",";
			}
			AllScanSol << s->SolProb << ",";
			AllScanSol << s->UNPM << ",";
			AllScanSol << s->ImpactValue << ",";
			AllScanSol << s->TotalCost << ",";
			AllScanSol << s->ImpactValue * s->SolProb << endl;
			//AllScanSol << endl;
		}
#endif


		return 1;
	}
	catch (exception& e)
	{
		TRACE("%s", e);

		return 0;
	}
}
void EnumerateNagureny(vector<CHROME> &Sol, const LINKPROMATRIX &Prob);

int ScanNagurenyNetwork(GRAPH &BaseGraph, vector<CHROME> &BestSol, const NODEPROMATRIX &Pmatrix,
	const LINKPROMATRIX &LinkMatrx)
{
	try
	{

		//Step 0: compute basic value 
		BaseGraph.EvaluteGraph();
		// step 1 enumerate all possibilities 
		int NumSol = 0;
		vector<CHROME> Sol;
		EnumerateNagureny(Sol, LinkMatrx);
		double BaseUNPM = BaseGraph.UNPM;

		CHROME tmc;
	/*	Sol.push_back(tmc);*/
		BestSol.push_back(tmc);
		BestSol.back().ImpactValue = 0.0;
		BestSol.back().Fitness = 0.0;

		int scount = 0;
		for (auto s = Sol.begin(); s != Sol.end(); s++)
		{
			//cout << scount << endl;
			//if (scount == 60)
			//{	
			//	cout << "Debug scount" << endl;
			//}
			if (scount ==13)
			{
				cout << "pause" << endl;
			}
			s->EvaluateSol(BaseGraph, BaseUNPM, Pmatrix,LinkMatrx);
			if (s->Fitness > BestSol.back().Fitness)
			{
				BestSol.back().Copy(*s);
			}
			scount++;
		}
		std::ofstream AllScanSol;
		AllScanSol.open("..//OutPut//ScanSol.txt", ios::trunc);
		for (auto s = Sol.begin(); s != Sol.end(); s++)
		{
			//for (auto r = s->NodeDof.begin(); r != s->NodeDof.end(); r++)
			for (auto r = s->LinkDof.begin(); r != s->LinkDof.end(); r++)
			{
				AllScanSol << *r << ",";
			}
			AllScanSol << s->SolProb << ",";
			AllScanSol << s->UNPM << ",";
			AllScanSol << s->ImpactValue << ",";
			AllScanSol << s->TotalCost << endl;
			//AllScanSol << endl;
		}

		return 1;
	}
	catch (exception& e)
	{
		TRACE("%s", e);

		return 0;
	}
}