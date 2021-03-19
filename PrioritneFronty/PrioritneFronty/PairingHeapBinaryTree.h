#pragma once
#include "PriorityQueue.h"
#include <stack>
#include <queue>

template <typename K, typename T>
class PairingHeapBT : public PriorityQueue<K, T>
{
private:
	size_t size_;
	PQBinaryTreeItem<K, T>* root_;
public:
	virtual ~PairingHeapBT();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PriorityQueue<K, T>* other_heap);
	void change_priority(PriorityQueueItem<K, T>* node, const K& new_priority);
protected:
	PairingHeapBT();
	virtual PQBinaryTreeItem<K, T>* consolidate(PQBinaryTreeItem<K, T>* node) = 0;
};

template <typename K, typename T>
class PairingHeapBTTwoPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTTwoPass();
protected:
	PQBinaryTreeItem<K, T>* consolidate(PQBinaryTreeItem<K, T>* node) override;
};

template <typename K, typename T>
class PairingHeapBTMultiPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTMultiPass();
protected:
	PQBinaryTreeItem<K, T>* consolidate(PQBinaryTreeItem<K, T>* node) override;
};

template<typename K, typename T>
inline PairingHeapBT<K, T>::PairingHeapBT() :
	PriorityQueue<K, T>(), size_(0), root_(nullptr)
{
}

template<typename K, typename T>
inline PairingHeapBT<K, T>::~PairingHeapBT()
{
	if (this->root_)
	{
		delete this->root_;
		this->size_ = 0;
	}
}

template<typename K, typename T>
inline size_t PairingHeapBT<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* PairingHeapBT<K, T>::push(const K& key, const T& data)
{
	PQBinaryTreeItem<K, T>* new_node = new PQBinaryTreeItem<K, T>(key, data);
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
inline T PairingHeapBT<K, T>::pop()
{
	if (this->root_)
	{
		PQBinaryTreeItem<K, T>* root = this->root_;
		this->root_ = this->consolidate(this->root_->left_son());
		this->size_--;
		root->left_son(nullptr);
		T data = root->data();
		delete root;
		return data;
	}
	throw new std::range_error("PairingHeapBT<K, T>::pop(): PriorityQueue is empty!");
}

template<typename K, typename T>
inline T& PairingHeapBT<K, T>::peek()
{
	if (this->root_)
	{
		return this->root_->data();
	}
	throw new std::range_error("PairingHeapBT<K, T>::peek(): PriorityQueue is empty!");
}

template<typename K, typename T>
inline const T PairingHeapBT<K, T>::peek() const
{
	if (this->root_)
	{
		return this->root_->data();
	}
	throw new std::range_error("PairingHeapBT<K, T>::peek(): PriorityQueue is empty!");
}

template<typename K, typename T>
inline K PairingHeapBT<K, T>::peekPriority()
{
	if (this->root_)
	{
		return this->root_->priority();
	}
	throw new std::range_error("PairingHeapBT<K, T>::peekPriority(): PriorityQueue is empty!");
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
inline void PairingHeapBT<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& new_priority)
{
	node->priority(new_priority);
	PQBinaryTreeItem<K, T>* casted_node = (PQBinaryTreeItem<K, T>*)node;
	if (casted_node->parent() && new_priority < casted_node->parent()->priority())
	{
		casted_node->cut();
		this->root_ = this->root_->merge(casted_node);
		return;
	}
	if (casted_node->left_son() && new_priority > casted_node->left_son()->priority())
	{
		PQBinaryTreeItem<K, T>* parent = casted_node->parent();
		bool is_left_son = parent->left_son() == this;

		casted_node->cut();
		casted_node->right_son(casted_node->left_son());
		casted_node->left_son(nullptr);

		casted_node = this->consolidate(casted_node);
		
		if (parent)
		{
			if (is_left_son)
			{
				casted_node->right_son(parent->left_son());
				parent->left_son(casted_node);
			}
			else
			{
				casted_node->right_son(parent->right_son());
				parent->right_son(casted_node);
			}
			if (casted_node->right_son())
			{
				casted_node->right_son()->parent(casted_node);
			}
			casted_node->parent(parent);
		}
		else
		{
			this->root_ = casted_node;
		}
		return;
	}
	return;
}

template<typename K, typename T>
inline PairingHeapBTTwoPass<K, T>::PairingHeapBTTwoPass() :
	PairingHeapBT<K, T>()
{
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PairingHeapBTTwoPass<K, T>::consolidate(PQBinaryTreeItem<K, T>* node)
{
	PQBinaryTreeItem<K, T>* node_ptr = node, *node_next_ptr;
	
	if (node_ptr && node_ptr->right_son())
	{
		std::stack<PQBinaryTreeItem<K, T>*> stack;
		while (node_ptr)
		{
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_next_ptr = node_ptr->right_son();
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
inline PQBinaryTreeItem<K, T>* PairingHeapBTMultiPass<K, T>::consolidate(PQBinaryTreeItem<K, T>* node)
{
	PQBinaryTreeItem<K, T>* node_ptr = node, * node_next_ptr;
	if (node_ptr && node_ptr->right_son())
	{
		std::queue<PQBinaryTreeItem<K, T>*> queue;
		while (node_ptr)
		{
			node_ptr = node_ptr->merge(node_ptr->right_son());
			node_next_ptr = node_ptr->right_son();
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