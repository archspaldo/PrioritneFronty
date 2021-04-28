#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

/// <summary>
/// Abstraktnı predok pre všetkı implementácie prioritného frontu
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class PriorityQueue
{
protected:
	/// <summary>
	/// Abstraktnı konštruktor
	/// </summary>
	PriorityQueue();
	/// <summary>
	/// Upraví prioritnı front po zvıšení prority prvku
	/// </summary>
	/// <param name="node">Prvok so zvıšenou prioritou</param>
	virtual void priority_was_increased(PriorityQueueItem<Priority, Data>* node) = 0;
	/// <summary>
	/// Upraví prioritnı front po zníení prority prvku
	/// </summary>
	/// <param name="node">Prvok so zníenı prioritou</param>
	virtual void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) = 0;
public:
	/// <summary>
	/// Virtuálny deštruktor
	/// </summary>
	virtual ~PriorityQueue();
	/// <summary>
	/// Vymáe všetky prvky z prioritného frontu
	/// </summary>
	virtual void clear() = 0;
	/// <summary>
	/// Vráti poèet prvkov v prioritnom fronte
	/// </summary>
	/// <returns>Poèet prvkov v prioritnom fronte</returns>
	virtual size_t size() const = 0;
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	virtual void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& node) = 0;
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>virtual Data pop(int& identifier) = 0;
	virtual Data pop(int& identifier) = 0;
	/// <summary>
	/// Vráti dáta s najväèšou prioritou
	/// </summary>
	/// <returns>Hodnota dát</returns>
	virtual Data& find_min() = 0;
	/// <summary>
	/// Pripojí k prioritnému frontu prvky z other_heap
	/// </summary>
	/// <param name="other_heap">Prioritnı front, ktorého prvky majú by pripojené</param>
	virtual void merge(PriorityQueue<Priority, Data>* other_heap) = 0;
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
	virtual void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority);
};

template<typename Priority, typename Data>
inline PriorityQueue<Priority, Data>::PriorityQueue()
{
}

template<typename Priority, typename Data>
inline PriorityQueue<Priority, Data>::~PriorityQueue()
{
}

template<typename Priority, typename Data>
inline void PriorityQueue<Priority, Data>::change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority)
{
	Priority old_priority = node->priority();
	node->priority() = priority;
	if (priority < old_priority)
	{
		this->priority_was_increased(node);
	}
	else if (priority > old_priority)
	{
		this->priority_was_decreased(node);
	}
}
