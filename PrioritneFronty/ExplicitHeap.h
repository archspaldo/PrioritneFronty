#pragma once
#include "PriorityQueue.h"

/// <summary>
/// Abstraktný predok pre prioritné fronty implementované explicitným binárnym stromom
/// </summary>
/// <typeparam name="Priority">Dátový typ priority</typeparam>
/// <typeparam name="Data">Dátový typ dát</typeparam>
template <typename Priority, typename Data>
class ExplicitPriorityQueue : public PriorityQueue<Priority, Data>
{
protected:
	/// <summary>
	/// Smerník na prvok s najvyššou prioritou
	/// </summary>
	BinaryTreeItem<Priority, Data>* root_;
	/// <summary>
	/// Poèet prvkov v prioritnom fronte
	/// </summary>
	size_t size_;
	/// <summary>
	/// Konštruktor
	/// </summary>
	ExplicitPriorityQueue();
public:
	/// <summary>
	/// Deštruktor
	/// </summary>
	/// <returns></returns>
	~ExplicitPriorityQueue();
	/// <summary>
	/// Vráti poèet prvkov v prioritnom fronte
	/// </summary>
	/// <returns>Poèet prvkov v prioritnom fronte</returns>
	size_t size() const override;
	/// <summary>
	/// Vráti dáta s najväèšou prioritou
	/// </summary>
	/// <returns>Hodnota dát</returns>
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
