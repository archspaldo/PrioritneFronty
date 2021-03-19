#pragma once
#include <stdexcept>

template <typename K, typename T>
class PriorityQueueItem
{
public:
	PriorityQueueItem(const K& priority, const T& data);
	virtual ~PriorityQueueItem() {};
	K& priority();
	void priority(const K& priority);
	T& data();
private:
	K priority_;
	T data_;
};


template <typename K, typename T>
class PriorityQueueLinkedItem : public PriorityQueueItem<K, T>
{
public:
	PriorityQueueLinkedItem(const K& priority, const T& data);
	~PriorityQueueLinkedItem();
	void heapify();
	PriorityQueueLinkedItem<K, T>* merge_with_right_sibling();
	PriorityQueueLinkedItem<K, T>* merge(PriorityQueueLinkedItem<K, T>* node);
	void link_on_right(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* isolate();
	void update_parent(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* parent();
	void parent(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* left_sibling();
	void left_sibling(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* right_sibling();
	void right_sibling(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* first_son();
	void first_son(PriorityQueueLinkedItem<K, T>* node);
protected:
	virtual PriorityQueueLinkedItem<K, T>* add_sub_tree(PriorityQueueLinkedItem<K, T>* node);
	PriorityQueueLinkedItem<K, T>* swap_with_parent();
private:
	PriorityQueueLinkedItem<K, T>* parent_;
	PriorityQueueLinkedItem<K, T>* left_sibling_;
	PriorityQueueLinkedItem<K, T>* right_sibling_;
	PriorityQueueLinkedItem<K, T>* first_son_;
};


template <typename K, typename T>
class PriorityQueue
{
public:
	virtual ~PriorityQueue() {};
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual PriorityQueueItem<K, T>* push(const K& key, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& peek() = 0;
	virtual const T peek() const = 0;
	virtual K peekPriority() = 0;
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
inline void PriorityQueueItem<K, T>::priority(const K& priority)
{
	this->priority_ = priority;
}

template<typename K, typename T>
inline T& PriorityQueueItem<K, T>::data()
{
	return data_;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>::PriorityQueueLinkedItem(const K& priority, const T& data) :
	PriorityQueueItem<K, T>(priority, data), parent_(nullptr), first_son_(nullptr)
{
	this->right_sibling_ = this;
	this->left_sibling_ = this;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>::~PriorityQueueLinkedItem()
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
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::heapify()
{
	while (this->parent_ && this->priority() < this->parent_->priority())
	{
		this->swap_with_parent();
	}
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::merge_with_right_sibling()
{
	if (this->right_sibling == this)
	{
		return this;
	}
	if (this->right_sibling_ == this->left_sibling_)
	{
		return this->merge(this->right_sibling_->isolate());
	}
	PriorityQueueLinkedItem<K, T>* left_sibling = this->left_sibling_, *right_sibling = this->right_sibling_->isolate();
	left_sibling->link_on_right(this->isolate()->merge(right_sibling));
	return left_sibling->right_sibling_;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::merge(PriorityQueueLinkedItem<K, T>* node)
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
inline void PriorityQueueLinkedItem<K, T>::link_on_right(PriorityQueueLinkedItem<K, T>* node)
{
	if (node)
	{
		this->right_sibling_->left_sibling_ = node->left_sibling_;
		node->left_sibling_->right_sibling_ = this->right_sibling_;
		node->left_sibling_ = this;
		this->right_sibling_ = node;
	}
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::parent()
{
	return this->parent_;
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::parent(PriorityQueueLinkedItem<K, T>* node)
{
	this->parent_ = node;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::left_sibling()
{
	return this->left_sibling_;
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::left_sibling(PriorityQueueLinkedItem<K, T>* node)
{
	this->left_sibling_ = node;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::right_sibling()
{
	return this->right_sibling_;
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::right_sibling(PriorityQueueLinkedItem<K, T>* node)
{
	this->right_sibling_ = node;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::first_son()
{
	return this->first_son_;
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::first_son(PriorityQueueLinkedItem<K, T>* node)
{
	this->first_son_ = node;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::add_sub_tree(PriorityQueueLinkedItem<K, T>* node)
{
	node->link_on_right(this->first_son_);
	this->first_son_ = node;
	if (this->first_son_)
	{
		this->first_son_->update_parent(this);
	}
	return this;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::swap_with_parent()
{
	PriorityQueueLinkedItem<K, T>* parent = this->parent_, * left_sibling = this->left_sibling_ == this ? this->left_sibling_ : nullptr,
		* parent_left_sibling = parent->left_sibling_ == parent ? parent->left_sibling_ : nullptr, * first_son = this->first_son_;
	parent->isolate();
	this->isolate();

	this->parent_ = parent->parent_;

	if (left_sibling)
	{
		left_sibling->link_on_right(parent);
	}
	if (parent_left_sibling)
	{
		parent_left_sibling->link_on_right(this);
	}
	if (parent->parent_ && parent->parent_->first_son_ == parent)
	{
		parent->parent_->first_son_ = this;
	}
	
	this->first_son_ = parent->first_son_ == this ? parent : parent->first_son_;
	parent->first_son_ = first_son;
	if (parent->first_son_)
	{
		parent->first_son_->update_parent(parent);
	}
	this->first_son_->update_parent(this);
	return this;
}

template<typename K, typename T>
inline PriorityQueueLinkedItem<K, T>* PriorityQueueLinkedItem<K, T>::isolate()
{
	if (this->right_sibling_ != this)
	{
		this->right_sibling_->left_sibling_ = this->left_sibling_;
		this->left_sibling_->right_sibling_ = this->right_sibling_;
		this->right_sibling_ = this;
		this->left_sibling_ = this;
	}
	return this;
}

template<typename K, typename T>
inline void PriorityQueueLinkedItem<K, T>::update_parent(PriorityQueueLinkedItem<K, T>* node)
{
	PriorityQueueLinkedItem<K, T>* son_ptr = this;
	son_ptr->parent_ = node;
	for (son_ptr = son_ptr->right_sibling_; son_ptr != this; son_ptr = son_ptr->right_sibling_)
	{
		son_ptr->parent_ = node;
	}

}

