#pragma once
#include "LazyBinomialQueue.h"
#include <stack>
#include <queue>

template <typename Priority, typename Data>
class PairingHeap : public LazyBinomialHeap<Priority, Data>
{
protected:
	PairingHeap();
	virtual BinaryTreeItem<Priority, Data>* consolidate(BinaryTreeItem<Priority, Data>* node) = 0;
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	~PairingHeap();
	void push(const int identifier, const Priority& key, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	void clear() override;
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
};

template <typename Priority, typename Data>
class PairingHeapTwoPass : public PairingHeap<Priority, Data>
{
public:
	PairingHeapTwoPass();
protected:
	BinaryTreeItem<Priority, Data>* consolidate(BinaryTreeItem<Priority, Data>* node) override;
};

template <typename Priority, typename Data>
class PairingHeapMultiPass : public PairingHeap<Priority, Data>
{
public:
	PairingHeapMultiPass();
protected:
	BinaryTreeItem<Priority, Data>* consolidate(BinaryTreeItem<Priority, Data>* node) override;
};

template<typename Priority, typename Data>
inline PairingHeap<Priority, Data>::PairingHeap() :
	LazyBinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* casted_node = (BinaryTreeItem<Priority, Data>*)node;
	if (casted_node != this->root_)
	{
		this->root_ = this->root_->merge(casted_node->cut());
	}
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = (BinaryTreeItem<Priority, Data>*)node, * parent = node_ptr->parent();
	bool is_left_son = parent ? parent->left_son() == node_ptr : false;

	node_ptr->cut();
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
		this->root_->parent() = nullptr;
	}
}

template<typename Priority, typename Data>
inline PairingHeap<Priority, Data>::~PairingHeap()
{
	this->clear();
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::push(const int identifier, const Priority& key, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	BinaryTreeItem<Priority, Data>* new_node = new BinaryTreeItem<Priority, Data>(identifier, key, data);
	if (this->root_)
	{
		this->root_ = this->root_->merge(new_node);
		this->root_->parent() = nullptr;
	}
	else
	{
		this->root_ = new_node;
	}
	this->size_++;
	data_item = new_node;
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::clear()
{
	delete this->root_;
	this->root_ = nullptr;
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	PairingHeap<Priority, Data>* heap = (PairingHeap<Priority, Data>*)other_heap;
	if (this->root_)
	{
		this->root_ = this->root_->merge(heap->root_);
		this->root_->parent() = nullptr;
		heap->root_ = nullptr;
	}
	else
	{
		std::swap(this->root_, heap->root_);
	}
	this->size_ += heap->size_;
	delete heap;
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->root_ = this->consolidate(this->root_->left_son());
	if (this->root_)
	{
		this->root_->parent() = nullptr;
	}
}

template<typename Priority, typename Data>
inline PairingHeapTwoPass<Priority, Data>::PairingHeapTwoPass() :
	PairingHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapTwoPass<Priority, Data>::consolidate(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr && node_ptr->right_son())
	{
		std::stack<BinaryTreeItem<Priority, Data>*>* stack = new std::stack<BinaryTreeItem<Priority, Data>*>();
		while (node_ptr)
		{
			node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_ptr->right_son() = nullptr;
			stack->push(node_ptr);
			node_ptr = node_next_ptr;
		}
		node_ptr = stack->top();
		stack->pop();
		while (!stack->empty())
		{
			node_ptr = node_ptr->merge(stack->top());
			stack->pop();
		}
		delete stack;
	}
	return node_ptr;
}

template<typename Priority, typename Data>
inline PairingHeapMultiPass<Priority, Data>::PairingHeapMultiPass() :
	PairingHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapMultiPass<Priority, Data>::consolidate(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr && node_ptr->right_son())
	{
		std::queue<BinaryTreeItem<Priority, Data>*> queue;
		while (node_ptr)
		{
			node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_ptr->right_son() = nullptr;
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