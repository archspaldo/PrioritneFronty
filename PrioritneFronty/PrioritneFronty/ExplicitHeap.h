#pragma once
#include "PriorityQueue.h"

/// <summary>
/// Abstraktn� predok pre prioritn� fronty implementovan� explicitn�m bin�rnym stromom
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class ExplicitPriorityQueue : public PriorityQueue<Priority, Data>
{
protected:
	/// <summary>
	/// Smern�k na prvok s najvy��ou prioritou
	/// </summary>
	BinaryTreeItem<Priority, Data>* root_;
	/// <summary>
	/// Po�et prvkov v prioritnom fronte
	/// </summary>
	size_t size_;
	/// <summary>
	/// Kon�truktor
	/// </summary>
	ExplicitPriorityQueue();
public:
	/// <summary>
	/// De�truktor
	/// </summary>
	/// <returns></returns>
	~ExplicitPriorityQueue();
	/// <summary>
	/// Vr�ti po�et prvkov v prioritnom fronte
	/// </summary>
	/// <returns>Po�et prvkov v prioritnom fronte</returns>
	size_t size() const override;
	/// <summary>
	/// Vr�ti d�ta s najv��ou prioritou
	/// </summary>
	/// <returns>Hodnota d�t</returns>
	Data& find_min() override;
};

template<typename Priority, typename Data>
inline ExplicitPriorityQueue<Priority, Data>::ExplicitPriorityQueue() :
	PriorityQueue<Priority, Data>(), root_(nullptr), size_(0)
{
}

template<typename Priority, typename Data>
inline ExplicitPriorityQueue<Priority, Data>::~ExplicitPriorityQueue()
{
}

template<typename Priority, typename Data>
inline size_t ExplicitPriorityQueue<Priority, Data>::size() const
{
	return this->size_;
}

template<typename Priority, typename Data>
inline Data& ExplicitPriorityQueue<Priority, Data>::find_min()
{
	if (this->size_ == 0)
	{
		throw new std::out_of_range("ExplicitPriorityQueue<Priority, Data>::find_min(): Zoznam je prazdny");
	}
	return this->root_->data();
}
