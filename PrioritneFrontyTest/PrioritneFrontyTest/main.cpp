#pragma once
#include "Test.h"
#include "heap_monitor.h"
#include <iostream>

int main()
{
	//srand(1);
	srand(time(NULL));
	initHeapMonitor();
	PriorityQueueList<int, int>* list = new PriorityQueueList<int, int>();
	TestSuit1Scenario* scenario = new TS1ScenarioA();
	TestSuit1::execute(list, scenario);
	delete list;
	delete scenario;
	//system("pause");
	return 0;
}