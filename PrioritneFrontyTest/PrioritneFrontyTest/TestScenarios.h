#pragma once
#include <string>

class Scenario
{
private:
protected:
	Scenario();
public:
	virtual ~Scenario() {};
};

class TestSuit1Scenario : public Scenario
{
private:
	int p_push_, p_pop_, p_change_, count_;
public:
	TestSuit1Scenario(int p_push, int p_pop, int p_change, int count);
	int p_push(), p_pop(), p_change(), count();
};

class TestSuit2Scenario : public Scenario
{
private:
	int p_push_, p_pop_, p_change_;
protected:
	TestSuit2Scenario();
public:
	int p_push(), p_pop(), p_change();
};

class TS1ScenarioA : public TestSuit1Scenario
{
public:
	TS1ScenarioA();
};

class TS1ScenarioB : public TestSuit1Scenario
{
public:
	TS1ScenarioB();
};

class TS1ScenarioC : public TestSuit1Scenario
{
public:
	TS1ScenarioC();
};

class TS1ScenarioD : public TestSuit1Scenario
{
public:
	TS1ScenarioD();
};

class TS1ScenarioE : public TestSuit1Scenario
{
public:
	TS1ScenarioE();
};

class TS2ScenarioA : public TestSuit2Scenario
{
public:
	TS2ScenarioA();
};

