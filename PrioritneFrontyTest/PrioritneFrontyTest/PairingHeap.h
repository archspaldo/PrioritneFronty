#pragma once
#include "LazyBinomialQueue.h"
#include <stack>
#include <queue>

template <typename K, typename T>
class PairingHeap : public LazyBinomialHeap<K, T>
{
protected:
	PairingHeap();
	virtual BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) = 0;
	void priority_was_increased(PriorityQueueItem<K, T>* node) override;
	void priority_was_decreased(PriorityQueueItem<K, T>* node) override;
public:
	~PairingHeap();
	void push(const int identifier, const K& key, const T& data, DataItem<K, T>*& data_item) override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void consolidate_root(BinaryTreeItem<K, T>* node) override;
};

template <typename K, typename T>
class PairingHeapTwoPass : public PairingHeap<K, T>
{
public:
	PairingHeapTwoPass();
protected:
	BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) override;
};

template <typename K, typename T>
class PairingHeapMultiPass : public PairingHeap<K, T>
{
public:
	PairingHeapMultiPass();
protected:
	BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) override;
};

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeap() :
	LazyBinomialHeap<K, T>()
{
}

template<typename K, typename T>
inline void PairingHeap<K, T>::priority_was_increased(PriorityQueueItem<K, T>* node)
{
	BinaryTreeItem<K, T>* casted_node = (BinaryTreeItem<K, T>*)node;
	if (casted_node->parent())
	{
		casted_node->cut();
		this->root_ = this->root_->merge(casted_node);
	}
}

template<typename K, typename T>
inline void PairingHeap<K, T>::priority_was_decreased(PriorityQueueItem<K, T>* node)
{
	BinaryTreeItem<K, T>* node_ptr = (BinaryTreeItem<K, T>*)node, * parent = node_ptr->parent();
	bool is_left_son = parent ? parent->left_son() == node_ptr : false;

	node_ptr->cut();
	node_ptr->degree() = 0;
	node_ptr->right_son(node_ptr->left_son());
	node_ptr->left_son(nullptr);

	node_ptr = this->consolidate(node_ptr);

	if (parent)
	{
		if (is_left_son)
		{
			parent->add_left_son(node_ptr);
		}
		else
		{
			parent->add_right_son(node_ptr);
		}
	}
	else
	{
		this->root_ = node_ptr;
	}
}

template<typename K, typename T>
inline PairingHeap<K, T>::~PairingHeap()
{
}

template<typename K, typename T>
inline void PairingHeap<K, T>::push(const int identifier, const K& key, const T& data, DataItem<K, T>*& data_item)
{
	BinaryTreeItem<K, T>* new_node = new BinaryTreeItem<K, T>(identifier, key, data);
	if (this->root_)
	{
		this->root_ = this->root_->merge(new_node);
	}
	else
	{
		this->root_ = new_node;
	}
	this->size_++;
	data_item = new_node->data_item();
}

template<typename K, typename T>
inline void PairingHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	PairingHeap<K, T>* heap = (PairingHeap<K, T>*)other_heap;
	if (this->root_)
	{
		this->root_ = this->root_->merge(heap->root_);
		this->size_ += heap->size_;
	}
	else
	{
		this->root_ = heap->root_;
		this->size_ = heap->size_;
	}
	heap->root_ = nullptr;
	delete heap;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::consolidate_root(BinaryTreeItem<K, T>* node)
{
	this->root_ = this->consolidate(this->root_->left_son());
}

template<typename K, typename T>
inline PairingHeapTwoPass<K, T>::PairingHeapTwoPass() :
	PairingHeap<K, T>()
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* PairingHeapTwoPass<K, T>::consolidate(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItem<K, T>* node_ptr = node, * node_next_ptr;

	if (node_ptr && node_ptr->right_son())
	{
		std::stack<BinaryTreeItem<K, T>*> stack;
		while (node_ptr)
		{
			node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_ptr->right_son(nullptr);
			stack.push(node_ptr);
			node_ptr = node_next_ptr;
		}
		node_ptr = stack.top();
		stack.pop();
		while (!stack.empty())
		{
			node_ptr = node_ptr->merge(stack.top());
			stack.pop();
		}
	}
	return node_ptr;
}

template<typename K, typename T>
inline PairingHeapMultiPass<K, T>::PairingHeapMultiPass() :
	PairingHeap<K, T>()
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* PairingHeapMultiPass<K, T>::consolidate(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItem<K, T>* node_ptr = node, * node_next_ptr;

	if (node_ptr && node_ptr->right_son())
	{
		std::queue<BinaryTreeItem<K, T>*> queue;
		while (node_ptr)
		{
			node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_ptr->right_son(nullptr);
			queue.push(node_ptr);
			node_ptr = node_next_ptr;
		}
		node_ptr = queue.front();
		queue.pop();
		while (!queue.empty())
		{
			node_ptr = node_ptr->merge(queue.front());
			queue.pop();
			queue.push(node_ptr);
			node_ptr = queue.front();
			queue.pop();
		}
	}
	return node_ptr;
}