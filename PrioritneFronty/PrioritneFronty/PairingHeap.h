#pragma once
#include "ExplicitHeap.h"
#include <stack>
#include <queue>

/// <summary>
/// Abstraktná párovacia halda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class PairingHeap : public ExplicitPriorityQueue<Priority, Data>
{
protected:
	/// <summary>
	/// Konštruktor
	/// </summary>
	PairingHeap();
	/// <summary>
	/// Vytvorí binárny strom z prvku node a jeho potomkov
	/// </summary>
	/// <param name="node">Prvı prvok v postupnosti prvkov, ktoré sa majú vytvori binárny strom</param>
	/// <returns>Prvok tvoriací koreò binárneho stromu</returns>
	virtual BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) = 0;
	/// <summary>
	/// Vystrihne prvok a prepojí ho s root_
	/// </summary>
	/// <param name="node">Prvok so zvıšenou prioritou</param>
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	/// <summary>
	/// Z prvku node a prvkov tvoriacích pravú chrbticu ¾avého potomka tohto prvku vytvorí binárny strom a pridá ho do binárneho stromu
	/// </summary>
	/// <param name="node">Prvok so zníenou prioritou</param>
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Deštruktor
	/// </summary>
	~PairingHeap();
	/// <summary>
	/// Vymáe všetky prvky z prioritného frontu
	/// </summary>
	void clear() override;
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& key, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>
	Data pop(int& identifier) override;
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
};

