#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

/// <summary>
/// Abstraktn� predok pre v�etk� implement�cie prioritn�ho frontu
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class PriorityQueue
{
protected:
	/// <summary>
	/// Abstraktn� kon�truktor
	/// </summary>
	PriorityQueue();
	/// <summary>
	/// Uprav� prioritn� front po zv��en� prority prvku
	/// </summary>
	/// <param name="node">Prvok so zv��enou prioritou</param>
	virtual void priority_was_increased(PriorityQueueItem<Priority, Data>* node) = 0;
	/// <summary>
	/// Uprav� prioritn� front po zn�en� prority prvku
	/// </summary>
	/// <param name="node">Prvok so zn�en� prioritou</param>
	virtual void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) = 0;
public:
	/// <summary>
	/// Virtu�lny de�truktor
	/// </summary>
	virtual ~PriorityQueue();
	/// <summary>
	/// Vym�e v�etky prvky z prioritn�ho frontu
	/// </summary>
	virtual void clear() = 0;
	/// <summary>
	/// Vr�ti po�et prvkov v prioritnom fronte
	/// </summary>
	/// <returns>Po�et prvkov v prioritnom fronte</returns>
	virtual size_t size() const = 0;
	/// <summary>
	/// Vlo�� d�ta do prioritn�ho frontu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren� prvok</param>
	virtual void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& node) = 0;
	/// <summary>
	/// Vyberie z prioritn�ho frontu d�ta s najv��ou prioritou
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku s najv��ou prioritou</param>
	/// <returns>Hodnota d�t</returns>virtual Data pop(int& identifier) = 0;
	virtual Data pop(int& identifier) = 0;
	/// <summary>
	/// Vr�ti d�ta s najv��ou prioritou
	/// </summary>
	/// <returns>Hodnota d�t</returns>
	virtual Data& find_min() = 0;
	/// <summary>
	/// Pripoj� k prioritn�mu frontu prvky z other_heap
	/// </summary>
	/// <param name="other_heap">Prioritn� front, ktor�ho prvky maj� by� pripojen�</param>
	virtual void merge(PriorityQueue<Priority, Data>* other_heap) = 0;
	/// <summary>
	/// Zmen� prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktor�mu m� by� zmenen� priorita</param>
	/// <param name="priority">Nov� priorita prvku</param>
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
