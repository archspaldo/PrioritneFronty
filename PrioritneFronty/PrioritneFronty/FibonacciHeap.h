#pragma once
#include "Array.h"
#include "PriorityQueue.h"
#include "LinkedList.h"
#include <math.h>

constexpr auto RESIZE = 2.1;

template <typename K, typename T>
class FibbonaciHeapItem : public PriorityQueueItem<K, T>
{
public:
	FibbonaciHeapItem(const K& priority, const T& data);
	~FibbonaciHeapItem();
	const int degree();
	void degree(const int degree);
	FibbonaciHeapItem<K, T>* parent();
	void parent(FibbonaciHeapItem<K, T>* parent);
	LinkedList<FibbonaciHeapItem<K, T>*>* children();
	void children(LinkedList<FibbonaciHeapItem<K, T>*>* children);
	const bool mark();
	void mark(const bool mark);
private:
	int degree_;
	FibbonaciHeapItem<K, T>* parent_;
	LinkedList<FibbonaciHeapItem<K, T>*>* children_;
	bool mark_;

};

template <typename K, typename T>
class FibbonaciHeap : public PriorityQueue<K, T>
{
public:
	FibbonaciHeap();
	~FibbonaciHeap();
	FibbonaciHeap<K, T>& operator=(const FibbonaciHeap<K, T>& other);
	FibbonaciHeap<K, T>& operator=(FibbonaciHeap<K, T>&& other);
	void clear() override;
	size_t size() const override;
	void push(const K& priority, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void decrease_key(const K& key, const K& new_key);
	static FibbonaciHeap<K, T>& unite(FibbonaciHeap<K, T>& other1, FibbonaciHeap<K, T>& other2);
private:
	void consolidate();
	void link(FibbonaciHeapItem<K, T>* node1, FibbonaciHeapItem<K, T>* node2);
	LinkedList<FibbonaciHeapItem<K, T>*>* rootList_;
	size_t size_;
	FibbonaciHeapItem* root_
};

template<typename K, typename T>
inline FibbonaciHeap<K, T>::FibbonaciHeap() :
	rootList_(nullptr), size_(0), root_(nullptr)
{
}

template<typename K, typename T>
inline FibbonaciHeap<K, T>::~FibbonaciHeap()
{
}

template<typename K, typename T>
inline FibbonaciHeap<K, T>& FibbonaciHeap<K, T>::operator=(const FibbonaciHeap<K, T>& other)
{
	// TODO: insert return statement here
}

template<typename K, typename T>
inline FibbonaciHeap<K, T>& FibbonaciHeap<K, T>::operator=(FibbonaciHeap<K, T>&& other)
{
	// TODO: insert return statement here
}

template<typename K, typename T>
inline void FibbonaciHeap<K, T>::clear()
{
}

template<typename K, typename T>
inline size_t FibbonaciHeap<K, T>::size() const
{
	return size_t();
}

template<typename K, typename T>
inline void FibbonaciHeap<K, T>::push(const K& priority, const T& data)
{
	FibbonaciHeapItem<K, T>* node = new FibbonaciHeapItem<K, T>(priority, data);
	if (!this->root_)
	{
		this->rootList_ = new LinkedList<FibbonaciHeapItem<K, T>*>();
		this->root_ = node;
	}
	else
	{
		if (node->priority() < this->root_->priority())
		{
			this->root_ = node;
		}
	}
	this->rootList_->add(node);
	this->size_++;
}

template<typename K, typename T>
inline T FibbonaciHeap<K, T>::pop()
{
	FibbonaciHeapItem<K, T>* node = this->root_;
	if (node)
	{
		for (FibbonaciHeapItem<K, T>* child : *node->children())
		{
			this->rootList_->add(child);
			child->degree(0);
		}
		this->rootList_->remove(node);
		if (this->rootList_->empty())
		{
			this->root_ = nullptr;
		}
		else
		{
			this->root_ = (*this->rootList_)[0];
			this->consolidate();
		}
		this->size_--;
	}
	T data = node ? node->data() : 0;
	delete node;
	return data;
}

template<typename K, typename T>
inline T& FibbonaciHeap<K, T>::peek()
{
	return this->root_->data();
}

template<typename K, typename T>
inline const T FibbonaciHeap<K, T>::peek() const
{
	return this->root_->data();
}

template<typename K, typename T>
inline K FibbonaciHeap<K, T>::peekPriority()
{
	return this->root_->priority();
}

template<typename K, typename T>
inline void FibbonaciHeap<K, T>::decrease_key(const K& key, const K& new_key)
{

}

template<typename K, typename T>
inline FibbonaciHeap<K, T>& FibbonaciHeap<K, T>::unite(FibbonaciHeap<K, T>& other1, FibbonaciHeap<K, T>& other2)
{
	FibbonaciHeap<K, T>* heap = new FibbonaciHeap<K, T>*();
	heap->root_ = other1->root_;
	heap->rootList_ = LinkedList<T>::concatenate(other1.rootList_, other2.rootList_);
	if (!heap->root_ || (other2.root_ && other2.root_->priority() < heap->root_->priority()))
	{
		heap->root_ = other2.root_;
	}
	heap->size_ = other1.size_ + other2.size_;
	other1.rootList_ = other2.rootList_ = nullptr;
	other1.root_ = other2.root_ = nullptr;
	other1.size_ = other2.size_ = 0;
	return *heap;
}

template<typename K, typename T>
inline void FibbonaciHeap<K, T>::consolidate()
{
	FibbonaciHeapItem<K, T>* node_x, node_y;
	int node_degree;
	int degree = (int)(log(this->size_) * RESIZE);
	Array<FibbonaciHeapItem<K, T>*>* node_list = new Array<FibbonaciHeapItem<K, T>*>(degree);
	for (FibbonaciHeapItem<K, T>* node : *this->rootList_)
	{
		node_x = node;
		node_degree = node_x.degree();
		while ((*node_list)[node_degree])
		{
			node_y = (*node_list)[node_degree];
			if (node_x->priority() > node_y.priority())
			{
				std::swap(node_x, node_y);
			}
			this->link(node_x, node_y);
			(*node_list)[node_degree++] = nullptr;
		}
		(*node_list)[node_degree] = node_x;
	}
	this->root_ = nullptr;
	for (int i = 0; i < degree; i++)
	{
		if ((*node_list)[i])
		{
			this->rootList_->add((*node_list)[i]);
			if (this->root_)
			{
				if ((*node_list)[i]->priority() < this->root_->priority())
				{
					this->root_ = (*node_list)[i];
				}
			}
			else
			{
				this->root_ = (*node_list)[i];
			}
		}
	}
}

template<typename K, typename T>
inline void FibbonaciHeap<K, T>::link(FibbonaciHeapItem<K, T>* node1, FibbonaciHeapItem<K, T>* node2)
{
	this->rootList_->remove(node2);
	node1->children()->add(node2);
	node1->degree(node1->degree()++);
	node2->mark(false);
}

template<typename K, typename T>
inline FibbonaciHeapItem<K, T>::FibbonaciHeapItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data),
	degree_(0), parent_(nullptr), children_(nullptr), mark_(false)
{
}

