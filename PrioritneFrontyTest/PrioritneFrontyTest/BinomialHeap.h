#pragma once
#include "LazyBinomialQueue.h"

template <typename Priority, typename Data>
class BinomialHeap : public LazyBinomialHeap<Priority, Data>
{
protected:
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
	void repair_broken_root_list();
	BinomialHeap();
public:
	~BinomialHeap();
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
};

template <typename Priority, typename Data>
class BinomialHeapMultiPass : public BinomialHeap<Priority, Data>
{
protected:
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	BinomialHeapMultiPass();
	~BinomialHeapMultiPass();

	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template <typename Priority, typename Data>
class BinomialHeapSinglePass : public BinomialHeap<Priority, Data>
{
protected:
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	BinomialHeapSinglePass();
	~BinomialHeapSinglePass();

	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline BinomialHeap<Priority, Data>::BinomialHeap() :
	LazyBinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeap<Priority, Data>::~BinomialHeap()
{
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	DegreeBinaryTreeItem<Priority, Data>* new_item = new DegreeBinaryTreeItem<Priority, Data>(identifier, priority, data);
	this->size_++;
	this->consolidate_root(new_item);
	data_item = new_item;
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	BinomialHeap<Priority, Data>* heap = (BinomialHeap<Priority, Data>*)other_heap;
	this->size_ += heap->size_;
	this->consolidate_root(heap->root_);
	heap->root_ = nullptr;
	delete other_heap;
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	DegreeBinaryTreeItem<Priority, Data>* casted_node = (DegreeBinaryTreeItem<Priority, Data>*)node;
	BinaryTreeItem<Priority, Data>* ordered_ancestor = casted_node->ancestor();
	while (ordered_ancestor && *casted_node < *ordered_ancestor)
	{
		casted_node->swap_with_ancestor_node(ordered_ancestor);
		ordered_ancestor = casted_node->ancestor();
	}
	if (!casted_node->parent() && casted_node != this->root_)
	{
		this->repair_broken_root_list();
	}
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	DegreeBinaryTreeItem<Priority, Data>* casted_node = (DegreeBinaryTreeItem<Priority, Data>*)node;
	BinaryTreeItem<Priority, Data>*minimal_son = casted_node->highest_priority_son();
	bool is_root_item = !casted_node->parent();
	while (minimal_son && *minimal_son < *casted_node)
	{
		minimal_son->swap_with_ancestor_node(casted_node);
		minimal_son = casted_node->highest_priority_son();
	}
	if (is_root_item)
	{
		this->repair_broken_root_list();
	}
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::repair_broken_root_list()
{
	auto get_highest_node = [](BinaryTreeItem<Priority, Data>* node)
	{
		if (node->parent())
		{
			BinaryTreeItem<Priority, Data>* node_ptr = node->parent();
			while (node_ptr->parent())
			{
				node_ptr = node_ptr->parent();
			}
			return node_ptr;
		}
		return node;
	};
	if (this->root_)
	{
		this->root_ = get_highest_node(this->root_);
		this->root_->right_son() = get_highest_node(this->root_->right_son());
		BinaryTreeItem<Priority, Data>* node_ptr = this->root_->right_son(), * root = this->root_;
		while (node_ptr != this->root_)
		{
			if (*node_ptr < *root)
			{
				root = node_ptr;
			}
			node_ptr->right_son() = get_highest_node(node_ptr->right_son());
			node_ptr = node_ptr->right_son();
		}
		this->root_ = root;
	}
}

template<typename Priority, typename Data>
inline void BinomialHeapMultiPass<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->LazyBinomialHeap<Priority, Data>::consolidate_root_using_multipass(node, (int)(log2(this->size_)) + 2);
}

template<typename Priority, typename Data>
inline BinomialHeapMultiPass<Priority, Data>::BinomialHeapMultiPass() :
	BinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeapMultiPass<Priority, Data>::~BinomialHeapMultiPass()
{
}

template<typename Priority, typename Data>
inline void BinomialHeapSinglePass<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->LazyBinomialHeap<Priority, Data>::consolidate_root_using_singlepass(node, (int)(log2(this->size_)) + 2);
}

template<typename Priority, typename Data>
inline BinomialHeapSinglePass<Priority, Data>::BinomialHeapSinglePass() :
	BinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeapSinglePass<Priority, Data>::~BinomialHeapSinglePass()
{
}
