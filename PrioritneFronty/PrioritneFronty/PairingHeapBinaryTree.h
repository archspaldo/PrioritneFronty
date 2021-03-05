#pragma once
#include "PriorityQueue.h"
#include <stack>
#include <queue>

template <typename K, typename T>
class PairingHeapBT : public PriorityQueue<K, T>
{
protected:
	class PairingHeapBTItem;
private:
	size_t size_;
	PairingHeapBTItem* root_;
public:
	virtual ~PairingHeapBT();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PairingHeapBT<K, T>* other_heap);
	void change_priority_of_node(PriorityQueueItem<K, T>* node, const K& new_priority);
protected:
	PairingHeapBT();
	virtual PairingHeapBTItem* consolidate(PairingHeapBTItem* node) = 0;
};

template <typename K, typename T>
class PairingHeapBTTwoPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTTwoPass();
	using PairingHeapBTItem = typename PairingHeapBT<K, T>::PairingHeapBTItem;
protected:
	PairingHeapBTItem* consolidate(PairingHeapBTItem* node) override;
};

template <typename K, typename T>
class PairingHeapBTMultiPass : public PairingHeapBT<K, T>
{
public:
	PairingHeapBTMultiPass();
	using PairingHeapBTItem = typename PairingHeapBT<K, T>::PairingHeapBTItem;
protected:
	PairingHeapBTItem* consolidate(PairingHeapBTItem* node) override;
};

template <typename K, typename T>
class PairingHeapBT<K, T>::PairingHeapBTItem : public PriorityQueueItem<K, T>
{
public:
	PairingHeapBTItem(const K& priority, const T& data);
	~PairingHeapBTItem();
	PairingHeapBTItem* merge(PairingHeapBTItem* node);
	void isolate();
	PairingHeapBTItem* left_son();
	PairingHeapBTItem* right_son();
	PairingHeapBTItem* parent();
	void left_son(PairingHeapBTItem* node);
	void right_son(PairingHeapBTItem* node);
	void parent(PairingHeapBTItem* node);
private:
	PairingHeapBTItem* add_subtree(PairingHeapBTItem*);

	PairingHeapBTItem* left_son_;
	PairingHeapBTItem* right_son_;
	PairingHeapBTItem* parent_;
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
	PairingHeapBTItem* new_node = new PairingHeapBTItem(key, data);
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
		PairingHeapBTItem* root = this->root_;
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
inline void PairingHeapBT<K, T>::merge(PairingHeapBT<K, T>* other_heap)
{
	if (this->root_)
	{
		this->root_ = this->root_->merge(other_heap->root_);
		this->size_ += other_heap->size_;
	}
	else
	{
		this->root_ = other_heap->root_;
		this->size_ = other_heap->size_;
	}
	other_heap->root_ = nullptr;
	delete other_heap;
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::change_priority_of_node(PriorityQueueItem<K, T>* node, const K& new_priority)
{
	node->priority(new_priority);
	PairingHeapBTItem* casted_node = (PairingHeapBTItem*)node;
	if (casted_node->parent() && new_priority < casted_node->parent()->priority())
	{
		casted_node->isolate();
		this->root_ = this->root_->merge(casted_node);
		return;
	}
	if (casted_node->left_son() && new_priority > casted_node->left_son()->priority())
	{
		PairingHeapBTItem* parent = casted_node->parent();
		bool is_left_son = parent->left_son() == this;

		casted_node->isolate();
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
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBTTwoPass<K, T>::consolidate(PairingHeapBTItem* node)
{
	PairingHeapBTItem* node_ptr = node, *node_next_ptr;
	
	if (node_ptr && node_ptr->right_son())
	{
		std::stack<PairingHeapBTItem*> stack;
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
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBTMultiPass<K, T>::consolidate(PairingHeapBTItem* node)
{
	PairingHeapBTItem* node_ptr = node, * node_next_ptr;
	if (node_ptr && node_ptr->right_son())
	{
		std::queue<PairingHeapBTItem*> queue;
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

template<typename K, typename T>
inline PairingHeapBT<K, T>::PairingHeapBTItem::PairingHeapBTItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data), parent_(nullptr), left_son_(nullptr), right_son_(nullptr)
{
}

template<typename K, typename T>
inline PairingHeapBT<K, T>::PairingHeapBTItem::~PairingHeapBTItem()
{
	if (this->right_son_)
	{
		delete this->right_son_;
		this->right_son_ = nullptr;
	}
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	this->parent_ = nullptr;
}

template<typename K, typename T>
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBT<K, T>::PairingHeapBTItem::merge(PairingHeapBTItem* node)
{
	if (node)

	{
		if (this->priority() < node->priority())
		{
			this->right_son_ = node->right_son_;
			return this->add_subtree(node);
		}
		else
		{
			return node->add_subtree(this);
		}
	}
	return this;
}
	

template <typename K, typename T>
inline void PairingHeapBT<K, T>::PairingHeapBTItem::isolate()
{
	if (this->parent_)
	{
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son_ = this->right_son_;
		}
		else
		{
			this->parent_->right_son_ = this->right_son_;
		}
	}
	if (this->right_son_)
	{
		this->right_son_->parent_ = this->parent_;
	}
	this->parent_ = nullptr;
}

template<typename K, typename T>
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBT<K, T>::PairingHeapBTItem::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBT<K, T>::PairingHeapBTItem::right_son()
{
	return this->right_son_;
}

template<typename K, typename T>
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBT<K, T>::PairingHeapBTItem::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::PairingHeapBTItem::left_son(PairingHeapBTItem* node)
{
	this->left_son_ = node;
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::PairingHeapBTItem::right_son(PairingHeapBTItem* node)
{
	this->right_son_ = node;
}

template<typename K, typename T>
inline void PairingHeapBT<K, T>::PairingHeapBTItem::parent(PairingHeapBTItem* node)
{
	this->parent_ = node;
}

template<typename K, typename T>
inline typename PairingHeapBT<K, T>::PairingHeapBTItem* PairingHeapBT<K, T>::PairingHeapBTItem::add_subtree(PairingHeapBTItem* node)
{
	if (node)
	{
		if (this->left_son_)
		{
			this->left_son_->parent_ = node;
		}
		node->parent_ = this;
		node->right_son_ = this->left_son_;
		this->left_son_ = node;
	}
	return this;
}
