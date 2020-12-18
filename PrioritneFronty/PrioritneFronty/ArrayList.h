#pragma once
#include "List.h"
#include "Array.h"

const int ARRAYLIST_DEFAULT_SIZE = 8;
template <typename T>
class ArrayList : public List<T>
{
public:
	ArrayList();
	ArrayList(size_t capacity);
	ArrayList(const ArrayList<T>& diff);
	ArrayList(ArrayList<T>&& diff);
	~ArrayList();
	List<T>& operator=(const List<T>& diff);
	ArrayList<T>& operator=(const ArrayList<T>& diff);
	ArrayList<T>& operator=(ArrayList<T>&& diff);
	const T operator[](const int index) const;
	T& operator[](const int index) override;
	size_t size() const override;
	bool empty() const override;
	bool full() const;
	void add(const T& data) override;
	void addAt(const T& data, const int index) override;
	void remove(const T& data) override;
	T removeAt(const int index) override;
	int find(const T& data) override;
	
private:
	void resize();
	Array* array_;
	size_t size_;
};

template<typename T>
inline ArrayList<T>::ArrayList() :
	ArrayList(ARRAYLIST_DEFAULT_SIZE)
{
}

template<typename T>
inline ArrayList<T>::ArrayList(size_t capacity) :
	List<T>(),
	array_(new Array<T>(capacity)),
	size_(0)
{
}

template<typename T>
inline ArrayList<T>::ArrayList(const ArrayList<T>& diff) :
	List<T>(),
	array_(new Array<T>(*diff.array_)),
	size_(diff.size_)
{
}

template<typename T>
inline ArrayList<T>::ArrayList(ArrayList<T>&& diff) :
	List<T>(),
	array_(std::move(*diff.array_)),
	size_(diff.size_)
{
}

template<typename T>
inline ArrayList<T>::~ArrayList()
{
	if (this->array_)
	{
		delete this->array_;
		this->size_ = 0;
	}
}

template<typename T>
inline List<T>& ArrayList<T>::operator=(const List<T>& diff)
{
	if (this != &diff)
	{
		*this = dynamic_cast<ArrayList<T>&>(diff);
	}
	return *this;
}

template<typename T>
inline ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& diff)
{
	if (this != &other)
	{
		*this->array_ = *diff.array_;
		this->size_ = diff.size_;
	}
}

template<typename T>
inline ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& diff)
{
	if (this != &other)
	{
		*this->array_ = std::move(*diff.array_);
		this->size_ = diff.size_;
	}
}

template<typename T>
inline const T ArrayList<T>::operator[](const int index) const
{
	return this->array_->operator[](index);
}

template<typename T>
inline T& ArrayList<T>::operator[](const int index)
{
	return this->array_->operator[](index);
}

template<typename T>
inline size_t ArrayList<T>::size() const
{
	return this->size_;
}

template<typename T>
inline bool ArrayList<T>::empty() const
{
	return this->size_ == 0;
}

template<typename T>
inline bool ArrayList<T>::full() const
{
	return this->size_ == this->array_->size();
}

template<typename T>
inline void ArrayList<T>::add(const T& data)
{
	if (this->full())
	{
		this->resize();
	}
	this->operator[](this->size_++) = data;
}

template<typename T>
inline void ArrayList<T>::addAt(const T& data, const int index)
{
	if (this->full())
	{
		this->resize();
	}
	if (index < this->size_ - 1)
	{
		Array<T>::copy(*this->array_, index, *this->array_, index + 1, this->size_ - index);
	}
	this->operator[](index) = data;
	size_++;
}

template<typename T>
inline void ArrayList<T>::remove(const T& data)
{
	int index = this->find(data);
	if (index != INT_MIN)
	{
		this->removeAt(index);
	}
}

template<typename T>
inline T ArrayList<T>::removeAt(const int index)
{
	T data = this->operator[](index);
	if (index < this->size_ - 1)
	{
		Array<T>::copy(*this->array_, index + 1, *this->array_, index, this->size_ - index - 1);
	}
	size_--;
	return data;
}

template<typename T>
inline int ArrayList<T>::find(const T& data)
{
	for (int i = 0; i < this->size_; i++)
	{
		if (this->operator[](i) == data)
		{
			return i;
		}
	}
	return INT_MIN;
}

template<typename T>
inline void ArrayList<T>::resize()
{
	Array<T>* array = new Array<T>(this->array_->size() * 2);
	Array<T>::copy(*this->array_, 0, *array, 0, this->array_->size());
	*this->array_ = *std::move(array);
}