template<typename K, typename T>
inline FibbonaciHeapItem<K, T>::~FibbonaciHeapItem()
{
	this->parent_ = nullptr;
	delete this->children_;
	this->degree_ = 0;
	this->mark_ = false;
}

template<typename K, typename T>
inline const int FibbonaciHeapItem<K, T>::degree()
{
	return this->degree_;
}

template<typename K, typename T>
inline void FibbonaciHeapItem<K, T>::degree(const int degree)
{
	this->degree_ = degree;
}

template<typename K, typename T>
inline FibbonaciHeapItem<K, T>* FibbonaciHeapItem<K, T>::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline void FibbonaciHeapItem<K, T>::parent(FibbonaciHeapItem<K, T>* parent)
{
	this->parent_ = parent;
}

template<typename K, typename T>
inline LinkedList<FibbonaciHeapItem<K, T>*>* FibbonaciHeapItem<K, T>::children()
{
	return this->children_;
}

template<typename K, typename T>
inline void FibbonaciHeapItem<K, T>::children(LinkedList<FibbonaciHeapItem<K, T>*>* children)
{
	this->children_ = children;
}

template<typename K, typename T>
inline const bool FibbonaciHeapItem<K, T>::mark()
{
	return this->mark_;
}

template<typename K, typename T>
inline void FibbonaciHeapItem<K, T>::mark(const bool mark)
{
	this->mark_ = mark;
}
