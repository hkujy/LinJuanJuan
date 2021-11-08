#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;

NODEPROMATRIX::NODEPROMATRIX(){
	//this->Matrix = Create2DAarray<double>(NumNodes + 1, MaxNumFairDegrees);
	this->Matrix = Create2DAarray<double>(MaxNumDof+1, NumNodes + 1);
}

NODEPROMATRIX::~NODEPROMATRIX(){
	//Free2DArrey<double>(this->Matrix, NumNodes + 1);
	Free2DArrey<double>(this->Matrix, MaxNumDof + 1);
};
int NODEPROMATRIX::PrintMatrix(ofstream &fout){
	for (unsigned int i = 0; i < this->Dof.size(); i++)
	{
		fout << this->Dof.at(i) << ",";
		for (unsigned int j = 0; j < (unsigned int)NumNodes; j++)
		{
			fout << this->Matrix[i][j] << ",";
		}
		fout << endl;
	}

	return 1;
}



LINKPROMATRIX::LINKPROMATRIX(){
	//this->Matrix = Create2DAarray<double>(NumNodes + 1, MaxNumFairDegrees);
	this->Matrix = Create2DAarray<double>(MaxNumDof + 1, NumLinks +1);
}
LINKPROMATRIX::~LINKPROMATRIX(){
	//Free2DArrey<double>(this->Matrix, NumNodes + 1);
	Free2DArrey<double>(this->Matrix, MaxNumDof + 1);
};
int LINKPROMATRIX::PrintMatrix(ofstream &fout){
	for (unsigned int i = 0; i < this->Dof.size(); i++)
	{
		fout << this->Dof.at(i) << ",";
		for (unsigned int j = 0; j < (unsigned int)NumLinks; j++)
		{
			fout << this->Matrix[i][j] << ",";
		}
		fout << endl;
	}
	return 1;
}


