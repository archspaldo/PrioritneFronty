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
	void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item) override;
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
inline void RankPairingHeap<K, T>::push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& data_item)
{
	data_item = this->LazyBinomialHeap<K, T>::push(new DegreeBinaryTreeItem<K, T>(identifier, priority, data));
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::restore_degree_rule(DegreeBinaryTreeItem<K, T>* node)
{
	if (node)
	{
		DegreeBinaryTreeItem<K, T>* node_ptr = node;
		int degree, left_son_degree, right_son_degree;
		while (true)
		{
			left_son_degree = node_ptr->left_son() ? dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr->left_son())->degree() : -1;
			right_son_degree = node_ptr->right_son() && node_ptr->right_son()->parent() ? dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr->right_son())->degree() : -1;
			if (node_ptr->parent())
			{
				if (abs(left_son_degree - right_son_degree) > 1)
				{
					degree = std::max(left_son_degree, right_son_degree);
				}
				else
				{
					degree = std::max(left_son_degree, right_son_degree + 1);
				}
				if (node_ptr->degree() < degree)
				{
					return;
				}
				else
				{
					node_ptr->degree() = degree;
				}
				node_ptr = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr->parent());
			}
			else
			{
				node_ptr->degree() = left_son_degree + 1;
				return;
			}
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
	DegreeBinaryTreeItem<K, T>* casted_node = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node);
	casted_node->degree() = casted_node->left_son() ? dynamic_cast<DegreeBinaryTreeItem<K, T>*>(casted_node->left_son())->degree() + 1 : 0;
	if (casted_node->parent())
	{
		DegreeBinaryTreeItem<K, T>* parent = (DegreeBinaryTreeItem<K, T>*)casted_node->parent();
		this->add_root_item(casted_node->cut());
		this->restore_degree_rule(parent);
	}
	else if (casted_node->priority() < this->root_->priority())
	{
		this->root_ = casted_node;
	}
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	BinaryTreeItem<K, T>* last_change = nullptr;
	if (this->root_ == node)
	{
		BinaryTreeItem<K, T>* new_root = this->root_;
		for (BinaryTreeItem<K, T>* node_ptr = this->root_->right_son(); node_ptr != this->root_; node_ptr = node_ptr->right_son())
		{
			if (node_ptr->priority() < new_root->priority())
			{
				new_root = node_ptr;
			}
		}
		this->root_ = new_root;
	}
	for (BinaryTreeItem<K, T>* node_ptr = dynamic_cast<BinaryTreeItem<K, T>*>(node)->left_son(), * node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (node_ptr->priority() < node->priority())
		{
			last_change = node_ptr->parent();
			dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr->cut())->degree() = node_ptr->left_son() ? dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr->left_son())->degree() + 1 : 0;
			this->add_root_item(node_ptr);
		}
	}
	this->restore_degree_rule(dynamic_cast<DegreeBinaryTreeItem<K, T>*>(last_change));
}
