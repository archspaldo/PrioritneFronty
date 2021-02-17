#pragma once
#include "Iterator.h"

template<typename T>
class ArrayList;

template<typename T>
class LinkedList;

template<typename T>
class LinkedListItem;


template<typename T>
class ArrayListIterator : public Iterator<T>
{
private:
	const ArrayList<T>* list_;
	int index_;
public:
	ArrayListIterator(const ArrayList<T>* list, int index) :
		list_(list), index_(index)
	{
	}
	~ArrayListIterator() {};
	Iterator<T>& operator=(const Iterator<T>& other)
	{
		if (this != &other)
		{
			index_ = dynamic_cast<const ArrayListIterator&>(other).index_;
			list_ = dynamic_cast<const ArrayListIterator&>(other).list_;
		}
		return *this;
	}
	bool operator==(const Iterator<T>& other)
	{
		return this->index_ == dynamic_cast<const ArrayListIterator&>(other).index_ && this->list_ == dynamic_cast<const ArrayListIterator&>(other).list_;
	}
	bool operator!=(const Iterator<T>& other)
	{
		return this->index_ != dynamic_cast<const ArrayListIterator&>(other).index_ || this->list_ != dynamic_cast<const ArrayListIterator&>(other).list_;
	}
	const T operator*()
	{
		return (*this->list_)[this->index_];
	}
	Iterator<T>& operator++()
	{
		this->index_++;
		return *this;
	}
};

template<typename T>
class LinkedListIterator : public Iterator<T>
{
private:
	LinkedListItem<T>* node_;
	bool flag_;
public:
	LinkedListIterator(LinkedListItem<T>* node, bool flag) :
		node_(node), flag_(flag)
	{
	}
	~LinkedListIterator() {};
	Iterator<T>& operator=(const Iterator<T>& other)
	{
		if (this != &other)
		{
			node_ = dynamic_cast<const LinkedListIterator&>(other).node_;
			flag_ = dynamic_cast<const LinkedListIterator&>(other).flag_;
		}
		return *this;
	}
	bool operator==(const Iterator<T>& other)
	{
		return this->node_ == dynamic_cast<const LinkedListIterator&>(other).node_ && this->flag_ == dynamic_cast<const LinkedListIterator&>(other).flag_;
	}
	bool operator!=(const Iterator<T>& other)
	{
		return this->node_ != dynamic_cast<const LinkedListIterator&>(other).node_ || this->flag_ != dynamic_cast<const LinkedListIterator&>(other).flag_;
	}
	const T operator*()
	{
		return this->node_->data();
	}
	Iterator<T>& operator++()
	{
		this->node_ = this->node_->next();
		this->flag_ = true;
		return *this;
	}
};