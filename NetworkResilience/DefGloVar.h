// define global variables
#pragma once
#ifndef DGV
#define DGV
#include <random>
#include <fstream>
//define global
double zero;
int numOrigin;
int numDest;
int numLinks;
int numNodes;
int numOD;
double  oneDimEps;
double  epsUE;
int	   maxIterUE;
std::default_random_engine GenRan;
int networkIndex;
int MaxGAIter;
bool isWriteConverge;
int MaxNumSolEval;
int StopCriteria;
std::vector<int> seedVecVal;
bool isUseMyOwnAlgo;
bool isTestSingleOperator;
int testSingleOperatorIndex;


//int GANumPop;
//int GANumChild;
//int CSACloneMethod;
//double GaMutationRate;
//int SeedRunCount;
//int CsaNumPop;
//int MaxCsaIter;
//double CsaCloneBeta;
//double CsaRepRatio;
#endif
