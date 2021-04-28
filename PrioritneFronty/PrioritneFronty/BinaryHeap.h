#pragma once
#include "PriorityQueue.h"
#include <vector>

/// <summary>
/// Bin�rna halda implementovan� implicitn�m zoznamom
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class BinaryHeap : public PriorityQueue<Priority, Data>
{
private:
	/// <summary>
	/// Implicitn� zoznam prvkov
	/// </summary>
	std::vector<ArrayItem<Priority, Data>*>* list_;
	/// <summary>
	/// Vr�ti index �av�ho potomka prvku na indexe index
	/// </summary>
	/// <param name="index">Index prvku</param>
	/// <returns>Index �av�ho potomka</returns>
	int left_son(const int index);
	/// <summary>
	/// Vr�ti index prav�ho potomka prvku na indexe index
	/// </summary>
	/// <param name="index">Index prvku</param>
	/// <returns>Index prav�ho potomka</returns>
	int righ_son(const int index);
	/// <summary>
	/// Vr�ti index piameho predka prvku na indexe index
	/// </summary>
	/// <param name="index">Index prvku</param>
	/// <returns>Index priam�ho predka</returns>
	int parent(const int index);
	/// <summary>
	/// Vr�ti index piameho predka prvku na indexe index
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	int greater_son(const int index);
	/// <summary>
	/// Vymie�a prvok na indexe index s priamym predkom, dokia� nie je splnen� haldov� usporiadanie
	/// </summary>
	/// <param name="index">Index prvku, ktor� sa vymie�a�</param>
	void heapify_up(const int index);
	/// <summary>
	/// Vymie�a prvok na indexe index s t�m z priam�ch potomkov, ktor� ma najv��iu prioritu, dokia� nie je splnen� haldov� usporiadanie
	/// </summary>
	/// <param name="index">Index prvku, ktor� sa vymie�a�</param>
	void heapify_down(const int index);
	/// <summary>
	/// Vymen� prvky ulo�en� v item_1 a item_2
	/// </summary>
	/// <param name="item_1"></param>
	/// <param name="item_2"></param>
	static void swap(ArrayItem<Priority, Data>*& item_1, ArrayItem<Priority, Data>*& item_2);
protected:
	/// <summary>
	/// Vymie�a prvok node s jeho priam�m predkom, dokia� nie je splnen� haldov� usporiadanie
	/// </summary>
	/// <param name="node">Vymie�an� prvok</param>
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	/// <summary>
	/// Vymie�a prvok node s t�m z priam�ch potomkov, ktor� ma najv��iu prioritu, dokia� nie je splnen� haldov� usporiadanie
	/// </summary>
	/// <param name="node">Vymie�an� prvok</param>
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	BinaryHeap();
	/// <summary>
	/// De�truktor
	/// </summary>
	~BinaryHeap();
	/// <summary>
	/// Vym�e v�etky prvky z prioritn�ho frontu
	/// </summary>
	void clear() override;
	/// <summary>
	/// Vr�ti po�et prvkov v prioritnom fronte
	/// </summary>
	/// <returns>Po�et prvkov v prioritnom fronte</returns>
	size_t size() const override;
	/// <summary>
	/// Vlo�� d�ta do prioritn�ho frontu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren� prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	/// <summary>
	/// Vyberie z prioritn�ho frontu d�ta s najv��ou prioritou
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku s najv��ou prioritou</param>
	/// <returns>Hodnota d�t</returns>
	Data pop(int& identifier) override;
	/// <summary>
	/// Vr�ti d�ta s najv��ou prioritou
	/// </summary>
	/// <returns>Hodnota d�t</returns>
	Data& find_min() override;
	/// <summary>
	/// Pripoj� k prioritn�mu frontu prvky z other_heap
	/// </summary>
	/// <param name="other_heap">Prioritn� front, ktor�ho prvky maj� by� pripojen�</param>
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
	/// <summary>
	/// Zmen� prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktor�mu m� by� zmenen� priorita</param>
	/// <param name="priority">Nov� priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline BinaryHeap<Priority, Data>::BinaryHeap() :
	PriorityQueue<Priority, Data>(),
	list_(new std::vector<ArrayItem<Priority, Data>*>())
{
}

template<typename Priority, typename Data>
inline BinaryHeap<Priority, Data>::~BinaryHeap()
{
	this->clear();
	delete this->list_;
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::clear()
{
	for (PriorityQueueItem<Priority, Data>* item : *this->list_)
	{
		delete item;
	}
	this->list_->clear();
}

template<typename Priority, typename Data>
inline size_t BinaryHeap<Priority, Data>::size() const
{
	return this->list_->size();
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	ArrayItem<Priority, Data>* new_node = new ArrayItem<Priority, Data>(identifier, priority, data, this->size());
	this->list_->push_back(new_node);
	this->heapify_up(this->size() - 1);
	data_item = new_node;
}

template<typename Priority, typename Data>
inline Data BinaryHeap<Priority, Data>::pop(int& identifier)
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<Priority, Data>::pop(): Zoznam je prazdny");
	}
	if (this->size() > 1)
	{
		swap((*this->list_)[0], (*this->list_)[this->size() - 1]);
	}

	PriorityQueueItem<Priority, Data>* item = this->list_->back();
	this->list_->pop_back(); 
	this->heapify_down(0);
	Data data = item->data();
	identifier = item->identifier();
	delete item;
	return data;
}