/// <summary>
/// Dvojprechodová párovacia halda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class PairingHeapTwoPass : public PairingHeap<Priority, Data>
{
private:
	/// <summary>
	/// Zásobník pouitı pri zluèovaní prvkov
	/// </summary>
	std::stack<BinaryTreeItem<Priority, Data>*>* stack_;
protected:
	/// <summary>
	/// Vytvorí binárny strom z prvku node a jeho potomkov
	/// </summary>
	/// <param name="node">Prvı prvok v postupnosti prvkov, ktoré sa majú vytvori binárny strom</param>
	/// <returns>Prvok tvoriací koreò binárneho stromu</returns>
	BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	PairingHeapTwoPass();
	/// <summary>
	/// Deštruktor
	/// </summary>
	~PairingHeapTwoPass();
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->PairingHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>
	Data pop(int& identifier) override { return this->PairingHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

/// <summary>
/// Viacprechodová párovacia halda
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class PairingHeapMultiPass : public PairingHeap<Priority, Data>
{
private:
	/// <summary>
	/// Front pouitı pri zluèovaní prvkov
	/// </summary>
	std::queue<BinaryTreeItem<Priority, Data>*>* queue_;
protected:
	/// <summary>
	/// Vytvorí binárny strom z prvku node a jeho potomkov
	/// </summary>
	/// <param name="node">Prvı prvok v postupnosti prvkov, ktoré sa majú vytvori binárny strom</param>
	/// <returns>Prvok tvoriací koreò binárneho stromu</returns>
	BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) override;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	PairingHeapMultiPass();
	/// <summary>
	/// Deštruktor
	/// </summary>
	~PairingHeapMultiPass();
	/// <summary>
	/// Vloí dáta do prioritného frontu
	/// </summary>
	/// <param name="identifier">Identifikátor prvku</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Data</param>
	/// <param name="data_item">Vytvorenı prvok</param>
	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->PairingHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	/// <summary>
	/// Vyberie z prioritného frontu dáta s najväèšou prioritou
	/// </summary>
	/// <param name="identifier">Identifikátor prvku s najväèšou prioritou</param>
	/// <returns>Hodnota dát</returns>
	Data pop(int& identifier) override { return this->PairingHeap<Priority, Data>::pop(identifier); };
	/// <summary>
	/// Zmení prioritu prvku
	/// </summary>
	/// <param name="node">Prvok, ktorému má by zmenená priorita</param>
	/// <param name="priority">Nová priorita prvku</param>
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template<typename Priority, typename Data>
inline PairingHeap<Priority, Data>::PairingHeap() :
	ExplicitPriorityQueue<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::priority_was_increased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* casted_node = (BinaryTreeItem<Priority, Data>*)node;
	if (casted_node != this->root_)
	{
		this->root_ = this->root_->merge(casted_node->cut());
	}
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::priority_was_decreased(PriorityQueueItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = (BinaryTreeItem<Priority, Data>*)node, * parent = node_ptr->parent();
	bool is_left_son = parent ? parent->left_son() == node_ptr : false;

	node_ptr->cut();
	node_ptr->right_son(node_ptr->left_son());
	node_ptr->left_son(nullptr);

	node_ptr = this->create_binary_tree(node_ptr);

	if (parent)
	{
		if (is_left_son)
		{
			parent->add_left_son(node_ptr);
		}
		else
		{
			parent->add_right_son(node_ptr);
		}
	}
	else
	{
		this->root_ = node_ptr;
		this->root_->parent() = nullptr;
	}
}

template<typename Priority, typename Data>
inline PairingHeap<Priority, Data>::~PairingHeap()
{
	this->clear();
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::push(const int identifier, const Priority& key, const Data& data, PriorityQueueItem<Priority, Data>*& data_item)
{
	BinaryTreeItem<Priority, Data>* new_node = new BinaryTreeItem<Priority, Data>(identifier, key, data);
	if (this->root_)
	{
		this->root_ = this->root_->merge(new_node);
	}
	else
	{
		this->root_ = new_node;
	}
	this->size_++;
	data_item = new_node;
}

template<typename Priority, typename Data>
inline Data PairingHeap<Priority, Data>::pop(int& identifier)
{
	if (this->root_)
	{
		BinaryTreeItem<Priority, Data>* root = this->root_;
		this->root_ = this->create_binary_tree(root->left_son());
		root->left_son() = nullptr;
		this->size_--;
		Data data = root->data();
		identifier = root->identifier();
		delete root;
		return data;
	}
	throw new std::range_error("PairingHeap<Priority, Data>::pop(): Priority queue is empty!");
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::clear()
{
	delete this->root_;
	this->root_ = nullptr;
	this->size_ = 0;
}

template<typename Priority, typename Data>
inline void PairingHeap<Priority, Data>::merge(PriorityQueue<Priority, Data>* other_heap)
{
	PairingHeap<Priority, Data>* heap = (PairingHeap<Priority, Data>*)other_heap;
	if (this->root_)
	{
		this->root_ = this->root_->merge(heap->root_);
		this->root_->parent() = nullptr;
		heap->root_ = nullptr;
	}
	else
	{
		std::swap(this->root_, heap->root_);
	}
	this->size_ += heap->size_;
	delete heap;
}

template<typename Priority, typename Data>
inline PairingHeapTwoPass<Priority, Data>::PairingHeapTwoPass() :
	PairingHeap<Priority, Data>(), stack_(new std::stack<BinaryTreeItem<Priority, Data>*>())
{
}

template<typename Priority, typename Data>
inline PairingHeapTwoPass<Priority, Data>::~PairingHeapTwoPass()
{
	delete this->stack_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapTwoPass<Priority, Data>::create_binary_tree(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr)
	{
		if (node_ptr->right_son())
		{
			while (node_ptr)
			{
				node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
				node_ptr = node_ptr->merge(node_ptr->right_son());
				node_ptr->right_son() = nullptr;
				this->stack_->push(node_ptr);
				node_ptr = node_next_ptr;
			}
			node_ptr = this->stack_->top();
			this->stack_->pop();
			while (!this->stack_->empty())
			{
				node_ptr = node_ptr->merge(this->stack_->top());
				this->stack_->pop();
			}
		}
		node_ptr->parent() = nullptr;
	}
	return node_ptr;
}

template<typename Priority, typename Data>
inline PairingHeapMultiPass<Priority, Data>::PairingHeapMultiPass() :
	PairingHeap<Priority, Data>(), queue_(new std::queue<BinaryTreeItem<Priority, Data>*>())
{
}

template<typename Priority, typename Data>
inline PairingHeapMultiPass<Priority, Data>::~PairingHeapMultiPass()
{
	delete this->queue_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapMultiPass<Priority, Data>::create_binary_tree(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr)
	{
		if (node_ptr->right_son())
		{
			while (node_ptr)
			{
				node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
				node_ptr = node_ptr->merge(node_ptr->right_son());
				node_ptr->right_son() = nullptr;
				this->queue_->push(node_ptr);
				node_ptr = node_next_ptr;
			}
			node_ptr = this->queue_->front();
			this->queue_->pop();
			while (!this->queue_->empty())
			{
				node_ptr = node_ptr->merge(this->queue_->front());
				this->queue_->pop();
				this->queue_->push(node_ptr);
				node_ptr = this->queue_->front();
				this->queue_->pop();
			}
		}
		node_ptr->parent() = nullptr;
	}
	return node_ptr;
}