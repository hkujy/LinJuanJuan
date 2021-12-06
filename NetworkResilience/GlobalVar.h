#pragma once
#ifndef GLOBALVAR
#define GLOBALVAR
#include <fstream>
#include <random>
//Constant
extern bool wtf;
extern double Zero;
extern int NumOrig;
extern int NumDest;
extern int NumLinks;
extern int NumNodes;
extern int NumOD;
//extern std::ofstream TestStepSize/*;*/
extern std::ofstream Log;
extern double OneDimEsp;
extern double UEeps;
extern int UEmaxIter;
extern std::default_random_engine GenRan;
extern std::ofstream  AssertLog;
extern int MaxCsaIter;
extern int ModelIndex;
extern int CsaNumPop;
extern double CsaCloneBeta;
extern double CsaRepRatio;
extern int MaxGAIter;
extern int GANumPop;
extern int GANumChild;
//extern int MaxNumSolEval;
extern bool isWriteConverge;
//extern int StopCriteria;  
extern int CSACloneMethod;
extern double GaMutationRate;
extern int SeedRunCount;
extern std::vector<int> SeedVecVal;
extern bool UseMyOwnAlgo;
// stop criteria =1: based on number of solution evaluated
// stop criteria =0: normal : based on number of iterations
#endif