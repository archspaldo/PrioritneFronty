#pragma once
#include "PriorityQueue.h"
#include <vector>

template <typename K, typename T>
class BinaryHeap : public PriorityQueue<K, T>
{
private:
	class ArrayItem;

	std::vector<ArrayItem*>* list_;

	int leftSon(const int index);
	int righSon(const int index);
	int parent(const int index);
	int greaterSon(const int index);
	void heapifyUp(const int index);
	void heapifyDown(const int index);

	static void swap(ArrayItem*& item_1, ArrayItem*& item_2);
protected:
	void priority_was_increased(PriorityQueueItem<K, T>* node) override;
	void priority_was_decreased(PriorityQueueItem<K, T>* node) override;
public:
	BinaryHeap();
	~BinaryHeap();
	void clear() override;
	size_t size() const override;
	void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item) override;
	T pop(int& identifier) override;
	T& find_min() override;
	void merge(PriorityQueue<K, T>* other_heap) override;
};

template<typename K, typename T>
inline BinaryHeap<K, T>::BinaryHeap() :
	PriorityQueue<K, T>(),
	list_(new std::vector<ArrayItem*>())
{
}

template<typename K, typename T>
inline BinaryHeap<K, T>::~BinaryHeap()
{
	this->clear();
	delete this->list_;
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::clear()
{
	for (PriorityQueueItem<K, T>* item : *this->list_)
	{
		delete item;
	}
	this->list_->clear();
}


template<typename K, typename T>
inline size_t BinaryHeap<K, T>::size() const
{
	return this->list_->size();
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item)
{
	ArrayItem* new_node = new ArrayItem(identifier, priority, data, this->size());
	this->list_->push_back(new_node);
	this->heapifyUp(this->size() - 1);
	data_item = new_node;
}

template<typename K, typename T>
inline T BinaryHeap<K, T>::pop(int& identifier)
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<K, T>::pop(): Zoznam je prazdny");
	}
	if (this->size() > 1)
	{
		swap((*this->list_)[0], (*this->list_)[this->size() - 1]);
	}
	PriorityQueueItem<K, T>* item = this->list_->back();
	this->list_->pop_back();
	this->heapifyDown(0);
	T data = item->data();
	identifier = item->identifier();
	delete item;
	return data;
}

template<typename K, typename T>
inline T& BinaryHeap<K, T>::find_min()
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<K, T>::peek(): Zoznam je prazdny");
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
				swap((*this->list_)[greater_son], (*this->list_)[i]);
			}
		}
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
		swap((*this->list_)[parent], (*this->list_)[i]);
	}
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::heapifyDown(const int index)
{
	for (int i = index, child = this->greaterSon(i); child < this->size() && (*this->list_)[child]->priority() < (*this->list_)[i]->priority(); i = child, child = this->greaterSon(i))
	{
		swap((*this->list_)[child], (*this->list_)[i]);
	}
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::swap(ArrayItem*& item_1, ArrayItem*& item_2)
{
	std::swap(item_1, item_2);
	std::swap(item_1->index(), item_2->index());
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::priority_was_increased(PriorityQueueItem<K, T>* node)
{
	int index = dynamic_cast<ArrayItem*>(node)->index();
	this->heapifyUp(index);
}

template<typename K, typename T>
inline void BinaryHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	int index = dynamic_cast<ArrayItem*>(node)->index();
	this->heapifyDown(index);
}

template <typename K, typename T>
class BinaryHeap<K, T>::ArrayItem : public PriorityQueueItem<K, T>
{
protected:
	int index_;
public:
	ArrayItem(const int identifier, const K& priority, const T& data, const int index);

	int& index();
};

template <typename K, typename T>
inline BinaryHeap<K, T>::ArrayItem::ArrayItem(const int identifier, const K& priority, const T& data, const int index) :
	PriorityQueueItem<K, T>(identifier, priority, data), index_(index)
{
}

template <typename K, typename T>
inline int& BinaryHeap<K, T>::ArrayItem::index()
{
	return this->index_;
}
