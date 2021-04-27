#pragma once
#include "PriorityQueue.h"
#include <vector>

template <typename Priority, typename Data>
class BinaryHeap : public PriorityQueue<Priority, Data>
{
private:
	std::vector<ArrayItem<Priority, Data>*>* list_;

	int leftSon(const int index);
	int righSon(const int index);
	int parent(const int index);
	int greater_son(const int index);
	void heapify_up(const int index);
	void heapify_down(const int index);
	static void swap(ArrayItem<Priority, Data>*& item_1, ArrayItem<Priority, Data>*& item_2);
protected:
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	BinaryHeap();
	~BinaryHeap();
	void clear() override;
	size_t size() const override;
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	Data pop(int& identifier) override;
	Data& find_min() override;
	void merge(PriorityQueue<Priority, Data>* other_heap) override;

	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline BinaryHeap<Priority, Data>::BinaryHeap() :
	PriorityQueue<Priority, Data>(),
	list_(new std::vector<ArrayItem<Priority, Data>*>())
{
}

template<typename Priority, typename Data>
inline BinaryHeap<Priority, Data>::~BinaryHeap()
{
	this->clear();
	delete this->list_;
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::clear()
{
	for (PriorityQueueItem<Priority, Data>* item : *this->list_)
	{
		delete item;
	}
	this->list_->clear();
}

template<typename Priority, typename Data>
inline size_t BinaryHeap<Priority, Data>::size() const
{
	return this->list_->size();
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	ArrayItem<Priority, Data>* new_node = new ArrayItem<Priority, Data>(identifier, priority, data, this->size());
	this->list_->push_back(new_node);
	this->heapify_up(this->size() - 1);
	data_item = new_node;
}

template<typename Priority, typename Data>
inline Data BinaryHeap<Priority, Data>::pop(int& identifier)
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<Priority, Data>::pop(): Zoznam je prazdny");
	}
	if (this->size() > 1)
	{
		swap((*this->list_)[0], (*this->list_)[this->size() - 1]);
	}

	PriorityQueueItem<Priority, Data>* item = this->list_->back();
	this->list_->pop_back(); 
	this->heapify_down(0);
	Data data = item->data();
	identifier = item->identifier();
	delete item;
	return data;
}

template<typename Priority, typename Data>
inline Data& BinaryHeap<Priority, Data>::find_min()
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<Priority, Data>::peek(): Zoznam je prazdny");
	}
	return (*this->list_)[0]->data();
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	BinaryHeap<Priority, Data>* heap = (BinaryHeap<Priority, Data>*)other_heap;
	this->list_->insert(this->list_->end(), heap->list_->begin(), heap->list_->end());
	if (this->size() > 1)
	{
		int greater_son;
		for (int i = (this->size() - 1) / 2; i <= 0; i--)
		{
			this->heapify_down(i);
		}
	}
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::leftSon(const int index)
{
	return 2 * index + 1;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::righSon(const int index)
{
	return 2 * index + 2;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::parent(const int index)
{
	return (index - 1) / 2;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::greater_son(const int index)
{
	PriorityQueueItem<Priority, Data>* lChild = this->leftSon(index) < this->size() ? (*this->list_)[this->leftSon(index)] : nullptr;
	PriorityQueueItem<Priority, Data>* rChild = this->righSon(index) < this->size() ? (*this->list_)[this->righSon(index)] : nullptr;
	if (lChild && rChild)
	{
		return *lChild < *rChild ? this->leftSon(index) : this->righSon(index);
	}
	return this->leftSon(index);
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::heapify_up(const int index)
{
	for (int i = index, parent = this->parent(i); i > 0 && *(*this->list_)[i] < *(*this->list_)[parent]; i = parent, parent = this->parent(i))
	{
		swap((*this->list_)[parent], (*this->list_)[i]);
	}
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::heapify_down(const int index)
{
	for (int i = index, child = this->greater_son(i); child < this->size() && *(*this->list_)[child] < *(*this->list_)[i]; i = child, child = this->greater_son(i))
	{
		swap((*this->list_)[child], (*this->list_)[i]);
	}
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::swap(ArrayItem<Priority, Data>*& item_1, ArrayItem<Priority, Data>*& item_2)
{
	std::swap(item_1, item_2);
	std::swap(item_1->index(), item_2->index());
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	int index = ((ArrayItem<Priority, Data>*)node)->index();
	this->heapify_up(index);
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	int index = ((ArrayItem<Priority, Data>*)node)->index();
	this->heapify_down(index);
}
