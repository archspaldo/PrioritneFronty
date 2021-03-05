#pragma once
#include "PriorityQueue.h"

template<typename K, typename T>
class PairingHeap : public PriorityQueue<K, T>
{
public:
	PairingHeap();
	~PairingHeap();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PairingHeap<K, T>* other_heap);
private:
	class PairingHeapItem : public PriorityQueueLinkedItem<K, T>
	{
	public:
		PairingHeapItem(const K& priority, const T& data) :
			PriorityQueueLinkedItem<K, T>(priority, data) {};
		~PairingHeapItem() {};
		PairingHeapItem* two_pass();
	};
	PairingHeapItem* root_;
	size_t size_;
};

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeap() :
	root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::~PairingHeap()
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
inline PriorityQueueItem<K, T>* PairingHeap<K, T>::push(const K& priority, const T& data)
{
	PairingHeapItem* node = new PairingHeapItem(priority, data);
	if (this->root_)
	{
		this->root_ = (PairingHeapItem*)this->root_->merge(node);
	}
	else
	{
		this->root_ = node;
	}
	this->size_++;
	return node;
}

template<typename K, typename T>
inline T PairingHeap<K, T>::pop()
{
	if (this->root_)
	{
		PairingHeapItem* root = this->root_;
		this->root_ = this->root_->first_son() ? ((PairingHeapItem*)this->root_->first_son())->two_pass() : nullptr;
		root->first_son(nullptr);
		T data = root->data();
		delete root;
		return data;
		this->size_--;
	}
	throw new std::range_error("PairingHeap<K, T>::pop(): PriorityQueue is empty!");
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
inline typename PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::two_pass()
{
	if (this->is_isolated())
	{
		return this;
	}
	else
	{
		PriorityQueueLinkedItem<K, T>* paired_nodes = nullptr, * paired_nodes_ptr = nullptr;
		paired_nodes = this->merge_with_right_sibling();
		paired_nodes_ptr = paired_nodes->right_sibling();
		while (paired_nodes_ptr != paired_nodes && paired_nodes_ptr->right_sibling() != paired_nodes)
		{
			paired_nodes_ptr = paired_nodes_ptr->merge_with_right_sibling();
			paired_nodes_ptr = paired_nodes_ptr->right_sibling();
		}
		while (!paired_nodes->is_isolated())
		{
			paired_nodes = paired_nodes->merge_with_left_sibling();
		}
		return (PairingHeapItem*)paired_nodes;
	}
}
