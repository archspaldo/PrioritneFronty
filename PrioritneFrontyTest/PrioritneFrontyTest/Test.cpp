#include "Test.h"

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario)
{
<<<<<<< Updated upstream
	//pq_list->start_write(scenario->scenario_name());
	for (int j = 0; j < 100; j++)
=======
	for (int j = 0; j < 1; j++)
>>>>>>> Stashed changes
	{
		for (int i = 0; i < scenario->count(); i++)
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
<<<<<<< Updated upstream
=======
		system("pause");
		pq_list->clear_structures();
	}
}

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit2Scenario* scenario)
{
	int push, pop, change, counter = 0, random, k = 0;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 1000; j < 2000000; j *= 1.5)
		{
			for (; k < j; k++)
			{
				pq_list->push(counter++, rand() % UINT32_MAX, rand() % UINT32_MAX);
			}
			pq_list->pop();
			system("pause");
			push = pop = change = sqrt(j) * 10;
			while (push > 0 || pop > 0 || change > 0)
			{
				random = rand() % (pop + change + push);
				if (random < push)
				{
					pq_list->push(counter++, rand() % UINT32_MAX, rand() % UINT32_MAX);
					push--;
				}
				else if (random  < push + pop)
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
			system("pause");
		}
>>>>>>> Stashed changes
		pq_list->clear_structures();
		counter = 0;
	}
}
