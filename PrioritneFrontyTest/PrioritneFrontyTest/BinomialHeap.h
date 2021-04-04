#pragma once
#include "LazyBinomialQueue.h"

template <typename K, typename T>
class BinomialHeap : public LazyBinomialHeap<K, T>
{
protected:
	void priority_was_increased(PriorityQueueItem<K, T>* node) override;
	void priority_was_decreased(PriorityQueueItem<K, T>* node) override;
	void fix_broken_root_list();
	BinomialHeap();
public:
	~BinomialHeap();
	void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item) override;
	void merge(PriorityQueue<K, T>* other_heap) override;
};

template <typename K, typename T>
class BinomialHeapMultiPass : public BinomialHeap<K, T>
{
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
public:
	BinomialHeapMultiPass();
	~BinomialHeapMultiPass();
};

template <typename K, typename T>
class BinomialHeapSinglePass : public BinomialHeap<K, T>
{
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
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
inline void BinomialHeap<K, T>::push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item)
{
	DegreeBinaryTreeItem<K, T>* new_item = new DegreeBinaryTreeItem<K, T>(identifier, priority, data);
	this->size_++;
	this->consolidate_root(new_item);
	data_item = new_item;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	this->LazyBinomialHeap<K, T>::merge(other_heap);
	this->consolidate_root(nullptr);
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::priority_was_increased(PriorityQueueItem<K, T>* node)
{
	DegreeBinaryTreeItem<K, T>* casted_node = (DegreeBinaryTreeItem<K, T>*)node;
	while (casted_node->parent() && casted_node->priority() < casted_node->parent()->priority())
	{
		casted_node->swap_with_parent();
	}
	if (!casted_node->parent() && casted_node != this->root_)
	{
		this->fix_broken_root_list();

	}
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	BinaryTreeItem<K, T>* casted_node = (BinaryTreeItem<K, T>*)node, * minimal_son = casted_node->higher_priority_son();
	bool is_root_item = !casted_node->parent();
	while (minimal_son && *minimal_son < *casted_node)
	{
		minimal_son->swap_with_parent();
		minimal_son = casted_node->higher_priority_son();
	}
	if (is_root_item)
	{
		this->fix_broken_root_list();
	}
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::fix_broken_root_list()
{
	auto get_highest_node = [](BinaryTreeItem<K, T>* node)
	{
		if (node->parent())
		{
			BinaryTreeItem<K, T>* node_ptr = node->parent();
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
		BinaryTreeItem<K, T>* node_ptr = this->root_->right_son(), * root = this->root_;
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

template<typename K, typename T>
inline void BinomialHeapMultiPass<K, T>::consolidate_root(BinaryTreeItem<K, T>* node)
{
	this->LazyBinomialHeap<K, T>::consolidate_root_using_multipass(node, (int)(log2(this->size_)) + 2);
	
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
inline void BinomialHeapSinglePass<K, T>::consolidate_root(BinaryTreeItem<K, T>* node)
{
	this->LazyBinomialHeap<K, T>::consolidate_root_using_singlepass(node, (int)(log2(this->size_)) + 2);
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
