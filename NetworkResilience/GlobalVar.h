#pragma once
#ifndef GlobalVar
#define GlobalVar
#include <fstream>
#include <random>
//Constant
extern bool isTestSingleOperator;
extern int testSingleOperatorIndex;
extern double zero;
extern int numOrigin;
extern int numDest;
extern int numLinks;
extern int numNodes;
extern int numOD;
//extern std::ofstream TestStepSize/*;*/
extern std::ofstream Log;
extern std::ofstream AssertLog;
extern double oneDimEps;
extern double epsUE;
extern int maxIterUE;
extern std::default_random_engine GenRan;
extern int networkIndex;
extern std::vector<int> seedVecVal;
extern bool isUseMyOwnAlgo;
extern bool isWriteConverge;
// stop criteria =1: based on number of solution evaluated
// stop criteria =0: normal : based on number of iterations

//extern int MaxCsaIter;
//extern int CsaNumPop;
//extern double CsaCloneBeta;
//extern double CsaRepRatio;
//extern int MaxGAIter;
//extern int GANumPop;
//extern int GANumChild;
//extern int CSACloneMethod;
//extern double GaMutationRate;
//extern int SeedRunCount;
#endif