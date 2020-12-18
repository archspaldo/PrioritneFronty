#pragma once
#include "Iterator.h"
template <typename T>
class List : public Iterable<T>
{
public:

	virtual List<T>& operator=(const List<T>& diff) = 0;
	virtual const T operator[](const int pos) const = 0;
	virtual T& operator[](const int pos) = 0;
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;
	virtual void add(const T& data) = 0;
	virtual void addAt(const T& data, const int index) = 0;
	virtual void remove(const T& data) = 0;
	virtual T removeAt(const int index) = 0;
	virtual int indexOf(const T& data) = 0;
	virtual Iterator<T>* getBeginIterator() const = 0;
	virtual Iterator<T>* getEndIterator() const = 0;
};
