#pragma once
#include "PriorityQueue.h"
#include <vector>

template <typename K, typename T>
class BinomialHeap : public PriorityQueue<K, T>
{
public:
	BinomialHeap();
	BinomialHeap(PQDoubleLinkedItem<K, T>* head, size_t size = 0);
	~BinomialHeap();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PriorityQueue<K, T>* other_heap);
	void change_priority(PriorityQueueItem<K, T>* node, const K& key);
private:
	void find_minimum();
	PQDoubleLinkedItem<K, T>* head_;
	PQDoubleLinkedItem<K, T>* root_;
	size_t size_;
};

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeap() :
	PriorityQueue<K, T>(), size_(0), head_(nullptr), root_(nullptr)
{
}

template<typename K, typename T>
inline BinomialHeap<K, T>::BinomialHeap(PQDoubleLinkedItem<K, T>* head, size_t size) :
	PriorityQueue<K, T>(), head_(head), root_(head), size_(size)
{
	this->find_minimum();
}

template<typename K, typename T>
inline BinomialHeap<K, T>::~BinomialHeap()
{
	delete this->head_;
	this->root_ = this->head_ = nullptr;
	this->size_ = 0;
}

template<typename K, typename T>
inline size_t BinomialHeap<K, T>::size() const
{
	return this->size_;
}

template<typename K, typename T>
inline PriorityQueueItem<K, T>* BinomialHeap<K, T>::push(const K& key, const T& data)
{
	PQDoubleLinkedItem<K, T>* new_item = new PQDoubleLinkedItem<K, T>(key, data);
	BinomialHeap<K, T>* new_heap = new BinomialHeap<K, T>(new_item, 1);
	this->merge(new_heap);
	return new_item;
}

template<typename K, typename T>
inline T BinomialHeap<K, T>::pop()
{
	if (this->head_)
	{
		PQDoubleLinkedItem<K, T>* root = this->root_;

		if (this->head_->is_isolated())
		{
			this->head_ = nullptr;
		}
		else
		{
			if (this->head_ == this->root_)
			{
				this->head_ = this->head_->right_sibling();
			}
			this->root_->isolate();
		}

		this->find_minimum();
		this->size_--;
		
		BinomialHeap<K, T>* new_heap = new BinomialHeap<K, T>(root->first_son());

		root->first_son(nullptr);

		this->merge(new_heap);
		T data = root->data();
		delete root;
		return data;
	}
	throw new std::range_error("BinomialHeap<K, T>::pop(): PriorityQueue is empty!");
}

template<typename K, typename T>
inline T& BinomialHeap<K, T>::peek()
{
	return this->root_->data();
}

template<typename K, typename T>
inline const T BinomialHeap<K, T>::peek() const
{
	return this->root_->data();
}

template<typename K, typename T>
inline K BinomialHeap<K, T>::peekPriority()
{
	return this->root_->priority();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::merge(PriorityQueue<K, T>* other_heap)
{
	BinomialHeap<K, T>* heap = (BinomialHeap<K, T>*)other_heap;
	if (heap)
	{
		if (this->head_)
		{
			if (heap->head_)
			{
				if (heap->root_->priority() < this->root_->priority())
				{
					this->root_ = heap->root_;
				}
				int size = (int)(std::max(log2(this->size_) + 1, log2(heap->size_) + 1));
				std::vector<PQDoubleLinkedItem<K, T>*> node_list(size + 1);
				PQDoubleLinkedItem<K, T>* node_ptr = heap->head_, * node_next_ptr;
				size_t degree;
				while (node_ptr)
				{
					node_next_ptr = node_ptr->is_isolated() ? nullptr : node_ptr->right_sibling();
					node_list[node_ptr->degree()] = node_ptr->isolate();
					node_ptr = node_next_ptr;
				}
				node_ptr = this->head_;
				this->head_ = nullptr;
				while (node_ptr)
				{
					node_next_ptr = node_ptr->is_isolated() ? nullptr : node_ptr->right_sibling();
					node_ptr->isolate();
					degree = node_ptr->degree();
					while (node_list[degree])
					{
						node_ptr = node_ptr->merge(node_list[degree]);
						node_list[degree++] = nullptr;
					}
					node_list[degree] = node_ptr;
					node_ptr = node_next_ptr;

				} 

				for (int i = 0; i < size + 1; i++)
				{
					if (node_list[i])
					{
						if (this->head_)
						{
							node_ptr->link_on_right(node_list[i]);
							node_ptr = node_ptr->right_sibling();
						}
						else
						{
							this->head_ = node_ptr = node_list[i];
						}
					}
				}

			}
		}
		else
		{
			this->head_ = heap->head_;
			this->root_ = heap->root_;
		}
		this->size_ += heap->size_;
		heap->head_ = nullptr;
		delete heap;
	}
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& key)
{
	node->priority() = key;
	PQDoubleLinkedItem<K, T>* changed_node = (PQDoubleLinkedItem<K, T>*)node;
	changed_node->heapify();
}

template<typename K, typename T>
inline void BinomialHeap<K, T>::find_minimum()
{
	this->root_ = this->head_;
	if (this->head_)
	{
		for (PQDoubleLinkedItem<K, T>* node = this->head_->right_sibling(); node != this->head_; node = node->right_sibling())
		{
			if (node->priority() < this->root_->priority())
			{
				this->root_ = node;
			}
		}
	}
}
