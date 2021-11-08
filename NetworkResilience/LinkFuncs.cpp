#include "CommonHeaders.h"
#include <assert.h>
#include <algorithm>    // std::max
#include <math.h>       /* pow */
using namespace std;

double LINK::CleanLinkFlow(){
	return 0.0f;
}
double LINK::IniCost(){
	return this->T0;
}
double LINK::BPRCost(){
	//assert(this->CaRevise > Zero);
	//return (this->T0*(1.0f + (this->AlphaBpr)*std::pow((this->Flow /std::max(this->CaRevise, Zero)), this->BetaBBpr)));
	return (this->T0*(1.0f + 0.15*std::pow((this->Flow /std::max(this->CaRevise, Zero)), 4.0)));
};

LINK::LINK(const LINK& OtherLink){
	this->ID = OtherLink.ID;
	this->Tail = OtherLink.Tail;
	this->Head = OtherLink.Head;
	this->Flow = OtherLink.Flow;
	this->T0 = OtherLink.T0;
	this->CaRevise = OtherLink.CaRevise;
	this->CaInput = OtherLink.CaInput;
	this->Cost = OtherLink.Cost;
}

LINK::LINK(){
	ID = Tail = Head = InvaildInt;
	Flow = InvaildFloat;
	Cost = InvaildFloat;
	CaRevise = InvaildFloat;
	CaInput = InvaildFloat;
	T0 = InvaildFloat;
}
LINK::~LINK(){
	//do sth
}

void LINK::IniCap(){
	this->CaRevise = this->CaInput;
}


