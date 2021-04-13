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
	TestSuit1Scenario* scenario = new TestSuit1Scenario("Custom1\\", 70, 10, 20);
	Tests::execute(list, scenario);
	delete list;
	delete scenario;
	return 0;
}