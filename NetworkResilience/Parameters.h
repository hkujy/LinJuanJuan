#include <string>
#ifndef PARA
#define PARA

//TODO code python and read globle parameters

const int InvaildInt=-1;
const double InvaildFloat = -999.9f;
const int NumOperators = 9;
const double LargeNum = 1.0e+20f;
const double RemoveLinkCost = 9999999999.0f;
const double InvalidMinCost = 9999999999.0f;
const int MaxNumOfSchPeriod = 50;
//const int PatternLocalImproveScore = 1;
//const int PatternGlobalImproveScore = 5;
// enumerate algorithm type
enum class AlgorithmType { CSA=0, GA=1, HH=2, ABC=3,UnDefined=4 };
// enumerate select operator method
enum class SelectOperatorType { Uniform, ALNS, None };
// enumerate the relationships
enum class LinkSchRelations { Before, After, Same, noDominated, None };
LinkSchRelations getReversRelation(LinkSchRelations &r); // get the reverse of a relation

// enumerate how to compare score method
enum class enum_CompareScoreMethod {Ave, Total, None};
std::string getAlgoTypeName(const AlgorithmType &alot);
void SetAlgoType(std::string _name, AlgorithmType& alot);

#endif // !PARA
