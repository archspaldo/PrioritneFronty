#pragma once
#include "PriorityQueue.h"
#include "ArrayList.h"

template <typename K, typename T>
class BinaryQueue : public PriorityQueue<K, T>
{
public:
	BinaryQueue();
	~BinaryQueue();
	void clear() override;
	size_t size() override;
	void push(K& key, T& data) override;
	T pop() override;
	T& peek() override;
	const T peek() const override;
	K peekPriority() override;
private:
	List<PriorityQueueItem<K key, T data>*>* list_;
};



template<typename K, typename T>
inline BinaryQueue<K, T>::BinaryQueue() :
	PriorityQueue<K, T>(),
	list_(new ArrayList<PriorityQueueItem<K key, T data>*>())
{
}

template<typename K, typename T>
inline BinaryQueue<K, T>::~BinaryQueue()
{
}

template<typename K, typename T>
inline void BinaryQueue<K, T>::clear()
{
}

template<typename K, typename T>
inline size_t BinaryQueue<K, T>::size()
{
	return size_t();
}

template<typename K, typename T>
inline void BinaryQueue<K, T>::push(K& key, T& data)
{
}

template<typename K, typename T>
inline T BinaryQueue<K, T>::pop()
{
	return T();
}

template<typename K, typename T>
inline T& BinaryQueue<K, T>::peek()
{
	// TODO: insert return statement here
}

template<typename K, typename T>
inline const T BinaryQueue<K, T>::peek() const
{
	// TODO: insert return statement here
}

template<typename K, typename T>
inline K BinaryQueue<K, T>::peekPriority()
{
	return K();
}
