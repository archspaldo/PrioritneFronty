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
public:
	TestSuit1Scenario(std::string scenario_name, int p_push, int p_pop, int p_change);
	int p_push(), p_pop(), p_change();
};

class TestSuit2Scenario : public Scenario
{
private:
protected:
	TestSuit2Scenario(std::string scenario_name);
public:
	virtual int p_push() = 0, p_pop() = 0, p_change() = 0;
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

class TS2ScenarioA : public TestSuit2Scenario
{
public:
	TS2ScenarioA();
	int p_push() override, p_pop() override, p_change() override;
};

