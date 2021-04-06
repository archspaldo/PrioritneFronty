#pragma once
#include "LazyBinomialQueue.h"

template <typename Priority, typename Data>
class RankPairingHeap : public LazyBinomialHeap<Priority, Data>
{
private:
	void restore_degree_rule(DegreeBinaryTreeItem<Priority, Data>* node);
protected:
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	RankPairingHeap();
	~RankPairingHeap();
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
};

template<typename Priority, typename Data>
inline RankPairingHeap<Priority, Data>::RankPairingHeap() :
	LazyBinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline RankPairingHeap<Priority, Data>::~RankPairingHeap()
{
}

template<typename Priority, typename Data>
inline void RankPairingHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	data_item = this->LazyBinomialHeap<Priority, Data>::push(new DegreeBinaryTreeItem<Priority, Data>(identifier, priority, data));
}

template<typename Priority, typename Data>
inline void RankPairingHeap<Priority, Data>::restore_degree_rule(DegreeBinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		DegreeBinaryTreeItem<Priority, Data>* node_ptr = node;
		int degree, left_son_degree, right_son_degree;
		while (true)
		{
			left_son_degree = node_ptr->left_son() ? dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node_ptr->left_son())->degree() : -1;
			right_son_degree = node_ptr->right_son() && node_ptr->right_son()->parent() ? dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node_ptr->right_son())->degree() : -1;
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
				node_ptr = dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node_ptr->parent());
			}
			else
			{
				node_ptr->degree() = left_son_degree + 1;
				return;
			}
		}
	}
}

template<typename Priority, typename Data>
inline void RankPairingHeap<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->consolidate_root_using_multipass(node, (int)(log2(this->size_)) + 2);
}

template<typename Priority, typename Data>
inline void RankPairingHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	DegreeBinaryTreeItem<Priority, Data>* casted_node = dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node);
	casted_node->degree() = casted_node->left_son() ? dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(casted_node->left_son())->degree() + 1 : 0;
	if (casted_node->parent())
	{
		DegreeBinaryTreeItem<Priority, Data>* parent = (DegreeBinaryTreeItem<Priority, Data>*)casted_node->parent();
		this->add_root_item(casted_node->cut());
		this->restore_degree_rule(parent);
	}
	else if (*casted_node < *this->root_)
	{
		this->root_ = casted_node;
	}
}

template<typename Priority, typename Data>
inline void RankPairingHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* last_change = nullptr;
	if (this->root_ == node)
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
	for (BinaryTreeItem<Priority, Data>* node_ptr = dynamic_cast<BinaryTreeItem<Priority, Data>*>(node)->left_son(), *node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (*node_ptr < *node)
		{
			last_change = node_ptr->parent();
			dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node_ptr->cut())->degree() = node_ptr->left_son() ? dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(node_ptr->left_son())->degree() + 1 : 0;
			this->add_root_item(node_ptr);
		}
	}
	this->restore_degree_rule(dynamic_cast<DegreeBinaryTreeItem<Priority, Data>*>(last_change));
}
