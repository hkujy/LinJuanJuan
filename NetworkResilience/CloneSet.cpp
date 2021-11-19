#include "CommonHeaders.h"
#include "RandomFuncs.h"
using namespace std;


//CSACloneMethod
// methon 0:  base method
// method 1: generate based on the fitness value

void GenerateCloneSet(const vector<CHROME> Chroms, const int NumPop,const int NumClone,
					vector<int> &CloneSet)
{
	vector<double> CloneProb(NumPop + 1, 0.0);
	double FitSum=0.0;
	switch (CSACloneMethod)
	{
	case 0:
		CloneSet.clear();
		for (int i = 0; i < NumPop / 2;i++)
		{
			for (int j = 0; j < NumClone/(NumPop/2);j++)
			{
				cout << NumClone / (NumPop / 2) << endl;
				CloneSet.push_back(i);
			}
		}
		break;
	case 1:
		for (int i = 0; i < NumPop; i++)
		{
			FitSum += Chroms.at(i).Fitness;
		}
		CloneProb.at(0) = 0.0f;
		for (int i = 0; i < NumPop; i++)
		{
			CloneProb.at(i + 1) = CloneProb.at(i) + Chroms.at(i).Fitness / FitSum;// cumulative prob
		}
		CloneSet.assign(NumClone, -1);
		for (int i = 0; i < NumClone; i++)
		{
			double f = GenRandomReal();
			for (size_t j = 0; j < CloneProb.size() - 1; j++)
			{
				if (f >= CloneProb.at(j) && f < CloneProb.at(j + 1))
				{
					CloneSet.at(i) = static_cast<int>(j);
					break;
				}
			}
		}

		break;
	default:
		cerr << "Clone Method is not defined" << endl;
		break;
	}


}