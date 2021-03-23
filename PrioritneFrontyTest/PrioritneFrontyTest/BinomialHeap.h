#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class BinomialHeap : public LazyBinomialHeap<K, T>
{
protected:
	void priority_was_increased(BinaryTreeItem<K, T>* node) override;
	void priority_was_decreased(BinaryTreeItem<K, T>* node) override;
	BinomialHeap();
public:
	~BinomialHeap();
	PriorityQueueItem<K, T>* push(const K& priority, const T& data) override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority) {};

};

template <typename K, typename T>
class BinomialHeapMultiPass : public BinomialHeap<K, T>
{
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root = true) override;
public:
	BinomialHeapMultiPass();
	~BinomialHeapMultiPass();
};

template <typename K, typename T>
class BinomialHeapSinglePass : public BinomialHeap<K, T>
{
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root = true) override;
public:
	BinomialHeapSinglePass();
	~BinomialHeapSinglePass();
};

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeap() :
	LazyBinomialHeap<K, T>()
{
}

template<typename K, typename T>
inline BinomialHeap<K, T>::~BinomialHeap()
{
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* BinomialHeap<K, T>::push(const K& priority, const T& data)
{
	PriorityQueueItem<K, T>* new_node = this->LazyBinomialHeap<K, T>::push<BinaryTreeItemWithAncestor<K, T>>(priority, data);
	this->consolidate_root(nullptr, false);
	return new_node;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	this->LazyBinomialHeap<K, T>::merge(other_heap);
	this->consolidate_root(nullptr, false);
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::priority_was_increased(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItemWithAncestor<K, T>* casted_node = (BinaryTreeItemWithAncestor<K, T>*)node;
	while (casted_node->ordered_ancestor() && casted_node->priority() < casted_node->ordered_ancestor()->priority())
	{
		casted_node->swap_with_ordered_ancestor();
	}
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItemWithAncestor<K, T>* casted_node = (BinaryTreeItemWithAncestor<K, T>*)node, * minimal_son = casted_node->find_minimal_left_son();
	while (minimal_son && minimal_son->priority() < casted_node->priority())
	{
		minimal_son->swap_with_ordered_ancestor();
		minimal_son = casted_node->find_minimal_left_son();
	}
}

template<typename K, typename T>
inline void BinomialHeapMultiPass<K, T>::consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root)
{
	const int degree = (int)(log2(this->size_)) + 2;
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
inline BinomialHeapMultiPass<K, T>::BinomialHeapMultiPass() :
	BinomialHeap<K, T>()
{
}

template<typename K, typename T>
inline BinomialHeapMultiPass<K, T>::~BinomialHeapMultiPass()
{
}

template<typename K, typename T>
inline void BinomialHeapSinglePass<K, T>::consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root)
{
	const int degree = (int)(log2(this->size_)) + 2;
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
inline BinomialHeapSinglePass<K, T>::BinomialHeapSinglePass() :
	BinomialHeap<K, T>()
{
	
}

template<typename K, typename T>
inline BinomialHeapSinglePass<K, T>::~BinomialHeapSinglePass()
{
}
