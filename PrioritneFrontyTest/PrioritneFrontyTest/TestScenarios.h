#pragma once
#include <string>

class Scenario
{
private:
	std::string scenario_name_;
protected:
	Scenario(std::string scenario_name);
public:
	virtual ~Scenario();
	std::string& scenario_name();
};

class TestSuit1Scenario : public Scenario
{
private:
	int p_push_, p_pop_, p_change_;
protected:
	TestSuit1Scenario(std::string scenario_name, int p_push, int p_pop, int p_change);
public:
	~TestSuit1Scenario();
	int p_push(), p_pop(), p_change();
};

class TestSuit2Scenario : public Scenario
{
private:
	int& p_push_, & p_pop_, & p_change_;
protected:
	TestSuit2Scenario(std::string scenario_name, int& p_push, int& p_pop, int& p_change);
public:
	~TestSuit2Scenario();
	int& p_push(), & p_pop(), & p_change();
};

class TS1ScenarioA : public TestSuit1Scenario
{
public:
	TS1ScenarioA();
	~TS1ScenarioA();
};

class TS1ScenarioB : public TestSuit1Scenario
{
public:
	TS1ScenarioB();
	~TS1ScenarioB();
};

class TS1ScenarioC : public TestSuit1Scenario
{
public:
	TS1ScenarioC();
	~TS1ScenarioC();
};

class TS2ScenarioA : public TestSuit2Scenario
{
public:
	TS2ScenarioA();
	~TS2ScenarioA();
};

