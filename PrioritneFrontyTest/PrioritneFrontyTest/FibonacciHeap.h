#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class FibonacciHeap : public LazyBinomialHeap<K, T>
{
protected:
	void consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root = true) override;
	void priority_was_increased(BinaryTreeItem<K, T>* node);
	void priority_was_decreased(BinaryTreeItem<K, T>* node);
public:
	FibonacciHeap();
	~FibonacciHeap();
	PriorityQueueItem<K, T>* push(const K& priority, const T& data);
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority);
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
inline PriorityQueueItem<K, T>* FibonacciHeap<K, T>::push(const K& priority, const T& data)
{
	return this->LazyBinomialHeap<K, T>::push<FibonacciHeapItem<K, T>>(priority, data);
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& priority)
{
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root)
{
	const int degree = (int)(log(this->size_) * 2.1) + 2;
	size_t node_degree;
	std::vector<BinaryTreeItem<K, T>*> node_list(degree);
	BinaryTreeItem<K, T>* node_ptr, * node_next_ptr;
	if (node)
	{
		for (node_ptr = node, node_next_ptr = node_ptr->right_son(); node_ptr; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_degree = node_ptr->degree();
			node_ptr->cut();
			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
		}
	}

	node_ptr = this->root_->right_son();
	this->root_->right_son(nullptr);
	for (node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr && node_ptr != this->root_; node_ptr = node_next_ptr, node_next_ptr = node_ptr->right_son())
	{
		node_degree = node_ptr->degree();
		node_ptr->right_son(nullptr);
		while (node_list[node_degree])
		{
			node_ptr = node_ptr->merge(node_list[node_degree]);
			node_list[node_degree++] = nullptr;
		}
		node_list[node_degree] = node_ptr;
	}

	if (!skip_root)
	{
		node_ptr = this->root_;
		node_degree = node_ptr->degree();
		while (node_list[node_degree])
		{
			node_ptr = node_ptr->merge(node_list[node_degree]);
			node_list[node_degree++] = nullptr;
		}
		node_list[node_degree] = node_ptr;
	}

	this->root_ = nullptr;
	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			if (this->root_)
			{
				this->root_->add_root_item(node);
				if (node->priority() < this->root_->priority())
				{
					this->root_ = node;
				}
			}
			else
			{
				this->root_ = node;
			}
		}
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::priority_was_increased(BinaryTreeItem<K, T>* node)
{
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
}
