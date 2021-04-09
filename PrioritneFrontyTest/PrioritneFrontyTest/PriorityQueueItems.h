#pragma once
#include <algorithm>

template <typename Priority, typename Data>
class PriorityQueueItem
{
protected:
	int identifier_;
	Priority priority_;
	Data data_;

	PriorityQueueItem(const int identifier, const Priority& priority, const Data& data);
public:
	virtual ~PriorityQueueItem();

	const int identifier() const;
	Priority& priority();
	Data& data();

	bool operator<(PriorityQueueItem<Priority, Data>& operand);
};

template <typename Priority, typename Data>
class ArrayItem : public PriorityQueueItem<Priority, Data>
{
protected:
	int index_;
public:
	ArrayItem(const int identifier, const Priority& priority, const Data& data, const int index);

	int& index();
};

template <typename Priority, typename Data>
class BinaryTreeItem : public PriorityQueueItem<Priority, Data>
{
protected:
	BinaryTreeItem<Priority, Data>* left_son_, * right_son_, * parent_;

	void swap_with_parent();
public:
	BinaryTreeItem(const int identifier, const Priority& priority, const Data& data);
	~BinaryTreeItem();

	virtual BinaryTreeItem* cut();
	virtual BinaryTreeItem* merge(BinaryTreeItem* node);
	
	virtual BinaryTreeItem* add_left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem* add_right_son(BinaryTreeItem* node);
	BinaryTreeItem* highest_priority_son();
	BinaryTreeItem* ancestor();
	virtual void swap_with_ancestor_node(BinaryTreeItem* node);

	BinaryTreeItem*& left_son();
	BinaryTreeItem*& right_son();
	BinaryTreeItem*& parent();

	virtual BinaryTreeItem* left_son(BinaryTreeItem* node);
	virtual BinaryTreeItem* right_son(BinaryTreeItem* node);

};

template <typename Priority, typename Data>
class DegreeBinaryTreeItem : public BinaryTreeItem<Priority, Data>
{
protected:
	int degree_;
public:
	DegreeBinaryTreeItem(const int identifier, const Priority& priority, const Data& data);

	void swap_with_ancestor_node(BinaryTreeItem<Priority, Data>* node) override;
	DegreeBinaryTreeItem* add_left_son(BinaryTreeItem<Priority, Data>* node) override;
	int& degree();
};

template <typename Priority, typename Data>
class FibonacciHeapItem : public DegreeBinaryTreeItem<Priority, Data>
{
protected:
	bool flag_;
	FibonacciHeapItem<Priority, Data>* ordered_ancestor_;
public:
	FibonacciHeapItem(const int identifier, const Priority& priority, const Data& data);
	~FibonacciHeapItem();

	FibonacciHeapItem* cut() override;
	FibonacciHeapItem* add_left_son(BinaryTreeItem<Priority, Data>* node) override;
	FibonacciHeapItem* add_right_son(BinaryTreeItem<Priority, Data>* node) override;

	FibonacciHeapItem*& ordered_ancestor();
	bool& flag();

	FibonacciHeapItem* left_son(BinaryTreeItem<Priority, Data>* node) override;
};

template <typename Priority, typename Data>
inline ArrayItem<Priority, Data>::ArrayItem(const int identifier, const Priority& priority, const Data& data, const int index) :
	PriorityQueueItem<Priority, Data>(identifier, priority, data), index_(index)
{
};

