#pragma once
#include "PriorityQueue.h"
#include "LinkedList.h"

template <typename K, typename T>
class BinomialHeap : public PriorityQueue<K, T>
{
public:
	BinomialHeap();
	~BinomialHeap();
	BinomialHeap<K, T>& operator=(BinomialHeap<K, T>&& other);
	void clear() override;
	size_t size() const override;
	void push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(BinomialHeap<K, T>* other_heap);
private:
	class BinomialHeapItem;

	void find_minimum();

	BinomialHeapItem* head_;
	BinomialHeapItem* root_;
	size_t size_;
};

template <typename K, typename T>
class BinomialHeap<K, T>::BinomialHeapItem : public PriorityQueue<K, T>::PriorityQueueItem
{
public:
	BinomialHeapItem(const K& priority, const T& data);
	~BinomialHeapItem();
	BinomialHeapItem* link(BinomialHeapItem* node);
	BinomialHeapItem* add_sub_tree(BinomialHeapItem* node);
	BinomialHeapItem* merge_tree(BinomialHeapItem* node);
	BinomialHeapItem* sibling();
	void sibling(BinomialHeapItem* node);
	BinomialHeapItem* left_son();
	void left_son(BinomialHeapItem* node);

	size_t order();
private:
	BinomialHeapItem* sibling_;
	BinomialHeapItem* left_son_;
	size_t order_;
};


template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeapItem::BinomialHeapItem(const K& priority, const T& data):
	PriorityQueueItem(priority, data), sibling_(nullptr), left_son_(nullptr), order_(1)
{
}

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeapItem::~BinomialHeapItem()
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
	this->order_ = 0;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::link(BinomialHeapItem* node)
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
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::add_sub_tree(BinomialHeapItem* node)
{
	if (node && this->order_ == node->order_)
	{
		node->sibling_ = this->left_son_;
		this->left_son_ = node;
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
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::sibling()
{
	return this->sibling_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::sibling(BinomialHeapItem* node)
{
	this->sibling_ = node;
}

template<typename K, typename T>
inline typename BinomialHeap<K, T>::BinomialHeapItem* BinomialHeap<K, T>::BinomialHeapItem::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::BinomialHeapItem::left_son(BinomialHeapItem* node)
{
	this->left_son_ = node;
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
	this->size_ = 0;
	this->root_ = nullptr;
	delete this->head_;
	this->head_ = nullptr;
}


template<typename K, typename T>
inline BinomialHeap<K, T>& BinomialHeap<K, T>::operator=(BinomialHeap<K, T>&& other)
{
	if (this != &other)
	{
		this->head_ = other.head_;
		this->root_ = other.root_;
		this->size_ = other.size_;
		other.head_ = other.root_ = nullptr;
		other.size_ = 0;
	}
	return *this;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::clear()
{
	this->root_ = nullptr;
	this->size_ = 0;
	delete this->head_;
}

template<typename K, typename T>
inline size_t BinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::push(const K& key, const T& data)
{
	BinomialHeap<K, T> new_heap;
	new_heap.head_ = new BinomialHeapItem(key, data);
	new_heap.size_++;
	this->merge(&new_heap);
}

template<typename K, typename T>
inline T BinomialHeap<K, T>::pop()
{
	if (this->head_)
	{
		BinomialHeap<K, T> temporary_heap;
		BinomialHeapItem* min = this->head_, * prev = nullptr, * next;
		if (min)
		{
			for (BinomialHeapItem* tmp = this->head_; tmp; tmp = tmp->sibling())
			{
				if (tmp->sibling() && tmp->sibling()->priority() < min->priority())
				{
					min = tmp->sibling();
					prev = tmp;
				}
			}
		}
		if (prev)
		{
			prev->sibling(min->sibling());
		}
		else
		{
			this->head_ = this->head_->sibling();
		}

		prev = min->left_son();
		min->left_son(nullptr);
		min->sibling(nullptr);
		while (prev)
		{
			next = prev->sibling();
			prev->sibling(temporary_heap.head_);
			temporary_heap.head_ = prev;
			prev = next;
		}
		this->merge(&temporary_heap);
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
inline void BinomialHeap<K, T>::merge(BinomialHeap<K, T>* other_tree)
{
	if (this->head_)
	{
		if (other_tree->head_)
		{
			BinomialHeap<K, T>* result_heap = new BinomialHeap<K, T>();
			BinomialHeapItem* current_1 = this->head_, * current_2 = other_tree->head_, * current_3 = nullptr;
			if (current_1)
			{
				if (current_1->order() < current_2->order())
				{
					current_3 = current_1;
					current_1 = current_1->sibling();
				}
				else
				{
					current_3 = current_2;
					current_2 = current_2->sibling();
				}
			}
			else
			{
				current_3 = current_2;
			}
			result_heap->head_ = current_3;
			while (current_1 && current_2)
			{
				if (current_1->order() < current_2->order())
				{
					current_3->sibling(current_1);
					current_1 = current_1->sibling();
				}
				else
				{
					current_3->sibling(current_2);
					current_2 = current_2->sibling();
				}
				current_3 = current_3->sibling();
			}
			while (current_1)
			{
				current_3->sibling(current_1);
				current_1 = current_1->sibling();
				current_3 = current_3->sibling();
			}
			while (current_2)
			{
				current_3->sibling(current_2);
				current_2 = current_2->sibling();
				current_3 = current_3->sibling();
			}

			result_heap->size_ = this->size_ + other_tree->size_;

			this->head_ = other_tree->head_ = nullptr;
			this->root_ = other_tree->root_ = nullptr;
			this->size_ = other_tree->size_ = 0;

			if (result_heap->head_)
			{
				current_3 = result_heap->head_;
				current_1 = current_3->sibling();
				current_2 = nullptr;
				while (current_1)
				{
					if (current_3->order() != current_1->order() || (current_1->sibling() && current_1->order() == current_1->sibling()->order()))
					{
						current_2 = current_3;
						current_3 = current_3->sibling();
					}
					else
					{
						current_3 = current_3->link(current_1);
						if (current_2)
						{
							current_2->sibling(current_3);
						}
						else
						{
							result_heap->head_ = current_3;
						}
					}
					current_1 = current_3->sibling();
				}
			}
			result_heap->find_minimum();
			this->head_ = result_heap->head_;
			this->root_ = result_heap->root_;
			this->size_ = result_heap->size_;
			result_heap->head_ = result_heap->root_ = nullptr;
			result_heap->size_ = 0;
			delete result_heap;
		}

	}
	else
	{
		this->head_ = other_tree->head_;
		this->root_ = other_tree->root_;
		this->size_ = other_tree->size_;
		other_tree->head_ = other_tree->root_ = nullptr;
		other_tree->size_ = 0;
	}
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::find_minimum()
{
	BinomialHeapItem* min = this->head_;
	if (min)
	{
		for (BinomialHeapItem* node = this->head_->sibling(); node; node = node->sibling())
		{
			if (node->priority() < min->priority())
			{
				min = node;
			}
		}
	}
	this->root_ = min;
}
