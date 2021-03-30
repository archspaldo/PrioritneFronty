#pragma once
#include "PriorityQueue.h"
#include <vector>

template <typename K, typename T>
class BinaryHeap : public PriorityQueue<K, T>
{
public:
	BinaryHeap();
	~BinaryHeap();
	size_t size() const override;
	DataItem<K, T>* push(const K& priority, const T& data) override;
	T pop() override;
	T& find_min() override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void change_priority(DataItem<K, T>* node, const K& priority) override;
private:
	std::vector<PriorityQueueItem<K, T>*>* list_;
	int leftSon(const int index);
	int righSon(const int index);
	int parent(const int index);
	int greaterSon(const int index);
	void heapifyUp(const int index);
	void heapifyDown(const int index);
};



template<typename K, typename T>
inline BinaryHeap<K, T>::BinaryHeap() :
	PriorityQueue<K, T>(),
	list_(new std::vector<PriorityQueueItem<K, T>*>())
{
}

template<typename K, typename T>
inline BinaryHeap<K, T>::~BinaryHeap()
{
	for (PriorityQueueItem<K, T>* item : *this->list_)
	{
		delete item;
	}
	this->list_->clear();
	delete this->list_;
}


template<typename K, typename T>
inline size_t BinaryHeap<K, T>::size() const
{
	return this->list_->size();
}

template<typename K, typename T>
inline DataItem<K, T>* BinaryHeap<K, T>::push(const K& priority, const T& data)
{
	PriorityQueueItem<K, T>* node = new PriorityQueueItem<K, T>(priority, data);
	this->list_->push_back(node);
	this->heapifyUp(this->size() - 1);
	return node->data_item();
}

template<typename K, typename T>
inline T BinaryHeap<K, T>::pop()
{
	if (this->list_->empty())
	{
		throw new std::logic_error("BinaryHeap<K, T>::pop(): Zoznam je prazdny");
	}
	if (this->size() > 1)
	{
		std::swap((*this->list_)[0], (*this->list_)[this->size() - 1]);
	}
	PriorityQueueItem<K, T>* item = this->list_->back();
	this->list_->pop_back();
	this->heapifyDown(0);
	T data = item->data();
	delete item;
	return data;
}

template<typename K, typename T>
inline T& BinaryHeap<K, T>::find_min()
{
	if (this->list_->empty())
	{
		throw new std::logic_error("BinaryHeap<K, T>::peek(): Zoznam je prazdny");
	}
	return (*this->list_)[0]->data();
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	BinaryHeap<K, T>* heap = (BinaryHeap<K, T>*)other_heap;
	this->list_->insert(this->list_->end(), heap->list_->begin(), heap->list_->end());
	if (this->size() > 1)
	{
		int greater_son;
		for (int i = (this->size() - 1) / 2; i <= 0; i--)
		{
			greater_son = this->greaterSon(i);
			if (greater_son < this->size() && (*this->list_)[greater_son]->priority() < (*this->list_)[i]->priority())
			{
				std::swap((*this->list_)[greater_son], (*this->list_)[i]);
			}
		}
	}
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::change_priority(DataItem<K, T>* node, const K& priority)
{
	size_t old_priority = node->priority();
	PriorityQueueItem<K, T>* item = node->tree_item();
	size_t index = &item - &(*this->list_)[0];
	if (priority < old_priority)
	{
		this->heapifyUp(index);
	}
	if (priority > old_priority)
	{
		this->heapifyDown(index);
	}
}

template<typename K, typename T>
inline int BinaryHeap<K, T>::leftSon(const int index)
{
	return 2 * index + 1;
}

template<typename K, typename T>
inline int BinaryHeap<K, T>::righSon(const int index)
{
	return 2 * index + 2;
}

template<typename K, typename T>
inline int BinaryHeap<K, T>::parent(const int index)
{
	return (index - 1) / 2;
}

template<typename K, typename T>
inline int BinaryHeap<K, T>::greaterSon(const int index)
{
	PriorityQueueItem<K, T>* lChild = this->leftSon(index) < this->size() ? (*this->list_)[this->leftSon(index)] : nullptr;
	PriorityQueueItem<K, T>* rChild = this->righSon(index) < this->size() ? (*this->list_)[this->righSon(index)] : nullptr;
	if (lChild && rChild)
	{
		return lChild->priority() < rChild->priority() ? this->leftSon(index) : this->righSon(index);
	}
	return this->leftSon(index);
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::heapifyUp(const int index)
{
	for (int i = index, parent = this->parent(i); i > 0 && (*this->list_)[parent]->priority() > (*this->list_)[i]->priority(); i = parent, parent = this->parent(i))
	{
		std::swap((*this->list_)[parent], (*this->list_)[i]);
	}
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::heapifyDown(const int index)
{
	for (int i = index, child = this->greaterSon(i); child < this->size() && (*this->list_)[child]->priority() < (*this->list_)[i]->priority(); i = child, child = this->greaterSon(i))
	{
		std::swap((*this->list_)[child], (*this->list_)[i]);
	}
}

