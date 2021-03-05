#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class BinomialHeap : public PriorityQueue<K, T>
{
public:
	BinomialHeap();
	~BinomialHeap();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(BinomialHeap<K, T>* other_heap);
	void decrease_key(PriorityQueueItem<K, T>* node, const K& key);
private:
	void find_minimum();
	PriorityQueueLinkedItem<K, T>* head_;
	PriorityQueueLinkedItem<K, T>* root_;
	size_t size_;
};

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeap() :
	PriorityQueue<K, T>(), size_(0), head_(nullptr), root_(nullptr)
{
}

template<typename K, typename T>
inline BinomialHeap<K, T>::~BinomialHeap()
{
	delete this->head_;
	this->root_ = this->head_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t BinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* BinomialHeap<K, T>::push(const K& key, const T& data)
{
	BinomialHeap<K, T>* temporary_heap = new BinomialHeap<K, T>();
	PriorityQueueLinkedItem<K, T>* new_item = new PriorityQueueLinkedItem<K, T>(key, data);
	temporary_heap->head_ = new_item;
	temporary_heap->size_++;
	this->merge(temporary_heap);
	return (PriorityQueueItem<K, T>*)new_item;
}

template<typename K, typename T>
inline T BinomialHeap<K, T>::pop()
{
	if (this->head_)
	{
		if (this->head_->is_isolated())
		{
			this->head_ = nullptr;
		}
		else
		{
			if (this->head_ == this->root_)
			{
				this->head_ = this->head_->right_sibling();
			}
			this->root_->isolate();
		}

		PriorityQueueLinkedItem<K, T>* min = this->root_, * child_list_ptr = this->root_->first_son(),
			* child_list_next_ptr = child_list_ptr && !child_list_ptr->is_isolated() ? child_list_ptr->right_sibling() : nullptr;
		BinomialHeap<K, T>* temporary_heap = new BinomialHeap<K, T>();

		this->root_->first_son(nullptr);
		this->root_->right_sibling(nullptr);
		
		if (child_list_ptr)
		{
			temporary_heap->head_ = child_list_ptr->isolate();
			temporary_heap->head_->parent(nullptr);
		}
		

		while (child_list_next_ptr)
		{
			child_list_ptr = child_list_next_ptr;
			child_list_next_ptr = child_list_ptr->is_isolated() ? nullptr : child_list_ptr->right_sibling();
			temporary_heap->head_->link_on_left(child_list_ptr->isolate());
			temporary_heap->head_ = child_list_ptr;
		}
		this->merge(temporary_heap);
		this->size_--;
		T data = min->data();
		delete min;
		return data;
	}
	return 0;
}

template<typename K, typename T>
inline T& BinomialHeap<K, T>::peek()
{
	return this->root_->data();
}

template<typename K, typename T>
inline const T BinomialHeap<K, T>::peek() const
{
	return this->root_->data();
}

template<typename K, typename T>
inline K BinomialHeap<K, T>::peekPriority()
{
	return this->root_->priority();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::merge(BinomialHeap<K, T>* other_heap)
{
	if (this->head_)
	{
		if (other_heap->head_)
		{
			BinomialHeap<K, T>* temporary_heap = new BinomialHeap<K, T>();
			PriorityQueueLinkedItem<K, T>* heap_1_root_list_ptr = this->head_, * heap_2_root_list_ptr = other_heap->head_;
			if (this->head_->degree() < other_heap->head_->degree())
			{
				heap_1_root_list_ptr = heap_1_root_list_ptr->is_isolated() ? nullptr : heap_1_root_list_ptr->right_sibling();
				temporary_heap->head_ = this->head_->isolate();
			}
			else
			{
				heap_2_root_list_ptr = heap_2_root_list_ptr->is_isolated() ? nullptr : heap_2_root_list_ptr->right_sibling();
				temporary_heap->head_ = other_heap->head_->isolate();
			}
			PriorityQueueLinkedItem<K, T>* heap_3_root_list_ptr = temporary_heap->head_;
			while (heap_1_root_list_ptr && heap_2_root_list_ptr)
			{
				if (heap_1_root_list_ptr->degree() < heap_2_root_list_ptr->degree())
				{
					heap_3_root_list_ptr = heap_1_root_list_ptr->is_isolated() ? nullptr : heap_1_root_list_ptr->right_sibling();
					temporary_heap->head_->link_on_left(heap_1_root_list_ptr->isolate());
					heap_1_root_list_ptr = heap_3_root_list_ptr;
				}
				else
				{
					heap_3_root_list_ptr = heap_2_root_list_ptr->is_isolated() ? nullptr : heap_2_root_list_ptr->right_sibling();
					temporary_heap->head_->link_on_left(heap_2_root_list_ptr->isolate());
					heap_2_root_list_ptr = heap_3_root_list_ptr;
				}
			}
			while (heap_1_root_list_ptr)
			{
				heap_3_root_list_ptr = heap_1_root_list_ptr->is_isolated() ? nullptr : heap_1_root_list_ptr->right_sibling();
				temporary_heap->head_->link_on_left(heap_1_root_list_ptr->isolate());
				heap_1_root_list_ptr = heap_3_root_list_ptr;
			}
			while (heap_2_root_list_ptr)
			{
				heap_3_root_list_ptr = heap_2_root_list_ptr->is_isolated() ? nullptr : heap_2_root_list_ptr->right_sibling();
				temporary_heap->head_->link_on_left(heap_2_root_list_ptr->isolate());
				heap_2_root_list_ptr = heap_3_root_list_ptr;
			}
			this->head_ = temporary_heap->head_;
			this->size_ += other_heap->size_;

			temporary_heap->head_ = other_heap->head_ = nullptr;
			delete temporary_heap;

			heap_1_root_list_ptr = this->head_;
			heap_2_root_list_ptr = heap_1_root_list_ptr->right_sibling();
			heap_3_root_list_ptr = nullptr;

			while (heap_2_root_list_ptr != this->head_)
			{
				if (heap_1_root_list_ptr->degree() != heap_2_root_list_ptr->degree() ||
					(heap_2_root_list_ptr->right_sibling()
						!= this->head_ && heap_2_root_list_ptr->degree() == heap_2_root_list_ptr->right_sibling()->degree()))
				{
					heap_3_root_list_ptr = heap_1_root_list_ptr;
					heap_1_root_list_ptr = heap_1_root_list_ptr->right_sibling();
				}
				else
				{
					heap_1_root_list_ptr = heap_1_root_list_ptr->merge_with_right_sibling();
					if (heap_3_root_list_ptr)
					{
						heap_3_root_list_ptr->link_on_right(heap_1_root_list_ptr);
						heap_3_root_list_ptr = heap_3_root_list_ptr->right_sibling();
					}
					else
					{
						this->head_ = heap_1_root_list_ptr;
					}
				}
				heap_2_root_list_ptr = heap_1_root_list_ptr->right_sibling();
			}
		}
	}
	else
	{
		this->head_ = other_heap->head_;
		this->size_ = other_heap->size_;
		other_heap->head_ = nullptr;
	}
	delete other_heap;
	this->find_minimum();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::decrease_key(PriorityQueueItem<K, T>* node, const K& key)
{
	node->priority(key);
	PriorityQueueLinkedItem<K, T>* decreased_node = (PriorityQueueLinkedItem<K, T>*)node;
	decreased_node->heapify();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::find_minimum()
{
	PriorityQueueLinkedItem<K, T>* min = this->head_;
	if (min)
	{
		for (PriorityQueueLinkedItem<K, T>* node = this->head_->right_sibling(); node != this->head_; node = node->right_sibling())
		{
			if (node->priority() < min->priority())
			{
				min = node;
			}
		}
	}
	this->root_ = min;
}
