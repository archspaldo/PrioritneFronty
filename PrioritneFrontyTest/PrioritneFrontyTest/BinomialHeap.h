#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class BinomialHeap : public LazyBinomialHeap<K, T>
{
protected:
	void consolidate_with(BinaryTreeItem<K, T>* node) override;
public:
	BinomialHeap();
	~BinomialHeap();

	PriorityQueueItem<K, T>* push(const K& priority, const T& data) override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority) {};
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
	BinaryTreeItemWithAncestor<K, T>* new_node = this->LazyBinomialHeap<K, T>::push<BinaryTreeItemWithAncestor<K, T>*>(priority, data);
	this->consolidate_with();
	return new_node;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	this->LazyBinomialHeap<K, T>::merge(other_heap);
	this->consolidate_with();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::consolidate_with(BinaryTreeItem<K, T>* node)
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
	if (this->root_)
	{
		node_ptr = this->root_->right_son();
		this->root_->right_son(nullptr);
		for (node_ptr ? node_next_ptr = node_ptr->right_son() : nullptr; node_ptr && node_ptr != this->root_; node_ptr = node_next_ptr, )
		{

		}
	}
	if (this->head_)
	{
		for (node_ptr = this->head_, node_next_ptr = node_ptr->right_son(); node_ptr; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
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
	this->head_ = this->root_ = node_ptr = nullptr;
	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			if (this->head_)
			{
				node_ptr->right_son(node);
				node_ptr = node_ptr->right_son();
				if (node->priority() < this->root_->priority())
				{
					this->root_ = node;
				}
			}
			else
			{
				this->head_ = this->root_ = node_ptr = node;
			}
		}
	}
	this->foot_ = node_ptr;
}