#include "TestScenarios.h"

Scenario::Scenario()
{
}


TestSuit1Scenario::TestSuit1Scenario(int p_push, int p_pop, int p_change, int count) :
	p_push_(p_push), p_pop_(p_pop), p_change_(p_change), count_(count)
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

int TestSuit1Scenario::count()
{
	return this->count_;
}

TS1ScenarioA::TS1ScenarioA() :
	TestSuit1Scenario(87, 13, 0, 1000000)
{
}

TS1ScenarioB::TS1ScenarioB() :
	TestSuit1Scenario(70, 10, 20, 1250000)
{
}

TS1ScenarioC::TS1ScenarioC() :
	TestSuit1Scenario(67, 33, 0, 1000000)
{
}

TS1ScenarioD::TS1ScenarioD() :
	TestSuit1Scenario(40, 20, 40, 1666667)
{
}

TS1ScenarioE::TS1ScenarioE() :
	TestSuit1Scenario(50, 40, 10, 1000000)
{
}

TestSuit2Scenario::TestSuit2Scenario(int operation_count, int iteration_count, double insert_ratio) :
	Scenario(), operation_count_(operation_count), iteration_count_(iteration_count), insert_ratio_(insert_ratio)
{
}

int TestSuit2Scenario::operation_count()
{
	return this->operation_count_;
}

int TestSuit2Scenario::iteration_count()
{
	return this->iteration_count_;
}

double TestSuit2Scenario::insert_ratio()
{
	return this->insert_ratio_;
}

TS2ScenarioA::TS2ScenarioA() :
	TestSuit2Scenario(100, 1, 1.0)
{
}

TS2ScenarioB::TS2ScenarioB() :
	TestSuit2Scenario(100, 10000, 0.66)
{
}
