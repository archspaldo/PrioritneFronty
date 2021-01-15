#pragma once
#include "List.h"

template<typename T>
class LinkedListItem
{
public:
	LinkedListItem(const T data);
	~LinkedListItem();
	T& data();
	LinkedListItem<T>* next();
	LinkedListItem<T>* prev();
	void next(LinkedListItem<T>* node);
	void prev(LinkedListItem<T>* node);
private:
	T data_;
	LinkedListItem<T>* next_;
	LinkedListItem<T>* prev_;
};

template<typename T>
class LinkedList : public List<T>
{
public:
	LinkedList();
	LinkedList(const LinkedList<T>& other);
	LinkedList(LinkedList<T>&& other);
	~LinkedList();
	List<T>& operator=(const List<T>& other);
	LinkedList<T>& operator=(const LinkedList<T>& other);
	LinkedList<T>& operator=(LinkedList<T>&& other);
	const T operator[](const int index) const;
	T& operator[](const int index) override;
	void clear();
	size_t size() const override;
	bool empty() const override;
	void add(const T& data) override;
	void addAt(const T& data, const int index) override;
	void remove(const T& data) override;
	T removeAt(const int index) override;
	int find(const T& data) override;
	Iterator<T>* getBeginIterator() const override;
	Iterator<T>* getEndIterator() const override;
	static LinkedList<T>& concatenate(LinkedList<T>& other1, LinkedList<T>& other2);
private:
	size_t size_;
	LinkedListItem<T>* node_;
	LinkedListItem<T>* itemAt(const int index);
};

template<typename T>
inline LinkedList<T>::LinkedList() :
	size_(0), node_(nullptr)
{
}

template<typename T>
inline LinkedList<T>::LinkedList(const LinkedList<T>& other) :
	LinkedList<T>()
{
	*this = other;
}

template<typename T>
inline LinkedList<T>::LinkedList(LinkedList<T>&& other) :
	LinkedList<T>()
{
	*this = std::move(other);
}

template<typename T>
inline LinkedList<T>::~LinkedList()
{
	this->clear();
}

template<typename T>
inline List<T>& LinkedList<T>::operator=(const List<T>& other)
{
	if (this != &other)
	{
		*this = dynamic_cast<const LinkedList<T>&>(other);
	}
	return *this;
}

template<typename T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
	if (this != &other)
	{
		this->clear();
		for (auto node : other)
		{
			this->add(node);
		}
	}
	return *this;
}

template<typename T>
inline LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other)
{
	if (this != &other)
	{
		this->clear();
		this->node_ = other.node_;
		this->size_ = other.size_;
		other.node_ = nullptr;
		other.size_ = 0;
	}
	return *this;
}

template<typename T>
inline const T LinkedList<T>::operator[](const int index) const
{
	return itemAt(index)->data();
}

template<typename T>
inline T& LinkedList<T>::operator[](const int index)
{
	return itemAt(index)->data();
}

template<typename T>
inline void LinkedList<T>::clear()
{
	if (node_) {
		LinkedListItem<T>* current = this->node_;
		LinkedListItem<T>* next;
		for (int i = 0; i < this->size_; i++) {
			next = current->next_;
			delete current;
			current = next;
		}
		this->node_ = nullptr;
		this->size_ = 0;
	}
}

template<typename T>
inline size_t LinkedList<T>::size() const
{
	return this->size_;
}

template<typename T>
inline bool LinkedList<T>::empty() const
{
	return this->size_ == 0;
}

template<typename T>
inline void LinkedList<T>::add(const T& data)
{
	LinkedListItem<T>* node = new LinkedListItem<T>(data);
	if (this->empty())
	{
		this->node_ = node;
		this->node_->prev(node);
		this->node_->next(node);
	}
	else
	{
		node->prev(this->node_->prev());
		node->next(this->node_);
		this->node_->prev()->next(node);
		this->node_->prev(node);
	}
	this->size_++;
}

