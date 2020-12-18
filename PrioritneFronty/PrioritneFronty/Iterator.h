#pragma once

template<typename T>
class Iterator
{
public:
	virtual ~Iterator() {};
	virtual Iterator<T>& operator=(const Iterator<T>& diff) = 0;
	virtual bool operator==(const Iterator<T>& diff) = 0;
	virtual bool operator!=(const Iterator<T>& diff) = 0;
	virtual const T operator*() = 0;
	virtual Iterator<T>& operator++() = 0;
};

template<typename T>
class PriorityQueueIterator
{
public:
	PriorityQueueIterator(Iterator<T>* iterator);
	virtual ~PriorityQueueIterator();
	PriorityQueueIterator<T>& operator=(const PriorityQueueIterator<T>& diff);
	bool operator==(const PriorityQueueIterator<T>& diff);
	bool operator!=(const PriorityQueueIterator<T>& diff);
	const T operator*();
	PriorityQueueIterator<T>& operator++();
private:
	Iterator<T>* iterator_;
};

template<typename T>
class Iterable
{
public:
	PriorityQueueIterator<T> begin() const;
	PriorityQueueIterator<T> end() const;
	virtual Iterator<T>* getBeginIterator() const = 0;
	virtual Iterator<T>* getEndIterator() const = 0;
};


template<typename T>
inline PriorityQueueIterator<T> Iterable<T>::begin() const
{
	return PriorityQueueIterator<T>(getBeginIterator());
}

template<typename T>
inline PriorityQueueIterator<T> Iterable<T>::end() const
{
	return PriorityQueueIterator<T>(getEndIterator());
}

template<typename T>
inline PriorityQueueIterator<T>::PriorityQueueIterator(Iterator<T>* iterator) :
	iterator_(iterator)
{
}

template<typename T>
inline PriorityQueueIterator<T>::~PriorityQueueIterator()
{
	delete this->iterator_;
}

template<typename T>
inline PriorityQueueIterator<T>& PriorityQueueIterator<T>::operator=(const PriorityQueueIterator<T>& diff)
{
	if (this != &diff)
	{
		*this->iterator_ = *diff.iterator_;
	}
	return *this;
}

template<typename T>
inline bool PriorityQueueIterator<T>::operator==(const PriorityQueueIterator<T>& diff)
{
	return *this->iterator_ == *diff.iterator_;
}

template<typename T>
inline bool PriorityQueueIterator<T>::operator!=(const PriorityQueueIterator<T>& diff)
{
	return !(*this->iterator_ == *diff.iterator_);
}

template<typename T>
inline const T PriorityQueueIterator<T>::operator*()
{
	return **this->iterator_;
}

template<typename T>
inline PriorityQueueIterator<T>& PriorityQueueIterator<T>::operator++()
{
	Iterator<T>* iterator = &(++ * this->iterator_);
	if (iterator != this->iterator_)
	{
		delete this->iterator_;
		this->iterator_ = iterator;
	}
	iterator = nullptr;
	return *this;
}

