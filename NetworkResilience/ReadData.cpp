#include "CommonHeaders.h"
#include <string>
using namespace std;


//bool ReadLinkData(std::vector<LINK> &Links,
//	FILE *fin){
//	LINK tl;
//	int IDcount = 0;
//	int tail, head;
//	float t0, ca0;
//	float BprAlph, BprBeta;
//	if (nullptr == fin){
//		perror("Link data file open err for link data");
//		return false;
//	}
//	else
//	{
//
//
//		while (!feof(fin))
//		{
//			fscanf_s(fin, "%i %i %f %f %f %f",
//				&tail, &head, &t0, &ca0, &BprAlph, &BprBeta);
//			if (tail != EOF){
//				Links.push_back(tl);
//				Links.back().ID = IDcount;
//				Links.back().Tail = tail;
//				Links.back().Head = head;
//				Links.back().CaInput = (double)ca0;
//				Links.back().T0 = (double)t0;
//				Links.back().CaRevise = (double)ca0;
//				Links.back().AlphaBpr = (double)BprAlph;
//				Links.back().BetaBBpr = (double)BprBeta;
//				IDcount++;
//			}
//		}
//	}
//
//	/*if (ModelIndex == 3)
//	{
//		ifstream ReadFile;
//		ReadFile.open("C://Users//yujian//GitCode//Resi//InPut//SiouxFallsNetwork//Cap.txt", ios::in);
//		int num = 0;
//		string tmp;
//		while (!ReadFile.eof())
//		{
//			cout << num << endl;
//			getline(ReadFile, tmp);
//			Links[num].CaInput = std::stod(tmp);
//			Links[num].CaRevise = std::stod(tmp);
//			num++;
//		}
//	}*/
//	return true;
//}

bool ReadLinkData(std::vector<LINK> &Links,
	ifstream &fin) {
	LINK tl;
	int IDcount = 0;
	//int tail, head;
	//float t0, ca0;
	//float BprAlph, BprBeta;
	vector<string> fields;
	string line;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if ( std::stoi(fields[0])<0)
			continue;
	/*	fscanf_s(fin, "%i %i %f %f %f %f",
			&tail, &head, &t0, &ca0, &BprAlph, &BprBeta);*/
		Links.push_back(tl);
		Links.back().ID = IDcount;
		Links.back().Tail = std::stoi(fields[0]);
		Links.back().Head = std::stoi(fields[1]);
		Links.back().T0 = std::stof(fields[2]);
		Links.back().CaInput = std::stof(fields[3]);
		Links.back().CaRevise = std::stof(fields[3]);
		Links.back().AlphaBpr = std::stof(fields[4]);
		Links.back().BetaBBpr = std::stof(fields[5]);
		IDcount++;
	}
	return true;
}

//bool ReadDemandData(vector<OD> &ODPairs,
//	FILE *fin){
//
//	int IDcount = 0;
//	int or, de;
//	float dd;
//	OD tod;
//	if (nullptr == fin){
//		perror("Demand data file open err for demand data");
//		return false;
//	}
//	else
//	{
//		while (!feof(fin))
//		{
//			fscanf_s(fin, "%i  %i  %f",
//				&or, &de, &dd);
//			if (or != EOF){
//				ODPairs.push_back(tod);
//				ODPairs.back().ID = IDcount;
//				ODPairs.back().Orign = or;
//				ODPairs.back().Dest = de;
//				ODPairs.back().Demand =(double) dd;
//				IDcount++;
//			}
//		}
//	}
//
//	return true;
//}


bool ReadDemandData(vector<OD> &ODPairs,
	ifstream &fin) {

	int IDcount = 0;
	//int or , de;
	//float dd;
	vector<string> fields;
	string line;
	OD tod;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0])<0)
			continue;

		ODPairs.push_back(tod);
		ODPairs.back().ID = IDcount;
		ODPairs.back().Orign = std::stoi(fields[0]);
		ODPairs.back().Dest = std::stoi(fields[1]);
		ODPairs.back().Demand = std::stof(fields[2]);
		IDcount++;
	}

	return true;
}


