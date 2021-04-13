#include "Test.h"

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario)
{
	//pq_list->start_write(scenario->scenario_name());
	for (int j = 0; j < 1; j++)
	{
		for (int i = 0; i < 100000; i++)
		{
			int number = rand() % scenario->p_change();
			if (number < scenario->p_push())
			{
				pq_list->push(i, rand() % UINT32_MAX, rand() % UINT32_MAX);
			}
			else if (number < scenario->p_pop() && pq_list->size() > 0)
			{
				pq_list->pop();
			}
			else if (pq_list->size() > 0)
			{
				pq_list->change_priority(pq_list->get_random_identifier(), rand() % UINT32_MAX);
			}
			else
			{
				i--;
			}
		}
		system("pause");
		pq_list->clear_structures();
	}
	//pq_list->end_write();
}

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit2Scenario* scenario)
{
	//pq_list->start_write(scenario->scenario_name());
	int pop, change;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j * scenario->p_push() < 1000000; j++)
		{
			for (int k = 0; k < scenario->p_push() + scenario->p_pop() + 1; k++)
			{
				pq_list->push(j * (scenario->p_push() + scenario->p_pop() + 1) + k, rand() % UINT32_MAX, rand() % UINT32_MAX);
			}
			pop = scenario->p_pop();
			change = scenario->p_change();
			while (pop > 0 && change > 0)
			{
				if (rand() % (pop + change) < pop)
				{
					pq_list->pop();
					pop--;
				}
				else
				{
					pq_list->change_priority(pq_list->get_random_identifier(), rand() % UINT32_MAX);
					change--;
				}
			}
			while (pop > 0)
			{
				pq_list->pop();
				pop--;
			}
			while (change > 0)
			{
				pq_list->change_priority(pq_list->get_random_identifier(), rand() % UINT32_MAX);
				change--;
			}
		}
		pq_list->clear_structures();
	}
	//pq_list->end_write();
}
