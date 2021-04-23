#pragma once
#include "Test.h"
#include "heap_monitor.h"
#include <iostream>

int main()
{
	srand(1);
	//srand(time(NULL));
	initHeapMonitor();
	PriorityQueueList<int, int>* list = new PriorityQueueList<int, int>();

	TestSuit1Scenario* scenario = new TS1ScenarioA();
	Tests::execute(list, scenario);
	delete scenario;
	scenario = new TS1ScenarioB();
	Tests::execute(list, scenario);
	delete scenario;
	scenario = new TS1ScenarioC();
	Tests::execute(list, scenario);
	delete scenario;
	scenario = new TS1ScenarioD();
	Tests::execute(list, scenario);
	delete scenario;
	scenario = new TS1ScenarioE();
	Tests::execute(list, scenario);
	delete scenario;
	/*TestSuit2Scenario* scenario = new TS2ScenarioA();
	Tests::execute(list, scenario);
	delete scenario; */
	delete list;
	return 0;
}