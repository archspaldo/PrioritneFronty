#pragma once
#include <algorithm>
#include <random>
#include <iostream>
#include <list>
#include <unordered_map>
#include <chrono>
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"

template <typename K, typename T>
class PriorityQueueList
{
	std::list<PriorityQueueWrapper<K, T>*>* priority_queue_list_;
	std::vector<int>* identifier_list_;
public:
	PriorityQueueList();
	~PriorityQueueList();
	void reset();
	int get_random_identifier();
	int size();
	void push(const int identifier, const K& priority, const T& data);
	void pop();
	void find_min();
	void change_priority(const int identifier, const K& priority);
	void merge(const int size);
};

template  <typename K, typename T>
class Test
{
public:
	virtual void execute(PriorityQueueList<K, T>* = nullptr) = 0;
};

class PushTest : public Test<int, int>
{
public:
	void execute(PriorityQueueList<int, int>* = nullptr);
};

class RandomTest : public Test<int, int>
{
public:
	void execute(PriorityQueueList<int, int>* = nullptr);
};

template<typename K, typename T>
inline PriorityQueueList<K, T>::PriorityQueueList() :
	priority_queue_list_(new std::list<PriorityQueueWrapper<K, T>*>()),
	identifier_list_(new std::vector<int>())
{
	//priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new BinaryHeap<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new PairingHeapTwoPass<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new PairingHeapMultiPass<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new RankPairingHeap<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new FibonacciHeap<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new BinomialHeapSinglePass<K, T>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new BinomialHeapMultiPass<K, T>()));
}

template<typename K, typename T>
inline PriorityQueueList<K, T>::~PriorityQueueList()
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		delete item;
	}
	delete this->priority_queue_list_;
	delete this->identifier_list_;
	this->priority_queue_list_ = nullptr;
	this->identifier_list_ = nullptr;
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::reset()
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		item->reset();
	}
	this->identifier_list_->clear();
}

template<typename K, typename T>
inline int PriorityQueueList<K, T>::get_random_identifier()
{
	return (*this->identifier_list_)[rand() % this->identifier_list_->size()];
}

template<typename K, typename T>
inline int PriorityQueueList<K, T>::size()
{
	return this->identifier_list_->size();
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::push(const int identifier, const K& priority, const T& data)
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		item->push(identifier, priority, data);
	}
	this->identifier_list_->push_back(identifier);
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::pop()
{
	int identifier;
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		identifier = item->pop();
	}
	std::vector<int>::iterator index_iterator = std::find(this->identifier_list_->begin(), this->identifier_list_->end(), identifier);
	if (index_iterator != this->identifier_list_->end())
	{
		this->identifier_list_->erase(index_iterator);
	}
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::find_min()
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		item->find_min();
	}
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::change_priority(const int identifier, const K& priority)
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		item->change_priority(identifier, priority);
	}
}

template<typename K, typename T>
inline void PriorityQueueList<K, T>::merge(const int size)
{
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		item->merge(0);
	}
}


inline void RandomTest::execute(PriorityQueueList<int, int>* pq_list)
{
	for (int i = 0; i < 10000000; i++)
	{
		int number = rand() % 100;
		if (number < 20)
		{
			pq_list->push(rand() % UINT32_MAX, rand() % UINT32_MAX, rand() % UINT32_MAX);
		}
		else if (number < 40 && pq_list->size() > 0)
		{
			pq_list->pop();
		}
		else if (number < 60 && pq_list->size() > 0)
		{
			pq_list->find_min();
		}
		else if (number < 80 && pq_list->size() > 0)
		{
			pq_list->change_priority(pq_list->get_random_identifier(), rand() % UINT32_MAX);
		}
	}
}
