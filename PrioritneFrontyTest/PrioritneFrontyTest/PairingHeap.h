#pragma once
#include "PriorityQueue.h"
#include <stack>
#include <queue>

template <typename K, typename T>
class PairingHeapBT : public LazyBinomialHeap<K, T>
{
protected:
	PairingHeapBT();
	virtual BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) = 0;
	void priority_was_increased(BinaryTreeItem<K, T>* node) override;
	void priority_was_decreased(BinaryTreeItem<K, T>* node) override;
public:
	~PairingHeapBT();
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root = true) override;

};

template <typename K, typename T>
class PairingHeapBTTwoPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTTwoPass();
protected:
	BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) override;
};

template <typename K, typename T>
class PairingHeapBTMultiPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTMultiPass();
protected:
	BinaryTreeItem<K, T>* consolidate(BinaryTreeItem<K, T>* node) override;
};

template<typename K, typename T>
inline PairingHeapBT<K, T>::PairingHeapBT() :
	LazyBinomialHeap<K, T>()
{
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::priority_was_increased(BinaryTreeItem<K, T>* node)
{
	if (node->parent())
	{
		node->cut();
		this->root_ = this->root_->merge(node);
	}
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::priority_was_decreased(BinaryTreeItem<K, T>* node)
{
	BinaryTreeItem<K, T>* parent = node->parent(), * node_ptr = node;
	bool is_left_son = parent ? parent->left_son() == node_ptr : false;

	node_ptr->cut();
	node_ptr->degree() = 0;
	node_ptr->right_son(node_ptr->left_son());
	node_ptr->left_son(nullptr);

	node_ptr = this->consolidate(node);

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
inline PairingHeapBT<K, T>::~PairingHeapBT()
{
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* PairingHeapBT<K, T>::push(const K& key, const T& data)
{
	BinaryTreeItem<K, T>* new_node = new BinaryTreeItem<K, T>(key, data);
	if (this->root_)
	{
		this->root_ = this->root_->merge(new_node);
	}
	else
	{
		this->root_ = new_node;
	}
	this->size_++;
	return new_node;
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	PairingHeapBT<K, T>* heap = (PairingHeapBT<K, T>*)other_heap;
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
inline void PairingHeapBT<K, T>::consolidate_with(BinaryTreeItem<K, T>* node, bool skip_root)
{
	this->root_ = this->consolidate(this->root_->left_son());
}

template<typename K, typename T>
inline PairingHeapBTTwoPass<K, T>::PairingHeapBTTwoPass() :
	PairingHeapBT<K, T>()
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* PairingHeapBTTwoPass<K, T>::consolidate(BinaryTreeItem<K, T>* node)
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
inline PairingHeapBTMultiPass<K, T>::PairingHeapBTMultiPass() :
	PairingHeapBT<K, T>()
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* PairingHeapBTMultiPass<K, T>::consolidate(BinaryTreeItem<K, T>* node)
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