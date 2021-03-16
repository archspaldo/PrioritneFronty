#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

template <typename K, typename T>
class PriorityQueue
{
protected:
	PriorityQueue();
public:
	virtual ~PriorityQueue();
	virtual size_t size() const = 0;
	virtual PriorityQueueItem<K, T>* push(const K& priority, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& find_min() = 0;
	virtual void merge(PriorityQueue<K, T>* other_heap) = 0;
	virtual void change_priority(PriorityQueueItem<K, T>* node, const K& priority) = 0;
};

template <typename K, typename T>
class LazyBinomialHeap : public PriorityQueue<K, T>
{
protected:
	BinaryTreeItem<K, T>* root_;
	size_t size_;
	template <class I> PriorityQueueItem<K, T>* push(const K& priority, const T& data);
	virtual void consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root = true) = 0;
	LazyBinomialHeap();
public:
	virtual ~LazyBinomialHeap();
	size_t size() const;
	virtual PriorityQueueItem<K, T>* push(const K& priority, const T& data) = 0;
	T pop();
	T& find_min();
	virtual void merge(PriorityQueue<K, T>* other_heap);
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority) = 0;
};

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::LazyBinomialHeap() :
	root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::~LazyBinomialHeap()
{
	delete this->root_;
	this->root_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t LazyBinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
template<class I>
inline PriorityQueueItem<K, T>* LazyBinomialHeap<K, T>::push(const K& priority, const T& data)
{
	BinaryTreeItem<K, T>* new_node = new I(priority, data);
	if (this->root_)
	{
		this->root_->add_root_item(new_node);
		if (priority < this->root_->priority())
		{
			this->root_ = new_node;
		}
	}
	else
	{
		this->root_ = new_node;
	}
	this->size_++;
	return new_node;
}

template<typename K, typename T>
inline T LazyBinomialHeap<K, T>::pop()
{
	if (this->root_)
	{
		BinaryTreeItem<K, T>* root = this->root_;
		this->consolidate_with(root->left_son());
		root->left_son(nullptr);
		this->size_--;
		T data = root->data();
		delete root;
		return data;
	}
	throw new std::range_error("LazyBinomialHeap<K, T>::pop(): Priority queue is empty!");
}

template<typename K, typename T>
inline T& LazyBinomialHeap<K, T>::find_min()
{
	return this->root_->data();
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	if (other_heap)
	{
		LazyBinomialHeap<K, T>* heap = (LazyBinomialHeap<K, T>*)other_heap;
		if (this->root_)
		{
			this->root_->add_root_item(heap->root_);
			if (heap->root_ && heap->root_->priority() < this->root_->priority())
			{
				this->root_ = heap->root_;
			}
		}
		else
		{
			this->root_ = heap->root_;
		}
		this->size_ += heap->size_;
		heap->root_ = nullptr;
		delete heap;
	}
}

template<typename K, typename T>
inline PriorityQueue<K, T>::PriorityQueue()
{
}

template<typename K, typename T>
inline PriorityQueue<K, T>::~PriorityQueue()
{
}
