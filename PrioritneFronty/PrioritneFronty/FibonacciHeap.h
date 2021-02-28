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
	void clear() override;
	size_t size() const override;
	void push(const K& priority, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(FibonacciHeap<K, T>* other_heap);
private:
	class FibonacciHeapItem;
	void consolidate();
	FibonacciHeapItem* head_;
	FibonacciHeapItem* foot_;
	size_t size_;
	FibonacciHeapItem* root_;
};

template <typename K, typename T>
class FibonacciHeap<K, T>::FibonacciHeapItem : public PriorityQueueItem<K, T>
{
public:
	FibonacciHeapItem(const K& priority, const T& data);
	~FibonacciHeapItem();
	const int degree();
	void degree(const int degree);
	FibonacciHeapItem* link(FibonacciHeapItem* node);
	FibonacciHeapItem* add_sub_tree(FibonacciHeapItem* node);
	void connect(FibonacciHeapItem* begin_node, FibonacciHeapItem* end_node);
	void connect(FibonacciHeapItem* node);
	FibonacciHeapItem* isolate();
	FibonacciHeapItem* left_son();
	void left_son(FibonacciHeapItem* node);
	FibonacciHeapItem* right_son();
	void right_son(FibonacciHeapItem* node);
	FibonacciHeapItem* left_sibling();
	void left_sibling(FibonacciHeapItem* node);
	FibonacciHeapItem* right_sibling();
	void right_sibling(FibonacciHeapItem* node);
	const bool mark();
	void mark(const bool mark);
private:
	int degree_; 
	FibonacciHeapItem* left_son_;
	FibonacciHeapItem* right_son_;
	FibonacciHeapItem* left_sibling_;
	FibonacciHeapItem* right_sibling_;
	bool mark_;
};

