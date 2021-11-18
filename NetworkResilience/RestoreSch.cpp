#include "RestoreSchClass.h"


Scenario SCHCLASS::ConvertToScenario() {
	Scenario s;
	for (auto l : Links) s.LinkIds.push_back(l->ID);
	for (auto t : StartTime) s.tau.push_back(t);
	return s;
}

void SCHCLASS::improve() {
	// improve the solutions

}