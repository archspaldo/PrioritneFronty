#pragma once
#include "LazyBinomialQueue.h"

template <typename K, typename T>
class RankPairingHeap : public LazyBinomialHeap<K, T>
{
private:
	void restore_degree_rule(DegreeBinaryTreeItem<K, T>* node);
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
	void priority_was_increased(PriorityQueueItem<K, T>* node) override;
	void priority_was_decreased(PriorityQueueItem<K, T>* node) override;
public:
	RankPairingHeap();
	~RankPairingHeap();
	void push(const int identifier, const K& priority, const T& data, DataItem<K, T>*& data_item) override;
};

template<typename K, typename T>
inline RankPairingHeap<K, T>::RankPairingHeap() :
	LazyBinomialHeap<K, T>()
{
}

template<typename K, typename T>
inline RankPairingHeap<K, T>::~RankPairingHeap()
{
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::push(const int identifier, const K& priority, const T& data, DataItem<K, T>*& data_item)
{
	data_item = this->LazyBinomialHeap<K, T>::push(new DegreeBinaryTreeItem<K, T>(identifier, priority, data));
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::restore_degree_rule(DegreeBinaryTreeItem<K, T>* node)
{
	if (node)
	{
		DegreeBinaryTreeItem<K, T>* node_ptr = node;
		size_t degree, left_son_degree, right_son_degree;
		while (true)
		{
			left_son_degree = node_ptr->left_son() ? node_ptr->left_son()->degree() : -1;
			right_son_degree = node_ptr->right_son() ? node_ptr->right_son()->degree() : -1;
			if (node_ptr == this->root_)
			{
				node_ptr->degree() = left_son_degree + 1;
				return;
			}
			else
			{
				if (abs(left_son_degree - right_son_degree) > 1)
				{
					degree = std::max(left_son_degree, right_son_degree);
				}
				else
				{
					degree = std::max(left_son_degree, right_son_degree + 1);
				}
			}
			if (node_ptr->degree() < degree)
			{
				return;
			}
			else
			{
				node_ptr->degree() = degree;
			}
			node_ptr = node_ptr->parent();
		}
	}
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::consolidate_root(BinaryTreeItem<K, T>* node)
{
	this->consolidate_root_using_multipass(node, (int)(log2(this->size_)) + 2);
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_increased(PriorityQueueItem<K, T>* node)
{
	DegreeBinaryTreeItem<K, T>* casted_node = (DegreeBinaryTreeItem<K, T>*)node;
	casted_node->degree() = casted_node->left_son() ? casted_node->left_son()->degree() + 1 : 0;
	if (casted_node->parent())
	{
		DegreeBinaryTreeItem<K, T>* parent = (DegreeBinaryTreeItem<K, T>*)casted_node->parent();
		casted_node->cut();
		this->add_root_item(casted_node);
		this->restore_degree_rule(parent);
	}
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	DegreeBinaryTreeItem<K, T>* last_change = nullptr, * casted_node = (DegreeBinaryTreeItem<K, T>*)node;
	for (DegreeBinaryTreeItem<K, T>* node_ptr = casted_node->left_son(), * node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (node_ptr->priority() < casted_node->priority())
		{
			last_change = casted_node->parent();
			node_ptr->cut();
			node_ptr->degree() = node_ptr->left_son() ? node_ptr->left_son()->degree() + 1 : 0;
		}
	}
	this->restore_degree_rule(last_change);
}
