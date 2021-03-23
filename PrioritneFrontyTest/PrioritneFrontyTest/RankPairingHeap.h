#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class RankPairingHeap : public LazyBinomialHeap<K, T>
{
private:
	void restore_degree_rule(BinaryTreeItem<K, T>* node);
protected:
	void consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root = true) override;
	void priority_was_increased(BinaryTreeItem<K, T>* node);
	void priority_was_decreased(BinaryTreeItem<K, T>* node);
public:
	RankPairingHeap();
	~RankPairingHeap();
	PriorityQueueItem<K, T>* push(const K& priority, const T& data);
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
inline PriorityQueueItem<K, T>* RankPairingHeap<K, T>::push(const K& priority, const T& data)
{
	return this->LazyBinomialHeap<K, T>::push<BinaryTreeItem<K, T>>(priority, data);;
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::restore_degree_rule(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		BinaryTreeItem<K, T>* node_ptr = node;
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
inline void RankPairingHeap<K, T>::consolidate_root(BinaryTreeItem<K, T>* node, bool skip_root)
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
inline void RankPairingHeap<K, T>::priority_was_increased(BinaryTreeItem<K, T>* node)
{
	node->degree() = node->left_son() ? node->left_son()->degree() + 1 : 0;
	if (node->parent())
	{
		BinaryTreeItem<K, T>* parent = node->parent();
		node->cut();
		this->root_->add_root_item(node);
		this->restore_degree_rule(parent);
	}
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItem<K, T>* last_change = nullptr;
	for (BinaryTreeItem<K, T>* node_ptr = node->left_son(), * node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (node_ptr->priority() < node->priority())
		{
			BinaryTreeItem<K, T>* parent = node->parent();
			node_ptr->cut();
			node_ptr->degree() = node_ptr->left_son() ? node_ptr->left_son()->degree() + 1 : 0;
			last_change = parent;
		}
	}
	this->restore_degree_rule(last_change);
}
