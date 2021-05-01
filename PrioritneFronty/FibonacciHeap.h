#pragma once
#include "LazyBinomialQueue.h"

/// <summary>
/// Fibonacciho halda
/// </summary>
/// <typeparam name="Priority">D·tov˝ typ priority</typeparam>
/// <typeparam name="Data">D·tov˝ typ d·t</typeparam>
template <typename Priority, typename Data>
class FibonacciHeap : public LazyBinomialHeap<Priority, Data>
{
private:
	/// <summary>
	/// Vystrihne prvok node a pripojÌ ho k root_
	/// </summary>
	/// <param name="node">Vystrihovan˝ prvok</param>
	void cut(FibonacciHeapItem<Priority, Data>* node);
	/// <summary>
	/// Ak je prvok oznaËen˝, vystrihne ho, inak ho oznaËÌ
	/// </summary>
	/// <param name="node">Vystrihovan˝ prvok</param>
	void cascading_cut(FibonacciHeapItem<Priority, Data>* node);
protected:
	/// <summary>
	/// Zl˙Ëi prvky v pravej chrbtici atrib˙tu root_ a parametra node viacprechodovou stratÈgiou
	/// </summary>
	/// <param name="node">Prv˝ prvok v postupnosti prvkov, ktorÈ sa maj˙ zl˙Ëiù</param>
	void consolidate_root(BinaryTreeItem<Priority, Data>* node) override;
	/// <summary>
	/// Vystrihne prvok a vykon· sÈriov˝ rez nad priam˝m predkom
	/// </summary>
	/// <param name="node">Prvok so zv˝öenou prioritou</param>
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	/// <summary>
	/// Vystrihne potomkov prvku s vyööou prioritou
	/// </summary>
	/// <param name="node">Prvok so znÌûenou prioritou</param>
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Konötruktor
	/// </summary>
	FibonacciHeap();
	/// <summary>
	/// Deötruktor
	/// </summary>
	~FibonacciHeap();
	/// <summary>
	/// VloûÌ d·ta do prioritnÈho frontu
	/// </summary>
	/// <param name="identifier">Identifik·tor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvoren˝ prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& = nullptr) override;
	/// <summary>
	/// Vyberie z prioritnÈho frontu d·ta s najv‰Ëöou prioritou
	/// </summary>
	/// <param name="identifier">Identifik·tor prvku s najv‰Ëöou prioritou</param>
	/// <returns>Hodnota d·t</returns>
	Data pop(int& identifier) override { return this->LazyBinomialHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// ZmenÌ prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorÈmu m· byù zmenen· priorita</param>
	/// <param name="priority">Nov· priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline FibonacciHeap<Priority, Data>::FibonacciHeap() :
	LazyBinomialHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline FibonacciHeap<Priority, Data>::~FibonacciHeap()
{
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	data_item = this->LazyBinomialHeap<Priority, Data>::push(new FibonacciHeapItem<Priority, Data>(identifier, priority, data));
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::cut(FibonacciHeapItem<Priority, Data>* node)
{
	node->ordered_ancestor()->degree()--;
	node->cut();
	this->add_root_item(node);
	node->flag() = false;
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::cascading_cut(FibonacciHeapItem<Priority, Data>* node)
{
	if (node->ordered_ancestor())
	{
		if (node->flag())
		{
			FibonacciHeapItem<Priority, Data>* ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)node->ordered_ancestor();
			this->cut(node);
			this->cascading_cut(ordered_ancestor);
		}
		else
		{
			node->flag() = true;
		}
	}
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::consolidate_root(BinaryTreeItem<Priority, Data>* node)
{
	this->consolidate_root_using_multipass(node, (int)(log(this->size_) * 2.1) + 2);
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	FibonacciHeapItem<Priority, Data>* casted_node = (FibonacciHeapItem<Priority, Data>*)node;
	if (casted_node->ordered_ancestor() && *casted_node < *casted_node->ordered_ancestor())
	{
		FibonacciHeapItem<Priority, Data>* ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)((FibonacciHeapItem<Priority, Data>*)node)->ordered_ancestor();
		this->cut(casted_node);
		this->cascading_cut(ordered_ancestor);
	}
	if (*casted_node < *this->root_)
	{
		this->root_ = casted_node;
	}
}

template<typename Priority, typename Data>
inline void FibonacciHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* casted_node = (BinaryTreeItem<Priority, Data>*)node;
	FibonacciHeapItem<Priority, Data>* ordered_ancestor;
	if (this->root_ == casted_node)
	{
		BinaryTreeItem<Priority, Data>* new_root = this->root_;
		for (BinaryTreeItem<Priority, Data>* node_ptr = this->root_->right_son(); node_ptr != this->root_; node_ptr = node_ptr->right_son())
		{
			if (*node_ptr < *new_root)
			{
				new_root = node_ptr;
			}
		}
		this->root_ = new_root;
	}
	for (BinaryTreeItem<Priority, Data>* node_ptr = casted_node->left_son(), *node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr; node_ptr;
		node_ptr = node_next_ptr, node_next_ptr = node_ptr ? node_ptr->right_son() : nullptr)
	{
		if (*node_ptr < *node)
		{
			ordered_ancestor = (FibonacciHeapItem<Priority, Data>*)((FibonacciHeapItem<Priority, Data>*)node_ptr)->ordered_ancestor();
			this->cut((FibonacciHeapItem<Priority, Data>*)node_ptr);
			this->cascading_cut(ordered_ancestor);
		}
	}
}