bool PrintModelParas(){

	ofstream fout;
	fout.open("..//OutPut//ModelPara.txt");

	fout << "NumNodes" << "," << NumNodes << endl;
	fout << "NumOD" << "," << NumOD << endl;
	fout << "NumLinks" << "," << NumLinks << endl;
	fout << "StopCriteria" << "," << StopCriteria << endl;
	fout << "MaxNumSol" << "," << MaxNumSolEval << endl;

	fout << "OneDimEsp" << "," << OneDimEsp << endl;
	fout << "UEmaxIter" << "," << UEmaxIter << endl;
	fout << "CsaNumPo" << "," << CsaNumPop << endl;
	fout << "CsaCloneBeta" << "," << CsaCloneBeta << endl;
	fout << "CsaRepRatio" << "," << CsaRepRatio << endl;

	fout << "MaxCsaIter" << "," << MaxCsaIter << endl;
	fout << "GANumPop" << "," << GANumPop << endl;
	fout << "GANumChild" << "," << GANumChild << endl;
	fout << "MaxGAIter" << "," << MaxGAIter << endl;
	fout << "GaMutationRate" << "," << GaMutationRate << endl;
	fout.close();



	return true;
}
bool ReadModelParas(){

	ifstream fin, fcsa,fga;
	if (ModelIndex == 1)
	{
		fin.open("..//InPut//MediumNetwork//Para.txt");
		fcsa.open("..//InPut//MediumNetwork//CsaPara.txt");
		/*fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");*/
	}
	else if (ModelIndex == 2)
	{
		fin.open("..//InPut//Nagureny2009Network//Para.txt");
		fcsa.open("..//InPut//Nagureny2009Network//CsaPara.txt");
	}
	else if (ModelIndex == 3)
	{
		fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		fcsa.open("..//InPut//SiouxFallsNetwork//CsaPara.txt");
		fga.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (ModelIndex == 4)
	{
		fin.open("..//InPut//ParadoxNet//Para.txt");
		//fcsa.open("..//InPut//ParadoxNet//CsaPara.txt");
		//fga.open("..//InPut//ParadoxNet//GAPara.txt");
	}
	else
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
		fin.open("..//InPut//Para.txt");
		fcsa.open("..//InPut//CsaPara.txt");
	}

	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		//cout << fields[1] << endl;
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
		if (fields[0] == "MaxNumSol")	MaxNumSolEval = stoi(fields[1]);
		if (fields[0] == "StopCriteria")	StopCriteria = stoi(fields[1]);
	}
	cout << "OneDimEsp = " << OneDimEsp << endl;
	fin.close();

	while (getline(fcsa, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		if (fields[0] == "CsaNumPop")	CsaNumPop = stoi(fields[1]);
		if (fields[0] == "CsaCloneBeta")	CsaCloneBeta = stof(fields[1]);
		if (fields[0] == "CsaRepRatio")	CsaRepRatio = stof(fields[1]);
		if (fields[0] == "MaxCsaIter")	MaxCsaIter = stoi(fields[1]);

	}
	fcsa.close();

	while (getline(fga, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2)
			continue;
		if (fields[0] == "GANumPop")	GANumPop = stoi(fields[1]);
		if (fields[0] == "GANumChild")	GANumChild = stoi(fields[1]);
		if (fields[0] == "MaxGAIter")	MaxGAIter = stoi(fields[1]);
		if (fields[0] == "GaMutationRate")	GaMutationRate = stof(fields[1]);
	}
	return PrintModelParas();
}



void ReadNodeProb(NODEPROMATRIX &Pmatrix)
{
	ifstream fin;

	if (ModelIndex == 1) // Scan
	{
		fin.open("..//Input//MediumNetwork//NodeProbMatrix.txt", ios::in);
	}
	else if (ModelIndex==2)
	{
		fin.open("..//Input//Nagureny2009Network//NodeProbMatrix.txt", ios::in);
	}
	else
	{
		fin.open("..//Input//NodeProbMatrix.txt", ios::in);
	}

	// Read Demand data
	int NumLines = 0;
	std::string::size_type Sub1Begin, Sub1End;
	std::vector<std::string> SubStrs, ResidualStr;
	
	vector<string> fields;
	string line;
	while (getline(fin, line))
	{

		NumLines++;
		Sub1Begin = 0;
		SubStrs.clear();
		Sub1End = line.find(",");
		do
		{
			SubStrs.push_back(line.substr(Sub1Begin, Sub1End - Sub1Begin));
			Sub1Begin = Sub1End + 1;
			Sub1End = line.find(",", Sub1End + 1);
		} while (Sub1End != std::string::npos);
		SubStrs.push_back(line.substr(Sub1Begin, Sub1End - Sub1Begin));
		if (SubStrs.size() <= 2) break;
		assert((SubStrs).size() == NumNodes + 1);
		// split nodes
		Pmatrix.Dof.push_back(stof(SubStrs.at(0)));
		for (unsigned int i = 0; i < (unsigned int)NumNodes; i++)
		{
			Pmatrix.Matrix[Pmatrix.Dof.size() - 1][i] = stof(SubStrs.at(i + 1));
		}
	
	}

	//
	//if (fin.fail()) cerr << "Open Pmatrix fails" << endl;
	//else
	//{
	//	while (!fin.eof())
	//	{
	//		NumLines++;
	//		Sub1Begin = 0;
	//		SubStrs.clear();
	//		getline(fin, tmp);
	//		Sub1End = tmp.find(",");
	//		do
	//		{
	//			SubStrs.push_back(tmp.substr(Sub1Begin, Sub1End - Sub1Begin));
	//			Sub1Begin = Sub1End + 1;
	//			Sub1End = tmp.find(",", Sub1End + 1);
	//		} while (Sub1End != std::string::npos);
	//		SubStrs.push_back(tmp.substr(Sub1Begin, Sub1End - Sub1Begin));
	//		if (SubStrs.size() <= 2) break;
	//		assert((SubStrs).size() == NumNodes + 1);
	//		// split nodes
	//		Pmatrix.Dof.push_back(stof(SubStrs.at(0)));
	//		for (unsigned int i = 0; i < (unsigned int)NumNodes; i++)
	//		{
	//			Pmatrix.Matrix[Pmatrix.Dof.size() - 1][i] = stof(SubStrs.at(i + 1));
	//		}
	//	}
	//}

	//assert(Pmatrix.PrintMatrix(AssertLog));

	fin.close();

}


