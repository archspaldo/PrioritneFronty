#pragma once
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <time.h>
#include <sys/timeb.h>

/// <summary>
/// Strukt�ra uchovavaj�ca identifik�tory s O(1) v�berom n�hodn�ho prvku
/// </summary>
class RandomizedSet;

/// <summary>
/// Oba�uj�ca trieda pre prioritn� front, ktor� obsahuje mapovanie identifik�torov k prvkom
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class PriorityQueueWrapper
{
private:
	/// <summary>
	/// Tabu�ka mapuj�ca identifik�tory k prvkom
	/// </summary>
	std::unordered_map<int, PriorityQueueItem<Priority, Data>*>* identifier_map_;
	/// <summary>
	/// Prioritn� front
	/// </summary>
	PriorityQueue<Priority, Data>* priority_queue_;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	/// <param name="priority_queue">Prioritn� front</param>
	PriorityQueueWrapper(PriorityQueue<Priority, Data>* priority_queue);
	/// <summary>
	/// De�truktor
	/// </summary>
	~PriorityQueueWrapper();
	/// <summary>
	/// Vyma�e prvky z prioritn�ho frontu
	/// </summary>
	void reset();
	/// <summary>
	/// Oper�cia oba�uj�ca oper�ciu vlo�
	/// </summary>
	/// <param name="identifier">Identifik�tor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">D�ta</param>
	void push(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Oper�cia oba�uj�ca oper�ciu vyber minimum
	/// </summary>
	/// <returns>Idenifik�tor odstranen�ho prvku</returns>
	int pop();
	/// <summary>
	/// Oper�cia oba�uj�ca oper�ciu zme� prioritu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Nov� priorita prvku</param>
	void change_priority(const int identifier, const Priority& priority);
};

/// <summary>
/// Zoznam prioritn�ch frontov
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class PriorityQueueList
{
	/// <summary>
	/// Zoznam obalen�ch prioritn�ch frontov
	/// </summary>
	std::list<PriorityQueueWrapper<Priority, Data>*>* priority_queue_list_;
	/// <summary>
	/// Tabu�ka identifik�torov
	/// </summary>
	RandomizedSet* identifier_set_;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	PriorityQueueList();
	/// <summary>
	/// De�truktor
	/// </summary>
	~PriorityQueueList();
	/// <summary>
	/// Vym�e prvky zo v�etk�ch prioritn�ch frontov
	/// </summary>
	void clear_structures();
	/// <summary>
	/// Vr�ti n�hodn� identifik�tor prvku
	/// </summary>
	/// <returns></returns>
	int get_random_identifier();
	/// <summary>
	/// Vr�ti po�et prvkov v jednotliv�ch prioritn�ch frontoch
	/// </summary>
	/// <returns></returns>
	int size();
	/// <summary>
	/// Vlo�� prvok s parametrami do v�etk�ch prioritn�ch frontov
	/// </summary>
	/// <param name="identifier">Identifik�tor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">D�ta</param>
	void push(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Vyberie minim�lny prvok zo v�etk�ch prioritn�ch frontov
	/// </summary>
	void pop();
	/// <summary>
	/// Zmen� prioritu prvku s identifik�torom identifier na priority 
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Nov� priorita</param>
	void change_priority(const int identifier, const Priority& priority);
};

template<typename Priority, typename Data>
inline PriorityQueueWrapper<Priority, Data>::PriorityQueueWrapper(PriorityQueue<Priority, Data>* priority_queue) :
	identifier_map_(new std::unordered_map<int, PriorityQueueItem<Priority, Data>*>()),
	priority_queue_(priority_queue)
{
}

template<typename Priority, typename Data>
inline PriorityQueueWrapper<Priority, Data>::~PriorityQueueWrapper()
{
	delete this->priority_queue_;
	delete this->identifier_map_;
	this->priority_queue_ = nullptr;
	this->identifier_map_ = nullptr;
}

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::reset()
{
	this->priority_queue_->clear();
	this->identifier_map_->clear();
}

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data)
{
	PriorityQueueItem<Priority, Data>* priority_queue_item;
	this->priority_queue_->push(identifier, priority, data, priority_queue_item);
	(*this->identifier_map_)[identifier] = priority_queue_item;
}

template<typename Priority, typename Data>
inline int PriorityQueueWrapper<Priority, Data>::pop()
{
	int identifier;
	this->priority_queue_->pop(identifier);
	this->identifier_map_->erase(identifier);
	return identifier;
}

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::change_priority(const int identifier, const Priority& priority)
{
	PriorityQueueItem<Priority, Data>* priority_queue_item = (*this->identifier_map_)[identifier];
	this->priority_queue_->change_priority(priority_queue_item, priority);
}

template<typename Priority, typename Data>
inline PriorityQueueList<Priority, Data>::PriorityQueueList() :
	priority_queue_list_(new std::list<PriorityQueueWrapper<Priority, Data>*>()),
	identifier_set_(new RandomizedSet())
{
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new BinaryHeap<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new PairingHeapTwoPass<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new PairingHeapMultiPass<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new RankPairingHeap<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new FibonacciHeap<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new BinomialHeapOnePass<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new BinomialHeapMultiPass<Priority, Data>()));
}

