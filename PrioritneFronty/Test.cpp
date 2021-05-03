#include "Test.h"

void Tests::execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario& scenario)
{
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < scenario.count(); i++)
		{
			int number = rand() % scenario.p_change();
			if (number < scenario.p_push())
			{
				pq_list.push(i, rand(), rand());
			}
			else if (number < scenario.p_pop() && pq_list.size() > 0)
			{
				pq_list.pop();
			}
			else if (pq_list.size() > 0)
			{
				pq_list.change_priority(pq_list.get_random_identifier(), rand());
			}
			else
			{
				i--;
			}
		}
		pq_list.clear_structures();
	}
	system("pause");
}

void Tests::execute(PriorityQueueList<int, int>& pq_list, TestSuit1Scenario&& scenario)
{
	TestSuit1Scenario ts = scenario;
	execute(pq_list, ts);
}

void Tests::execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario& scenario)
{
	int push, pop, change, counter = 0, random;
	for (double item_count = 10000; item_count < 2000000; item_count *= 1.5)
	{
		while (pq_list.size() < item_count)
		{
			if ((double)rand() / RAND_MAX < scenario.insert_ratio() || pq_list.size() == 0)
			{
				pq_list.push(counter++, rand(), rand());
			}
			else
			{
				pq_list.pop();
			}
		}
		system("pause");
		for (int iterations = 0; iterations < scenario.iteration_count(); iterations++)
		{
			push = pop = change = scenario.operation_count();
			while (push > 0 || pop > 0 || change > 0)
			{
				random = rand() % (pop + change + push);
				if (random < push || pq_list.size() == 0)
				{
					pq_list.push(counter++, rand(), rand());
					push--;
				}
				else if (random  < push + pop)
				{
					pq_list.pop();
					pop--;
				}
				else
				{
					pq_list.change_priority(pq_list.get_random_identifier(), rand());
					change--;
				}
			}
		}
		system("pause");
		pq_list.clear_structures();
	}
}

void Tests::execute(PriorityQueueList<int, int>& pq_list, TestSuit2Scenario&& scenario)
{
	TestSuit2Scenario ts = scenario;
	execute(pq_list, ts);
}

