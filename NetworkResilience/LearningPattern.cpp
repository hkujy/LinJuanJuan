// record learning related functions
#include "CommonHeaders.h"
#include "ABC.h"

using namespace std;
size_t findPatternIndex_fun(int lid, const vector<PatternClass>& Pat);

int FindValIndex(const vector<int>& vec, int key);
// compare three number and return the largest number
// 1: A is the largest, 2: B is the largest, 3: C is the largest
// 0: all three are equal
// Notes: when use this, follow the sequence
// A--After, B--Before,  C--Same
int CompareThreeNumber(double A, double B, double C)
{
	// case 0: if all three numbers are equal
	if (A == B && A == C) return 0;
	// other three cases
	if (A >= B && A >= C) return 1;
	if (B >= A && B >= C) return 2;
	if (C >= A && C >= B) return 3;
	TRACE("Debug Compare Three Number");
	return -1;
}


LinkSchRelations ScheduleClass::findDominantRelation(int aLink, int bLink, const vector<PatternClass> &pat,
	enum_CompareScoreMethod &CompareScoreMethod)
{
	size_t Aid = findPatternIndex_fun(aLink, pat);
	size_t Bid = pat[Aid].findRelationId(bLink);
	int flag = -1;
	switch (CompareScoreMethod)
	{
	case enum_CompareScoreMethod::Ave:
		flag = CompareThreeNumber(pat[Aid].Relation[Bid].AfterScore_Ave,
			pat[Aid].Relation[Bid].BeforeScore_Ave,
			pat[Aid].Relation[Bid].SamePeriodScore_Ave);
		// if the three have the same value, then consider it is no dominated solution status
		if (0 == flag) return LinkSchRelations::noDominated;
		if (1 == flag) return LinkSchRelations::After;
		if (2 == flag) return LinkSchRelations::Before;
		if (3 == flag) return LinkSchRelations::Same;
		break;
	case enum_CompareScoreMethod::Total:
		flag = CompareThreeNumber(
			pat[Aid].Relation[Bid].AfterScore_Total,
			pat[Aid].Relation[Bid].BeforeScore_Total,
			pat[Aid].Relation[Bid].SamePeriodScore_Total);
		if (0 == flag) return LinkSchRelations::noDominated;
		if (1 == flag) return LinkSchRelations::After;
		if (2 == flag) return LinkSchRelations::Before;
		if (3 == flag) return LinkSchRelations::Same;
		break;
	case enum_CompareScoreMethod::None:
		TRACE("ComareScoreMethod is not set");
		break;
	default:
		TRACE("ComareScoreMethod is not set");
		break;
	}
	return LinkSchRelations::None;


}
// compare the score of the different relationship
LinkSchRelations Algorithm::findDominantRelation(int ALink, int BLink)
{
	// step 1 : find the patten index
	size_t Aid = findPatternIndex(ALink);
	size_t Bid = Pattern[Aid].findRelationId(BLink);
	int flag = -1;
	switch (CompareScoreMethod)
	{
	case enum_CompareScoreMethod::Ave:
		 flag = CompareThreeNumber(Pattern[Aid].Relation[Bid].AfterScore_Ave,
									  Pattern[Aid].Relation[Bid].BeforeScore_Ave,
									  Pattern[Aid].Relation[Bid].SamePeriodScore_Ave);
		// if the three have the same value, then consider it is no dominated solution status
		if (0 == flag) return LinkSchRelations::noDominated;
		if (1 == flag) return LinkSchRelations::After;
		if (2 == flag) return LinkSchRelations::Before;
		if (3 == flag) return LinkSchRelations::Same;
		break;
	case enum_CompareScoreMethod::Total:
		 flag = CompareThreeNumber(
						Pattern[Aid].Relation[Bid].AfterScore_Total,
						Pattern[Aid].Relation[Bid].BeforeScore_Total,
						Pattern[Aid].Relation[Bid].SamePeriodScore_Total);
		if (0 == flag) return LinkSchRelations::noDominated;
		if (1 == flag) return LinkSchRelations::After;
		if (2 == flag) return LinkSchRelations::Before;
		if (3 == flag) return LinkSchRelations::Same;
		break;
	case enum_CompareScoreMethod::None:
		TRACE("ComareScoreMethod is not set");
		break;
	default:
		TRACE("ComareScoreMethod is not set");
		break;
	}
	return LinkSchRelations::None;

}

