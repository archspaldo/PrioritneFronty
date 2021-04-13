#pragma once
#include "ExplicitPriorityQueue.h"
#include <stack>
#include <queue>

template <typename Priority, typename Data>
class PairingHeap : public ExplicitPriorityQueue<Priority, Data>
{
protected:
	PairingHeap();
	virtual BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) = 0;
	void priority_was_increased(PriorityQueueItem<Priority, Data>* node) override;
	void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) override;
public:
	~PairingHeap();
	void clear() override;
	void push(const int identifier, const Priority& key, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override;
	Data pop(int& identifier) override;
	void merge(PriorityQueue<Priority, Data>* other_heap) override;
};

template <typename Priority, typename Data>
class PairingHeapTwoPass : public PairingHeap<Priority, Data>
{
protected:
	BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) override;
public:
	PairingHeapTwoPass();

	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->PairingHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	Data pop(int& identifier) override { return this->PairingHeap<Priority, Data>::pop(identifier); };
	void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority) override { this->PriorityQueue<Priority, Data>::change_priority(node, priority); };
};

template <typename Priority, typename Data>
class PairingHeapMultiPass : public PairingHeap<Priority, Data>
{
protected:
	BinaryTreeItem<Priority, Data>* create_binary_tree(BinaryTreeItem<Priority, Data>* node) override;
public:
	PairingHeapMultiPass();

	void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& data_item) override { this->PairingHeap<Priority, Data>::push(identifier, priority, data, data_item); };
	Data pop(int& identifier) override { return this->PairingHeap<Priority, Data>::pop(identifier); };
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
		//std::cout << "LH\t" << root->priority() << "\t" << root->identifier() << "\n";
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
	PairingHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapTwoPass<Priority, Data>::create_binary_tree(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr)
	{
		if (node_ptr->right_son())
		{
			std::stack<BinaryTreeItem<Priority, Data>*> stack;
			while (node_ptr)
			{
				node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
				node_ptr = node_ptr->merge(node_ptr->right_son());
				node_ptr->right_son() = nullptr;
				stack.push(node_ptr);
				node_ptr = node_next_ptr;
			}
			node_ptr = stack.top();
			stack.pop();
			while (!stack.empty())
			{
				node_ptr = node_ptr->merge(stack.top());
				stack.pop();
			}
		}
		node_ptr->parent() = nullptr;
	}
	return node_ptr;
}

template<typename Priority, typename Data>
inline PairingHeapMultiPass<Priority, Data>::PairingHeapMultiPass() :
	PairingHeap<Priority, Data>()
{
}

template<typename Priority, typename Data>
inline BinaryTreeItem<Priority, Data>* PairingHeapMultiPass<Priority, Data>::create_binary_tree(BinaryTreeItem<Priority, Data>* node)
{
	BinaryTreeItem<Priority, Data>* node_ptr = node, * node_next_ptr;

	if (node_ptr)
	{
		if (node_ptr->right_son())
		{
			std::queue<BinaryTreeItem<Priority, Data>*> queue;
			while (node_ptr)
			{
				node_next_ptr = node_ptr->right_son() ? node_ptr->right_son()->right_son() : nullptr;
				node_ptr = node_ptr->merge(node_ptr->right_son());
				node_ptr->right_son() = nullptr;
				queue.push(node_ptr);
				node_ptr = node_next_ptr;
			}
			node_ptr = queue.front();
			queue.pop();
			while (!queue.empty())
			{
				node_ptr = node_ptr->merge(queue.front());
				queue.pop();
				queue.push(node_ptr);
				node_ptr = queue.front();
				queue.pop();
			}
		}
		node_ptr->parent() = nullptr;
	}
	return node_ptr;
}