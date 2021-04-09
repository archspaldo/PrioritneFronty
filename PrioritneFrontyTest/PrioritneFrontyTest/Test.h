#pragma once
#include <algorithm>
#include <random>
#include <iostream>
#include <list>
#include <unordered_map>
#include <chrono>
#include "Wrapper.h"
#include "TestScenarios.h"

class TestSuit1
{
public:
	static void execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario);
};

class TestSuit2
{
public:
	static void execute(PriorityQueueList<int, int>* pq_list, TestSuit2Scenario* scenario);
};