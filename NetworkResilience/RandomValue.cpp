// uniform_int_distribution
#include "CommonHeaders.h"
#include <random>
#include <vector>
int GenRandomInt(const std::vector<int> &Vec)
{
	std::uniform_int_distribution<unsigned int> const distribution(0,(unsigned int) (Vec.size()-1));
	unsigned int const num = distribution(GenRan);
	return Vec.at(num);
}
int GenRandomInt(int MinVal, int MaxVal)
{
	std::uniform_int_distribution<unsigned int> const distribution(MinVal,MaxVal);
	return static_cast<int>(distribution(GenRan));
}

double GenRandomFloat(const std::vector<double> &Vec)
{
	std::uniform_int_distribution<unsigned int> const distribution(0, (unsigned int)(Vec.size()-1));
	unsigned int const num = distribution(GenRan);
	return Vec.at(num);
}

int GenRandomPos(unsigned int size)
{
	std::uniform_int_distribution<unsigned int> const distribution(0, size-1);
	return static_cast<int>(distribution(GenRan));
}
double GenRandomReal()
{
	std::uniform_real_distribution<double> const distribution(0.0,1.0);
	return distribution(GenRan);
}
