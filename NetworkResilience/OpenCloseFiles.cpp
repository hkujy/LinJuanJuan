// check 6Dec2022
#include "CommonHeaders.h"
using namespace std;
void LogMsg(const char *format, ...);
std::ofstream Log;
void OpenAndCleanFiles(){
	Log.open("..//OutPut//Log.txt", ios::trunc | ios::app);
	if (Log.is_open()) printf("Log File Not Open\n");
	ofstream  OutFile;
	OutFile.open("..//OutPut//ABCSummary.txt", ios::trunc);
	OutFile.close();
	//OutFile.open("..//OutPut//HHSummary.txt", ios::trunc);
	//OutFile.close();
	//OutFile.open("..//OutPut//ModelPara.txt", ios::trunc);
	//OutFile.close();

	OutFile.open("..//OutPut//ABCPrintSols.txt", ios::trunc);
	OutFile << "Seed,Link,St,Et" << endl;
	OutFile.close();

	OutFile.open("..//OutPut//ABCPrintSeedBestSolVal.txt", ios::trunc);
	OutFile << "Seed,BestVal" << endl;
	OutFile.close();

	//OutFile.open("..//OutPut//HHPrintSols.txt", ios::trunc);
	//OutFile << "Seed,Link,St,Et" << endl;
	//OutFile.close();

	//OutFile.open("..//OutPut//PrintPeriod.txt", ios::trunc);
	//OutFile << "Seed,Period,Cost,UNPM" << endl;
	//OutFile.close();

	OutFile.open("..//OutPut//ABCPrintPeriod.txt", ios::trunc);
	OutFile << "Seed,Period,Cost,UNPM" << endl;
	OutFile.close();

	//OutFile.open("..//OutPut//HHPrintPeriod.txt", ios::trunc);
	//OutFile << "Seed,Period,Cost,UNPM" << endl;
	//OutFile.close();

	OutFile.open("..//OutPut//LinkData.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//DemandData.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//ABCConverge.txt", ios::trunc);
	OutFile.close();
	//OutFile.open("..//OutPut//HHConverge.txt", ios::trunc);
	//OutFile.close();

	//OutFile.open("..//OutPut//HHOperatorsMeasure.txt", ios::trunc);
	//OutFile << "Seed,Iter,Id,Good,Bad,Sum,Gp,Bp,Prob,Score,Weight" << endl;
	//OutFile.close();

	OutFile.open("..//OutPut//ABCOperatorsMeasure.txt", ios::trunc);
	OutFile << "Seed,Iter,Id,Good,Bad,Sum,Gp,Bp,Prob,Score,Weight" << endl;
	OutFile.close();

	OutFile.open("..//OutPut/LinkEI.txt", ios::trunc);
	OutFile << "LinkId,EI" << endl;
	OutFile.close();

	OutFile.open("..//OutPut//ABCPrintPatternScore.txt", ios::trunc);
	OutFile << "Seed,First,Second,Score,Prob" << endl;
	OutFile.close();


	OutFile.open("..//OutPut//DomRelation.txt", ios::trunc);
	OutFile << "Seed,First,Second,AferScore,BeforeScore,SameScore,Dom" << endl;
	OutFile.close();

	//OutFile.open("..//OutPut//HHPrintPatternScore.txt", ios::trunc);
	//OutFile << "Seed,First,Second,Score,Prob" << endl;
	//OutFile.close();

}
void CloseFiles(){
	Log.close();
}
