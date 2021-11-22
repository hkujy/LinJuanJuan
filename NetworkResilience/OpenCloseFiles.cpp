#include "CommonHeaders.h"
using namespace std;
void LogMsg(const char *format, ...);

std::ofstream Log;
void OpenAndCleanFiles(){
	Log.open("..//OutPut//Log.txt", ios::trunc | ios::app);
	if (Log.is_open()) printf("Log File Not Open\n");

	ofstream  OutFile;
	OutFile.open("..//OutPut//Summary.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//ModelPara.txt", ios::trunc);
	OutFile.close();
	//OutFile.open("..//OutPut//CSAconverge.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//GAconverge.txt", ios::trunc);
	//OutFile.close();
	OutFile.open("..//OutPut//Summary.txt", ios::trunc);
	OutFile.close();
	//OutFile.open("..//OutPut//BestSolLink.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//BestSolOD.txt", ios::trunc);
	//OutFile.close();
	OutFile.open("..//OutPut//PrintSols.txt", ios::trunc);
	OutFile << "Seed,Link,St,Et" << endl;
	OutFile.close();

	OutFile.open("..//OutPut//PrintPeriod.txt", ios::trunc);
	OutFile << "Seed,Period,Cost" << endl;
	OutFile.close();

	OutFile.open("..//OutPut//LinkData.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//DemandData.txt", ios::trunc);
	OutFile.close();
	//OutFile.open("..//OutPut//NodeProbMatrix.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//LinkProbMatrix.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//CSAConvergeBasedOnSol.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//GAConvergeBasedOnSol.txt", ios::trunc);
	//OutFile.close();
	OutFile.open("..//OutPut//ABCConverge.txt", ios::trunc);
	OutFile.close();
}
//void WriteFileCaseHead(){
//
//	ofstream  OutFile;
//
//	OutFile.open("..//OutPut//Summary.txt", ios::app);
//	OutFile << CaseSperation << endl;
//	OutFile.close();
//
//	OutFile.open("..//OutPut//Paras.txt", ios::app);
//	OutFile << CaseSperation << endl;
//	OutFile.close();
//
//}

void CloseFiles(){
	Log.close();
	AssertLog.close();
	//TestPara.close();
	//OutPutFlow.close();
	//SettingSuammry.close();
	//Converge.close();
	//ParadoxSumary.close();
}
