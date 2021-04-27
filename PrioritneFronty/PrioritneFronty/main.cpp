#pragma once
#include "Test.h"
#include "heap_monitor.h"
#include <iostream>

int main()
{
	srand(time(NULL));
	initHeapMonitor();
	PriorityQueueList<int, int>* list = new PriorityQueueList<int, int>();
	/*TestSuit1Scenario* scenario;
	scenario = new TS1ScenarioA();
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
	delete scenario;*/
	TestSuit2Scenario* scenario1 = new TestSuit2Scenario();
	Tests::execute(list, scenario1);
	delete scenario1;
	delete list;
	return 0;
}