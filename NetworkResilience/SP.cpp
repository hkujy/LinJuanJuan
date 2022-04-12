
#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;
bool UpdateLable(const double CurrentCost, const double LinkCost, const double OldNextNodeCost, double &NewCost)
{

	NewCost = InvaildFloat;
	//printf("ModeType = %d, link Type = %d \n", ModeType, Links.at(LinkIndex).Type);
	NewCost = CurrentCost + LinkCost;
#ifdef __DEBUG__  
	if (NewCost < 0.0)
		DEBUG("Label New Cos is less 0, linkid = %d, CurrentCost= %f", LinkIndex, CurrentCost);
#endif
	if (NewCost <= OldNextNodeCost) return true;
	else return false;
}

int GraphClass::SP(const int origin, std::vector<double> &label)
{
//int *PredLink, double *Labl
try
	{
		int  WasInQueue = -7;
		int now, NewNode, Return2Q_Count = 0;
		double NewCost = InvaildFloat;
		int QueueFirst, QueueLast;
		label.clear();
		std::vector<int> QueueNext;
		
		QueueNext.reserve(numNodes);
	
		for (int node = 0; node < numNodes; node++){
			QueueNext.push_back(InvaildInt);
			//label[node] = LargeNum;
			label.push_back(LargeNum);
			//PredLink[node] = InvaildInt;
			this->MinPathPredLink[origin][node] = InvaildInt;
		}
		now = origin;
		QueueNext.at(now) = WasInQueue;
		//PredLink[now] = InvaildInt;
		this->MinPathPredLink[origin][now] = InvaildInt;

		label[now] = 0.0;
		QueueFirst = QueueLast = InvaildInt;
		while ((now != InvaildInt) && (now != WasInQueue)) {
			for (auto k =this->Nodes.at(now).OutLinks.begin();
				k != this->Nodes.at(now).OutLinks.end(); k++) {

				// in case the capacity is zero
				if ((*k)->CaRevise <zero) continue;
				NewNode = (*k)->Head;
				//printf("current Cost= %f \n", label[now]);
				if (UpdateLable(label[now], (*k)->Cost, label[NewNode], NewCost))
				{
				//if (UpdateLable(label[now], thiLinks,  *k, label[NewNode], NewCost)){
					//if (label[NewNode] > NewCost) {
					//printf("new node is = %d, cost = %f \n", NewNode,NewCost);
					label[NewNode] = NewCost;
					//if (label[NewNode] > 1000.0f&&isPrint)
					//{
					//	DEBUG("Cost is %f, node is %i, PLink is %i", label[NewNode], NewNode, *k);
					//}
					//PredLink[NewNode] = *k;
					this->MinPathPredLink[origin][NewNode] = (*k)->Id;

					if (QueueNext.at(NewNode) == WasInQueue) {
						QueueNext.at(NewNode) = QueueFirst;
						QueueFirst = NewNode;
						if (QueueLast == InvaildInt)
							QueueLast = NewNode;
						Return2Q_Count++;
					}
					else if (QueueNext.at(NewNode) == InvaildInt && NewNode != QueueLast) {
						if (QueueLast != InvaildInt) { 					/*Usually*/
							QueueNext.at(QueueLast) = NewNode;
							QueueLast = NewNode;
						}
						else {			  /* If the queue is empty, initialize it. */
							QueueFirst = QueueLast = NewNode;
							QueueNext.at(QueueLast) = InvaildInt;
						}
					}
					/* If the new node is in the queue, just leave it there. (Do nothing) */
				}//if (label[NewNode] > NewCost) {
			} // for incoming link cycle

			/* Get the first node out of the queue, and use it as the current node. */
			now = QueueFirst;
			if ((now == InvaildInt) || (now == WasInQueue))	break;

			QueueFirst = QueueNext[now];
			QueueNext.at(now) = WasInQueue;
			if (QueueLast == now) QueueLast = InvaildInt;
		}
		return 1;
	}//try
	catch (exception& e)
	{
		printf("MinPath: %s", e.what());
		TRACE("MinPath: %s", e.what());
		return 0;
	}

}
