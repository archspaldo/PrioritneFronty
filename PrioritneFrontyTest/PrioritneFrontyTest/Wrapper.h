#pragma once
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"

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
	void find_min();
	void change_priority(const int identifier, const Priority& priority);
	void merge(const int size);
};

template <typename Priority, typename Data>
class PriorityQueueList
{
	std::list<PriorityQueueWrapper<Priority, Data>*>* priority_queue_list_;
	std::vector<int>* identifier_list_;
public:
	PriorityQueueList();
	~PriorityQueueList();
	void reset();
	int get_random_identifier();
	int size();
	void push(const int identifier, const Priority& priority, const Data& data);
	void pop();
	void find_min();
	void change_priority(const int identifier, const Priority& priority);
	void merge(const int size);
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
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point end_time, begin_time = std::chrono::high_resolution_clock::now();
	this->priority_queue_->push(identifier, priority, data, priority_queue_item);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
	(*this->identifier_map_)[identifier] = priority_queue_item;
}

template<typename Priority, typename Data>
inline int PriorityQueueWrapper<Priority, Data>::pop()
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

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::find_min()
{
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->find_min();
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
}

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::change_priority(const int identifier, const Priority& priority)
{
	PriorityQueueItem<Priority, Data>* priority_queue_item = (*this->identifier_map_)[identifier];
	std::chrono::duration<long double> time_difference;
	std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now(), end_time;
	this->priority_queue_->change_priority(priority_queue_item, priority);
	end_time = std::chrono::high_resolution_clock::now();
	time_difference = end_time - begin_time;
}

template<typename Priority, typename Data>
inline void PriorityQueueWrapper<Priority, Data>::merge(const int size)
{
}

template<typename Priority, typename Data>
inline PriorityQueueList<Priority, Data>::PriorityQueueList() :
	priority_queue_list_(new std::list<PriorityQueueWrapper<Priority, Data>*>()),
	identifier_list_(new std::vector<int>())
{
	/*priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new BinaryHeap<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new PairingHeapTwoPass<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new PairingHeapMultiPass<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new RankPairingHeap<Priority, Data>()));
	priority_queue_list_->push_back(new PriorityQueueWrapper<Priority, Data>(new FibonacciHeap<Priority, Data>()));*/
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
	delete this->identifier_list_;
	this->priority_queue_list_ = nullptr;
	this->identifier_list_ = nullptr;
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::reset()
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->reset();
	}
	this->identifier_list_->clear();
}

template<typename Priority, typename Data>
inline int PriorityQueueList<Priority, Data>::get_random_identifier()
{
	return (*this->identifier_list_)[rand() % this->identifier_list_->size()];
}

template<typename Priority, typename Data>
inline int PriorityQueueList<Priority, Data>::size()
{
	return this->identifier_list_->size();
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data)
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->push(identifier, priority, data);
	}
	this->identifier_list_->push_back(identifier);
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::pop()
{
	int identifier = -1, last;
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		last = identifier;
		identifier = item->pop();
		if (last != -1 && last != identifier)
		{
			std::cout << "error on: " << identifier <<"\n";
		}
	}
	std::vector<int>::iterator index_iterator = std::find(this->identifier_list_->begin(), this->identifier_list_->end(), identifier);
	if (index_iterator != this->identifier_list_->end())
	{
		this->identifier_list_->erase(index_iterator);
	}
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::find_min()
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->find_min();
	}
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::change_priority(const int identifier, const Priority& priority)
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->change_priority(identifier, priority);
	}
}

template<typename Priority, typename Data>
inline void PriorityQueueList<Priority, Data>::merge(const int size)
{
	for (PriorityQueueWrapper<Priority, Data>* item : *this->priority_queue_list_)
	{
		item->merge(0);
	}
}
