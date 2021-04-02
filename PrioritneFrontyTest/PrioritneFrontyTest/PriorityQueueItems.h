#pragma once
#include <algorithm>

template <typename K, typename T>
class PriorityQueueItem
{
protected:
	int identifier_;
	K priority_;
	T data_;

	PriorityQueueItem(const int identifier, const K& priority, const T& data);

	void swap(PriorityQueueItem<K, T>* other_tree_item);
public:
	virtual ~PriorityQueueItem();

	const int identifier() const;
	K& priority();
	T& data();
};



template <typename K, typename T>
class BinaryTreeItem : public PriorityQueueItem<K, T>
{
protected:
	BinaryTreeItem<K, T>* left_son_, * right_son_, * parent_;
public:
	BinaryTreeItem(const int identifier, const K& priority, const T& data);
	~BinaryTreeItem();

	virtual BinaryTreeItem* cut();
	virtual BinaryTreeItem* merge(BinaryTreeItem* node);

	virtual BinaryTreeItem* add_left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem* add_right_son(BinaryTreeItem* node);

	BinaryTreeItem*& left_son();
	BinaryTreeItem*& right_son();
	BinaryTreeItem*& parent();

	virtual BinaryTreeItem<K, T>* left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem<K, T>* right_son(BinaryTreeItem* node);
};

template <typename K, typename T>
class DegreeBinaryTreeItem : public BinaryTreeItem<K, T>
{
protected:
	int degree_;
public:
	DegreeBinaryTreeItem(const int identifier, const K& priority, const T& data);

	DegreeBinaryTreeItem<K, T>* add_left_son(BinaryTreeItem<K, T>* node) override;
	
	int& degree();
};

template <typename K, typename T>
class BinaryTreeItemWithAncestor : public DegreeBinaryTreeItem<K, T>
{
protected:
	BinaryTreeItemWithAncestor<K, T>* ordered_ancestor_;

public:
	BinaryTreeItemWithAncestor(const int identifier, const K& priority, const T& data);
	~BinaryTreeItemWithAncestor();

	BinaryTreeItemWithAncestor<K, T>* cut() override;
	BinaryTreeItemWithAncestor<K, T>* add_left_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItemWithAncestor<K, T>* add_right_son(BinaryTreeItem<K, T>* node) override;

	BinaryTreeItemWithAncestor* find_minimal_left_son();
	void swap_with_ordered_ancestor();

	BinaryTreeItemWithAncestor<K, T>* ordered_ancestor();

	BinaryTreeItemWithAncestor<K, T>* left_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItemWithAncestor<K, T>* right_son(BinaryTreeItem<K, T>* node) override;
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
inline BinaryTreeItem<K, T>::BinaryTreeItem(const int identifier, const K& priority, const T& data) :
	PriorityQueueItem<K, T>(identifier, priority, data), left_son_(nullptr), right_son_(nullptr), parent_(nullptr)
{
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>::~BinaryTreeItem()
{
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	if (this->right_son_)
	{
		delete this->right_son_;
		this->right_son_ = nullptr;
	}
	this->parent_ = nullptr;
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
inline BinaryTreeItem<K, T>*& BinaryTreeItem<K, T>::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>*& BinaryTreeItem<K, T>::right_son()
{
	return this->right_son_;
}

template<typename K, typename T>
inline BinaryTreeItem<K, T>*& BinaryTreeItem<K, T>::parent()
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
inline BinaryTreeItemWithAncestor<K, T>::BinaryTreeItemWithAncestor(const int identifier, const K& priority, const T& data) :
	DegreeBinaryTreeItem<K, T>(identifier, priority, data), ordered_ancestor_(nullptr)
{
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>::~BinaryTreeItemWithAncestor()
{
	this->ordered_ancestor_ = nullptr;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::cut()
{
	this->BinaryTreeItem<K, T>::cut();
	this->ordered_ancestor_ = nullptr;
	return this;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::add_left_son(BinaryTreeItem<K, T>* node)
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
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::add_right_son(BinaryTreeItem<K, T>* node)
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
		this->swap(this->ordered_ancestor_);
	}
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::ordered_ancestor()
{
	return this->ordered_ancestor_;
}

template<typename K, typename T>
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::left_son(BinaryTreeItem<K, T>* node)
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
inline BinaryTreeItemWithAncestor<K, T>* BinaryTreeItemWithAncestor<K, T>::right_son(BinaryTreeItem<K, T>* node)
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
	identifier_(identifier), priority_(priority), data_(data)
{
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>::~PriorityQueueItem()
{
}


template<typename K, typename T>
inline const int PriorityQueueItem<K, T>::identifier() const
{
	return this->identifier_;
}

template<typename K, typename T>
inline void PriorityQueueItem<K, T>::swap(PriorityQueueItem<K, T>* other_priority_queue_item)
{
	std::swap(this->identifier_, other_priority_queue_item->identifier_);
	std::swap(this->priority_, other_priority_queue_item->priority_);
	std::swap(this->data_, other_priority_queue_item->data_);
}

template<typename K, typename T>
inline K& PriorityQueueItem<K, T>::priority()
{
	return this->priority_;
}

template<typename K, typename T>
inline T& PriorityQueueItem<K, T>::data()
{
	return this->data_;
}

template<typename K, typename T>
inline DegreeBinaryTreeItem<K, T>::DegreeBinaryTreeItem(const int identifier, const K& priority, const T& data) :
	BinaryTreeItem<K, T>(identifier, priority, data), degree_(0)
{
}

template<typename K, typename T>
inline DegreeBinaryTreeItem<K, T>* DegreeBinaryTreeItem<K, T>::add_left_son(BinaryTreeItem<K, T>* node)
{
	this->degree_++;
	return (DegreeBinaryTreeItem*)this->BinaryTreeItem<K, T>::add_left_son(node);
}

template<typename K, typename T>
inline int& DegreeBinaryTreeItem<K, T>::degree()
{
	return this->degree_;
}