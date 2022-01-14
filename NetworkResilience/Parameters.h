#include <string>
#ifndef PARA
#define PARA

const int InvaildInt=-1;
const double InvaildFloat = -999.9f;
const int NumOperators = 9;
const double LargeNum = 1.0e+20f;
const double RemoveLinkCost = 9999999999.0f;
const double InvalidMinCost = 9999999999.0f;
const int MaxNumOfSchPeriod = 50;
const int PatternLocalImproveScore = 1;
const int PatternGlobalImproveScore = 5;
//const int NumOfDisruptLinks = 3;
enum AlgorithmType
{
	CSA=0, GA=1, HH=2, ABC=3,UnDefined=4
};
std::string getAlgoTypeName(const AlgorithmType &alot);
void SetAlgoType(std::string _name, AlgorithmType& alot);
enum SelectOperatorType
{
	Uniform, ALNS, None
};

#endif // !PARA
