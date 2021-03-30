#pragma once
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
class PriorityQueueWrapper
{
private:
	std::unordered_map<const int, const DataItem<K, T>*>* identifier_map_;
	PriorityQueue<K, T>* priority_queue_;
public:
	template <class P> PriorityQueueWrapper();
	~PriorityQueueWrapper();
	void reset();
	void push(const int identifier, const K& priority, const T& data);
	void pop();
	void find_min();
	void change_priority(const int identifier, const K& priority);
	void merge(const int size);
};

template <typename K, typename T>
class PriorityQueueList
{
	std::list<PriorityQueueWrapper<K, T>*>* priority_queue_list_;
	std::vector<const int>* identifier_list_;
public:
	PriorityQueueList();
	~PriorityQueueList();
	void reset();
	void push(const int identifier, const K& priority, const T& data);
	void pop();
	void find_min();
	void change_priority(const int identifier, const K& priority);
	void merge(const int size);
};

template <typename K, typename T>
class Test
{
public:
	virtual void execute(PriorityQueueList<K, T>* = nullptr) = 0;
};

template <typename K, typename T>
class PushTest : public Test<K, T>
{
public:
	void execute(PriorityQueueList<K, T>* = nullptr);
};

template <typename K, typename T>
class RandomTest : public Test<K, T>
{
public:
	void execute(PriorityQueueList<K, T>* = nullptr);
};

template<typename K, typename T>
template<class P>
inline PriorityQueueWrapper<K, T>::PriorityQueueWrapper() :
	identifier_map_(new std::unordered_map<const int, const DataItem<K, T>*>()),
	priority_queue_(new P())
{
}

template<typename K, typename T>
inline PriorityQueueWrapper<K, T>::~PriorityQueueWrapper()
{
	delete this->priority_queue_;
	delete this->identifier_map_;
	this->priority_queue_ = nullptr;
	this->identifier_map_ = nullptr;
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::reset()
{
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::push(const int identifier, const K& priority, const T& data)
{
	DataItem<K, T>* data_item;
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	data_item = this->priority_queue_->push(priority, data);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = std::chrono::duration_cast<long double>(end_time - begin_time);

	(*this->identifier_map_)[identifier] = data_item;
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::pop()
{

}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::find_min()
{
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->find_min();
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = std::chrono::duration_cast<long double>(end_time - begin_time);
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::change_priority(const int identifier, const K& priority)
{
	DataItem<K, T>* data_item = (*this->identifier_map_)[identifier];
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	data_item = this->priority_queue_->change_priority(data_item, priority);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = std::chrono::duration_cast<long double>(end_time - begin_time);
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::merge(const int size)
{

}