void ReadLinkProb(LINKPROMATRIX &LinkProbMatrix)
{
	ifstream fin;

	if (ModelIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//LinkProbMatrix.txt", ios::in);
	}
	else if (ModelIndex == 1)
	{
		fin.open("..//Input//MediumNetwork//LinkProbMatrix.txt", ios::in);
	}
	else
	{
		cerr << "wrong model index for read link prob matrix" << endl;
	}

	// Read Demand data
	int NumLines = 0;
	std::string::size_type Sub1Begin, Sub1End;
	std::vector<std::string> SubStrs, ResidualStr;
	vector<string> fields;
	string line;

	while (getline(fin, line))
	{
		NumLines++;
		Sub1Begin = 0;
		SubStrs.clear();
		Sub1End = line.find(",");
		do
		{
			SubStrs.push_back(line.substr(Sub1Begin, Sub1End - Sub1Begin));
			Sub1Begin = Sub1End + 1;
			Sub1End = line.find(",", Sub1End + 1);
		} while (Sub1End != std::string::npos);
		SubStrs.push_back(line.substr(Sub1Begin, Sub1End - Sub1Begin));
		if (SubStrs.size() <= 2) break;
		assert((SubStrs).size() == NumLinks + 1);
		// split nodes
		LinkProbMatrix.Dof.push_back(stof(SubStrs.at(0)));
		for (unsigned int i = 0; i < (unsigned int)NumLinks; i++)
		{
			LinkProbMatrix.Matrix[LinkProbMatrix.Dof.size() - 1][i] = stof(SubStrs.at(i + 1));
		}

	}

	//if (fin.fail()) cerr << "Open Pmatrix fails" << endl;
	//else
	//{
	//	while (!fin.eof())
	//	{
	//		NumLines++;
	//		Sub1Begin = 0;
	//		SubStrs.clear();
	//		getline(fin, tmp);
	//		Sub1End = tmp.find(",");
	//		do
	//		{
	//			SubStrs.push_back(tmp.substr(Sub1Begin, Sub1End - Sub1Begin));
	//			Sub1Begin = Sub1End + 1;
	//			Sub1End = tmp.find(",", Sub1End + 1);
	//		} while (Sub1End != std::string::npos);
	//		SubStrs.push_back(tmp.substr(Sub1Begin, Sub1End - Sub1Begin));
	//		if (SubStrs.size() <= 2) break;
	//		assert((SubStrs).size() == NumLinks + 1);
	//		// split nodes
	//		LinkProbMatrix.Dof.push_back(stof(SubStrs.at(0)));
	//		for (unsigned int i = 0; i < (unsigned int)NumLinks; i++)
	//		{
	//			LinkProbMatrix.Matrix[LinkProbMatrix.Dof.size() - 1][i] = stof(SubStrs.at(i + 1));
	//		}
	//	}
	//}

	//assert(LinkProbMatrix.PrintMatrix(AssertLog));

	fin.close();

}






