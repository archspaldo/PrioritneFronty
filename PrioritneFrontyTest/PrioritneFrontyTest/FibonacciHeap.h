#pragma once
#include "LazyBinomialQueue.h"

template <typename K, typename T>
class FibonacciHeap : public LazyBinomialHeap<K, T>
{
private:
	void cut(FibonacciHeapItem<K, T>* node);
	void cascading_cut(FibonacciHeapItem<K, T>* node);
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
	void priority_was_increased(PriorityQueueItem<K, T>* node) override;
	void priority_was_decreased(PriorityQueueItem<K, T>* node) override;
public:
	FibonacciHeap();
	~FibonacciHeap();
	void push(const int identifier, const K& priority, const T& data, DataItem<K, T>*& = nullptr) override;
};

template<typename K, typename T>
inline FibonacciHeap<K, T>::FibonacciHeap()
{
}

template<typename K, typename T>
inline FibonacciHeap<K, T>::~FibonacciHeap()
{
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::push(const int identifier, const K& priority, const T& data, DataItem<K, T>*& data_item)
{
	FibonacciHeapItem<K, T>* new_node = new FibonacciHeapItem<K, T>(identifier, priority, data);
	data_item =  this->LazyBinomialHeap<K, T>::push(new_node);
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::cut(FibonacciHeapItem<K, T>* node)
{
	node->cut();
	node->parent()->degree()--;
	this->add_root_item(node);
	node->flag() = false;
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::cascading_cut(FibonacciHeapItem<K, T>* node)
{
	if (node->parent())
	{
		if (node->flag())
		{
			this->cut(node);
			this->cascading_cut((FibonacciHeapItem<K, T>*)node->ordered_ancestor());
		}
		else
		{
			node->flag() = true;
		}
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::consolidate_root(BinaryTreeItem<K, T>* node)
{
	this->consolidate_root_using_multipass(node, (int)(log(this->size_) * 2.1) + 2);
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::priority_was_increased(PriorityQueueItem<K, T>* node)
{
	FibonacciHeapItem<K, T>* casted_node = (FibonacciHeapItem<K, T>*)node;
	if (casted_node->ordered_ancestor() && casted_node->priority() < casted_node->ordered_ancestor()->priority())
	{
		this->cut(casted_node);
		this->cascading_cut((FibonacciHeapItem<K, T>*)casted_node->ordered_ancestor());
	}
	if (casted_node->priority() < this->root_->priority())
	{
		this->root_ = casted_node;
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	BinaryTreeItem<K, T>* casted_node = (BinaryTreeItem<K, T>*)node;
	for (BinaryTreeItem<K, T>* node_ptr = casted_node->left_son(), * node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (node_ptr->priority() < node->priority())
		{
			node_ptr->cut();
			this->cut((FibonacciHeapItem<K, T>*)node_ptr);
			this->cascading_cut((FibonacciHeapItem<K, T>*)((FibonacciHeapItem<K, T>*)node_ptr)->ordered_ancestor());
		}
	}
}
