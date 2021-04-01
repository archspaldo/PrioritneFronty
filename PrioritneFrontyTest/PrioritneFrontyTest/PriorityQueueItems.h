#pragma once
#include <algorithm>

template <typename K, typename T>
class PriorityQueueItem;

template <typename K, typename T>
class DataItem
{
public:
	DataItem(const int identifier, const K& priority, const T& data);
	~DataItem() {};
	PriorityQueueItem<K, T>*& tree_item();
	const int identifier() const;
	K& priority();
	T& data();
protected:
	int identifier_;
	K priority_;
	T data_;
	PriorityQueueItem<K, T>* tree_item_;
};

template <typename K, typename T>
class PriorityQueueItem
{
public:
	PriorityQueueItem(const int identifier, const K& priority, const T& data);
	virtual ~PriorityQueueItem();
	DataItem<K, T>*& data_item();
	const int identifier() const;
	K& priority();
	T& data();
protected:
	void swap_data_items(PriorityQueueItem<K, T>* other_tree_item);
	DataItem<K, T>* data_item_;
};

template <typename K, typename T>
class BinaryTreeItem : public PriorityQueueItem<K, T>
{
protected:
	BinaryTreeItem<K, T>* left_son_, * right_son_, * parent_;
	size_t degree_; // item with negative degree is to be deleted, as such it is not be taken into consideration in algorithms
public:
	BinaryTreeItem(const int identifier, const K& priority, const T& data);
	~BinaryTreeItem();

	virtual BinaryTreeItem* cut();
	virtual BinaryTreeItem* merge(BinaryTreeItem* node);

	virtual BinaryTreeItem* add_left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem* add_right_son(BinaryTreeItem* node);
	void weak_link(BinaryTreeItem* node);

	BinaryTreeItem* left_son();
	BinaryTreeItem* right_son();
	BinaryTreeItem* parent();

	virtual BinaryTreeItem<K, T>* left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem<K, T>* right_son(BinaryTreeItem* node);
	BinaryTreeItem<K, T>* parent(BinaryTreeItem* node);

	size_t& degree();
};

template <typename K, typename T>
class BinaryTreeItemWithAncestor : public BinaryTreeItem<K, T>
{
protected:
	BinaryTreeItemWithAncestor<K, T>* ordered_ancestor_;

public:
	BinaryTreeItemWithAncestor(const int identifier, const K& priority, const T& data);
	~BinaryTreeItemWithAncestor();

	BinaryTreeItem<K, T>* cut() override;
	BinaryTreeItem<K, T>* add_left_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItem<K, T>* add_right_son(BinaryTreeItem<K, T>* node) override;

	BinaryTreeItemWithAncestor* find_minimal_left_son();
	void swap_with_ordered_ancestor();

	BinaryTreeItemWithAncestor<K, T>* ordered_ancestor();

	BinaryTreeItem<K, T>* left_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItem<K, T>* right_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItemWithAncestor<K, T>* ordered_ancestor(BinaryTreeItemWithAncestor<K, T>* node);
};

template <typename K, typename T>
class FibonacciHeapItem : public BinaryTreeItemWithAncestor<K, T>
{
protected:
	bool flag_;
public:
	FibonacciHeapItem(const int identifier, const K& priority, const T& data);
	~FibonacciHeapItem();
	bool& flag();
};

template<typename K, typename T>
inline DataItem<K, T>::DataItem(const int identifier, const K& priority, const T& data) :
	identifier_(identifier), priority_(priority), data_(data)
{
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>*& DataItem<K, T>::tree_item()
{
	return this->tree_item_;
}

template<typename K, typename T>
inline K& DataItem<K, T>::priority()
{
	return this->priority_;
}

template<typename K, typename T>
inline T& DataItem<K, T>::data()
{
	return this->data_;
}

template<typename K, typename T>
inline const int DataItem<K, T>::identifier() const
{
	return this->identifier_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>::BinaryTreeItem(const int identifier, const K& priority, const T& data) :
	PriorityQueueItem<K, T>(identifier, priority, data), left_son_(nullptr), right_son_(nullptr), parent_(nullptr), degree_(0)
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>::~BinaryTreeItem()
{
	this->degree_ = -1;
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	if (this->right_son_ && this->right_son_->degree_ != -1)
	{
		delete this->right_son_;
		this->right_son_ = nullptr;
	}
	this->parent_ = nullptr;
	this->degree_ = 0;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::cut()
{
	if (this->parent_)
	{
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son(this->right_son_);
		}
		else
		{
			this->parent_->right_son(this->right_son_);
		}
	}
	else
	{
		if (this->right_son_)
		{
			this->right_son_->parent_ = nullptr;
		}
	}
	this->right_son_ = this->parent_ = nullptr;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::merge(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		if (this->priority() < node->priority())
		{
			return this->add_left_son(node);
		}
		else
		{
			return node->add_left_son(this);
		}
	}
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::add_left_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		this->left_son(node->right_son(this->left_son_));
		this->degree_++;
	}
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::add_right_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		this->right_son(node->right_son(this->right_son_));
	}
	return this;
}

