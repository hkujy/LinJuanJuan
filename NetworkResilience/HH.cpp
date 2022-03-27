#include "CommonHeaders.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "ABC.h"
#include <assert.h>     /* assert */
#include <string>
using namespace std;

void Algorithm::HHGenNewNei()
{
	NumEmployedBee = 1;
	EmployBeePhase();
}
void Algorithm::HHGenIni()
{
	NumEmployedBee = 1;
	GenerateIniSol();
}


void Algorithm::HHMain()
{
	ofstream ConvergeFile;
	ConvergeFile.open("..//OutPut//HHConverge.txt", ios::app);

	vector<double> CpuTimes;
	clock_t St, Et;
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		// step 0: prepare
		St = clock();
		GenRan.seed((unsigned)SeedVecVal.at(s));
		ConvergeMeasure.clear();
		GlobalBest.Fitness = std::numeric_limits<double>::max();
		IniOperatorProb();
		// step 1: generate an initial solution 
		HHGenIni();
		for (int i = 0; i < MaxIter; i++)
		{
			if (SelectOp != SelectOperatorType::Uniform)
			{
				cout << "C++: Warning: in HH, the operator should not be uniform" << endl;
			}
#ifdef _DEBUG
			cout << "---------------------HH iter = " << i << "--------------" << endl;
#endif // _DEBUG
			EmployBeePhase();  // generate only one neighborhood solution
#ifdef _DEBUG
			cout << "---------------------HH iter" << i << " complete get prob" << endl;
#endif // _DEBUG
			ConvergeMeasure.push_back(GlobalBest.Fitness);
			if (isWriteConverge) ConvergeFile << s << "," << i << "," << fixed << setprecision(2) << ConvergeMeasure.back() << endl;
			PrintOperator(s, i);
		}
		Et = clock();
		CpuTimes.push_back((double)((Et - St) / CLOCKS_PER_SEC)); // time unit is second
		PrintFinal(s);
		printPattern(s);
	}
	ofstream CpuTimeFile;
	CpuTimeFile.open("..//OutPut//HHCpuTime.txt", ios::trunc);
	CpuTimeFile << "Seed,Time" << endl;
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		CpuTimeFile << s << "," << fixed << setprecision(2) << CpuTimes[s] << endl;
	}
	CpuTimeFile.close();

#ifdef _DEBUG
	cout << "*************************ABC completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
#endif // _DEBUG
}

