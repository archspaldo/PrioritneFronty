#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class LazyBinomialHeap : public PriorityQueue<K, T>
{
protected:
	BinaryTreeItem<K, T>* root_;
	size_t size_;
	DataItem<K, T>* push(BinaryTreeItem<K, T>* node);
	void add_root_item(BinaryTreeItem<K, T>* node);
	virtual void consolidate_root(BinaryTreeItem<K, T>* node) = 0;
	void consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size);
	void consolidate_root_using_singlepass(BinaryTreeItem<K, T>* node, size_t array_size);
	LazyBinomialHeap();
public:
	virtual ~LazyBinomialHeap();
	void clear() override;
	size_t size() const override;
	virtual void push(const int identifier, const K& priority, const T& data, DataItem<K, T>*&) = 0;
	T pop(int& identifier) override;
	T& find_min() override;
	virtual void merge(PriorityQueue<K, T>* other_heap) override;
};

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size)
{
	auto combine = [](std::vector<BinaryTreeItem<K, T>*>& node_list, BinaryTreeItem<K, T>* node, BinaryTreeItem<K, T>* node_end = nullptr)
	{
		BinaryTreeItem<K, T>* node_ptr, * node_next_ptr;
		size_t node_degree;

		for (node_ptr = node, node_next_ptr = node_ptr->right_son(); node_ptr != node_end; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_ptr->cut();
			node_degree = node_ptr->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
		}
	};

	std::vector<BinaryTreeItem<K, T>*> node_list(array_size);

	if (node)
	{
		combine(node_list, node);
	}

	if (this->root_)
	{
		BinaryTreeItem<K, T>* node_ptr = this->root_->right_son();
		this->root_->right_son(nullptr);

		if (node_ptr)
		{
			combine(node_list, node_ptr, this->root_);
		}

		if (this->root_->degree() != -1)
		{
			combine(node_list, this->root_);
		}

		this->root_ = nullptr;
	}

	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			this->add_root_item(node);
		}
	}
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::consolidate_root_using_singlepass(BinaryTreeItem<K, T>* node, size_t array_size)
{
	auto combine = [](std::vector<BinaryTreeItem<K, T>*>& node_list, BinaryTreeItem<K, T>*& root, BinaryTreeItem<K, T>* node, BinaryTreeItem<K, T>* node_end = nullptr)
	{
		BinaryTreeItem<K, T>* node_ptr, * node_next_ptr;
		size_t degree;
		for (node_ptr = node, node_next_ptr = node_ptr->right_son(); node_ptr != node_end; node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			degree = node_ptr->degree();
			node_ptr->cut();

			if (node_list[degree])
			{
				node_ptr = node_ptr->merge(node_list[degree]);
				node_list[degree] = nullptr;
				if (root)
				{
					root->weak_link(node_ptr);
					if (node->priority() < root->priority())
					{
						root = node_ptr;
					}
				}
				else
				{
					root = node_ptr;
				}
			}
			else
			{
				node_list[degree] = node_ptr;
			}
		}
	};

	std::vector<BinaryTreeItem<K, T>*> node_list(array_size);
	BinaryTreeItem<K, T>* node_ptr, * node_next_ptr, * root = this->root_;
	this->root_ = nullptr;

	if (node)
	{
		combine(node_list, this->root_, node);
	}

	if (root)
	{
		node_ptr = root->right_son();
		root->right_son(nullptr);

		if (node_ptr)
		{
			combine(node_list, this->root_, node_ptr, root);
		}

		if (root->degree() != -1)
		{
			combine(node_list, this->root_, root);
		}
	}

	for (BinaryTreeItem<K, T>* node : node_list)
	{
		if (node)
		{
			this->add_root_item(node);
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
	this->clear();
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::clear()
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
	this->add_root_item(node);
	this->size_++;
	return node->data_item();
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::add_root_item(BinaryTreeItem<K, T>* node)
{
	if (this->root_)
	{
		this->root_->weak_link(node);
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

template<typename K, typename T>
inline T LazyBinomialHeap<K, T>::pop(int& identifier)
{
	if (this->root_)
	{
		BinaryTreeItem<K, T>* root = this->root_;
		root->degree() = -1;
		this->consolidate_root(root->left_son());
		root->left_son(nullptr);
		this->size_--;
		T data = root->data();
		identifier = root->identifier();
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
		this->add_root_item(heap->root_);
		this->size_ += heap->size_;
		heap->root_ = nullptr;
		delete heap;
	}
}