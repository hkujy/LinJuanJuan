// check 6Dec2022
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
	OutFile.open("..//OutPut//Summary.txt", ios::trunc);
	OutFile.close();
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
	OutFile.open("..//OutPut//ABCConverge.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//OperatorsMeasure.txt", ios::trunc);
	OutFile << "seed,id,good,bad,sum,gp,bp,prob,score,weight" << endl;
	OutFile.close();
}
void CloseFiles(){
	Log.close();
}
