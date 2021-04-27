#include "Test.h"

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario)
{
	for (int j = 0; j < 2; j++)
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
		pq_list->clear_structures();
	}
	system("pause");
}

void Tests::execute(PriorityQueueList<int, int>* pq_list, TestSuit2Scenario* scenario)
{
	int push, pop, change, counter = 0, random;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 10000; j < 2000000; j *= 1.5)
		{
			
			for (; pq_list->size() < j;)
			{
				for (int k = 0; k < 4; k++)
				{
					pq_list->push(counter++, rand() % UINT32_MAX, rand() % UINT32_MAX);
				}
				pq_list->pop();
			}
			system("pause");
			for (int l = 0; l < 10000; l++)
			{
				push = pop = change = 100;
				while (push > 0 || pop > 0 || change > 0)
				{
					random = rand() % (pop + change + push);
					if (random < push || pq_list->size() == 0)
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
			}
			system("pause");
			pq_list->clear_structures();
		}
		counter = 0;
	}
}
