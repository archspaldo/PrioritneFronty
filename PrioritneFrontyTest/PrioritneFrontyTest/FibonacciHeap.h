#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class FibonacciHeap : public LazyBinomialHeap<K, T>
{
private:
	void cut(FibonacciHeapItem<K, T>* node);
	void cascading_cut(FibonacciHeapItem<K, T>* node);
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
	void priority_was_increased(BinaryTreeItem<K, T>* node) override;
	void priority_was_decreased(BinaryTreeItem<K, T>* node) override;
public:
	FibonacciHeap();
	~FibonacciHeap();
	DataItem<K, T>* push(const K& priority, const T& data);
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
inline DataItem<K, T>* FibonacciHeap<K, T>::push(const K& priority, const T& data)
{
	FibonacciHeapItem<K, T>* new_node = new FibonacciHeapItem<K, T>(priority, data);
	return this->LazyBinomialHeap<K, T>::push(new_node);
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::cut(FibonacciHeapItem<K, T>* node)
{
	node->cut();
	node->parent()->degree()--;
	this->root_->add_root_item(node);
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

	if (this->root_)
	{
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

		if (this->root_->degree() != -1)
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
inline void FibonacciHeap<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
	for (BinaryTreeItem<K, T>* node_ptr = node->left_son(), * node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
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
