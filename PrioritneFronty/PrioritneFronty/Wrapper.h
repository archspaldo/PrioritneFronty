#pragma once
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"
#include <iostream>
#include <chrono>
#include <unordered_set>
#include <time.h>
#include <sys/timeb.h>

class RandomizedSet;

template <typename Priority, typename Data>
class PriorityQueueWrapper
{
private:
	std::unordered_map<int, PriorityQueueItem<Priority, Data>*>* identifier_map_;
	PriorityQueue<Priority, Data>* priority_queue_;
public:
	PriorityQueueWrapper(PriorityQueue<Priority, Data>* priority_queue);
	~PriorityQueueWrapper();
	void reset();
	void push(const int identifier, const Priority& priority, const Data& data);
	int pop();
	void change_priority(const int identifier, const Priority& priority);
};

template <typename Priority, typename Data>
class PriorityQueueList
{
	std::list<PriorityQueueWrapper<Priority, Data>*>* priority_queue_list_;
	RandomizedSet* identifier_set_;
public:
	PriorityQueueList();
	~PriorityQueueList();
	void clear_structures();
	int get_random_identifier();
	int size();
	void push(const int identifier, const Priority& priority, const Data& data);
	void pop();
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
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new BinomialHeapSinglePass<Priority, Data>()));
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
	int identifier;
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

// algoritmus je variacia https://www.geeksforgeeks.org/design-a-data-structure-that-supports-insert-delete-getrandom-in-o1-with-duplicates/
class RandomizedSet {
	std::unordered_map<int, int> map_;
	std::vector<int> vector_;

public:
	RandomizedSet() {}

	void insert(int val)
	{
		this->vector_.push_back(val);
		int index = this->vector_.size() - 1;
		this->map_[val] = index;
	}

	void remove(int val)
	{
		int index = this->map_[val];
		int last_val = this->vector_.back();
		this->vector_[index] = last_val;
		this->vector_.pop_back();
		this->map_[last_val] = index;
		this->map_.erase(val);
	}

	int get_random()
	{
		int index = rand() % (this->vector_.size());
		return this->vector_[index];
	}

	void clear()
	{
		this->map_.clear();
		this->vector_.clear();
	}

	size_t size()
	{
		return this->vector_.size();
	}
};