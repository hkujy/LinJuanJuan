// record learning related functions
#include "CommonHeaders.h"
#include "ABC.h"

using namespace std;


/// <summary>
/// update the score of an operator
/// </summary>
/// <param name="OpId">Id of the operator</param>
/// <param name="ImprovedFit"> if negative, then it is an improvement</param>
void Algorithm::UpdateOperatorScore_ALNS(int OpId, double ResultFit, double LocalFit, double GlobalFit)
{
	//TODO: write function to read reward 
	//      change the prob for selecting nei
	//      write python to read parameter 
	//      and select which nei select method is used 
	assert(OpId >= 0); assert(OpId <= NumOperators - 1);
	if (ResultFit < GlobalFit)
	{
		Operators.at(OpId).Score += RewardImproveGlobal;
		return;
	}
	if (ResultFit < LocalFit)
	{
		Operators.at(OpId).Score += RewardImproveLocal;
	}
	else
	{
		Operators.at(OpId).Score += RewardWorse;
	}
}

/// <summary>
/// update the weight of all operators
/// </summary>
void Algorithm::UpdateOperatorWeight_ALNS()
{
	for (int i = 0; i < Operators.size(); i++)
	{
		Operators.at(i).calWeight(ReactionFactor);
	}
}

/// <summary>
/// cal the weight 
/// </summary>
void OperatorClass::calWeight(double r)
{
	if (TotalCounterSum > 0)
	{
		Weight = (1 - r) * Weight + r * Score / TotalCounterSum;
	}
}


// update operator measures
void Algorithm::UpdateOperatorMeaures(int _id, bool isImproved)
{
	//TODO: write the update the counters of the operators
	assert(_id <= NumOperators - 1);
	assert(_id >= 0);
	if (isImproved)
	{
		this->Operators.at(_id).TotalCounterGood++;
	}
	else
	{
		this->Operators.at(_id).TotalCounterBad++;
	}
	this->Operators.at(_id).TotalCounterSum++;
}

