#pragma once
#include <cstdlib>
#include <cstring>
#include <utility>

typedef unsigned char byte;
template<typename T>
class Array {
public:
	Array(const size_t capacity);
	Array(const Array<T>& diff);
	Array(Array<T>&& diff);
	~Array();
	size_t size() const;
	Array<T>& operator=(const Array<T>& diff);
	Array<T>& operator=(Array<T>&& diff);
	bool operator==(const Array<T>& diff) const;
	T& operator[](const size_t index);
	const T operator[](const size_t index) const;
	static void copy(const Array<T>& src, const size_t srcStartIndex,
		Array<T>& dest, const size_t destStartIndex, const size_t length);
private:
	void* pointer_;
	size_t capacity_;
};


template<typename T>
inline Array<T>::Array(const size_t capacity) :
	pointer_(calloc(capacity, sizeof(T))),
	capacity_(capacity)
{
}

template<typename T>
inline Array<T>::Array(const Array<T>& diff) :
	Array<T>(diff.size())
{
	memcpy(this->pointer_, diff.pointer_, this->capacity_ * sizeof(T));
}

template<typename T>
inline Array<T>::Array(Array<T>&& diff) :
	pointer_(diff.pointer_),
	capacity_(diff.capacity_)
{
	diff.pointer_ = NULL;
	diff.capacity_ = 0;
}
template<typename T>
inline Array<T>::~Array()
{
	if (this->pointer_ != NULL)
	{
		free(this->pointer_);
		this->pointer_ = NULL;
		this->capacity_ = 0;
	}
}
template<typename T>
inline size_t Array<T>::size() const
{
	return this->capacity_;
}
template<typename T>
inline Array<T>& Array<T>::operator=(const Array<T>& diff)
{
	if (this != &diff)
	{
		this->capacity_ = diff.capacity_;
		realloc(this->pointer_, this->capacity_ * sizeof(T));
		memcpy(this->pointer_, diff.pointer_, this->capacity_ * sizeof(T));
	}
	return *this;
}
template<typename T>
inline Array<T>& Array<T>::operator=(Array<T>&& diff)
{
	if (this != &diff)
	{
		free(this->pointer_);
		this->capacity_ = diff.capacity_;
		diff.capacity_ = 0;
		this->pointer_ = diff.pointer_;
		diff.pointer_ = NULL;
	}
	return *this;
}
template<typename T>
inline bool Array<T>::operator==(const Array<T>& diff) const
{
	return this->capacity_ == diff.capacity_ &&
		memcmp(this->pointer_, diff.pointer_, this->capacity_ * sizeof(T));
}
template<typename T>
inline T& Array<T>::operator[](const size_t index)
{
	return *reinterpret_cast<T*>(reinterpret_cast<byte*>(this->pointer_) + index * sizeof(T));
}
template<typename T>
inline const T Array<T>::operator[](const size_t index) const
{
	return *reinterpret_cast<T*>(reinterpret_cast<byte*>(this->pointer_) + index * sizeof(T));
}
template<typename T>
inline void Array<T>::copy(const Array<T>& src, const size_t srcStartIndex, Array<T>& dest, const size_t destStartIndex, const size_t length)
{
	if (&src == &dest && destStartIndex + length > srcStartIndex && destStartIndex < srcStartIndex + length)
		memmove(reinterpret_cast<byte*>(dest.pointer_) + destStartIndex * sizeof(T),
			reinterpret_cast<byte*>(src.pointer_) + srcStartIndex * sizeof(T), length * sizeof(T));
	else
		memcpy(reinterpret_cast<byte*>(dest.pointer_) + destStartIndex * sizeof(T),
			reinterpret_cast<byte*>(src.pointer_) + srcStartIndex * sizeof(T), length * sizeof(T));
}