LinkSchRelations getReversRelation(LinkSchRelations& r)
{
	switch (r)
	{
	case LinkSchRelations::After: return LinkSchRelations::Before;
		break;
	case LinkSchRelations::Before: return LinkSchRelations::After;
		break;
	case LinkSchRelations::Same: return LinkSchRelations::Same;
		break;
	case LinkSchRelations::None: 
		TRACE("Debug get reverse relationship");
		return LinkSchRelations::None;
		break;
	default:
		TRACE("Debug get reverse relationship");
		return LinkSchRelations::None;
		break;
	}
}

// update the score based on the input solution 
// only call this when it is improved
void Algorithm::LearnPattern_Score(const ScheduleClass& sol, bool isGlobalImprove)
{
	//TOOD: update the pattern of the improve solution
	for (int i = 0; i < sol.LinkId.size() - 1; i++)
	{
		//int first = (*sol.LinkId.at(i)).Id;
		int first = sol.LinkId.at(i);
		int next = sol.LinkId.at(i + 1);

		size_t PtLoc = findPatternIndex(first);
		size_t VecLoc = static_cast<size_t>(FindValIndex(SetOfFailureLinks, next));
		if (isGlobalImprove)
		{
			Pattern.at(PtLoc).AbsScore.at(VecLoc) += RewardImproveGlobal;
			Pattern.at(PtLoc).Count.at(VecLoc)++;
			Pattern.at(PtLoc).AveScore.at(VecLoc) += Pattern.at(PtLoc).AbsScore.at(VecLoc) / Pattern.at(PtLoc).Count.
				at(VecLoc);
		}
		else
		{
			Pattern.at(PtLoc).AbsScore.at(VecLoc) += RewardImproveLocal;
			Pattern.at(PtLoc).Count.at(VecLoc)++;
			Pattern.at(PtLoc).AveScore.at(VecLoc) = Pattern.at(PtLoc).AbsScore.at(VecLoc) / Pattern.at(PtLoc).Count.
				at(VecLoc);
		}
	}
	//todo : need to check whether this score is improved
	for (auto& p : Pattern)
	{
		p.updateProb();
	}
}

//Update the relation score
void Algorithm::LearnPatternRelation_Score(const ScheduleClass& sol, bool isGlobalImprove)
{
	for (int i = 0; i < sol.LinkId.size(); i++)
	{
		for (int j = 0; j < sol.LinkId.size(); j++)
		{
			if (i==j) continue;
			const int ALink = sol.LinkId[i];
			const int ComparedLink = sol.LinkId[j];
			// step 1 : update the forward direction relationship, e.g. A - B
			LinkSchRelations r = sol.getRelation(ALink, ComparedLink);
			const size_t AlinkId = findPatternIndex(ALink);
			const size_t ComRelationLinkId = Pattern[AlinkId].findRelationId(ComparedLink);
			if (isGlobalImprove) Pattern[AlinkId].Relation[ComRelationLinkId].UpdateScore(r, RewardImproveGlobal);
			else Pattern[AlinkId].Relation[ComRelationLinkId].UpdateScore(r, RewardImproveLocal);
		}
	}

}

// Print the relationship into files
void Algorithm::printDomRelation(int seed) const
{
	ofstream OutFile;
	//OutFile << "Seed,First,Second,AferScore,BeforeScore,SameScore,Dom" << endl;
	OutFile.open("..//OutPut//DomRelation.txt", ios::app);
	for (size_t i = 0; i < SetOfFailureLinks.size(); i++)
	{
		for (size_t j = 0; j < SetOfFailureLinks.size(); j++)
		{
			int Alink = SetOfFailureLinks.at(i);
			int Blink = SetOfFailureLinks.at(j);
			double AfterScore, BeforeScore, SameScore;
			if (CompareScoreMethod == enum_CompareScoreMethod::Ave)
			{
				AfterScore = Pattern[i].Relation[j].AfterScore_Ave;
				BeforeScore = Pattern[i].Relation[j].BeforeScore_Ave;
				SameScore = Pattern[i].Relation[j].SamePeriodScore_Ave;
			}
			else if (CompareScoreMethod == enum_CompareScoreMethod::Total)
			{
				AfterScore = Pattern[i].Relation[j].AfterScore_Total;
				BeforeScore = Pattern[i].Relation[j].BeforeScore_Total;
				SameScore = Pattern[i].Relation[j].SamePeriodScore_Total;
			}
			int DomVal = CompareThreeNumber(AfterScore, BeforeScore, SameScore);
			OutFile << seed << ",";
			OutFile << Alink << ",";
			OutFile << Blink << ",";
			OutFile << AfterScore << ",";
			OutFile << BeforeScore << ",";
			OutFile << SameScore << ",";
			OutFile << DomVal << endl;
		}
	}

	OutFile.close();

}
