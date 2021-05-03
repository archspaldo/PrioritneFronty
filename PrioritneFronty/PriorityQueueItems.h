#pragma once
#include <algorithm>

/// <summary>
/// Abstraktnı predok pre prvky prioritného frontu
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class PriorityQueueItem
{
protected:
	/// <summary>
	/// Identifikátor prvku
	/// </summary>
	int identifier_;
	/// <summary>
	/// Priorita prvku
	/// </summary>
	Priority priority_;
	/// <summary>
	/// Dáta prvku
	/// </summary>
	Data data_;
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	PriorityQueueItem(const int identifier, const Priority& priority, const Data& data);
public:
	/// <summary>
	/// Deštruktor
	/// </summary>
	virtual ~PriorityQueueItem();
	/// <summary>
	/// Vráti identifikátor prvku
	/// </summary>
	/// <returns>Identifikátor prvku</returns>
	const int identifier() const;
	/// <summary>
	/// Vráti prioritu prvku
	/// </summary>
	/// <returns>Priorita prvku</returns>
	Priority& priority();
	/// <summary>
	/// Vráti dáta prvku
	/// </summary>
	/// <returns>Dáta prvku</returns>
	Data& data();
	/// <summary>
	/// Vráti, èi je priorita prvku väèšia ako priorita node
	/// </summary>
	/// <param name="node">Prvok, ktorého prioritu porovnávame</param>
	/// <returns>True, ak je priorita prvku vyššia, alebo je rovná a jeho identifikátor je niší</returns>
	bool operator<(PriorityQueueItem<Priority, Data>& node);
};

/// <summary>
/// Prvok pouitı v binárnej halde
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class ArrayItem : public PriorityQueueItem<Priority, Data>
{
protected:
	/// <summary>
	/// Index prvku v implicitnom zozname
	/// </summary>
	int index_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	ArrayItem(const int identifier, const Priority& priority, const Data& data, const int index);
	/// <summary>
	/// Vráti index prvku
	/// </summary>
	/// <returns>Index prvku</returns>
	int& index();
};

