#include <string>
#ifndef PARAMETERS
#define PARAMETERS

constexpr int  INVALID_INT=-1;
constexpr double INVALID_FLOAT = -999.9;
constexpr int NUM_OPERATORS = 9;
constexpr double LARGE_NUM = 1.0e+20;
constexpr double REMOVE_LINK_COST = 9999999999.0;
constexpr double INVALID_MIN_COST = 9999999999.0;
constexpr int MAX_NUM_OF_SCH_PERIOD = 50;
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
std::string getAlgorithmTypeName(const AlgorithmType &algorithmType);
//void SetAlgorithmType(std::string name, AlgorithmType& algorithmType);

#endif // !PARAMETERS
