#include "CommonHeaders.h"
#include <string>
using namespace std;

bool ReadModelParas(){
	ifstream fin,fabc;
	if (networkIndex == 1)
	{
		fin.open("..//InPut//MediumNetwork//Para.txt");
	}
	else if (networkIndex == 2)
	{
		fin.open("..//InPut//Nagureny2009Network//Para.txt");
	}
	else if (networkIndex == 3)
	{
		fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		fabc.open("..//InPut//SiouxFallsNetwork//ABCPara.txt");
	}
	else if (networkIndex == 4)
	{
		fin.open("..//InPut//ParadoxNet//Para.txt");
		fabc.open("..//InPut//ParadoxNet//ABCPara.txt");
	}
	else
	{
		cout << "C++ Warning: Model Index is not specified" << endl;
		system("Pause");
	}

	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "oneDimEsp")	oneDimEsp = stof(fields[1]);
		if (fields[0] == "epsUE")	epsUE = stof(fields[1]);
		if (fields[0] == "maxIterUE")	maxIterUE = stoi(fields[1]);
		if (fields[0] == "numNodes")	numNodes = stoi(fields[1]);
		if (fields[0] == "numOD")	numOD = stoi(fields[1]);
		if (fields[0] == "numLinks")	numLinks = stoi(fields[1]);
	}
	cout << "oneDimEsp = " << oneDimEsp << endl;
	fin.close();


	return PrintModelParas();
}





#pragma region NotUsedInRestor
//bool PrintModelParas(){
//
//	ofstream fout;
//	fout.open("..//OutPut//ModelPara.txt");
//
//	fout << "numNodes" << "," << numNodes << endl;
//	fout << "numOD" << "," << numOD << endl;
//	fout << "numLinks" << "," << numLinks << endl;
//	//fout << "StopCriteria" << "," << StopCriteria << endl;
//	//fout << "MaxNumSol" << "," << MaxNumSolEval << endl;
//
//	fout << "oneDimEsp" << "," << oneDimEsp << endl;
//	fout << "maxIterUE" << "," << maxIterUE << endl;
//	fout << "CsaNumPo" << "," << CsaNumPop << endl;
//	fout << "CsaCloneBeta" << "," << CsaCloneBeta << endl;
//	fout << "CsaRepRatio" << "," << CsaRepRatio << endl;
//
//	fout << "MaxCsaIter" << "," << MaxCsaIter << endl;
//	fout << "GANumPop" << "," << GANumPop << endl;
//	fout << "GANumChild" << "," << GANumChild << endl;
//	fout << "MaxGAIter" << "," << MaxGAIter << endl;
//	fout << "GaMutationRate" << "," << GaMutationRate << endl;
//	fout.close();
//	return true;
//}

#pragma endregion

