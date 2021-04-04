#pragma once
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"

template <typename K, typename T>
class PriorityQueueWrapper
{
private:
	std::unordered_map<int, PriorityQueueItem<K, T>*>* identifier_map_;
	PriorityQueue<K, T>* priority_queue_;
public:
	PriorityQueueWrapper(PriorityQueue<K, T>* priority_queue);
	~PriorityQueueWrapper();
	void reset();
	void push(const int identifier, const K& priority, const T& data);
	int pop();
	void find_min();
	void change_priority(const int identifier, const K& priority);
	void merge(const int size);
};

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

template<typename K, typename T>
inline PriorityQueueWrapper<K, T>::PriorityQueueWrapper(PriorityQueue<K, T>* priority_queue) :
	identifier_map_(new std::unordered_map<int, PriorityQueueItem<K, T>*>()),
	priority_queue_(priority_queue)
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
	this->priority_queue_->clear();
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::push(const int identifier, const K& priority, const T& data)
{
	PriorityQueueItem<K, T>* priority_queue_item;
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point end_time, begin_time = std::chrono::high_resolution_clock::now();
	this->priority_queue_->push(identifier, priority, data, priority_queue_item);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
	(*this->identifier_map_)[identifier] = priority_queue_item;
}

template<typename K, typename T>
inline int PriorityQueueWrapper<K, T>::pop()
{
	int identifier;
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->pop(identifier);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
	this->identifier_map_->erase(identifier);
	return identifier;
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::find_min()
{
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->find_min();
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::change_priority(const int identifier, const K& priority)
{
	PriorityQueueItem<K, T>* priority_queue_item = (*this->identifier_map_)[identifier];
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->change_priority(priority_queue_item, priority);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
}

template<typename K, typename T>
inline void PriorityQueueWrapper<K, T>::merge(const int size)
{

}

template<typename K, typename T>
inline PriorityQueueList<K, T>::PriorityQueueList() :
	priority_queue_list_(new std::list<PriorityQueueWrapper<K, T>*>()),
	identifier_list_(new std::vector<int>())
{
	priority_queue_list_->push_back(new PriorityQueueWrapper<K, T>(new BinaryHeap<K, T>()));
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
	std::queue<int> queue;
	int identifier;
	for (PriorityQueueWrapper<K, T>* item : *this->priority_queue_list_)
	{
		identifier = item->pop();
		queue.push(identifier);
	}
	while (!queue.empty())
	{
		std::vector<int>::iterator index_iterator = std::find(this->identifier_list_->begin(), this->identifier_list_->end(), queue.front());
		if (index_iterator != this->identifier_list_->end())
		{
			this->identifier_list_->erase(index_iterator);
		}
		queue.pop();
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
