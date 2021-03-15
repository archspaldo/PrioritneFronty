#pragma once

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
class PQBinaryTreeItem : public PriorityQueueItem<K, T>
{
public:
	PQBinaryTreeItem(const K& priority, const T& data);
	~PQBinaryTreeItem();
	PQBinaryTreeItem<K, T>* merge(PQBinaryTreeItem<K, T>* node);
	virtual PQBinaryTreeItem<K, T>* cut();
	PQBinaryTreeItem<K, T>* left_son();
	PQBinaryTreeItem<K, T>* right_son();
	PQBinaryTreeItem<K, T>* parent();
	size_t degree();
	void left_son(PQBinaryTreeItem<K, T>* node);
	void right_son(PQBinaryTreeItem<K, T>* node);
	void parent(PQBinaryTreeItem<K, T>* node);
	void degree(size_t degree);
protected:
	PQBinaryTreeItem<K, T>* add_subtree(PQBinaryTreeItem<K, T>* node);

	PQBinaryTreeItem<K, T>* left_son_;
	PQBinaryTreeItem<K, T>* right_son_;
	PQBinaryTreeItem<K, T>* parent_;
	size_t degree_;
};

template <typename K, typename T>
class PQBinaryTreeItemA : public PQBinaryTreeItem<K, T>
{
public:
	PQBinaryTreeItemA(const K& priority, const T& data);
	~PQBinaryTreeItemA();
	PQBinaryTreeItem<K, T>* cut() override;
	PQBinaryTreeItemA<K, T>* ancestral_parent();
	void swap_with_ancestral_parent();
	void ancestral_parent(PQBinaryTreeItemA<K, T>* node);
protected:
	PQBinaryTreeItemA<K, T>* ancestral_parent_;
};


template <typename K, typename T>
class PQDoubleLinkedItem : public PriorityQueueItem<K, T>
{
public:
	PQDoubleLinkedItem(const K& priority, const T& data);
	~PQDoubleLinkedItem();