/// <summary>
/// Prvok pouitı v explicitnom binárnom strome
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class BinaryTreeItem : public PriorityQueueItem<Priority, Data>
{
protected:
	/// <summary>
	/// Smerníky na súvisiace prvky
	/// </summary>
	BinaryTreeItem<Priority, Data>* left_son_, * right_son_, * parent_;
	/// <summary>
	/// Vymení prvok s jeho priamım predchodcom
	/// </summary>
	void swap_with_parent();
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	BinaryTreeItem(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Deštruktor
	/// </summary>
	~BinaryTreeItem();
	/// <summary>
	/// Vystrihne prvok a nahradí ho jeho pravım potomkom
	/// </summary>
	/// <returns>Vystrihnutı prvok</returns>
	virtual BinaryTreeItem* cut();
	/// <summary>
	/// Prepojí dva prvky na základe priority
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok s vyššou prioritou</returns>
	virtual BinaryTreeItem* merge(BinaryTreeItem* node);
	/// <summary>
	/// Prvok node sa pripojí ako ¾avı potomok prvku, pôvodnı potomok sa pripojí k nemu ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	virtual BinaryTreeItem* add_left_son(BinaryTreeItem* node);
	/// <summary>
	/// Prvok node sa pripojí ako pravı potomok prvku, pôvodnı potomok sa pripojí k nemu ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	virtual BinaryTreeItem* add_right_son(BinaryTreeItem* node);
	/// <summary>
	/// Vráti prvok s najvyššou prioritou z pravej chrbtice ¾avého potomka
	/// </summary>
	/// <returns>Potomok s najvyššou prioritou</returns>
	BinaryTreeItem* highest_priority_son();
	/// <summary>
	/// Vráti prvok, ktorého pravá chrbtica ¾avého potomka obsahuje inštanciu
	/// </summary>
	/// <returns>Prvok</returns>
	BinaryTreeItem* ancestor();
	/// <summary>
	/// Vymení prvok s prvkom node, prièom prvok node musí tvori predka prvku
	/// V prípade, e prvok node netvorí predka prvku, správanie nie je definované
	/// </summary>
	/// <param name="node">Prvok, s ktorım sa má inštancia vymeni</param>
	virtual void swap_with_ancestor_node(BinaryTreeItem* node);
	/// <summary>
	/// Vráti ¾avého potomka prvku
	/// </summary>
	/// <returns>¼avı potomok prvku</returns>
	BinaryTreeItem*& left_son();
	/// <summary>
	/// Vráti pravého potomka prvku
	/// </summary>
	/// <returns>Pravı potomok prvku</returns>
	BinaryTreeItem*& right_son();
	/// <summary>
	/// Vráti priamého predka prvku
	/// </summary>
	/// <returns>Priamy predok prvku</returns>
	BinaryTreeItem*& parent();
	/// <summary>
	/// Prvok node sa nastaví ako ¾avı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	virtual BinaryTreeItem* left_son(BinaryTreeItem* node);
	/// <summary>
	/// Prvok node sa nastaví ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	virtual BinaryTreeItem* right_son(BinaryTreeItem* node);
};

/// <summary>
/// Prvok pouitı v binomickom strome
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class DegreeBinaryTreeItem : public BinaryTreeItem<Priority, Data>
{
protected:
	/// <summary>
	/// Stupeò prvku
	/// </summary>
	int degree_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	DegreeBinaryTreeItem(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Vymení prvok s prvkom node, prièom prvok node musí tvori predka prvku
	/// V prípade, e prvok node netvorí predka prvku, správanie nie je definované
	/// </summary>
	/// <param name="node">Prvok, s ktorım sa má inštancia vymeni</param>
	void swap_with_ancestor_node(BinaryTreeItem<Priority, Data>* node) override;
	/// <summary>
	/// Prvok node sa pripojí ako ¾avı potomok prvku, pôvodnı potomok sa pripojí k nemu ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	DegreeBinaryTreeItem* add_left_son(BinaryTreeItem<Priority, Data>* node) override;
	/// <summary>
	/// Vráti stupeò prvku
	/// </summary>
	/// <returns>Stupeò prvku</returns>
	int& degree();
};

/// <summary>
/// Prvok pouitı vo Fibonacciho halde
/// </summary>
/// <typeparam name="Priority">Dátovı typ priority</typeparam>
/// <typeparam name="Data">Dátovı typ dát</typeparam>
template <typename Priority, typename Data>
class FibonacciHeapItem : public DegreeBinaryTreeItem<Priority, Data>
{
protected:
	/// <summary>
	/// Oznaèenie, èi u prvok stratil potomka
	/// </summary>
	bool flag_;
	/// <summary>
	/// Prístup k usporiadanému predkovi
	/// </summary>
	FibonacciHeapItem<Priority, Data>* ordered_ancestor_;
public:
	/// <summary>
	/// Konštruktor
	/// </summary>
	/// <param name="identifier">Identifikátor</param>
	/// <param name="priority">Priorita</param>
	/// <param name="data">Dáta</param>
	FibonacciHeapItem(const int identifier, const Priority& priority, const Data& data);
	/// <summary>
	/// Deštruktor
	/// </summary>
	~FibonacciHeapItem();
	/// <summary>
	/// Vystrihne prvok a nahradí ho jeho pravım potomkom
	/// </summary>
	/// <returns>Vystrihnutı prvok</returns>
	FibonacciHeapItem* cut() override;
	/// <summary>
	/// Prvok node sa pripojí ako ¾avı potomok prvku, pôvodnı potomok sa pripojí k nemu ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	FibonacciHeapItem* add_left_son(BinaryTreeItem<Priority, Data>* node) override;
	/// <summary>
	/// Prvok node sa pripojí ako pravı potomok prvku, pôvodnı potomok sa pripojí k nemu ako pravı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	FibonacciHeapItem* add_right_son(BinaryTreeItem<Priority, Data>* node) override;
	/// <summary>
	/// Vráti usporiadaného predka prvku
	/// </summary>
	/// <returns>Usporiadanı predok</returns>
	FibonacciHeapItem*& ordered_ancestor();
	/// <summary>
	/// Vráti oznaèenie prvku
	/// </summary>
	/// <returns>Oznaèenie prvku</returns>
	bool& flag();
	/// <summary>
	/// Prvok node sa nastaví ako ¾avı potomok
	/// </summary>
	/// <param name="node">Pripájanı prvok</param>
	/// <returns>Prvok</returns>
	FibonacciHeapItem* left_son(BinaryTreeItem<Priority, Data>* node) override;
};

template <typename Priority, typename Data>
inline ArrayItem<Priority, Data>::ArrayItem(const int identifier, const Priority& priority, const Data& data, const int index) :
	PriorityQueueItem<Priority, Data>(identifier, priority, data), index_(index)
{
};

template <typename Priority, typename Data>
inline int& ArrayItem<Priority, Data>::index()
{
	return this->index_;
};

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>::BinaryTreeItem(const int identifier, const Priority& priority, const Data& data) :
	PriorityQueueItem<Priority, Data>(identifier, priority, data), left_son_(nullptr), right_son_(nullptr), parent_(nullptr)
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>::~BinaryTreeItem()
{
	BinaryTreeItem<Priority, Data>* left_son, * right_son;
	if (this->left_son_)
	{
		delete this->left_son_;
		this->left_son_ = nullptr;
	}
	while (this->right_son_)
	{
		right_son = this->right_son_;
		this->right_son_ = this->right_son_->right_son_;
		right_son->right_son_ = nullptr;
		delete right_son;
	}
	this->parent_ = nullptr;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::cut()
{
	if (this->parent_)
	{
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son(this->right_son_);
		}
		else
		{
			this->parent_->right_son(this->right_son_);
		}
	}
	else
	{
		if (this->right_son_)
		{
			this->right_son_->parent_ = nullptr;
		}
	}
	this->right_son_ = this->parent_ = nullptr;
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::merge(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		if (*this < *node)
		{
			return this->add_left_son(node);
		}
		else
		{
			return node->add_left_son(this);
		}
	}
	return this;
}

template<typename Priority, typename Data>
inline void BinaryTreeItem<Priority, Data>::swap_with_parent()
{
	if (this->parent_)
	{
		BinaryTreeItem* parent = this->parent_, * son;
		if (parent->parent_)
		{
			if (parent->parent_->left_son_ == parent)
			{
				parent->parent_->left_son(this);
			}
			else
			{
				parent->parent_->right_son(this);
			}
		}
		else
		{
			this->parent_ = nullptr;
		}
		if (parent->left_son_ == this)
		{
			parent->left_son(this->left_son_);
			this->left_son(parent);
			son = this->right_son();
			this->right_son_ = parent->right_son_;
			if (this->right_son_ && this->right_son_->parent_)
			{
				this->right_son_->parent_ = this;
			}
			parent->right_son(son);
		}
		else
		{
			parent->right_son(this->right_son_);
			this->right_son(parent);
			son = this->left_son();
			this->left_son(parent->left_son_);
			parent->left_son(son);
		}
	}
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		this->left_son(node->right_son(this->left_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::add_right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		this->right_son(node->right_son(this->right_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::highest_priority_son()
{
	BinaryTreeItem<Priority, Data>* node = this, * node_ptr = this->left_son_;
	while (node_ptr)
	{
		if (*node_ptr < *node)
		{
			node = node_ptr;
		}
		node_ptr = node_ptr->right_son_;
	}
	return node == this ? nullptr : node;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::ancestor()
{
	BinaryTreeItem* node_ptr = this, * parent_ptr = this->parent_;
	while (parent_ptr && parent_ptr->right_son_ == node_ptr)
	{
		node_ptr = parent_ptr;
		parent_ptr = parent_ptr->parent_;
	}
	return parent_ptr;
}

template<typename Priority, typename Data>
inline void BinaryTreeItem<Priority, Data>::swap_with_ancestor_node(BinaryTreeItem* node)
{
	if (node)
	{
		if (this->parent() == node)
		{
			this->swap_with_parent();
			return;
		}
		BinaryTreeItem* parent = node->parent_, * son = this->left_son_;
		this->left_son(node->left_son_);
		node->left_son(son);
		std::swap(this->right_son_, node->right_son_);
		if (this->right_son_ && this->right_son_->parent_)
		{
			this->right_son_->parent_ = this;
		}
		if (node->right_son_)
		{
			node->right_son_->parent_ = node;
		}
		if (this->parent_->left_son_ == this)
		{
			this->parent_->left_son(node);
		}
		else
		{
			this->parent_->right_son(node);
		}
		if (parent)
		{
			if (parent->left_son_ == node)
			{
				parent->left_son(this);
			}
			else
			{
				parent->right_son(this);
			}
		}
		else
		{
			this->parent_ = nullptr;
		}
	}
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::left_son()
{
	return this->left_son_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::right_son()
{
	return this->right_son_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>*& BinaryTreeItem<Priority, Data>::parent()
{
	return this->parent_;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->left_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* BinaryTreeItem<Priority, Data>::right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		node->parent_ = this;
	}
	this->right_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>::FibonacciHeapItem(const int identifier, const Priority& priority, const Data& data) :
	DegreeBinaryTreeItem<Priority, Data>(identifier, priority, data), ordered_ancestor_(nullptr), flag_(false)
{
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>::~FibonacciHeapItem()
{
	this->ordered_ancestor_ = nullptr;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::cut()
{
	this->BinaryTreeItem<Priority, Data>::cut();
	this->ordered_ancestor_ = nullptr;
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->ordered_ancestor_ = this;
		this->BinaryTreeItem<Priority, Data>::left_son(casted_node->BinaryTreeItem<Priority, Data>::right_son(this->left_son_));
		this->degree_++;
	}
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::add_right_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->ordered_ancestor_ = this->ordered_ancestor_;
		this->BinaryTreeItem<Priority, Data>::right_son(casted_node->BinaryTreeItem<Priority, Data>::right_son(this->right_son_));
	}
	return this;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>*& FibonacciHeapItem<Priority, Data>::ordered_ancestor()
{
	return this->ordered_ancestor_;
}

template<typename Priority, typename Data>
inline bool& FibonacciHeapItem<Priority, Data>::flag()
{
	return this->flag_;
}

template<typename Priority, typename Data>
inline FibonacciHeapItem<Priority, Data>* FibonacciHeapItem<Priority, Data>::left_son(BinaryTreeItem<Priority, Data>* node)
{
	if (node)
	{
		FibonacciHeapItem* casted_node = (FibonacciHeapItem*)node;
		casted_node->parent_ = this;
		if (casted_node->ordered_ancestor_ != this)
		{
			for (FibonacciHeapItem* node_ptr = casted_node; node_ptr; node_ptr = (FibonacciHeapItem*)(node_ptr->right_son_))
			{
				node_ptr->ordered_ancestor_ = this;
			}
		}
	}
	this->left_son_ = node;
	return this;
}

template<typename Priority, typename Data>
inline PriorityQueueItem<Priority, Data>::PriorityQueueItem(const int identifier, const Priority& priority, const Data& data) :
	identifier_(identifier), priority_(priority), data_(data)
{
}

template<typename Priority, typename Data>
inline PriorityQueueItem<Priority, Data>::~PriorityQueueItem()
{
}

template<typename Priority, typename Data>
inline const int PriorityQueueItem<Priority, Data>::identifier() const
{
	return this->identifier_;
}

template<typename Priority, typename Data>
inline Priority& PriorityQueueItem<Priority, Data>::priority()
{
	return this->priority_;
}

template<typename Priority, typename Data>
inline Data& PriorityQueueItem<Priority, Data>::data()
{
	return this->data_;
}

template<typename Priority, typename Data>
inline bool PriorityQueueItem<Priority, Data>::operator<(PriorityQueueItem<Priority, Data>& operand)
{
	return this->priority_ <= operand.priority_ && (this->priority_ < operand.priority_ || this->identifier_ <= operand.identifier_);
}

template<typename Priority, typename Data>
inline DegreeBinaryTreeItem<Priority, Data>::DegreeBinaryTreeItem(const int identifier, const Priority& priority, const Data& data) :
	BinaryTreeItem<Priority, Data>(identifier, priority, data), degree_(0)
{
}

template<typename Priority, typename Data>
inline void DegreeBinaryTreeItem<Priority, Data>::swap_with_ancestor_node(BinaryTreeItem<Priority, Data>* node)
{
	std::swap(this->degree_, ((DegreeBinaryTreeItem*)node)->degree_);
	this->BinaryTreeItem<Priority, Data>::swap_with_ancestor_node(node);
}

template<typename Priority, typename Data>
inline DegreeBinaryTreeItem<Priority, Data>* DegreeBinaryTreeItem<Priority, Data>::add_left_son(BinaryTreeItem<Priority, Data>* node)
{
	this->degree_++;
	return (DegreeBinaryTreeItem*)this->BinaryTreeItem<Priority, Data>::add_left_son(node);
}

template<typename Priority, typename Data>
inline int& DegreeBinaryTreeItem<Priority, Data>::degree()
{
	return this->degree_;
}