template<typename Priority, typename Data>
inline Data& BinaryHeap<Priority, Data>::find_min()
{
	if (this->list_->empty())
	{
		throw new std::out_of_range("BinaryHeap<Priority, Data>::find_min(): Zoznam je prazdny");
	}
	return (*this->list_)[0]->data();
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	BinaryHeap<Priority, Data>* heap = (BinaryHeap<Priority, Data>*)other_heap;
	this->list_->insert(this->list_->end(), heap->list_->begin(), heap->list_->end());
	if (this->size() > 1)
	{
		int greater_son;
		for (int i = (this->size() - 1) / 2; i <= 0; i--)
		{
			this->heapify_down(i);
		}
	}
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::left_son(const int index)
{
	return 2 * index + 1;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::righ_son(const int index)
{
	return 2 * index + 2;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::parent(const int index)
{
	return (index - 1) / 2;
}

template<typename Priority, typename Data>
inline int BinaryHeap<Priority, Data>::greater_son(const int index)
{
	PriorityQueueItem<Priority, Data>* lChild = this->left_son(index) < this->size() ? (*this->list_)[this->left_son(index)] : nullptr;
	PriorityQueueItem<Priority, Data>* rChild = this->righ_son(index) < this->size() ? (*this->list_)[this->righ_son(index)] : nullptr;
	if (lChild && rChild)
	{
		return *lChild < *rChild ? this->left_son(index) : this->righ_son(index);
	}
	return this->left_son(index);
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::heapify_up(const int index)
{
	for (int i = index, parent = this->parent(i); i > 0 && *(*this->list_)[i] < *(*this->list_)[parent]; i = parent, parent = this->parent(i))
	{
		swap((*this->list_)[parent], (*this->list_)[i]);
	}
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::heapify_down(const int index)
{
	for (int i = index, child = this->greater_son(i); child < this->size() && *(*this->list_)[child] < *(*this->list_)[i]; i = child, child = this->greater_son(i))
	{
		swap((*this->list_)[child], (*this->list_)[i]);
	}
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::swap(ArrayItem<Priority, Data>*& item_1, ArrayItem<Priority, Data>*& item_2)
{
	std::swap(item_1, item_2);
	std::swap(item_1->index(), item_2->index());
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	int index = ((ArrayItem<Priority, Data>*)node)->index();
	this->heapify_up(index);
}

template<typename Priority, typename Data>
inline void BinaryHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	int index = ((ArrayItem<Priority, Data>*)node)->index();
	this->heapify_down(index);
}
