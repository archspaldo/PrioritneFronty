#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class LazyBinomialHeap : public PriorityQueue<K, T>
{
protected:
	class RootItem;
	RootItem* root_;
	size_t size_;
	DataItem<K, T>* push(BinaryTreeItem<K, T>* node);
	void add_root_item(BinaryTreeItem<K, T>* node);
	void add_root_item(RootItem* node);
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

template <typename K, typename T>
class LazyBinomialHeap<K, T>::RootItem
{
private:
	BinaryTreeItem<K, T>* node_;
	RootItem* next_root_item_;
public:
	RootItem(RootItem* node);
	~RootItem();
	RootItem* add_next_root_item(RootItem* node);
	RootItem* add_next_root_item(BinaryTreeItem<K, T>* node);
	RootItem* split();
	bool is_valid();
	K& priority();
	BinaryTreeItem<K, T>*& node();
	RootItem* next_node();
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
			node_degree = node_ptr->degree();

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
		RootItem* root_node_ptr, * root_node_next_ptr;;
		BinaryTreeItem<K, T>* node_ptr;

		for (root_node_ptr = this->root_->split(), root_node_next_ptr = root_node_ptr->next_node(); root_node_ptr != this->root_;
			root_node_ptr = root_node_next_ptr, root_node_next_ptr = root_node_ptr->next_node())
		{
			node_ptr = root_node_ptr->node();
			root_node_ptr->node() = nullptr;

			node_degree = node_ptr->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;

			delete root_node_ptr;
		}

		if (this->root_->node())
		{
			node_ptr = this->root_->node();
			this->root_->node() = nullptr;

			node_degree = node_ptr->degree();

			while (node_list[node_degree])
			{
				node_ptr = node_ptr->merge(node_list[node_degree]);
				node_list[node_degree++] = nullptr;
			}
			node_list[node_degree] = node_ptr;
		}

		delete this->root_;
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
	RootItem* root = this->root_;
	size_t node_degree;
	this->root_ = nullptr;

	if (node)
	{
		for (BinaryTreeItem<K, T>* node_ptr = node, *node_next_ptr = node_ptr->right_son(); node_ptr;
			node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
		{
			node_ptr->cut();
			node_degree = node_ptr->degree();

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
		RootItem* root_node_ptr, * root_node_next_ptr;;
		BinaryTreeItem<K, T>* node_ptr;

		for (root_node_ptr = root->split(), root_node_next_ptr = root_node_ptr->next_node(); root_node_ptr != root;
			root_node_ptr = root_node_next_ptr, root_node_next_ptr = root_node_ptr->next_node())
		{
			node_ptr = root_node_ptr->node();
			root_node_ptr->node() = nullptr;

			node_degree = node_ptr->degree();

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

			delete root_node_ptr;
		}

		if (root->node())
		{
			node_ptr = root->node();
			root->node() = nullptr;

			node_degree = node_ptr->degree();

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

		delete this->root_;
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
		for (RootItem* node_ptr = this->root_->split(), * next_node = node_ptr->next_node(); node_ptr; node_ptr = next_node, next_node = node_ptr ? node_ptr->next_node() : nullptr)
		{
			delete node_ptr;
		}
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
		RootItem* root_item = this->root_->add_next_root_item(node);
		if (root_item->priority() < this->root_->priority())
		{
			this->root_ = root_item;
		}
	}
	else
	{
		this->root_ = RootItem(node);
	}
}

template<typename K, typename T>
inline void LazyBinomialHeap<K, T>::add_root_item(RootItem* node)
{
	this->root_->add_next_root_item(node);
	if (this->root_->priority() < node->priority())
	{
		this->root_ = node;
	}
}

template<typename K, typename T>
inline T LazyBinomialHeap<K, T>::pop(int& identifier)
{
	if (this->root_)
	{
		BinaryTreeItem<K, T>* root = this->root_->node();
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
	if (this->root_)
	{
		return this->root_->node()->data();
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

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::RootItem::RootItem(RootItem* node) :
	node_(node), next_root_item_(this)
{
}

template<typename K, typename T>
inline LazyBinomialHeap<K, T>::RootItem::~RootItem()
{
	delete this->node_;
	this->next_root_item_ = nullptr;
	this->node_ = nullptr;
}

template<typename K, typename T>
inline typename LazyBinomialHeap<K, T>::RootItem* LazyBinomialHeap<K, T>::RootItem::add_next_root_item(RootItem* node)
{
	if (node)
	{
		std::swap(this->next_root_item_, node->next_root_item_);
	}
	return this;
}

template<typename K, typename T>
inline typename LazyBinomialHeap<K, T>::RootItem* LazyBinomialHeap<K, T>::RootItem::add_next_root_item(BinaryTreeItem<K, T>* node)
{
	RootItem* new_node = new RootItem(node);
	new_node->next_root_item_ = this->next_root_item_;
	this->next_root_item_ = new_node;
	return new_node;
}

template<typename K, typename T>
inline typename LazyBinomialHeap<K, T>::RootItem* LazyBinomialHeap<K, T>::RootItem::split()
{
	RootItem* node = this->next_root_item_;
	this->next_root_item_ = nullptr;
	return node;
}

template<typename K, typename T>
inline bool LazyBinomialHeap<K, T>::RootItem::is_valid()
{
	return this->node_;
}

template<typename K, typename T>
inline K& LazyBinomialHeap<K, T>::RootItem::priority()
{
	return this->node_->priority();
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>*& LazyBinomialHeap<K, T>::RootItem::node()
{
	return this->node_;
}

template<typename K, typename T>
inline typename LazyBinomialHeap<K, T>::RootItem* LazyBinomialHeap<K, T>::RootItem::next_node()
{
	return this->next_root_item_;
}
