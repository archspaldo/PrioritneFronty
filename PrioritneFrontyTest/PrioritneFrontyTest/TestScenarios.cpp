#include "TestScenarios.h"

Scenario::Scenario(std::string scenario_name) :
	scenario_name_(scenario_name)
{
}

Scenario::~Scenario()
{
}

std::string& Scenario::scenario_name()
{
	return this->scenario_name_;
}

TestSuit1Scenario::TestSuit1Scenario(std::string scenario_name, int p_push, int p_pop, int p_change) :
	Scenario("TestSuit1\\" + scenario_name), p_push_(p_push), p_pop_(p_pop), p_change_(p_change)
{
}

int TestSuit1Scenario::p_push()
{
	return this->p_push_;
}

int TestSuit1Scenario::p_pop()
{
	return this->p_pop_ + this->p_push();
}

int TestSuit1Scenario::p_change()
{
	return this->p_change_ + this->p_pop();
}

TS1ScenarioA::TS1ScenarioA() :
	TestSuit1Scenario("ScenarioA\\", 80, 20, 0)
{
}

TS1ScenarioB::TS1ScenarioB() :
	TestSuit1Scenario("ScenarioB\\", 50, 20, 30)
{
}

TS1ScenarioC::TS1ScenarioC() :
	TestSuit1Scenario("ScenarioC\\", 40, 30, 30)
{
}

TS1ScenarioD::TS1ScenarioD() :
	TestSuit1Scenario("ScenarioD\\", 70, 10, 20)
{
}

TestSuit2Scenario::TestSuit2Scenario(std::string scenario_name) :
	Scenario("TestSuit2\\" + scenario_name)
{
	
}

TS2ScenarioA::TS2ScenarioA() :
	TestSuit2Scenario("ScenarioA\\")
{
}

int TS2ScenarioA::p_push()
{
	return 100;
}

int TS2ScenarioA::p_pop()
{
	return 30;
}

int TS2ScenarioA::p_change()
{
	return 70;
}
