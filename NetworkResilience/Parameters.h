#ifndef PARA
#define PARA

const int InvaildInt=-1;
const double InvaildFloat = -999.9f;
const int NumOperators = 8;
const double LargeNum = 1.0e+20f;
const double RemoveLinkCost = 9999999999.0f;
const double InvalidMinCost = 9999999999.0f;
const int MaxNumOfSchPeriod = 50;
const int PatternLocalImproveScore = 1;
const int PatternGlobalImproveScore = 5;
//const int NumOfDisruptLinks = 3;
enum AlgorithmType
{
	CSA, GA
};
enum SelectOperatorType
{
	Uniform, ALNS, None
};
#endif // !PARA
