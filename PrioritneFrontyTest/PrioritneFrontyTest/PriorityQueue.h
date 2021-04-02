#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

template <typename K, typename T>
class PriorityQueue
{
protected:
	PriorityQueue();
	virtual void priority_was_increased(PriorityQueueItem<K, T>* node) = 0;
	virtual void priority_was_decreased(PriorityQueueItem<K, T>* node) = 0;
public:
	virtual ~PriorityQueue();
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& node) = 0;
	virtual T pop(int& identifier) = 0;
	virtual T& find_min() = 0;
	virtual void merge(PriorityQueue<K, T>* other_heap) = 0;
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority);
};

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

template<typename K, typename T>
inline PriorityQueue<K, T>::PriorityQueue()
{
}

template<typename K, typename T>
inline PriorityQueue<K, T>::~PriorityQueue()
{
}

template<typename K, typename T>
inline void PriorityQueue<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& priority)
{
	K old_priority = node->priority();
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
