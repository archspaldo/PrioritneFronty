#pragma once
#include "Test.h"
#include "heap_monitor.h"
#include <iostream>

int main()
{
	srand(time(NULL));
	initHeapMonitor();
	PriorityQueueList<int, int> list;
	Tests::execute(list, TS1ScenarioA());
	Tests::execute(list, TS1ScenarioB());
	Tests::execute(list, TS1ScenarioC());
	Tests::execute(list, TS1ScenarioD());
	Tests::execute(list, TS1ScenarioE());
	Tests::execute(list, TS2ScenarioA());
	Tests::execute(list, TS2ScenarioB());
	return 0;
}