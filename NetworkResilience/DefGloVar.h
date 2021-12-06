// define global variables
#pragma once
#ifndef DGV
#define DGV
#include <random>
#include <fstream>
//define global
bool wtf;
double Zero;
int NumOrig;
int NumDest;
int NumLinks;
int NumNodes;
int NumOD;
double  OneDimEsp;
double  UEeps;
int	   UEmaxIter;
std::default_random_engine GenRan;
int MaxCsaIter;
int ModelIndex;
std::ofstream  AssertLog;
int CsaNumPop;
double CsaCloneBeta;
double CsaRepRatio;
int MaxGAIter;
int GANumPop;
int GANumChild;
bool isWriteConverge;
int MaxNumSolEval;
int StopCriteria;
int CSACloneMethod;
double GaMutationRate;
int SeedRunCount;
std::vector<int> SeedVecVal;
bool UseMyOwnAlgo;


#endif
