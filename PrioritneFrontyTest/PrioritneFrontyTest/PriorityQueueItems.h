#pragma once
#include <algorithm>

template <typename K, typename T>
class PriorityQueueItem
{
public:
	PriorityQueueItem(const K& priority, const T& data);
	virtual ~PriorityQueueItem() {};
	K& priority();
	T& data();
protected:
	K priority_;
	T data_;
};

template <typename K, typename T>
class BinaryTreeItem : public PriorityQueueItem<K, T>
{
protected:
	BinaryTreeItem<K, T>* left_son_, * right_son_, * parent_;
	size_t degree_;
public:
	BinaryTreeItem(const K& priority, const T& data);
	~BinaryTreeItem();

	virtual BinaryTreeItem* cut();
	virtual BinaryTreeItem* merge(BinaryTreeItem* node);

	virtual BinaryTreeItem* add_left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem* add_right_son(BinaryTreeItem* node);
	void add_root_item(BinaryTreeItem* node);

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
	BinaryTreeItemWithAncestor(const K& priority, const T& data);
	~BinaryTreeItemWithAncestor();

	BinaryTreeItem<K, T>* cut() override;

	BinaryTreeItem<K, T>* add_left_son(BinaryTreeItem<K, T>* node) override;
	BinaryTreeItem<K, T>* add_right_son(BinaryTreeItem<K, T>* node) override;
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
	FibonacciHeapItem(const K& priority, const T& data);
	~FibonacciHeapItem();
	bool& flag();
};

template<typename K, typename T>
inline PriorityQueueItem<K, T>::PriorityQueueItem(const K& priority, const T& data) :
	priority_(priority), data_(data)
{
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
inline BinaryTreeItem<K, T>::BinaryTreeItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data), left_son_(nullptr), right_son_(nullptr), parent_(nullptr), degree_(0)
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
		if (this->priority_ < node->priority_)
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
inline void BinaryTreeItem<K, T>::add_root_item(BinaryTreeItem* node)
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
inline BinaryTreeItemWithAncestor<K, T>::BinaryTreeItemWithAncestor(const K& priority, const T& data) :
	BinaryTreeItem<K, T>(priority, data), ordered_ancestor_(nullptr)
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
inline void BinaryTreeItemWithAncestor<K, T>::swap_with_ordered_ancestor()
{
	if (this->ordered_ancestor_)
	{
		if (this->parent_ == this->ordered_ancestor_)
		{
			BinaryTreeItemWithAncestor<K, T>* node = this->right_son_, * ordered_ancestor = (BinaryTreeItemWithAncestor<K, T>*)(this->ordered_ancestor_);
			if (ordered_ancestor->parent_)
			{
				if (ordered_ancestor_->parent_ == ordered_ancestor_->ordered_ancestor_)
				{
					ordered_ancestor_->parent_->left_son_ = this;
				}
				else
				{
					ordered_ancestor_->parent_->right_son = this;
				}
			}

			this->parent_ = ordered_ancestor_->parent_;
			this->ordered_ancestor_ = ordered_ancestor_->ordered_ancestor_;

			ordered_ancestor->parent_ = this;
			this->BinaryTreeItem<K, T>::right_son(ordered_ancestor->right_son_);
			ordered_ancestor->BinaryTreeItem<K, T>::right_son(node);


			this->parent_->left_son(this->left_son_);
			this->left_son(this->parent_);
			this->right_son(this->parent_->right_son_);
			this->parent_->right_son(node);
			std::swap(this->degree_, this->parent_->degree_);
		}
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
inline FibonacciHeapItem<K, T>::FibonacciHeapItem(const K& priority, const T& data) :
	BinaryTreeItemWithAncestor<K, T>(priority, data)
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