template<typename T>
inline void LinkedList<T>::addAt(const T& data, const int index)
{
	if (index == this->size_)
	{
		this->add(data);
	}
	else
	{
		LinkedListItem<T>* node = new LinkedListItem<T>(data);
		LinkedListItem<T>* current = this->itemAt(index);
		node->prev(current->prev());
		node->next(current);
		current->prev()->next(node);
		current->prev(node);
		if (index == 0)
		{
			this->node_ = current;
		}
	}
	this->size_++;
	
}

template<typename T>
inline void LinkedList<T>::remove(const T& data)
{
	if (!this->empty())
	{
		LinkedListItem<T>* node = this->node_;
		int index = 0;
		do
		{
			if (node->data() == data)
			{
				if (this->size_ == 1)
				{
					this->node_ = nullptr;
				}
				else
				{
					if (index == 0) {
						this->node_ = node->next();
					}
					node->prev()->next(node->next());
					node->next()->prev(node->prev());
				}
				delete node;
				this->size_--;
				return;
			}
			node = node->next();
		} while (index++ < this->size_);
	}
}

template<typename T>
inline T LinkedList<T>::removeAt(const int index)
{
	LinkedListItem<T>* node = this->itemAt(index);
	if (this->size_ == 1)
	{
		this->node_ = nullptr;
	}
	else
	{
		if (index == 0) {
			this->node_ = node->next();
		}
		node->prev()->next(node->next());
		node->next()->prev(node->prev());
	}
	this->size_--;
	T data = node->data();
	delete node;
	return data;
}

template<typename T>
inline int LinkedList<T>::find(const T& data)
{
	if (!this->empty())
	{
		LinkedListItem<T>* node = this->node_;
		int i = 0;
		for (i = 0; node->data() != data &&
			i < this->size_; node = node->next(), i++);
		return i;
	}
	return 0;
}

template<typename T>
inline Iterator<T>* LinkedList<T>::getBeginIterator() const
{
	return new LinkedListIterator<T>(this->node_, false);
}

template<typename T>
inline Iterator<T>* LinkedList<T>::getEndIterator() const
{
	return new LinkedListIterator<T>(this->node_, true);
}

template<typename T>
inline LinkedList<T>& LinkedList<T>::concatenate(LinkedList<T>& other1, LinkedList<T>& other2)
{
	LinkedList<T>* list = new LinkedList<T>();
	std::swap(list->node_, other1.node_ ? other1.node_ : other2.node_);
	if (list->node_ && other2.node_)
	{
		LinkedListItem<T>* last1, last2;
		last1 = list->node_->prev();
		last2 = other2->node_->prev();
		last1->next(other2.node_);
		other2.node_->prev(last1);
		last2->next(list->node_);
		list->node_->prev(last2);
		other2.node_ = nullptr;
	}
	list->size_ = other1.size_ + other2.size_;
	return *list;
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::itemAt(const int index)
{
	LinkedListItem<T>* node = this->node_;
	if (index < this->size_ / 2)
	{
		for (int i = 0; i++ < index; node = node->next());
	}
	else
	{
		for (int i = this->size_ ; i-- > index; node = node->prev());
	}
	return node;
}

template<typename T>
inline LinkedListItem<T>::LinkedListItem(const T data) :
	data_(data), next_(nullptr), prev_(nullptr)
{
}

template<typename T>
inline LinkedListItem<T>::~LinkedListItem()
{
	next_ = nullptr;
	prev_ = nullptr;
}

template<typename T>
inline T& LinkedListItem<T>::data()
{
	return data_;
}

template<typename T>
inline LinkedListItem<T>* LinkedListItem<T>::next()
{
	return next_;
}

template<typename T>
inline LinkedListItem<T>* LinkedListItem<T>::prev()
{
	return prev_;
}

template<typename T>
inline void LinkedListItem<T>::next(LinkedListItem<T>* node)
{
	next_ = node;
}

template<typename T>
inline void LinkedListItem<T>::prev(LinkedListItem<T>* node)
{
	prev_ = node;
}
