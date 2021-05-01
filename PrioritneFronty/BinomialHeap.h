#pragma once
#include "LazyBinomialQueue.h"

/// <summary>
/// Abstraktná binomická trieda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class BinomialHeap : public LazyBinomialHeap<Priority, Data>
{
protected:
	/// <summary>
	/// Vymieòa prvok node s jeho priamım predkom, dokia¾ nie je splnené haldové usporiadanie
	/// </summary>
	/// <param name="node">Vymieòanı prvok</param>
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	/// <summary>
	/// Vymieòa prvok node s tım z priamıch potomkov, ktorı ma najväèšiu prioritu, dokia¾ nie je splnené haldové usporiadanie
	/// </summary>
	/// <param name="node">Vymieòanı prvok</param>
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
	/// <summary>
	/// Upraví smerníky v pravej chrbtici atribútu root_, aby smerovali ku koreòom stromov
	/// </summary>
	void repair_broken_root_list();
	/// <summary>
	/// Abstraktnı konštruktor
	/// </summary>
	BinomialHeap();
public:
	/// <summary>
	/// Deštruktor
	/// </summary>
	~BinomialHeap();
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	/// <summary>
	/// Pripojí k prioritnému frontu prvky z other_heap
	/// </summary>
	/// <param name="other_heap">Prioritnı front, ktorého prvky majú by pripojené</param>
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
};


/// <summary>
/// Viacprechodová binomická halda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class BinomialHeapMultiPass : public BinomialHeap<Priority, Data>
{
protected:
	/// <summary>
	/// Zlúèi prvky v pravej chrbtici atribútu root_ a parametra node viacprechodovou stratégiou
	/// </summary>
	/// <param name="node">Prvı prvok v postupnosti prvkov, ktoré sa majú zlúèi</param>
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	BinomialHeapMultiPass();
	/// <summary>
	/// Deštruktor
	/// </summary>
	~BinomialHeapMultiPass();
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

/// <summary>
/// Jednoprechodová binomická halda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class BinomialHeapOnePass : public BinomialHeap<Priority, Data>
{
protected:
	/// <summary>
	/// Zlúèi prvky v pravej chrbtici atribútu root_ a parametra node jednoprechodovou stratégiou
	/// </summary>
	/// <param name="node">Prvı prvok v postupnosti prvkov, ktoré sa majú zlúèi</param>
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	BinomialHeapOnePass();
	/// <summary>
	/// Deštruktor
	/// </summary>
	~BinomialHeapOnePass();
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->BinomialHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
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