void ReadGraphData(GRAPH &Graph)
{
	
	//FILE * fin;
	std::ifstream fin;
	// Read Demand data
	if (ModelIndex == 1) // Scan
	{
		//fopen_s(&fin, "..//Input//MediumNetwork//DeamdData.txt", "r");
		fin.open("..//Input//MediumNetwork//DeamdData.txt");
		//fopen_s(&fin, "..//Input//MediumNetwork//DeamdData.txt", "r");
	}
	else if (ModelIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else if (ModelIndex == 3)
	{
		fin.open("..//Input//Nagureny2009Network//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else if (ModelIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//DeamdData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//DeamdData.txt", "r");
	}
	else
	{
		cout << "Mode Index is not specified" << endl;
		system("PAUSE");
		fin.open("..//Input//DeamdData.txt");
		//fopen_s(&fin, "..//Input//DeamdData.txt", "r");
	}

	if (!ReadDemandData(Graph.OdPairs, fin))
	{
		cerr << "read demand data fails" << endl;
		system("PAUSE");
	}

	fin.close();
	
	// Read link data
	if (ModelIndex == 1) // Scan
	{
		fin.open("..//Input//MediumNetwork//LinkData.txt");
		//fopen_s(&fin, "..//Input//MediumNetwork//LinkData.txt", "r");
	}
	else if (ModelIndex == 2)
	{
		fin.open("..//Input//Nagureny2009Network//LinkData.txt");
		//fin.open("C://GitCodes//NRI//InPut//Nagureny2009Network//LinkData.txt");
			//LinkData.txt");
		//fopen_s(&fin, "..//Input//Nagureny2009Network//LinkData.txt", "r");
	}
	else if (ModelIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else if (ModelIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else
	{
		fin.open("..//Input//LinkData.txt");
		//fopen_s(&fin, "..//Input//LinkData.txt", "r");
	}

	if (!fin.is_open())
		cout << "link data file is not open" << endl;


	if (!ReadLinkData(Graph.Links, fin))
	{
		cerr << "read link data fails" << endl;
		system("PAUSE");
	}
	fin.close();
	Graph.CreateOriginSet();
	Graph.CreateNodes();
}

void ReadLinkProbMatrix(LINKPROMATRIX &LinkProbMatrix);
void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &Pmatrix){

	ReadNodeProb(Pmatrix);
	ReadGraphData(BaseGraph);
	//assert(BaseGraph.PrintLinks(AssertLog));
	//assert(BaseGraph.PrintOD(AssertLog));
}

void ReadDataMain(GRAPH &BaseGraph, NODEPROMATRIX &NodePmatrix, LINKPROMATRIX &LinkPmatrix){

	ofstream OutPutFile;
	ReadGraphData(BaseGraph);
	if (ModelIndex == 3)
	{
		// for Sioux fall network specify data
		// the link failure prob is zero 
		// and only consider the failure of the nodse
		LinkPmatrix.Dof.push_back(0.0);
		for (int i = 0; i < NumLinks;i++)
		{
			LinkPmatrix.Matrix[0][i] = 1.0f;  
		}

		// degree of failure for nodes
		NodePmatrix.Dof.push_back(0.0f); 
		NodePmatrix.Dof.push_back(0.3f);
		NodePmatrix.Dof.push_back(0.6f);
		NodePmatrix.Dof.push_back(0.9f);
		/////////////////
		for (int i = 0; i < NumNodes;i++)
		{
			NodePmatrix.Matrix[0][i] = 0.60f;
			NodePmatrix.Matrix[1][i] = 0.20f;
			NodePmatrix.Matrix[2][i] = 0.15f;
			NodePmatrix.Matrix[3][i] = 0.05f;
		}
		OutPutFile.open("..//OutPut//NodeProbMatrix.txt");
		NodePmatrix.PrintMatrix(OutPutFile);
		OutPutFile.close();
		OutPutFile.open("..//OutPut//LinkProbMatrix.txt");
		LinkPmatrix.PrintMatrix(OutPutFile);
		OutPutFile.close();

	}
	else
	{
		ReadNodeProb(NodePmatrix);
		ReadLinkProb(LinkPmatrix);
		OutPutFile.open("..//OutPut//NodeProbMatrix.txt");
		NodePmatrix.PrintMatrix(OutPutFile);
		OutPutFile.close();
		OutPutFile.open("..//OutPut//LinkProbMatrix.txt");
		LinkPmatrix.PrintMatrix(OutPutFile);
		OutPutFile.close();
	}
	OutPutFile.open("..//OutPut//LinkData.txt");
	BaseGraph.PrintLinks(OutPutFile);
	OutPutFile.close();
	OutPutFile.open("..//OutPut//DemandData.txt");
	BaseGraph.PrintOD(OutPutFile);
	OutPutFile.close();
}


void ReadDataMain(GRAPH& BaseGraph) {

	ofstream OutPutFile;
	ReadGraphData(BaseGraph);
	OutPutFile.open("..//OutPut//LinkData.txt");
	BaseGraph.PrintLinks(OutPutFile);
	OutPutFile.close();
	OutPutFile.open("..//OutPut//DemandData.txt");
	BaseGraph.PrintOD(OutPutFile);
	OutPutFile.close();
}



