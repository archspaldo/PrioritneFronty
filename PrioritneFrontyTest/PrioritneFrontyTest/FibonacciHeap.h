#pragma once
#include "LazyBinomialQueue.h"

template <typename Priority, typename Data>
class FibonacciHeap : public LazyBinomialHeap<Priority, Data>
{
private:
	void cut(FibonacciHeapItem<Priority, Data>* node);
	void cascading_cut(FibonacciHeapItem<Priority, Data>* node);
protected:
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	FibonacciHeap();
	~FibonacciHeap();
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& = nullptr) override;
};

template<typename Priority, typename Data>
inline FibonacciHeap<Priority, Data>::FibonacciHeap()
{
}

template<typename Priority, typename Data>
inline FibonacciHeap<Priority, Data>::~FibonacciHeap()
{
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	data_item = this->LazyBinomialHeap<Priority, Data>::push(new FibonacciHeapItem<Priority, Data>(identifier, priority, data));
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::cut(FibonacciHeapItem<Priority, Data>* node)
{
	node->ordered_ancestor()->degree()--;
	node->cut();
	this->add_root_item(node);
	node->flag() = false;
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::cascading_cut(FibonacciHeapItem<Priority, Data>* node)
{
	if (node->ordered_ancestor())
	{
		if (node->flag())
		{
			FibonacciHeapItem<Priority, Data>* ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)node->ordered_ancestor();
			this->cut(node);
			this->cascading_cut(ordered_ancestor);
		}
		else
		{
			node->flag() = true;
		}
	}
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->consolidate_root_using_multipass(node, (int)(log(this->size_) * 2.1) + 2);
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	FibonacciHeapItem<Priority, Data>* casted_node = (FibonacciHeapItem<Priority, Data>*)node;
	if (casted_node->ordered_ancestor() && *casted_node < *casted_node->ordered_ancestor())
	{
		FibonacciHeapItem<Priority, Data>* ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)((FibonacciHeapItem<Priority, Data>*)node)->ordered_ancestor();
		this->cut(casted_node);
		this->cascading_cut(ordered_ancestor);
	}
	if (*casted_node < *this->root_)
	{
		this->root_ = casted_node;
	}
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* casted_node = (BinaryTreeItem<Priority, Data>*)node;
	FibonacciHeapItem<Priority, Data>* ordered_ancestor;
	if (this->root_ == casted_node)
	{
		BinaryTreeItem<Priority, Data>* new_root = this->root_;
		for (BinaryTreeItem<Priority, Data>* node_ptr = this->root_->right_son(); node_ptr != this->root_; node_ptr = node_ptr->right_son())
		{
			if (*node_ptr < *new_root)
			{
				new_root = node_ptr;
			}
		}
		this->root_ = new_root;
	}
	for (BinaryTreeItem<Priority, Data>* node_ptr = casted_node->left_son(), *node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (*node_ptr < *node)
		{
			ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)((FibonacciHeapItem<Priority, Data>*)node_ptr)->ordered_ancestor();
			this->cut((FibonacciHeapItem<Priority, Data>*)node_ptr);
			this->cascading_cut(ordered_ancestor);
		}
	}
}
