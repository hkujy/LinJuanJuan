#include <vector>
#include "RandomFuncs.h"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <numeric>
using namespace std;


int RouletteSelect(const vector<double>& cumProb)
{
	int selected = -1;
	double f = GenRandomReal();
	for (size_t i = 0; i < cumProb.size() - 1; i++)
	{
		if (f >= cumProb.at(i) && f < cumProb.at(i + 1))
		{
			selected = static_cast<int>(i);
			break;
		}
	}
	if (f >= cumProb.back()) selected = static_cast<int>(cumProb.size() - 1);
	if (selected < 0)
	{
		std::cout << "C++ Warning: RouletteSelect does not select a proper val" << endl;
		std::cout << f << endl;
		for (auto v : cumProb) std::cout << v << endl;
		std::cout << "wtf" << endl;
	}
	assert(selected >= 0);
	return selected;
}

vector<double> getCumProb(const vector<double>& prob) {

	assert(accumulate(prob.begin(), prob.end(), 0.0) > 0.99);
	vector<double> cumProb;

	cumProb.assign(prob.size(), 0.0);
	cumProb.at(0) = 0.0;
	for (size_t i = 0; i < cumProb.size() - 1; i++)
	{
		cumProb.at(i + 1) = cumProb.at(i) + prob[i];
	}
	return cumProb;
}

// given a candidate list and its prob 
// select one node based on prob
int SelectOneIndexFrom(const vector<int>& candy, const vector<double>& prob)
{
	assert(candy.size() == prob.size());
	vector<double> normProb(prob);
	double sum = accumulate(prob.begin(), prob.end(), 0.0);
	for (int i = 0; i < prob.size(); i++)
	{
		normProb[i] = prob[i] / sum;
	}
	//vector<double> cump = getCumProb(normProb);
	return (RouletteSelect(getCumProb(normProb)));

}