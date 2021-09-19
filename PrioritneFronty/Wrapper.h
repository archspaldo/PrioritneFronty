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
/// Struktúra uchovavajúca identifikátory s O(1) výberom náhodného prvku
/// </summary>
class RandomizedSet;

/// <summary>
/// Oba¾ujúca trieda pre prioritný front, ktorá obsahuje mapovanie identifikátorov k prvkom
/// </summary>
/// <typeparam name="Priority">Dátový typ priority</typeparam>
/// <typeparam name="Data">Dátový typ dát</typeparam>
template <typename Priority, typename Data>
class PriorityQueueWrapper
{
private:
	/// <summary>
	/// Tabu¾ka mapujúca identifikátory k prvkom
	/// </summary>
	std::unordered_map<int, PriorityQueueItem<Priority, Data>*>* identifier_map_;
	/// <summary>
	/// Prioritný front
	/// </summary>
	PriorityQueue<Priority, Data>* priority_queue_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="priority_queue">Prioritný front</param>
	PriorityQueueWrapper(PriorityQueue<Priority, Data>* priority_queue);
	/// <summary>
	/// Deštruktor
	/// </summary>
	~PriorityQueueWrapper();
	/// <summary>
	/// Vymaže prvky z prioritného frontu
	/// </summary>
	void reset();
	/// <summary>
	/// Operácia oba¾ujúca operáciu vlož
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	void push(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Operácia oba¾ujúca operáciu vyber minimum
	/// </summary>
	/// <returns>Idenifikátor odstraneného prvku</returns>
	int pop();
	/// <summary>
	/// Operácia oba¾ujúca operáciu zmeò prioritu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Nová priorita prvku</param>
	void change_priority(const int identifier, const Priority& priority);
};

/// <summary>
/// Zoznam prioritných frontov
/// </summary>
/// <typeparam name="Priority">Dátový typ priority</typeparam>
/// <typeparam name="Data">Dátový typ dát</typeparam>
template <typename Priority, typename Data>
class PriorityQueueList
{
	/// <summary>
	/// Zoznam obalených prioritných frontov
	/// </summary>
	std::list<PriorityQueueWrapper<Priority, Data>*>* priority_queue_list_;
	/// <summary>
	/// Tabu¾ka identifikátorov
	/// </summary>
	RandomizedSet* identifier_set_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	PriorityQueueList();
	/// <summary>
	/// Deštruktor
	/// </summary>
	~PriorityQueueList();
	/// <summary>
	/// Vymáže prvky zo všetkých prioritných frontov
	/// </summary>
	void clear_structures();
	/// <summary>
	/// Vráti náhodný identifikátor prvku
	/// </summary>
	/// <returns></returns>
	int get_random_identifier();
	/// <summary>
	/// Vráti poèet prvkov v jednotlivých prioritných frontoch
	/// </summary>
	/// <returns></returns>
	int size();
	/// <summary>
	/// Vloží prvok s parametrami do všetkých prioritných frontov
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	void push(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Vyberie minimálny prvok zo všetkých prioritných frontov
	/// </summary>
	void pop();
	/// <summary>
	/// Zmení prioritu prvku s identifikátorom identifier na priority 
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Nová priorita</param>
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
/// Tabu¾ka s O(1) prístupom k náhodnému prvku
/// Variacia https://www.geeksforgeeks.org/design-a-data-structure-that-supports-insert-delete-getrandom-in-o1-with-duplicates/
/// </summary>
class RandomizedSet {
	/// <summary>
	/// Tabu¾ka mapujúca identifikátory k indexom v implicitnom zozname
	/// </summary>
	std::unordered_map<int, int> map_;
	/// <summary>
	/// Implicitný zoznam identifikátorov
	/// </summary>
	std::vector<int> vector_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	RandomizedSet() {}
	/// <summary>
	/// Vloží identifikátor do tabu¾ky
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	void insert(int identifier)
	{
		this->vector_.push_back(identifier);
		this->map_[identifier] = this->vector_.size() - 1;
	}
	/// <summary>
	/// Odstáni identifikátor z tabu¾ky
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
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
	/// Vráti náhodný identifikátor z tabu¾ky
	/// </summary>
	/// <returns>Náhodný identifikátor</returns>
	int get_random()
	{
		return this->vector_[rand() % (this->vector_.size())];
	}
	/// <summary>
	/// Odstráni všetky identifikátory z tabu¾ky
	/// </summary>
	void clear()
	{
		this->map_.clear();
		this->vector_.clear();
	}
	/// <summary>
	/// Vráti poèet identifikátorov v tabu¾ke
	/// </summary>
	/// <returns>Poèet identifikátorov</returns>
	size_t size()
	{
		return this->vector_.size();
	}
};