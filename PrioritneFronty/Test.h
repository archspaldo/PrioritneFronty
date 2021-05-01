#pragma once
#include <algorithm>
#include <random>
#include <iostream>
#include <list>
#include <unordered_map>
#include <chrono>
#include "Wrapper.h"
#include "TestScenarios.h"

/// <summary>
/// Testy
/// </summary>
class Tests
{
public:
	/// <summary>
	/// Test pre testovú sadu 1
	/// </summary>
	/// <param name="pq_list">Zoznam prioritných frontov</param>
	/// <param name="scenario">Scenár</param>
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario& scenario);
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario&& scenario);
	/// <summary>
	/// Test pre testovú sadu 2
	/// </summary>
	/// <param name="pq_list">Zoznam prioritných frontov</param>
	/// <param name="scenario">Scenár</param>
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario& scenario);
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario&& scenario);
};
