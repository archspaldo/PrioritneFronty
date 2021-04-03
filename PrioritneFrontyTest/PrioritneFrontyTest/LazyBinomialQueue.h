#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class LazyBinomialHeap : public PriorityQueue<K, T>
{
protected:
	BinaryTreeItem<K, T>* root_;
	size_t size_;
	PriorityQueueItem<K, T>* push(BinaryTreeItem<K, T>* node);
	void add_root_item(BinaryTreeItem<K, T>* node);
	virtual void consolidate_root(BinaryTreeItem<K, T>* node) = 0;
	void consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size);
	void consolidate_root_using_singlepass(BinaryTreeItem<K, T>* node, size_t array_size);
	LazyBinomialHeap();
public:
	virtual ~LazyBinomialHeap();
	void clear() override;
	size_t size() const override;
	virtual void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*&) = 0;
	T pop(int& identifier) override;
	T& find_min() override;
	virtual void merge(PriorityQueue<K, T>* other_heap) override;
};


template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::consolidate_root_using_multipass(BinaryTreeItem<K, T>* node, size_t array_size)
{
	std::vector<BinaryTreeItem<K, T>*> node_list(array_size);
	size_t node_degree;

	if (node)
	{
		for (BinaryTreeItem<K, T>* node_ptr = node, * node_next_ptr = node_ptr->right_son(); node_ptr;
			node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_ptr->cut();
			node_degree = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
		}
	}

	if (this->root_)
	{
		BinaryTreeItem<K, T>* node_ptr = this->root_->right_son();
		this->root_->right_son() = nullptr;

		for (BinaryTreeItem<K, T>* node_next_ptr = node_ptr->right_son(); node_ptr != this->root_;
			node_ptr = node_next_ptr, node_next_ptr = node_ptr->right_son())
		{
			node_ptr->right_son() = nullptr;
			node_degree = static_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
		}

		if (!this->root_->parent())
		{
			node_ptr = this->root_;
			node_degree = static_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
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
	std::vector<BinaryTreeItem<K, T>*> node_list(array_size);
	BinaryTreeItem<K, T>* root = this->root_;
	size_t node_degree;
	this->root_ = nullptr;

	if (node)
	{
		for (BinaryTreeItem<K, T>* node_ptr = node, * node_next_ptr = node_ptr->right_son(); node_ptr;
			node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_ptr->cut();
			node_degree = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			if (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree] = nullptr;
				this->add_root_item(node_ptr);
			}
			else
			{
				node_list[node_degree] = node_ptr;
			}
		}
	}

	if (root)
	{
		BinaryTreeItem<K, T>* node_ptr = this->root_->right_son();
		this->root_->right_son() = nullptr;

		for (BinaryTreeItem<K, T>*  node_next_ptr = node_ptr->right_son(); node_ptr != root;
			node_ptr = node_next_ptr, node_next_ptr = node_ptr->right_son())
		{
			node_ptr->right_son() = nullptr;

			node_degree = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			if (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree] = nullptr;
				this->add_root_item(node_ptr);
			}
			else
			{
				node_list[node_degree] = node_ptr;
			}

		}

		if (!root->parent())
		{
			node_ptr = root;
			node_degree = dynamic_cast<DegreeBinaryTreeItem<K, T>*>(node_ptr)->degree();

			if (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree] = nullptr;
				this->add_root_item(node_ptr);
			}
			else
			{
				node_list[node_degree] = node_ptr;
			}
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
	if (this->root_)
	{
		BinaryTreeItem<K, T>* root = this->root_->right_son();
		this->root_->right_son(nullptr);
		delete root;
	}
	this->root_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t LazyBinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* LazyBinomialHeap<K, T>::push(BinaryTreeItem<K, T>* node)
{
	this->add_root_item(node);
	this->size_++;
	return node;
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::add_root_item(BinaryTreeItem<K, T>* node)
{
	if (this->root_)
	{
		if (node->right_son())
		{
			std::swap(this->root_->right_son(), node->right_son());
		}
		else
		{
			node->right_son() = this->root_->right_son();
			this->root_->right_son() = node;
		}
		if (node->priority() < this->root_->priority())
		{
			this->root_ = node;
		}
	}
	else
	{
		this->root_ = node;
		this->root_->right_son() = this->root_;
	}
}


template<typename K, typename T>
inline T LazyBinomialHeap<K, T>::pop(int& identifier)
{
	if (this->root_)
	{
		
		BinaryTreeItem<K, T>* root = this->root_;
		this->root_->parent() = this->root_;
		this->consolidate_root(root->left_son());
		root->left_son(nullptr);
		this->size_--;
		T data = root->data();
		identifier = root->identifier();
		//std::cout << "LH\t" << root->priority() << "\t" << root->identifier() << "\n";
		delete root;
		return data;
	}
	throw new std::range_error("LazyBinomialHeap<K, T>::pop(): Priority queue is empty!");
}

template<typename K, typename T>
inline T& LazyBinomialHeap<K, T>::find_min()
{
	if (this->root_)
	{
		return this->root_->data();
	}
	throw new std::range_error("LazyBinomialHeap<K, T>::find_min(): Priority queue is empty!");
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
		delete other_heap;
	}
}