template<typename K, typename T>
inline void BinaryTreeItem<K, T>::weak_link(BinaryTreeItem* node)
{
	if (node)
	{
		if (this->right_son_)
		{
			if (node->right_son_)
			{
				std::swap(this->right_son_, node->right_son_);
			}
			else
			{
				node->right_son_ = this->right_son_;
				this->right_son_ = node;
			}
		}
		else
		{
			if (node->right_son_)
			{
				this->right_son_ = node->right_son_;
				node->right_son_ = this;
			}
			else
			{
				this->right_son_ = node;
				node->right_son_ = this;
			}
		}
		this->parent_ = nullptr;
	}
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::right_son()
{
	return this->right_son_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::parent()
{
	return this->parent_;
}


template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::left_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->left_son_ = node;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::right_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->right_son_ = node;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItem<K, T>::parent(BinaryTreeItem<K, T>* node)
{
	this->parent_ = node;
	return this;
}

template<typename K, typename T>
inline size_t& BinaryTreeItem<K, T>::degree()
{
	return this->degree_;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>::BinaryTreeItemWithAncestor(const int identifier, const K& priority, const T& data) :
	BinaryTreeItem<K, T>(identifier, priority, data), ordered_ancestor_(nullptr)
{
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>::~BinaryTreeItemWithAncestor()
{
	this->ordered_ancestor_ = nullptr;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItemWithAncestor<K, T>::cut()
{
	this->BinaryTreeItem<K, T>::cut();
	this->ordered_ancestor_ = nullptr;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItemWithAncestor<K, T>::add_left_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		BinaryTreeItemWithAncestor* casted_node = (BinaryTreeItemWithAncestor*)node;
		casted_node->ordered_ancestor_ = this;
		this->BinaryTreeItem<K, T>::left_son(casted_node->BinaryTreeItem<K, T>::right_son(this->left_son_));
		this->degree_++;
	}
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItemWithAncestor<K, T>::add_right_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		BinaryTreeItemWithAncestor* casted_node = (BinaryTreeItemWithAncestor*)node;
		casted_node->ordered_ancestor_ = this->ordered_ancestor_;
		this->BinaryTreeItem<K, T>::right_son(casted_node->BinaryTreeItem<K, T>::right_son(this->right_son_));
	}
	return this;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::find_minimal_left_son()
{
	BinaryTreeItem<K, T>* node = this->BinaryTreeItem<K, T>::left_son();
	for (BinaryTreeItem<K, T>* node_ptr = this->BinaryTreeItem<K, T>::left_son(); node_ptr; node_ptr = node_ptr->right_son())
	{
		if (node_ptr->priority() < node->priority())
		{
			node = node_ptr;
		}
	}
	return (BinaryTreeItemWithAncestor*)node;
}

template<typename K, typename T>
inline void BinaryTreeItemWithAncestor<K, T>::swap_with_ordered_ancestor()
{
	if (this->ordered_ancestor_)
	{
		this->swap_data_items(this->ordered_ancestor_);
	}
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::ordered_ancestor()
{
	return this->ordered_ancestor_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItemWithAncestor<K, T>::left_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		BinaryTreeItemWithAncestor* casted_node = (BinaryTreeItemWithAncestor*)node;
		casted_node->parent_ = this;
		if (casted_node->ordered_ancestor_ != this)
		{
			for (BinaryTreeItemWithAncestor* node_ptr = casted_node; node_ptr; node_ptr = (BinaryTreeItemWithAncestor*)(node_ptr->right_son_))
			{
				node_ptr->ordered_ancestor_ = this;
			}
		}
	}
	this->left_son_ = node;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>* BinaryTreeItemWithAncestor<K, T>::right_son(BinaryTreeItem<K, T>* node)
{
	if (node)
	{
		BinaryTreeItemWithAncestor* casted_node = (BinaryTreeItemWithAncestor*)node;
		casted_node->parent_ = this;
		if (casted_node->ordered_ancestor_ != this->ordered_ancestor_)
		{
			for (BinaryTreeItemWithAncestor* node_ptr = casted_node; node_ptr; node_ptr = (BinaryTreeItemWithAncestor*)(node_ptr->right_son_))
			{
				node_ptr->ordered_ancestor_ = this->ordered_ancestor_;
			}
		}
	}
	this->right_son_ = node;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::ordered_ancestor(BinaryTreeItemWithAncestor<K, T>* node)
{
	this->ordered_ancestor_ = node;
	return this;
}

template<typename K, typename T>
inline FibonacciHeapItem<K, T>::FibonacciHeapItem(const int identifier, const K& priority, const T& data) :
	BinaryTreeItemWithAncestor<K, T>(identifier, priority, data), flag_(false)
{
}

template<typename K, typename T>
inline FibonacciHeapItem<K, T>::~FibonacciHeapItem()
{
}

template<typename K, typename T>
inline bool& FibonacciHeapItem<K, T>::flag()
{
	return this->flag_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>::PriorityQueueItem(const int identifier, const K& priority, const T& data) :
	data_item_(new DataItem<K, T>(identifier, priority, data))
{
	data_item_->tree_item() = this;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>::~PriorityQueueItem()
{
	delete this->data_item_;
}

template<typename K, typename T>
inline DataItem<K, T>*& PriorityQueueItem<K, T>::data_item()
{
	return this->data_item_;
}

template<typename K, typename T>
inline const int PriorityQueueItem<K, T>::identifier() const
{
	return this->data_item_->identifier();
}

template<typename K, typename T>
inline void PriorityQueueItem<K, T>::swap_data_items(PriorityQueueItem<K, T>* other_tree_item)
{
	std::swap(this->data_item_, other_tree_item->data_item_);
	std::swap(this->data_item_->tree_item(), other_tree_item->data_item_->tree_item());
}

template<typename K, typename T>
inline K& PriorityQueueItem<K, T>::priority()
{
	return this->data_item_->priority();
}

template<typename K, typename T>
inline T& PriorityQueueItem<K, T>::data()
{
	return this->data_item_->data();
}
