#include "CommonHeaders.h"
#include <assert.h>
#include <algorithm>    // std::max
#include <math.h>       /* pow */
using namespace std;

double LinkClass::CleanLinkFlow(){
	return 0.0f;
}
double LinkClass::IniCost(){
	return this->T0;
}
double LinkClass::BPRCost(){
	//assert(this->CaRevise > zero);
	//return (this->T0*(1.0f + (this->AlphaBpr)*std::pow((this->Flow /std::max(this->CaRevise, zero)), this->BetaBBpr)));
	//return (this->T0*(1.0f + 0.15*std::pow((this->Flow /std::max(this->CaRevise, zero)), 4.0)));
	//if (this->Id == 0)
	//{
	//	cout << "linkid = 0" << ",";
	//	cout << "flow = " << Flow << ",";
	//	cout << "AlphaBpr = " << AlphaBpr << ",";
	//	cout << "BetaBBpr = " << BetaBBpr << ",";
	//	cout << "CaRevise = " << CaRevise << ",";
	//	cout << "Cost = " << (this->T0 * (1.0f + AlphaBpr * std::pow((this->Flow / std::max(this->CaRevise, zero)), BetaBBpr))) << endl;
	//}
	double BPRvalue = this->T0 * (1.0f + AlphaBpr * std::pow((this->Flow / std::max(this->CaRevise, zero)), BetaBBpr));
	if (this->CaRevise < 0.01)
	{
		return 100 + BPRvalue;
	}
	else
	{
		return BPRvalue;
	}
	//return (this->T0*(1.0f + AlphaBpr*std::pow((this->Flow /std::max(this->CaRevise, zero)),BetaBBpr)));
};

LinkClass::LinkClass(const LinkClass& otherLink){
	this->Id = otherLink.Id;
	this->Tail = otherLink.Tail;
	this->Head = otherLink.Head;
	this->Flow = otherLink.Flow;
	this->T0 = otherLink.T0;
	this->CaRevise = otherLink.CaRevise;
	this->CaInput = otherLink.CaInput;
	this->Cost = otherLink.Cost;
	this->RecoverTime = otherLink.RecoverTime;
	this->RequiredRes = otherLink.RequiredRes;
	this->EI = 0.0;
	this->AlphaBpr = 0.15;
	this->BetaBBpr = 4;
}


//LinkClass::~LinkClass(){
//	//do sth
//}

void LinkClass::IniCap(){
	this->CaRevise = this->CaInput;
}


