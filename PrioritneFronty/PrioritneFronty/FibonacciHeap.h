#pragma once
#include "PriorityQueue.h"
#include <vector>
#include <math.h>

template <typename K, typename T>
class FibonacciHeap : public PriorityQueue<K, T>
{
public:
	FibonacciHeap();
	~FibonacciHeap();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& priority, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(FibonacciHeap<K, T>* other_heap);
	void decrease_key(PriorityQueueItem<K, T>* node, const K& key);
	
private:
	class FibonacciHeapItem : public PriorityQueueLinkedItem<K, T>
	{
	public:
		FibonacciHeapItem(const K& priority, const T& data) :
			PriorityQueueLinkedItem<K, T>(priority, data), flag_(false) {};
		~FibonacciHeapItem() {};
		bool flag()
		{
			return this->flag_;
		};
		void flag(bool flag)
		{
			this->flag_ = flag;
		};
	private:
		bool flag_;
	};
	
	void cut(FibonacciHeapItem* node);
	void cascading_cut(FibonacciHeapItem* node);
	void consolidate();
	FibonacciHeapItem* head_;
	FibonacciHeapItem* root_;
	size_t size_;
};

template<typename K, typename T>
inline FibonacciHeap<K, T>::FibonacciHeap() :
	head_(nullptr), root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline FibonacciHeap<K, T>::~FibonacciHeap()
{
	delete this->head_;
	this->head_ = this->root_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t FibonacciHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* FibonacciHeap<K, T>::push(const K& priority, const T& data)
{
	FibonacciHeapItem* node = new FibonacciHeapItem(priority, data);
	if (this->head_)
	{
		this->head_->link_on_right(node);
		if (node->priority() < this->root_->priority())
		{
			this->root_ = node;
		}
	}
	else
	{
		this->head_ = this->root_ = node;
	}
	this->size_++;
	return node;
}

template<typename K, typename T>
inline T FibonacciHeap<K, T>::pop()
{
	if (this->root_)
	{
		if (this->head_->is_isolated())
		{
			this->head_ = (FibonacciHeapItem*)this->root_->first_son();
		}
		else
		{
			if (this->head_ == this->root_)
			{
				this->head_ = (FibonacciHeapItem*)this->head_->right_sibling();
			}
			this->head_->link_on_right(this->root_->isolate()->first_son());
		}
		this->root_->first_son(nullptr);
		FibonacciHeapItem* node = this->root_;
		this->consolidate();
		this->size_--;
		T data = node->data();
		delete node;
		return data;
	}
	throw new std::logic_error("FibonacciHeap<K, T>::pop(): Priority queue is empty");
}

template<typename K, typename T>
inline T& FibonacciHeap<K, T>::peek()
{
	return this->root_->data();
}

template<typename K, typename T>
inline const T FibonacciHeap<K, T>::peek() const
{
	return this->root_->data();
}

template<typename K, typename T>
inline K FibonacciHeap<K, T>::peekPriority()
{
	return this->root_->priority();
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::merge(FibonacciHeap<K, T>* other_heap)
{
	if (other_heap)
	{
		if (this->head_)
		{
			this->head_->link_on_left(other_heap->head);
			this->size_ += other_heap->size_;
			if (other_heap->root_ && other_heap->root_->priority() < this->root_->priority())
			{
				this->root_ = other_heap->root_;
			}
		}
		else
		{
			this->head_ = other_heap->head_;
			this->size_ = other_heap->size_;
			this->root_ = other_heap->root_;
		}
		other_heap->head_ = nullptr;
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::decrease_key(PriorityQueueItem<K, T>* node, const K& key)
{
	node->priority(key);
	FibonacciHeapItem* decreased_node = (FibonacciHeapItem*)node;
	if (decreased_node->parent() && decreased_node->priority() < decreased_node->parent()->priority)
	{
		this->cut(decreased_node);
		this->cascading_cut(decreased_node->parent());
	}
	if (decreased_node->priority() < this->root_->priority())
	{
		this->root_ = decreased_node;
	}
}



template<typename K, typename T>
inline void FibonacciHeap<K, T>::consolidate()
{
	constexpr auto RESIZE = 2.1;
	int node_degree;
	const int degree = (int)(log(this->size_) * RESIZE);
	std::vector<PriorityQueueLinkedItem<K, T>*> node_list(degree);

	for (PriorityQueueLinkedItem<K, T>* node = this->head_, * right_sibling = node && !node->is_isolated() ? node->right_sibling() : nullptr; node;
		node = right_sibling, right_sibling = (node && !node->is_isolated()) ? node->right_sibling() : nullptr)
	{
		node_degree = node->degree();
		node->isolate();
		while (node_list[node_degree])
		{
			node = node->merge(node_list[node_degree]);
			node_list[node_degree++] = nullptr;
		}
		node_list[node_degree] = node;
	}

	this->head_ = this->root_ = nullptr;
	for (int i = 0; i < degree; i++)
	{
		if (node_list[i])
		{
			if (this->head_)
			{
				this->head_->link_on_right((FibonacciHeapItem*)node_list[i]);
				this->head_ = (FibonacciHeapItem*)node_list[i];
				if (node_list[i]->priority() < this->root_->priority())
				{
					this->root_ = (FibonacciHeapItem*)node_list[i];
				}
			}
			else
			{
				this->head_ = this->root_ = (FibonacciHeapItem*)node_list[i];
			}
			node_list[i] = nullptr;
		}
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::cut(FibonacciHeapItem* node)
{
	if (node->is_isolated())
	{
		node->parent()->first_son(nullptr);
	}
	else
	{
		node->isolate();
	}
	node->parent()->degree(node->parent()->degree() - 1);
	this->head_->link_on_right(node);
	node->flag(false);
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::cascading_cut(FibonacciHeapItem* node)
{
	if (node->parent())
	{
		if (node->flag())
		{
			this->cut(node);
			this->cascading_cut(node->parent());
		}
		else
		{
			node->flag(true);
		}
	}
}
