// record learning related functions
#include "CommonHeaders.h"
#include "ABC.h"

using namespace std;


/// <summary>
/// update the score of an operator
/// </summary>
/// <param name="opId">Id of the operator</param>
/// <param name="ImprovedFit"> if negative, then it is an improvement</param>
void Algorithm::UpdateOperatorScore_ALNS(int opId, double resultFit, double localFit, double globalFit)
{
	//TODO: write function to read reward 
	//      change the prob for selecting nei
	//      write python to read parameter 
	//      and select which nei select method is used 
	assert(opId >= 0); assert(opId <= NUM_OPERATORS - 1);
	if (resultFit < globalFit)
	{
		Operators.at(opId).Score += RewardImproveGlobal;
		return;
	}
	if (resultFit < localFit)
	{
		Operators.at(opId).Score += RewardImproveLocal;
	}
	else
	{
		Operators.at(opId).Score += RewardWorse;
	}
}

/// <summary>
/// update the weight of all operators
/// </summary>
void Algorithm::UpdateOperatorWeight_ALNS()
{
	//for (int i = 0; i < Operators.size(); i++)
	//{
	//	Operators.at(i).CalWeight(ReactionFactor);
	//}
	for (auto &o:Operators)
	{
		o.CalWeight(ReactionFactor);
	}
}


// update operator measures
void Algorithm::UpdateOperatorMeasure(int id, bool isImproved)
{
	//TODO: write the update the counters of the operators
	assert(id <= NUM_OPERATORS - 1);
	assert(id >= 0);
	if (isImproved)
	{
		this->Operators.at(id).TotalCounterGood++;
	}
	else
	{
		this->Operators.at(id).TotalCounterBad++;
	}
	this->Operators.at(id).TotalCounterSum++;
}

