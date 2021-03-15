#pragma once
#include "PriorityQueue.h"

template <typename K, typename T>
class LinearTreeItem
{
private:
	LinearTreeItem* left_sibling_;
	PriorityQueueItem<K, T>* data_;

public:
	LinearTreeItem();
	~LinearTreeItem();

};


template <typename K, typename T>
class RankPairingHeap : public PriorityQueue<K, T>
{
public:
	RankPairingHeap();
	~RankPairingHeap();
	size_t size() const override;
	PriorityQueueItem<K, T>* push(const K& key, const T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
	void merge(PriorityQueue<K, T>* other_heap) override;
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority) override;
private:

};
