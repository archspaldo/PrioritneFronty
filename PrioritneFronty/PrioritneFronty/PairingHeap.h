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
	static PairingHeap<K, T>* unite(PairingHeap<K, T>* heap_1, PairingHeap<K, T>* heap_2);
	
private:
	class PairingHeapItem : public PriorityQueueItem<K, T>
	{
	public:
		PairingHeapItem(const K& priority, const T& data);
		~PairingHeapItem();
		const bool is_left();
		void is_left(bool left);
		PairingHeapItem* next_node();
		void next_node(PairingHeapItem* node);
		PairingHeapItem* left_child();
		void left_child(PairingHeapItem* node);
		void add_child(PairingHeapItem* node);
		PairingHeapItem* two_pass(PairingHeapItem* node);
		const T 
	private:
		bool is_left_;
		PairingHeapItem* next_node_;
		PairingHeapItem* left_child_;
	};


	PairingHeapItem* root_;
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
	PairingHeap<K, T>::PairingHeapItem* node = new PairingHeap<K, T>::PairingHeapItem(priority, data);
	if (!this->root_)
	{
		this->root_ = node;
	}
	else
	{
		if (priority < this->peekPriority())
		{
			node->add_child(this->root_);
			this->root_ = node;
		}
		else
		{
			this->root_->add_child(node);
		}
	}
}

template<typename K, typename T>
inline T PairingHeap<K, T>::pop()
{
	typedef PairingHeap<K, T>::PairingHeapItem PairingHeapItem;
	PairingHeapItem* left_child = this->root_->left_child();
	if (!this->root_ || this->root_)
	{

	}

	return T();
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
	return return this->root_->priority();;
}

template<typename K, typename T>
inline PairingHeap<K, T>* PairingHeap<K, T>::unite(PairingHeap<K, T>* heap_1, PairingHeap<K, T>* heap_2)
{
	PairingHeap<K, T>* heap = new PairingHeap<K, T>();
	if (!heap_1)
	{
		*heap = std::move(*heap_2);
		delete heap_2;
	}

	if (!heap_2)
	{
		*heap = std::move(*heap_1);
		delete heap_1;
	}

	if (heap_1->peekPriority() < heap_2->peekPriority())
	{
		heap_1->root_->add_child(heap_2->root_);
		*heap = std::move(*heap_1);
		delete heap_2;
	}
	else
	{
		heap_2->root_->add_child(heap_1->root_);
		*heap = std::move(*heap_2);
		delete heap_1;
	}
	
	return heap;
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem::PairingHeapItem(const K& priority, const T& data)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem::~PairingHeapItem()
{
}

template<typename K, typename T>
inline const bool PairingHeap<K, T>::PairingHeapItem::is_left()
{
	return false;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::is_left(bool left)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::next_node()
{
	return NULL;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::next_node(PairingHeapItem* node)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::left_child()
{
	return NULL;
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::left_child(PairingHeapItem* node)
{
}

template<typename K, typename T>
inline void PairingHeap<K, T>::PairingHeapItem::add_child(PairingHeapItem* node)
{
}

template<typename K, typename T>
inline PairingHeap<K, T>::PairingHeapItem* PairingHeap<K, T>::PairingHeapItem::two_pass(PairingHeapItem* node)
{
	typedef PairingHeap<K, T>::PairingHeapItem PairingHeapItem;
	if (!node || !node->next_node())
	{
		return node;
	}
	else
	{
		PairingHeapItem* A, * B, * new_node;
		A = node;
		B = node->next_node();
		new_node = B->next_node();

		A->next_node(nullptr);
		B->next_node(nullptr);

		return 
	}
}
