#pragma once
#include "PriorityQueue.h"

template<typename K, typename T>
class PairingHeap : public PriorityQueue<K, T>
{
public:
	PairingHeap();
	~PairingHeap();
	void clear() override;
	size_t size() const override;
	void push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PairingHeap<K, T>* other_heap);
private:
	class PairingHeapItem;
	PairingHeapItem* root_;
	size_t size_;
};

template<typename K, typename T>
class PairingHeap<K, T>::PairingHeapItem : public PriorityQueueItem<K, T>
{
public:
	PairingHeapItem(const K& priority, const T& data);
	~PairingHeapItem();
	PairingHeapItem* sibling();
	void sibling(PairingHeapItem* node);
	PairingHeapItem* left_son();
	void left_son(PairingHeapItem* node);
	PairingHeapItem* add_sub_tree(PairingHeapItem* node);
	PairingHeapItem* link(PairingHeapItem* node);
	PairingHeapItem* merge(PairingHeapItem* node);
	static PairingHeapItem* two_pass(PairingHeapItem* node);
private:
	PairingHeapItem* sibling_;
	PairingHeapItem* left_son_;
};

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeap() :
	root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::~PairingHeap()
{
	this->clear();
}

template<typename K, typename T>
inline void PairingHeap<K, T>::clear()
{
	delete this->root_;
	this->size_ = 0;
	this->root_ = nullptr;
}

template<typename K, typename T>
inline size_t PairingHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::push(const K& priority, const T& data)
{
	PairingHeap<K, T> new_heap;
	new_heap.root_ = new PairingHeap<K, T>::PairingHeapItem(priority, data);
	new_heap.size_++;
	this->merge(&new_heap);
}

template<typename K, typename T>
inline T PairingHeap<K, T>::pop()
{
	PairingHeapItem* root;
	if (this->root_)
	{
		root = this->root_;
		this->root_ = PairingHeapItem::two_pass(this->root_->left_son());
		root->sibling(nullptr);
		root->left_son(nullptr);
		T data = root->data();
		delete root;
		return data;
		this->size_--;
	}
	return 0;
}

template<typename K, typename T>
inline T& PairingHeap<K, T>::peek()
{
	return this->root_->data();
}

template<typename K, typename T>
inline const T PairingHeap<K, T>::peek() const
{
	return this->root_->data();
}

template<typename K, typename T>
inline K PairingHeap<K, T>::peekPriority()
{
	return this->root_->priority();;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::merge(PairingHeap<K, T>* other_heap)
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
	other_heap->size_ = 0;
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem::PairingHeapItem(const K& priority, const T& data) :
	PriorityQueueItem(priority, data), left_son_(nullptr), sibling_(nullptr)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem::~PairingHeapItem()
{
	if (this->sibling_)
	{
		delete this->sibling_;
		this->sibling_ = nullptr;
	}
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::sibling()
{
	return this->sibling_;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::sibling(PairingHeapItem* node)
{
	this->sibling_ = node;
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::left_son(PairingHeapItem* node)
{
	this->left_son_ = node;
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::add_sub_tree(PairingHeapItem* node)
{
	node->sibling_ = this->left_son_;
	this->left_son_ = node;
	return this;
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::link(PairingHeapItem* node)
{
	if (node)
	{
		if (this->priority() < node->priority())
		{
			this->sibling_ = node->sibling_;
			return this->add_sub_tree(node);
		}
		else
		{
			return node->add_sub_tree(this);
		}
	}
	return this;
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::merge(PairingHeapItem* node)
{
	if (node)
	{
		if (this->priority() < node->priority())
		{
			return this->add_sub_tree(node);
		}
		else
		{
			return node->add_sub_tree(this);
		}
	}
	return this;
}

template<typename K, typename T>
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::two_pass(PairingHeapItem* node)
{
	if (node)
	{
		PairingHeapItem* paired_nodes = nullptr, * paired_nodes_ptr = nullptr, * node_next = node->sibling_, * node_next_ptr = node_next ? node_next->sibling_ : nullptr;
		while (node_next)
		{
			if (paired_nodes)
			{
				paired_nodes_ptr->sibling(node->merge(node_next));
				paired_nodes_ptr = paired_nodes_ptr->sibling();
			}
			else
			{
				paired_nodes = paired_nodes_ptr = node->merge(node_next);
			}
			node = node_next_ptr;
			node_next = node ? node->sibling() : nullptr;
			node_next_ptr = node_next ? node_next->sibling() : nullptr;
		}
		if (node)
		{
			if (paired_nodes)
			{
				paired_nodes_ptr->sibling(node);
			}
			else
			{
				paired_nodes = node;
			}
		}
		node_next = paired_nodes->sibling();
		while (node_next)
		{
			paired_nodes = paired_nodes->link(node_next);
			node_next = paired_nodes->sibling();
		}
		node = paired_nodes;
		return node;
	}
	return nullptr;
}
