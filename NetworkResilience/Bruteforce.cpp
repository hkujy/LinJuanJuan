#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;

// input the p matrix and brute force all the impossibility 

void Enumerate_Samll_1(vector<CHROME> &Sol, const NODEPROMATRIX &Prob)
{
	unsigned int n1 = 1, n2 = 2, n3 = 3;
	CHROME tmc;
	tmc.Nodes.push_back(1);
	tmc.Nodes.push_back(2);
	tmc.Nodes.push_back(3);
	for (unsigned int d1 = 0; d1 < Prob.Dof.size(); d1++)
	{
		for (unsigned int d2 = 0; d2 < Prob.Dof.size(); d2++)
		{
			for (unsigned int d3 = 0; d3 < Prob.Dof.size(); d3++)
			{
				Sol.push_back(tmc);
				Sol.back().NodeDof.push_back(Prob.Dof.at(d1));
				Sol.back().NodeDof.push_back(Prob.Dof.at(d2));
				Sol.back().NodeDof.push_back(Prob.Dof.at(d3));

				Sol.back().NodeDofProb.push_back(Prob.Matrix[d1][n1]);
				Sol.back().NodeDofProb.push_back(Prob.Matrix[d2][n2]);
				Sol.back().NodeDofProb.push_back(Prob.Matrix[d3][n3]);

			}
		}
	}
}

void BruteForceFor3NodeSmall(NODEPROMATRIX &NodeProbMatrix,
	GRAPH &Graph, LINKPROMATRIX &LinkMatrix)
{
	// step 1 enumerate all possibilities 
	int NumSol = 0;
	vector<CHROME> Sol;
	Enumerate_Samll_1(Sol, NodeProbMatrix);

	for (unsigned int i = 0; i < Sol.size(); i++)
	{
		for (unsigned int j = 0; j < Sol.at(i).Nodes.size(); j++) //each node associated with one degree
		{
			AssertLog << Sol.at(i).NodeDof.at(j) << ",";
		}
		AssertLog << endl;
	}
	// step 2 compute base scenario 
	Graph.EvaluteGraph();
	double BaseUNPM = Graph.UNPM;
	for (auto s = Sol.begin(); s != Sol.end(); s++)
	{
		s->EvaluateSol(Graph, BaseUNPM, NodeProbMatrix, LinkMatrix);
	}
	for (auto s = Sol.begin(); s != Sol.end(); s++)
	{
		assert(s->PrintSol(AssertLog));
	}

}


void EnumerateMedium(vector<CHROME> &Sol, const NODEPROMATRIX &Prob)
{
	unsigned int n[5];
	for (unsigned int i = 0; i < 5; i++)
	{
		n[i] = i;
	}
	CHROME tmc;




	for (int i = 0; i < 5; i++)
	{
		tmc.Nodes.push_back(i);
	}
	int NumSol = 0;
	for (unsigned int d0 = 0; d0 < Prob.Dof.size(); d0++)
	{
		for (unsigned int d1 = 0; d1 < Prob.Dof.size(); d1++)
		{
			for (unsigned int d2 = 0; d2 < Prob.Dof.size(); d2++)
			{
				for (unsigned int d3 = 0; d3 < Prob.Dof.size(); d3++)
				{
					for (unsigned int d4 = 0; d4 < Prob.Dof.size(); d4++)
					{
						

									//if (NumSol > 10) continue;
									Sol.push_back(tmc);
									Sol.back().NodeDof.push_back(Prob.Dof.at(d0));
									Sol.back().NodeDof.push_back(Prob.Dof.at(d1));
									Sol.back().NodeDof.push_back(Prob.Dof.at(d2));
									Sol.back().NodeDof.push_back(Prob.Dof.at(d3));
									Sol.back().NodeDof.push_back(Prob.Dof.at(d4));
									



									Sol.back().NodeDofProb.push_back(Prob.Matrix[d0][n[0]]);
									Sol.back().NodeDofProb.push_back(Prob.Matrix[d1][n[1]]);
									Sol.back().NodeDofProb.push_back(Prob.Matrix[d2][n[2]]);
									Sol.back().NodeDofProb.push_back(Prob.Matrix[d3][n[3]]);
									Sol.back().NodeDofProb.push_back(Prob.Matrix[d4][n[4]]);
									
									NumSol++;
							
						
					
					}
				}
			}
		}
	}
}


void EnumerateNagureny(vector<CHROME> &Sol, const LINKPROMATRIX &Prob)
{

	unsigned int n[5];   // number of links
	for (unsigned int i = 0; i < 5; i++)
	{
		n[i] = i;
	}
	CHROME tmc;




	for (int i = 0; i < 5; i++)
	{
		tmc.Links.push_back(i);
	}
	int NumSol = 0;
	for (unsigned int d0 = 0; d0 < Prob.Dof.size(); d0++)
	{
		for (unsigned int d1 = 0; d1 < Prob.Dof.size(); d1++)
		{
			for (unsigned int d2 = 0; d2 < Prob.Dof.size(); d2++)
			{
				for (unsigned int d3 = 0; d3 < Prob.Dof.size(); d3++)
				{
					for (unsigned int d4 = 0; d4 < Prob.Dof.size(); d4++)
					{

						bool isRemove = false;
						//if (NumSol > 10) continue;
						Sol.push_back(tmc);
						Sol.back().LinkDof.push_back(Prob.Dof.at(d0));
						Sol.back().LinkDof.push_back(Prob.Dof.at(d1));
						Sol.back().LinkDof.push_back(Prob.Dof.at(d2));
						Sol.back().LinkDof.push_back(Prob.Dof.at(d3));
						Sol.back().LinkDof.push_back(Prob.Dof.at(d4));



						Sol.back().LinkDofProb.push_back(Prob.Matrix[d0][n[0]]);
						Sol.back().LinkDofProb.push_back(Prob.Matrix[d1][n[1]]);
						Sol.back().LinkDofProb.push_back(Prob.Matrix[d2][n[2]]);
						Sol.back().LinkDofProb.push_back(Prob.Matrix[d3][n[3]]);
						Sol.back().LinkDofProb.push_back(Prob.Matrix[d4][n[4]]);
						NumSol++;


						// exclude the disconnectivities cases

						//if (Sol.back().LinkDof.at(3) > 0.9)
						//{
						//	if (Sol.back().LinkDof.at(0) > 0.99 ||
						//		Sol.back().LinkDof.at(1) > 0.99)
						//	{
						//		isRemove = true;
						//	}
						//}
						//if (Sol.back().LinkDof.at(4)> 0.9)
						//{
						//	if (Sol.back().LinkDof.at(0) > 0.9 ||
						//		Sol.back().LinkDof.at(2) > 0.9)
						//	{
						//		isRemove = true;
						//	}
						//}
						//if (isRemove) 
						//{
						//	Sol.pop_back();
						//	NumSol--;
						//}

					}
				}
			}
		}
	}


}