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
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(BinomialHeap<K, T>* other_heap);
	void decrease_key(PriorityQueueItem<K, T>* node, const K& key);
private:
	class BinomialHeapItem;
	void find_minimum();
	BinomialHeapItem* head_;
	BinomialHeapItem* root_;
	size_t size_;
};

template <typename K, typename T>
class BinomialHeap<K, T>::BinomialHeapItem : public PriorityQueueItem<K, T>
{
public:
	BinomialHeapItem(const K& priority, const T& data);
	~BinomialHeapItem();
	bool heapify();
	BinomialHeapItem* link(BinomialHeapItem* node);
	BinomialHeapItem* add_sub_tree(BinomialHeapItem* node);
	BinomialHeapItem* merge_tree(BinomialHeapItem* node);
	BinomialHeapItem* parent();
	void parent(BinomialHeapItem* node);
	BinomialHeapItem* right_sibling();
	void right_sibling(BinomialHeapItem* node);
	BinomialHeapItem* left_sibling();
	void left_sibling(BinomialHeapItem* node);
	BinomialHeapItem* first_child();
	void first_child(BinomialHeapItem* node);
	size_t order();
private:
	BinomialHeapItem* parent_;
	BinomialHeapItem* right_sibling_;
	BinomialHeapItem* left_sibling_;
	BinomialHeapItem* first_child_;
	size_t order_;
};


template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeapItem::BinomialHeapItem(const K& priority, const T& data):
	PriorityQueueItem<K, T>(priority, data), parent_(nullptr), right_sibling_(nullptr), left_sibling_(nullptr), first_child_(nullptr), order_(0)
{
}

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeapItem::~BinomialHeapItem()
{
	if (this->right_sibling_)
	{
		delete this->right_sibling_;
		this->right_sibling_ = nullptr;
	}
	if (this->first_child_)
	{
		delete this->first_child_;
		this->first_child_ = nullptr;
	}
	this->parent_ = this->left_sibling_ = nullptr;
	this->order_ = 0;
}

template<typename K, typename T>
inline bool BinomialHeap<K, T>::BinomialHeapItem::heapify()
{
	if (this->parent_)
	{
		if (this->priority() < this->parent_->priority())
		{
			std::swap(this->right_sibling_, this->parent_->right_sibling_);
			std::swap(this->first_child_, this->parent_->first_child_);

		}
	}
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::link(BinomialHeapItem* node)
{
	if (this->priority() < node->priority())
	{
		this->right_sibling_ = node->right_sibling_;
		if (this->right_sibling_)
		{
			this->right_sibling_->left_sibling_ = this;
		}
		return this->add_sub_tree(node);
	}
	else
	{
		node->left_sibling_ = this->left_sibling_;
		if (node->left_sibling_)
		{
			node->left_sibling_->right_sibling_ = node;
		}
		return node->add_sub_tree(this);
	}
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::add_sub_tree(BinomialHeapItem* node)
{
	if (node)
	{
		if (this->first_child_)
		{
			this->first_child_->left_sibling_ = node;
		}
		else
		{
			this->first_child_ = node;
		}
		node->right_sibling_ = this->first_child_;
		node->left_sibling_ = nullptr;
		this->first_child_ = node;
		this->order_++;
	}
	return this;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::merge_tree(BinomialHeapItem* node)
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
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::parent(BinomialHeapItem* node)
{
	this->parent_ = node;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::right_sibling()
{
	return this->right_sibling_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::right_sibling(BinomialHeapItem* node)
{
	this->right_sibling_ = node;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::left_sibling()
{
	return this->left_sibling_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::left_sibling(BinomialHeapItem* node)
{
	this->left_sibling_ = node;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::first_child()
{
	return this->first_child_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::first_child(BinomialHeapItem* node)
{
	this->first_child_ = node;
}

template<typename K, typename T>
inline size_t BinomialHeap<K, T>::BinomialHeapItem::order()
{
	return this->order_;
}

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
inline PriorityQueueItem<K, T>* BinomialHeap<K, T>::push(const K& key, const T& data)
{
	BinomialHeap<K, T>* temporary_heap = new BinomialHeap<K, T>();
	BinomialHeapItem* new_item = new BinomialHeapItem(key, data);
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
		BinomialHeapItem* min = this->root_, * child_list_ptr = this->root_->first_child(), * child_list_next_ptr;

		this->root_->first_child(nullptr);
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
			BinomialHeapItem* heap_1_root_list_ptr = this->head_, * heap_2_root_list_ptr = other_heap->head_;
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
			BinomialHeapItem* heap_3_root_list_ptr = temporary_heap->head_;
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
