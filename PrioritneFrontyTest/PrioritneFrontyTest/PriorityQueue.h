#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

template <typename K, typename T>
class PriorityQueue
{
protected:
	PriorityQueue();
public:
	virtual ~PriorityQueue();
	virtual size_t size() const = 0;
	virtual DataItem<K, T>* push(const K& priority, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& find_min() = 0;
	virtual void merge(PriorityQueue<K, T>* other_heap) = 0;
	virtual void change_priority(DataItem<K, T>* node, const K& priority) = 0;
};

template <typename K, typename T>
class LazyBinomialHeap : public PriorityQueue<K, T>
{
protected:
	BinaryTreeItem<K, T>* root_;
	size_t size_;
	DataItem<K, T>* push(BinaryTreeItem<K, T>* node);
	virtual void consolidate_root(BinaryTreeItem<K, T>* node) = 0;
	virtual void priority_was_increased(BinaryTreeItem<K, T>* node) = 0;
	virtual void priority_was_decreased(BinaryTreeItem<K, T>* node) = 0;
	void consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size);
	void consolidate_root_using_singlepass(BinaryTreeItem<K, T>* node, size_t array_size);
	LazyBinomialHeap();
public:
	virtual ~LazyBinomialHeap();
	size_t size() const;
	virtual DataItem<K, T>* push(const K& priority, const T& data) = 0;
	T pop();
	T& find_min();
	virtual void merge(PriorityQueue<K, T>* other_heap);
	void change_priority(DataItem<K, T>* node, const K& priority);
};

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size)
{
	size_t node_degree;
	std::vector<BinaryTreeItem<K, T>*> node_list(array_size);
	BinaryTreeItem<K, T>* node_ptr, * node_next_ptr;

	auto combine = [&](BinaryTreeItem<K, T>*& node)
	{
		node_degree = node->degree();
		while (node_list[node_degree])
		{
			node = node->merge(node_list[node_degree]);
			node_list[node_degree++] = nullptr;
		}
		node_list[node_degree] = node;
	};

	if (node)
	{
		for (node_ptr = node, node_next_ptr = node_ptr->right_son(); node_ptr; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_ptr->cut();
			combine(node_ptr);
		}
	}

	if (this->root_)
	{
		node_ptr = this->root_->right_son();
		this->root_->right_son(nullptr);

		if (node_ptr)
		{
			for (node_next_ptr = node_ptr->right_son(); node_ptr != this->root_; node_ptr = node_next_ptr, node_next_ptr = node_ptr->right_son())
			{
				node_ptr->right_son(nullptr);
				combine(node_ptr);
			}
		}

		if (this->root_->degree() != -1)
		{
			combine(this->root_);
		}

		this->root_ = nullptr;
	}

	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			if (this->root_)
			{
				this->root_->add_root_item(node);
				if (node->priority() < this->root_->priority())
				{
					this->root_ = node;
				}
			}
			else
			{
				this->root_ = node;
			}
		}
	}
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::consolidate_root_using_singlepass(BinaryTreeItem<K, T>* node, size_t array_size)
{
	const int degree = array_size;
	size_t node_degree;
	std::vector<BinaryTreeItem<K, T>*> node_list(degree);
	BinaryTreeItem<K, T>* node_ptr, * node_next_ptr, * root = this->root_;

	this->root_ = nullptr;

	if (node)
	{
		for (node_ptr = node, node_next_ptr = node_ptr->right_son(); node_ptr; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			size_t degree = node_ptr->degree();
			node_ptr->cut();
			if (node_list[degree])
			{
				node_ptr = node_ptr->merge(node_list[degree]);
				node_list[degree] = nullptr;
				if (this->root_)
				{
					this->root_->add_root_item(node_ptr);
					if (node_ptr->priority() < this->root_->priority())
					{
						this->root_ = node_ptr;
					}
				}
				else
				{
					this->root_ = node_ptr;
				}
			}
			else
			{
				node_list[degree] = node_ptr;
			}
			
		}
	}

	if (root)
	{
		node_ptr = root->right_son();
		root->right_son(nullptr);

		for (node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr != this->root_; node_ptr = node_next_ptr, node_next_ptr = node_ptr->right_son())
		{
			node_degree = node_ptr->degree();
			node_ptr->right_son(nullptr);
			if (node_list[degree])
			{
				node_ptr = node_ptr->merge(node_list[degree]);
				node_list[degree] = nullptr;
				if (this->root_)
				{
					this->root_->add_root_item(node_ptr);
					if (node_ptr->priority() < this->root_->priority())
					{
						this->root_ = node_ptr;
					}
				}
				else
				{
					this->root_ = node_ptr;
				}
			}
			else
			{
				node_list[degree] = node_ptr;
			}
		}

		if (root->degree() != -1)
		{
			node_ptr = root;
			node_degree = node_ptr->degree();
			if (node_list[degree])
			{
				node_ptr = node_ptr->merge(node_list[degree]);
				node_list[degree] = nullptr;
				if (this->root_)
				{
					this->root_->add_root_item(node_ptr);
					if (node_ptr->priority() < this->root_->priority())
					{
						this->root_ = node_ptr;
					}
				}
				else
				{
					this->root_ = node_ptr;
				}
			}
			else
			{
				node_list[degree] = node_ptr;
			}
		}
	}

	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			if (this->root_)
			{
				this->root_->add_root_item(node);
				if (node->priority() < this->root_->priority())
				{
					this->root_ = node;
				}
			}
			else
			{
				this->root_ = node;
			}
		}
	}
}

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::LazyBinomialHeap() :
	root_(nullptr), size_(0)
{
}

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::~LazyBinomialHeap()
{
	delete this->root_;
	this->root_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t LazyBinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline DataItem<K, T>* LazyBinomialHeap<K, T>::push(BinaryTreeItem<K, T>* node)
{
	if (this->root_)
	{
		this->root_->add_root_item(node);
		if (node->priority() < this->root_->priority())
		{
			this->root_ = node;
		}
	}
	else
	{
		this->root_ = node;
	}
	this->size_++;
	return node->data_item();
}

template<typename K, typename T>
inline T LazyBinomialHeap<K, T>::pop()
{
	if (this->root_)
	{
		BinaryTreeItem<K, T>* root = this->root_;
		root->degree() = -1;
		this->consolidate_root(root->left_son());
		root->left_son(nullptr);
		this->size_--;
		T data = root->data();
		delete root;
		return data;
	}
	throw new std::range_error("LazyBinomialHeap<K, T>::pop(): Priority queue is empty!");
}

template<typename K, typename T>
inline T& LazyBinomialHeap<K, T>::find_min()
{
	return this->root_->data();
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	if (other_heap)
	{
		LazyBinomialHeap<K, T>* heap = (LazyBinomialHeap<K, T>*)other_heap;
		if (this->root_)
		{
			this->root_->add_root_item(heap->root_);
			if (heap->root_ && heap->root_->priority() < this->root_->priority())
			{
				this->root_ = heap->root_;
			}
		}
		else
		{
			this->root_ = heap->root_;
		}
		this->size_ += heap->size_;
		heap->root_ = nullptr;
		delete heap;
	}
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::change_priority(DataItem<K, T>* node, const K& priority)
{
	size_t old_priority = node->priority();
	BinaryTreeItem<K, T>* item = node->tree_item();
	if (priority < old_priority)
	{
		this->priority_was_increased((BinaryTreeItem<K, T>*)item);
	}
	if (priority > old_priority)
	{
		this->priority_was_decreased((BinaryTreeItem<K, T>*)item);
	}
}

template<typename K, typename T>
inline PriorityQueue<K, T>::PriorityQueue()
{
}

template<typename K, typename T>
inline PriorityQueue<K, T>::~PriorityQueue()
{
}
