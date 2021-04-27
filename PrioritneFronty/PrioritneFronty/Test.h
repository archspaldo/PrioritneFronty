#pragma once
#include <algorithm>
#include <random>
#include <iostream>
#include <list>
#include <unordered_map>
#include <chrono>
#include "Wrapper.h"
#include "TestScenarios.h"

class Tests
{
public:
	static void execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario);
	static void execute(PriorityQueueList<int, int>* pq_list, TestSuit2Scenario* scenario);
};