template<typename K, typename T>
inline FibonacciHeap<K, T>::FibonacciHeap() :
	head_(nullptr), root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline FibonacciHeap<K, T>::~FibonacciHeap()
{
	this->clear();
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::clear()
{
	delete this->head_;
	this->head_ = this->root_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t FibonacciHeap<K, T>::size() const
{
	return size_t();
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::push(const K& priority, const T& data)
{
	FibonacciHeapItem* node = new FibonacciHeapItem(priority, data);
	if (this->head_)
	{
		this->head_->connect(node);
		this->head_ = node;
		if (node->priority() < this->root_->priority())
		{
			this->root_ = node;
		}
	}
	else
	{
		this->head_ = this->foot_ = this->root_ = node;
	}
	this->size_++;
}

template<typename K, typename T>
inline T FibonacciHeap<K, T>::pop()
{
	FibonacciHeapItem* node = this->root_;
	if (node)
	{
		if (node->left_son())
		{
			FibonacciHeap<K, T>* new_heap = new FibonacciHeap<K, T>();
			new_heap->head_ = new_heap->root_ = node->left_son();
			new_heap->foot_ = node->right_son();
			node->left_son(nullptr);
			node->right_son(nullptr);
			this->merge(new_heap);
		}
		if (this->head_ == this->root_)
		{
			this->head_ = this->head_->right_sibling();
		}
		this->root_->isolate();
		if (!this->head_)
		{
			this->root_ = nullptr;
		}
		else
		{
			this->root_ = this->head_;
			this->consolidate();
		}
		this->size_--;
		T data = node->data();
		delete node;
		return data;
	}
	return 0;
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
		if (other_heap->head_)
		{
			other_heap->head_->connect(this->head_, this->foot_);
			this->size_ += other_heap->size_;
			if (!(this->root_ && this->root_->priority() < other_heap->root_->priority()))
			{
				this->root_ = other_heap->root_;
			}
		}
		if (!this->head_)
		{
			this->head_ = other_heap->head_;
		}
		this->foot_ = other_heap->foot_;
		other_heap->root_ = other_heap->head_ = nullptr;
		delete other_heap;
	}
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::consolidate()
{
	constexpr auto RESIZE = 2.1;
	int node_degree;
	const int degree = (int)(log(this->size_) * RESIZE);
	std::vector<FibonacciHeapItem*> node_list(degree);
	for (FibonacciHeapItem* node = this->head_, *right_sibling = this->head_ ? this->head_->right_sibling() : nullptr; node;
		node = right_sibling, right_sibling = node ? node->right_sibling() : nullptr)
	{
		node_degree = node->degree();
		while (node_list[node_degree])
		{
			node = node->link(node_list[node_degree]);
			node_list[node_degree++] = nullptr;
		}
		node_list[node_degree] = node->isolate();
	}
	this->head_ = this->root_ = nullptr;
	for (int i = 0; i < degree; i++)
	{
		if (node_list[i])
		{
			if (this->head_)
			{
				this->head_->connect(node_list[i]);
				this->head_ = node_list[i];
				if (node_list[i]->priority() < this->root_->priority())
				{
					this->root_ = node_list[i];
				}
			}
			else
			{
				this->head_ = this->foot_ = this->root_ = node_list[i];
			}
			node_list[i] = nullptr;
		}
	}
}

template<typename K, typename T>
inline FibonacciHeap<K, T>::FibonacciHeapItem::FibonacciHeapItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data),
	degree_(0), left_son_(nullptr), right_son_(nullptr), right_sibling_(nullptr), left_sibling_(nullptr), mark_(false)
{
}

template<typename K, typename T>
inline FibonacciHeap<K, T>::FibonacciHeapItem::~FibonacciHeapItem()
{
	if (this->right_sibling_)
	{
		delete this->right_sibling_;
		this->right_sibling_ = nullptr;
	}
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	this->left_sibling_ = this->right_son_ = nullptr;
	this->degree_ = 0;
	this->mark_ = false;
}

template<typename K, typename T>
inline const int FibonacciHeap<K, T>::FibonacciHeapItem::degree()
{
	return this->degree_;
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::degree(const int degree)
{
	this->degree_ = degree;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::link(FibonacciHeapItem* node)
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

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::add_sub_tree(FibonacciHeapItem* node)
{
	if (node)
	{
		if (this->left_son_)
		{
			this->left_son_->left_sibling_ = node;
		}
		else
		{
			this->right_son_ = node;
		}
		node->right_sibling_ = this->left_son_;
		node->left_sibling_ = nullptr;
		this->left_son_ = node;
		this->degree_++;
	}
	return this;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::connect(FibonacciHeapItem* begin_node, FibonacciHeapItem* end_node)
{
	if (this->left_sibling_)
	{	
		this->left_sibling_->right_sibling_ = begin_node;
	}
	begin_node->left_sibling_ = this->left_sibling_;
	end_node->right_sibling_ = this;
	this->left_sibling_ = end_node;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::connect(FibonacciHeapItem* node)
{
	if (this->left_sibling_)
	{
		this->left_sibling_->right_sibling_ = node;
	}
	node->left_sibling_ = this->left_sibling_;
	node->right_sibling_ = this;
	this->left_sibling_ = node;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::isolate()
{
	if (this->left_sibling_)
	{
		this->left_sibling_->right_sibling_ = this->right_sibling_;
	}
	if (this->right_sibling_)
	{
		this->right_sibling_->left_sibling_ = this->left_sibling_;
	}
	this->left_sibling_ = this->right_sibling_ = nullptr;
	return this;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::left_son()
{
	return this->left_son_;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::left_son(FibonacciHeapItem* node)
{
	this->left_son_ = node;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::right_son()
{
	return this->right_son_;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::right_son(FibonacciHeapItem* node)
{
	this->right_son_ = node;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::left_sibling()
{
	return this->left_sibling_;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::left_sibling(FibonacciHeapItem* node)
{
	this->left_sibling_ = node;
}

template <typename K, typename T>
inline typename FibonacciHeap<K, T>::FibonacciHeapItem* FibonacciHeap<K, T>::FibonacciHeapItem::right_sibling()
{
	return this->right_sibling_;
}

template <typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::right_sibling(FibonacciHeapItem* node)
{
	this->right_sibling_ = node;
}

template<typename K, typename T>
inline const bool FibonacciHeap<K, T>::FibonacciHeapItem::mark()
{
	return this->mark_;
}

template<typename K, typename T>
inline void FibonacciHeap<K, T>::FibonacciHeapItem::mark(const bool mark)
{
	this->mark_ = mark;
}
