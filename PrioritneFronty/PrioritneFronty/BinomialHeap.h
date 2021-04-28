#pragma once
#include "LazyBinomialQueue.h"

/// <summary>
/// Abstraktn� binomick� trieda
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class BinomialHeap : public LazyBinomialHeap<Priority, Data>
{
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
	/// <summary>
	/// Uprav� smern�ky v pravej chrbtici atrib�tu root_, aby smerovali ku kore�om stromov
	/// </summary>
	void repair_broken_root_list();
	/// <summary>
	/// Abstraktn� kon�truktor
	/// </summary>
	BinomialHeap();
public:
	/// <summary>
	/// De�truktor
	/// </summary>
	~BinomialHeap();
	/// <summary>
	/// Vlo�� d�ta do prioritn�ho frontu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren� prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	/// <summary>
	/// Pripoj� k prioritn�mu frontu prvky z other_heap
	/// </summary>
	/// <param name="other_heap">Prioritn� front, ktor�ho prvky maj� by� pripojen�</param>
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
};


/// <summary>
/// Viacprechodov� binomick� halda
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class BinomialHeapMultiPass : public BinomialHeap<Priority, Data>
{
protected:
	/// <summary>
	/// Zl��i prvky v pravej chrbtici atrib�tu root_ a parametra node viacprechodovou strat�giou
	/// </summary>
	/// <param name="node">Prv� prvok v postupnosti prvkov, ktor� sa maj� zl��i�</param>
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	BinomialHeapMultiPass();
	/// <summary>
	/// De�truktor
	/// </summary>
	~BinomialHeapMultiPass();
	/// <summary>
	/// Vlo�� d�ta do prioritn�ho frontu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren� prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritn�ho frontu d�ta s najv��ou prioritou
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku s najv��ou prioritou</param>
	/// <returns>Hodnota d�t</returns>
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmen� prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktor�mu m� by� zmenen� priorita</param>
	/// <param name="priority">Nov� priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

/// <summary>
/// Jednoprechodov� binomick� halda
/// </summary>
/// <typeparam name="Priority">D�tov� typ priority</typeparam>
/// <typeparam name="Data">D�tov� typ d�t</typeparam>
template <typename Priority, typename Data>
class BinomialHeapOnePass : public BinomialHeap<Priority, Data>
{
protected:
	/// <summary>
	/// Zl��i prvky v pravej chrbtici atrib�tu root_ a parametra node jednoprechodovou strat�giou
	/// </summary>
	/// <param name="node">Prv� prvok v postupnosti prvkov, ktor� sa maj� zl��i�</param>
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Kon�truktor
	/// </summary>
	BinomialHeapOnePass();
	/// <summary>
	/// De�truktor
	/// </summary>
	~BinomialHeapOnePass();
	/// <summary>
	/// Vlo�� d�ta do prioritn�ho frontu
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren� prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritn�ho frontu d�ta s najv��ou prioritou
	/// </summary>
	/// <param name="identifier">Identifik�tor prvku s najv��ou prioritou</param>
	/// <returns>Hodnota d�t</returns>
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmen� prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktor�mu m� by� zmenen� priorita</param>
	/// <param name="priority">Nov� priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline BinomialHeap<Priority, Data>::BinomialHeap() :
	LazyBinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeap<Priority, Data>::~BinomialHeap()
{
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	DegreeBinaryTreeItem<Priority, Data>* new_item = new DegreeBinaryTreeItem<Priority, Data>(identifier, priority, data);
	this->size_++;
	this->consolidate_root(new_item);
	data_item = new_item;
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	BinomialHeap<Priority, Data>* heap = (BinomialHeap<Priority, Data>*)other_heap;
	this->size_ += heap->size_;
	this->consolidate_root(heap->root_);
	heap->root_ = nullptr;
	delete other_heap;
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	DegreeBinaryTreeItem<Priority, Data>* casted_node = (DegreeBinaryTreeItem<Priority, Data>*)node;
	BinaryTreeItem<Priority, Data>* ordered_ancestor = casted_node->ancestor();
	while (ordered_ancestor && *casted_node < *ordered_ancestor)
	{
		casted_node->swap_with_ancestor_node(ordered_ancestor);
		ordered_ancestor = casted_node->ancestor();
	}
	if (!casted_node->parent() && casted_node != this->root_)
	{
		this->repair_broken_root_list();
	}
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	DegreeBinaryTreeItem<Priority, Data>* casted_node = (DegreeBinaryTreeItem<Priority, Data>*)node;
	BinaryTreeItem<Priority, Data>* minimal_son = casted_node->highest_priority_son();
	bool is_root_item = !casted_node->parent();
	while (minimal_son && *minimal_son < *casted_node)
	{
		minimal_son->swap_with_ancestor_node(casted_node);
		minimal_son = casted_node->highest_priority_son();
	}
	if (is_root_item)
	{
		this->repair_broken_root_list();
	}
}

template<typename Priority, typename Data>
inline void BinomialHeap<Priority, Data>::repair_broken_root_list()
{
	auto get_highest_node = [](BinaryTreeItem<Priority, Data>* node)
	{
		if (node->parent())
		{
			BinaryTreeItem<Priority, Data>* node_ptr = node->parent();
			while (node_ptr->parent())
			{
				node_ptr = node_ptr->parent();
			}
			return node_ptr;
		}
		return node;
	};
	if (this->root_)
	{
		this->root_ = get_highest_node(this->root_);
		this->root_->right_son() = get_highest_node(this->root_->right_son());
		BinaryTreeItem<Priority, Data>* node_ptr = this->root_->right_son(), * root = this->root_;
		while (node_ptr != this->root_)
		{
			if (*node_ptr < *root)
			{
				root = node_ptr;
			}
			node_ptr->right_son() = get_highest_node(node_ptr->right_son());
			node_ptr = node_ptr->right_son();
		}
		this->root_ = root;
	}
}

template<typename Priority, typename Data>
inline void BinomialHeapMultiPass<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->LazyBinomialHeap<Priority, Data>::consolidate_root_using_multipass(node, (int)(log2(this->size_)) + 2);
}

template<typename Priority, typename Data>
inline BinomialHeapMultiPass<Priority, Data>::BinomialHeapMultiPass() :
	BinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeapMultiPass<Priority, Data>::~BinomialHeapMultiPass()
{
}

template<typename Priority, typename Data>
inline void BinomialHeapOnePass<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->LazyBinomialHeap<Priority, Data>::consolidate_root_using_onepass(node, (int)(log2(this->size_)) + 2);
}

template<typename Priority, typename Data>
inline BinomialHeapOnePass<Priority, Data>::BinomialHeapOnePass() :
	BinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinomialHeapOnePass<Priority, Data>::~BinomialHeapOnePass()
{
}