template<typename Priority, typename Data>
inline PriorityQueueList<Priority, Data>::~PriorityQueueList()
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		delete item;
	}
	delete this->priority_queue_list_;
	delete this->identifier_set_;
	this->priority_queue_list_ = nullptr;
	this->identifier_set_ = nullptr;
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::clear_structures()
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->reset();
	}
	this->identifier_set_->clear();
}

template<typename Priority, typename Data>
inline int PriorityQueueList<Priority, Data>::get_random_identifier()
{
	return this->identifier_set_->get_random();
}

template<typename Priority, typename Data>
inline int PriorityQueueList<Priority, Data>::size()
{
	return this->identifier_set_->size();
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data)
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->push(identifier, priority, data);
	}
	this->identifier_set_->insert(identifier);
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::pop()
{
	int identifier = 0;
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		identifier = item->pop();
	}
	this->identifier_set_->remove(identifier);
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::change_priority(const int identifier, const Priority& priority)
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->change_priority(identifier, priority);
	}
}

/// <summary>
/// Tabu�ka s O(1) pr�stupom k n�hodn�mu prvku
/// Variacia https://www.geeksforgeeks.org/design-a-data-structure-that-supports-insert-delete-getrandom-in-o1-with-duplicates/
/// </summary>
class RandomizedSet {
	/// <summary>
	/// Tabu�ka mapuj�ca identifik�tory k indexom v implicitnom zozname
	/// </summary>
	std::unordered_map<int, int> map_;
	/// <summary>
	/// Implicitn� zoznam identifik�torov
	/// </summary>
	std::vector<int> vector_;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	RandomizedSet() {}
	/// <summary>
	/// Vlo�� identifik�tor do tabu�ky
	/// </summary>
	/// <param name="identifier">Identifik�tor</param>
	void insert(int identifier)
	{
		this->vector_.push_back(identifier);
		this->map_[identifier] = this->vector_.size() - 1;
	}
	/// <summary>
	/// Odst�ni identifik�tor z tabu�ky
	/// </summary>
	/// <param name="identifier">Identifik�tor</param>
	void remove(int identifier)
	{
		int index = this->map_[identifier];
		int last_identifier = this->vector_.back();
		this->vector_[index] = last_identifier;
		this->vector_.pop_back();
		this->map_[last_identifier] = index;
		this->map_.erase(identifier);
	}
	/// <summary>
	/// Vr�ti n�hodn� identifik�tor z tabu�ky
	/// </summary>
	/// <returns>N�hodn� identifik�tor</returns>
	int get_random()
	{
		return this->vector_[rand() % (this->vector_.size())];
	}
	/// <summary>
	/// Odstr�ni v�etky identifik�tory z tabu�ky
	/// </summary>
	void clear()
	{
		this->map_.clear();
		this->vector_.clear();
	}
	/// <summary>
	/// Vr�ti po�et identifik�torov v tabu�ke
	/// </summary>
	/// <returns>Po�et identifik�torov</returns>
	size_t size()
	{
		return this->vector_.size();
	}
};