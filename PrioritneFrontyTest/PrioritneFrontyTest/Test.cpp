#include "Test.h"

void TestSuit1::execute(PriorityQueueList<int, int>* pq_list, TestSuit1Scenario* scenario)
{
	pq_list->start_write(scenario->scenario_name());
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
		pq_list->clear_structures();
	}
	pq_list->end_write();
}
