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
	/// Test pre testov� sadu 1
	/// </summary>
	/// <param name="pq_list">Zoznam prioritn�ch frontov</param>
	/// <param name="scenario">Scen�r</param>
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario& scenario);
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario&& scenario);
	/// <summary>
	/// Test pre testov� sadu 2
	/// </summary>
	/// <param name="pq_list">Zoznam prioritn�ch frontov</param>
	/// <param name="scenario">Scen�r</param>
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario& scenario);
	static void execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario&& scenario);
};