	void heapify();
	PQDoubleLinkedItem<K, T>* merge_with_right_sibling();
	PQDoubleLinkedItem<K, T>* merge_with_left_sibling();
	PQDoubleLinkedItem<K, T>* merge(PQDoubleLinkedItem<K, T>* node);
	void link_on_right(PQDoubleLinkedItem<K, T>* node);
	void link_on_left(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* isolate();
	void update_parent(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* minimal_son();
	PQDoubleLinkedItem<K, T>* parent();
	void parent(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* left_sibling();
	void left_sibling(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* right_sibling();
	void right_sibling(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* first_son();
	void first_son(PQDoubleLinkedItem<K, T>* node);
	size_t degree();
	void degree(size_t degree);

	bool is_isolated()
	{
		return this->right_sibling_ == this;
	}
protected:
	virtual PQDoubleLinkedItem<K, T>* add_sub_tree(PQDoubleLinkedItem<K, T>* node);
	PQDoubleLinkedItem<K, T>* swap_with_parent();
private:
	PQDoubleLinkedItem<K, T>* parent_;
	PQDoubleLinkedItem<K, T>* left_sibling_;
	PQDoubleLinkedItem<K, T>* right_sibling_;
	PQDoubleLinkedItem<K, T>* first_son_;
	size_t degree_;
};

template<typename K, typename T>
inline PriorityQueueItem<K, T>::PriorityQueueItem(const K& priority, const T& data) :
	priority_(priority), data_(data)
{
}

template<typename K, typename T>
inline K& PriorityQueueItem<K, T>::priority()
{
	return priority_;
}

template<typename K, typename T>
inline T& PriorityQueueItem<K, T>::data()
{
	return data_;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>::PQDoubleLinkedItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data), parent_(nullptr), first_son_(nullptr), degree_(0)
{
	this->right_sibling_ = this;
	this->left_sibling_ = this;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>::~PQDoubleLinkedItem()
{
	if (this->left_sibling_)
	{
		this->left_sibling_->right_sibling_ = nullptr;
		this->left_sibling_ = nullptr;
	}
	if (this->right_sibling_)
	{
		delete this->right_sibling_;
		this->right_sibling_ = nullptr;
	}
	if (this->first_son_)
	{
		delete this->first_son_;
		this->first_son_ = nullptr;
	}
	this->parent_ = nullptr;
	this->degree_ = 0;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::heapify()
{
	while (this->parent_ && this->priority() < this->parent_->priority())
	{
		this->swap_with_parent();
	}
	PQDoubleLinkedItem<K, T>* minimal_son = this->minimal_son();
	while (minimal_son && minimal_son->priority_ < this->priority_)
	{
		minimal_son->swap_with_parent();
		minimal_son = this->minimal_son();
	}
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::merge_with_right_sibling()
{
	if (this->right_sibling_ == this)
	{
		return this;
	}
	if (this->right_sibling_ == this->left_sibling_)
	{
		return this->merge(this->right_sibling_->isolate());
	}
	PQDoubleLinkedItem<K, T>* left_sibling = this->left_sibling_, * right_sibling = this->right_sibling_->isolate();
	left_sibling->link_on_right(this->isolate()->merge(right_sibling));
	return left_sibling->right_sibling_;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::merge_with_left_sibling()
{
	if (this->left_sibling_ == this)
	{
		return this;
	}
	if (this->right_sibling_ == this->left_sibling_)
	{
		return this->merge(this->left_sibling_->isolate());
	}
	PQDoubleLinkedItem<K, T>* right_sibling = this->right_sibling_, * left_sibling = this->left_sibling_->isolate();
	right_sibling->link_on_right(this->isolate()->merge(left_sibling));
	return right_sibling->left_sibling_;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::merge(PQDoubleLinkedItem<K, T>* node)
{
	if (this->priority() < node->priority())
	{
		return this->add_sub_tree(node);
	}
	else
	{
		return node->add_sub_tree(this);
	}
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::link_on_right(PQDoubleLinkedItem<K, T>* node)
{
	if (node)
	{
		node->update_parent(this->parent_);
		this->right_sibling_->left_sibling_ = node->left_sibling_;
		node->left_sibling_->right_sibling_ = this->right_sibling_;
		node->left_sibling_ = this;
		this->right_sibling_ = node;
	}
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::link_on_left(PQDoubleLinkedItem<K, T>* node)
{
	if (node)

	{
		node->update_parent(this->parent_);
		this->left_sibling_->right_sibling_ = node;
		node->left_sibling_->right_sibling_ = this;
		node->left_sibling_ = this->left_sibling_;
		this->left_sibling_ = node;
	}
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::parent(PQDoubleLinkedItem<K, T>* node)
{
	this->parent_ = node;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::left_sibling()
{
	return this->left_sibling_;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::left_sibling(PQDoubleLinkedItem<K, T>* node)
{
	this->left_sibling_ = node;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::right_sibling()
{
	return this->right_sibling_;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::right_sibling(PQDoubleLinkedItem<K, T>* node)
{
	this->right_sibling_ = node;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::first_son()
{
	return this->first_son_;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::first_son(PQDoubleLinkedItem<K, T>* node)
{
	this->first_son_ = node;
}

template<typename K, typename T>
inline size_t PQDoubleLinkedItem<K, T>::degree()
{
	return this->degree_;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::degree(size_t degree)
{
	this->degree_ = degree;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::add_sub_tree(PQDoubleLinkedItem<K, T>* node)
{
	if (this->first_son_)
	{
		this->first_son_->link_on_left(node);
	}
	else
	{
		node->parent(this);
	}
	this->first_son_ = node;
	this->degree_++;
	return this;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::swap_with_parent()
{
	PQDoubleLinkedItem<K, T>* parent = this->parent_, * left_sibling = this->left_sibling_ != this ? this->left_sibling_ : nullptr,
		* parent_left_sibling = parent->left_sibling_ != parent ? parent->left_sibling_ : nullptr, * first_son = this->first_son_;
	parent->isolate();
	this->isolate();

	if (parent_left_sibling)
	{
		parent_left_sibling->link_on_right(this);
	}
	else
	{
		this->parent_ = parent->parent_;
	}
	if (parent->parent_ && parent->parent_->first_son_ == parent)
	{
		parent->parent_->first_son_ = this;
	}
	if (parent->first_son_ == this)
	{
		this->first_son_ = parent;
	}
	else
	{
		this->first_son_ = parent->first_son_;
	}

	if (left_sibling)
	{
		left_sibling->update_parent(this);
		left_sibling->link_on_right(parent);
	}
	else
	{
		parent->parent_ = this;
	}
	parent->first_son_ = first_son;

	return this;
}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::isolate()
{
	if (!this->is_isolated())
	{
		this->right_sibling_->left_sibling_ = this->left_sibling_;
		this->left_sibling_->right_sibling_ = this->right_sibling_;
		this->right_sibling_ = this;
		this->left_sibling_ = this;
	}
	this->parent_ = nullptr;
	return this;
}

template<typename K, typename T>
inline void PQDoubleLinkedItem<K, T>::update_parent(PQDoubleLinkedItem<K, T>* node)
{
	PQDoubleLinkedItem<K, T>* son_ptr = this;
	son_ptr->parent_ = node;
	for (son_ptr = son_ptr->right_sibling_; son_ptr != this; son_ptr = son_ptr->right_sibling_)
	{
		son_ptr->parent_ = node;
	}

}

template<typename K, typename T>
inline PQDoubleLinkedItem<K, T>* PQDoubleLinkedItem<K, T>::minimal_son()
{
	PQDoubleLinkedItem<K, T>* min = this->first_son_;
	if (min)
	{
		for (PQDoubleLinkedItem<K, T>* node_ptr = this->first_son_->right_sibling_; node_ptr != this->first_son_; node_ptr = node_ptr->right_sibling_)
		{
			if (node_ptr->priority_ < min->priority_)
			{
				min = node_ptr;
			}
		}
	}
	return min;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>::PQBinaryTreeItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data), parent_(nullptr), left_son_(nullptr), right_son_(nullptr)
{
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>::~PQBinaryTreeItem()
{
	if (this->right_son_)
	{
		delete this->right_son_;
		this->right_son_ = nullptr;
	}
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	this->parent_ = nullptr;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::merge(PQBinaryTreeItem<K, T>* node)
{
	if (node)

	{
		if (this->priority() < node->priority())
		{
			this->right_son_ = node->right_son_;
			return this->add_subtree(node);
		}
		else
		{
			return node->add_subtree(this);
		}
	}
	return this;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::cut()
{
	if (this->parent_)
	{
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son_ = this->right_son_;
		}
		else
		{
			this->parent_->right_son_ = this->right_son_;
		}
	}
	if (this->right_son_)
	{
		this->right_son_->parent_ = this->parent_;
	}
	this->parent_ = nullptr;
	this->right_son_ = nullptr;
	return this;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::left_son()
{
	return this->left_son_;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::right_son()
{
	return this->right_son_;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline size_t PQBinaryTreeItem<K, T>::degree()
{
	return this->degree_;
}

template<typename K, typename T>
inline void PQBinaryTreeItem<K, T>::left_son(PQBinaryTreeItem* node)
{
	this->left_son_ = node;
}

template<typename K, typename T>
inline void PQBinaryTreeItem<K, T>::right_son(PQBinaryTreeItem* node)
{
	this->right_son_ = node;
}

template<typename K, typename T>
inline void PQBinaryTreeItem<K, T>::parent(PQBinaryTreeItem* node)
{
	this->parent_ = node;
}

template<typename K, typename T>
inline void PQBinaryTreeItem<K, T>::degree(size_t degree)
{
	this->degree_ = degree;
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItem<K, T>::add_subtree(PQBinaryTreeItem<K, T>* node)
{
	if (node)
	{
		if (this->left_son_)
		{
			this->left_son_->parent_ = node;
		}
		node->parent_ = this;
		node->right_son_ = this->left_son_;
		this->left_son_ = node;
		this->degree_++;
	}
	return this;
}

template<typename K, typename T>
inline PQBinaryTreeItemA<K, T>::PQBinaryTreeItemA(const K& priority, const T& data) :
	PQBinaryTreeItem<K, T>(priority, data), ancestral_parent_(nullptr)
{
}

template<typename K, typename T>
inline PQBinaryTreeItemA<K, T>::~PQBinaryTreeItemA()
{
}

template<typename K, typename T>
inline PQBinaryTreeItem<K, T>* PQBinaryTreeItemA<K, T>::cut()
{
	PQBinaryTreeItemA<K, T>* node = (PQBinaryTreeItemA<K, T>*)(PQBinaryTreeItem<K, T>::cut());
	node->ancestral_parent_ = nullptr;
	return node;
}

template<typename K, typename T>
inline PQBinaryTreeItemA<K, T>* PQBinaryTreeItemA<K, T>::ancestral_parent()
{
	return this->ancestral_parent_;
}

template<typename K, typename T>
inline void PQBinaryTreeItemA<K, T>::swap_with_ancestral_parent()
{
	if (this->ancestral_parent_)
	{
		if (this->parent_ == this->ancestral_parent_)
		{
			//Ak ma moj predok otca, nastavim sa mu ako potomok namiesto predka
			if (this->ancestral_parent_->ancestral_parent_)
			{
				if (this->ancestral_parent_->ancestral_parent_ == this->ancestral_parent_->parent_)
				{
					this->ancestral_parent_->parent_->left_son_ = this;
				}
				else
				{
					this->ancestral_parent_->parent_->right_son_ = this;
				}
			}

			std::swap(this->right_son_, this->ancestral_parent_->right_son_);
			this->parent_ = this->ancestral_parent_->parent_;
			this->ancestral_parent_->parent_ = this;
			this->ancestral_parent_->left_son_ = this->left_son_;
			this->left_son_ = this->ancestral_parent_;

			if (this->ancestral_parent_->left_son_)
			{
				this->ancestral_parent_->left_son_->parent_ = this->left_son_;
				for (PQBinaryTreeItemA<K, T>* node_ptr = this->ancestral_parent_->left_son_; node_ptr; node_ptr = node_ptr->right_son_)
				{
					node_ptr->ancestral_parent_ = this->ancestral_parent_;
				}
			}
			this->ancestral_parent_ = this->ancestral_parent_->ancestral_parent_;
			for (PQBinaryTreeItemA<K, T>* node_ptr = this->left_son_; node_ptr; node_ptr = node_ptr->right_son_)
			{
				node_ptr->ancestral_parent_ = this;
			}
		}
		else
		{
			if (this->ancestral_parent_->ancestral_parent_)
			{
				if (this->ancestral_parent_->ancestral_parent_ == this->ancestral_parent_->parent_)
				{
					this->ancestral_parent_->parent_->left_son_ = this;
				}
				else
				{
					this->ancestral_parent_->parent_->right_son_ = this;
				}
			}
			std::swap(this->left_son_, this->ancestral_parent_->left_son_);
			std::swap(this->right_son_, this->ancestral_parent_->right_son_);
			std::swap(this->parent_, this->ancestral_parent_->parent_);
			this->ancestral_parent_->parent_->right_son_ = this->ancestral_parent_;
			if (this->ancestral_parent_->left_son_)
			{
				this->ancestral_parent_->left_son_->parent_ = this->left_son_;
				for (PQBinaryTreeItemA<K, T>* node_ptr = this->ancestral_parent_->left_son_; node_ptr; node_ptr = node_ptr->right_son_)
				{
					node_ptr->ancestral_parent_ = this->ancestral_parent_;
				}
			}
			this->ancestral_parent_ = this->ancestral_parent_->ancestral_parent_;
			for (PQBinaryTreeItemA<K, T>* node_ptr = this->left_son_; node_ptr; node_ptr = node_ptr->right_son_)
			{
				node_ptr->ancestral_parent_ = this;
			}
		}
		
	}
}

template<typename K, typename T>
inline void PQBinaryTreeItemA<K, T>::ancestral_parent(PQBinaryTreeItemA<K, T>* node)
{
	this->ancestral_parent_ = node;
}
