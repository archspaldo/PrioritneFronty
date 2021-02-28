#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class BinomialHeap : public PriorityQueue<K, T>
{
public:
	BinomialHeap();
	~BinomialHeap();
	void clear() override;
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) const override;
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
	this->clear();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::clear()
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
inline PriorityQueueItem<K, T>* BinomialHeap<K, T>::push(const K& key, const T& data) const
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
		if (this->root_->left_sibling())
		{
			this->root_->left_sibling()->right_sibling(this->root_->right_sibling());
		}
		else
		{
			this->head_ = this->head_->right_sibling();
		}

		BinomialHeap<K, T>* temporary_heap = new BinomialHeap<K, T>();
		PriorityQueueLinkedItem<K, T>* min = this->root_, * child_list_ptr = this->root_->first_son(), * child_list_next_ptr;

		this->root_->first_son(nullptr);
		this->root_->right_sibling(nullptr);

		while (child_list_ptr)
		{
			child_list_next_ptr = child_list_ptr->right_sibling();
			child_list_ptr->parent(nullptr);
			child_list_ptr->right_sibling(temporary_heap->head_);
			temporary_heap->head_ = child_list_ptr;
			child_list_ptr = child_list_next_ptr;
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
			if (this->head_)
			{
				if (this->head_->order() < other_heap->head_->order())
				{
					temporary_heap->head_ = this->head_;
					heap_1_root_list_ptr = this->head_->right_sibling();
				}
				else
				{
					temporary_heap->head_ = other_heap->head_;
					heap_2_root_list_ptr = other_heap->head_->right_sibling();
				}
			}
			else
			{
				temporary_heap->head_ = other_heap->head_;
			}
			PriorityQueueLinkedItem<K, T>* heap_3_root_list_ptr = temporary_heap->head_;
			while (heap_1_root_list_ptr && heap_2_root_list_ptr)
			{
				if (heap_1_root_list_ptr->order() < heap_2_root_list_ptr->order())
				{
					heap_3_root_list_ptr->right_sibling(heap_1_root_list_ptr);
					heap_1_root_list_ptr = heap_1_root_list_ptr->right_sibling();
				}
				else
				{
					heap_3_root_list_ptr->right_sibling(heap_2_root_list_ptr);
					heap_2_root_list_ptr = heap_2_root_list_ptr->right_sibling();
				}
				heap_3_root_list_ptr = heap_3_root_list_ptr->right_sibling();
			}
			while (heap_1_root_list_ptr)
			{
				heap_3_root_list_ptr->right_sibling(heap_1_root_list_ptr);
				heap_1_root_list_ptr = heap_1_root_list_ptr->right_sibling();
				heap_3_root_list_ptr = heap_3_root_list_ptr->right_sibling();
			}
			while (heap_2_root_list_ptr)
			{
				heap_3_root_list_ptr->right_sibling(heap_2_root_list_ptr);
				heap_2_root_list_ptr = heap_2_root_list_ptr->right_sibling();
				heap_3_root_list_ptr = heap_3_root_list_ptr->right_sibling();
			}

			temporary_heap->size_ = this->size_ + other_heap->size_;

			this->head_ = temporary_heap->head_;
			this->size_ = temporary_heap->size_;

			temporary_heap->head_ = other_heap->head_ = nullptr;
			delete temporary_heap;

			if (this->head_)
			{
				heap_1_root_list_ptr = this->head_;
				heap_2_root_list_ptr = this->head_->right_sibling();
				heap_3_root_list_ptr = nullptr;
				while (heap_2_root_list_ptr)
				{
					if (heap_1_root_list_ptr->order() != heap_2_root_list_ptr->order() ||
						(heap_2_root_list_ptr->right_sibling() && heap_2_root_list_ptr->order() == heap_2_root_list_ptr->right_sibling()->order()))
					{
						heap_3_root_list_ptr = heap_1_root_list_ptr;
						heap_1_root_list_ptr = heap_1_root_list_ptr->right_sibling();
					}
					else
					{
						heap_1_root_list_ptr = heap_1_root_list_ptr->link(heap_2_root_list_ptr);
						if (heap_3_root_list_ptr)
						{
							heap_3_root_list_ptr->right_sibling(heap_1_root_list_ptr);
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
	}
	else
	{
		this->head_ = other_heap->head_;
		other_heap->head_ = nullptr;	
	}
	delete other_heap;
	this->find_minimum();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::decrease_key(PriorityQueueItem<K, T>* node, const K& key)
{
	node->priority(key);
	BinomialHeapItem* decreased_node = (BinomialHeapItem*)node;

}

template<typename K, typename T>
inline void BinomialHeap<K, T>::find_minimum()
{
	BinomialHeapItem* min = this->head_;
	if (min)
	{
		for (BinomialHeapItem* node = this->head_->right_sibling(); node; node = node->right_sibling())
		{
			if (node->priority() < min->priority())
			{
				min = node;
			}
		}
	}
	this->root_ = min;
}