template <typename Priority, typename Data>
inline int& ArrayItem<Priority, Data>::index()
{
	return this->index_;
};

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>::BinaryTreeItem(const int identifier, const Priority& priority, const Data& data) :
	PriorityQueueItem<Priority, Data>(identifier, priority, data), left_son_(nullptr), right_son_(nullptr), parent_(nullptr)
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>::~BinaryTreeItem()
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

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::cut()
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

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::merge(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		if (*this < *node)
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

template<typename Priority, typename Data>
inline void BinaryTreeItem<Priority, Data>::swap_with_parent()
{
	if (this->parent_)
	{
		BinaryTreeItem* parent = this->parent_, * son;
		if (parent->parent_)
		{
			if (parent->parent_->left_son_ == parent)
			{
				parent->parent_->left_son(this);
			}
			else
			{
				parent->parent_->right_son(this);
			}
		}
		else
		{
			this->parent_ = nullptr;
		}
		if (parent->left_son_ == this)
		{
			parent->left_son(this->left_son_);
			this->left_son(parent);
			son = this->right_son();
			this->right_son_ = parent->right_son_;
			if (this->right_son_ && this->right_son_->parent_)
			{
				this->right_son_->parent_ = this;
			}
			parent->right_son(son);
		}
		else
		{
			parent->right_son(this->right_son_);
			this->right_son(parent);
			son = this->left_son();
			this->left_son(parent->left_son_);
			parent->left_son(son);
		}
	}
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		this->left_son(node->right_son(this->left_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::add_right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		this->right_son(node->right_son(this->right_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::highest_priority_son()
{
	BinaryTreeItem<Priority, Data>* node = this, * node_ptr = this->left_son_;
	while (node_ptr)
	{
		if (*node_ptr < *node)
		{
			node = node_ptr;
		}
		node_ptr = node_ptr->right_son_;
	}
	return node == this ? nullptr : node;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::ancestor()
{
	BinaryTreeItem* node_ptr = this, * parent_ptr = this->parent_;
	while (parent_ptr && parent_ptr->right_son_ == node_ptr)
	{
		node_ptr = parent_ptr;
		parent_ptr = parent_ptr->parent_;
	}
	return parent_ptr;
}

template<typename Priority, typename Data>
inline void BinaryTreeItem<Priority, Data>::swap_with_ancestor_node(BinaryTreeItem* node)
{
	if (node)
	{
		if (this->parent() == node)
		{
			this->swap_with_parent();
			return;
		}
		BinaryTreeItem* parent = node->parent_, * son = this->left_son_;
		this->left_son(node->left_son_);
		node->left_son(son);
		std::swap(this->right_son_, node->right_son_);
		if (this->right_son_ && this->right_son_->parent_)
		{
			this->right_son_->parent_ = this;
		}
		if (node->right_son_)
		{
			node->right_son_->parent_ = node;
		}
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son(node);
		}
		else
		{
			this->parent_->right_son(node);
		}
		if (parent)
		{
			if (parent->left_son_ == node)
			{
				parent->left_son(this);
			}
			else
			{
				parent->right_son(this);
			}
		}
		else
		{
			this->parent_ = nullptr;
		}
	}
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::left_son()
{
	return this->left_son_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::right_son()
{
	return this->right_son_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::parent()
{
	return this->parent_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->left_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->right_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>::FibonacciHeapItem(const int identifier, const Priority& priority, const Data& data) :
	DegreeBinaryTreeItem<Priority, Data>(identifier, priority, data), ordered_ancestor_(nullptr), flag_(false)
{
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>::~FibonacciHeapItem()
{
	this->ordered_ancestor_ = nullptr;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::cut()
{
	this->BinaryTreeItem<Priority, Data>::cut();
	this->ordered_ancestor_ = nullptr;
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->ordered_ancestor_ = this;
		this->BinaryTreeItem<Priority, Data>::left_son(casted_node->BinaryTreeItem<Priority, Data>::right_son(this->left_son_));
		this->degree_++;
	}
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::add_right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->ordered_ancestor_ = this->ordered_ancestor_;
		this->BinaryTreeItem<Priority, Data>::right_son(casted_node->BinaryTreeItem<Priority, Data>::right_son(this->right_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>*& FibonacciHeapItem<Priority, Data>::ordered_ancestor()
{
	return this->ordered_ancestor_;
}

template<typename Priority, typename Data>
inline bool& FibonacciHeapItem<Priority, Data>::flag()
{
	return this->flag_;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->parent_ = this;
		if (casted_node->ordered_ancestor_ != this)
		{
			for (FibonacciHeapItem* node_ptr = casted_node; node_ptr; node_ptr = (FibonacciHeapItem*)(node_ptr->right_son_))
			{
				node_ptr->ordered_ancestor_ = this;
			}
		}
	}
	this->left_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline PriorityQueueItem<Priority, Data>::PriorityQueueItem(const int identifier, const Priority& priority, const Data& data) :
	identifier_(identifier), priority_(priority), data_(data)
{
}

template<typename Priority, typename Data>
inline PriorityQueueItem<Priority, Data>::~PriorityQueueItem()
{
}

template<typename Priority, typename Data>
inline const int PriorityQueueItem<Priority, Data>::identifier() const
{
	return this->identifier_;
}

template<typename Priority, typename Data>
inline Priority& PriorityQueueItem<Priority, Data>::priority()
{
	return this->priority_;
}

template<typename Priority, typename Data>
inline Data& PriorityQueueItem<Priority, Data>::data()
{
	return this->data_;
}

template<typename Priority, typename Data>
inline bool PriorityQueueItem<Priority, Data>::operator<(PriorityQueueItem<Priority, Data>& operand)
{
	return this->priority_ <= operand.priority_ && (this->priority_ < operand.priority_ || this->identifier_ <= operand.identifier_);
}

template<typename Priority, typename Data>
inline DegreeBinaryTreeItem<Priority, Data>::DegreeBinaryTreeItem(const int identifier, const Priority& priority, const Data& data) :
	BinaryTreeItem<Priority, Data>(identifier, priority, data), degree_(0)
{
}

template<typename Priority, typename Data>
inline void DegreeBinaryTreeItem<Priority, Data>::swap_with_ancestor_node(BinaryTreeItem<Priority, Data>* node)
{
	std::swap(this->degree_, ((DegreeBinaryTreeItem*)node)->degree_);
	this->BinaryTreeItem<Priority, Data>::swap_with_ancestor_node(node);
}

template<typename Priority, typename Data>
inline DegreeBinaryTreeItem<Priority, Data>* DegreeBinaryTreeItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	this->degree_++;
	return (DegreeBinaryTreeItem*)this->BinaryTreeItem<Priority, Data>::add_left_son(node);
}

template<typename Priority, typename Data>
inline int& DegreeBinaryTreeItem<Priority, Data>::degree()
{
	return this->degree_;
}