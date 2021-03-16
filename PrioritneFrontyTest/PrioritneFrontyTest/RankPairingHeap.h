#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class RankPairingHeap : public LazyBinomialHeap<K, T>
{
protected:
	void consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root = true) override;
	void priority_was_increased(BinaryTreeItem<K, T>* node);
	void priority_was_decreased(BinaryTreeItem<K, T>* node);
public:
	RankPairingHeap();
	~RankPairingHeap();
	PriorityQueueItem<K, T>* push(const K& priority, const T& data);
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority);
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
inline void RankPairingHeap<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& priority)
{
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root)
{
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_increased(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItem<K, T>* parent = node->parent();
	size_t degree;
	node->cut();
	if ()
	node->degree() = node->left_son() ? node->left_son()->degree() + 1 : 0;
	this->root_->add_root_item(node);
	while (true)
	{
		if (parent == this->root_)
		{
			parent->degree() = parent->left_son() ? parent->left_son()->degree() + 1 : 0;
		}
		else
		{

		}
	}
	
}

template<typename K, typename T>
inline void RankPairingHeap<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
}
