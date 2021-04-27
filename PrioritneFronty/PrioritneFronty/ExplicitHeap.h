#pragma once
#include "PriorityQueue.h"

template <typename Priority, typename Data>
class ExplicitPriorityQueue : public PriorityQueue<Priority, Data>
{
protected:
	BinaryTreeItem<Priority, Data>* root_;
	size_t size_;
	ExplicitPriorityQueue<Priority, Data>();
public:
	virtual ~ExplicitPriorityQueue();
	size_t size() const override;
	Data& find_min() override;
};

template<typename Priority, typename Data>
inline ExplicitPriorityQueue<Priority, Data>::ExplicitPriorityQueue() :
	PriorityQueue<Priority, Data>(), root_(nullptr), size_(0)
{
}

template<typename Priority, typename Data>
inline ExplicitPriorityQueue<Priority, Data>::~ExplicitPriorityQueue()
{
}

template<typename Priority, typename Data>
inline size_t ExplicitPriorityQueue<Priority, Data>::size() const
{
	return this->size_;
}

template<typename Priority, typename Data>
inline Data& ExplicitPriorityQueue<Priority, Data>::find_min()
{
	return this->root_->data();
}