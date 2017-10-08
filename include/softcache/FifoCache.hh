#pragma once

#include <utility>
#include <unordered_map>
#include "CircularQueue.hh"

namespace sc
{
	template <typename TKey, typename TValue> class FifoCache : public Container< std::pair<TKey,TValue> >
	{
	private:
		std::unordered_map<TKey,TValue> m_map;
		CircularQueue<TKey> *m_queue;

	public:
		FifoCache(std::size_t capacity);

		std::size_t capacity() const;
		std::size_t size() const;
		bool empty() const;
		bool full() const;

		bool push(const std::pair<TKey,TValue> &obj);
		bool pull(std::pair<TKey,TValue> &obj);
		bool peak(std::pair<TKey,TValue> &obj) const;
	};
}

// implementations
template <typename TKey, typename TValue> sc::FifoCache<TKey,TValue>::FifoCache(std::size_t capacity)
{
	m_queue = new CircularQueue<TKey>(capacity);
}

template <typename TKey, typename TValue> std::size_t sc::FifoCache<TKey,TValue>::capacity() const
{
	return m_queue->capacity();
}

template <typename TKey, typename TValue> std::size_t sc::FifoCache<TKey,TValue>::size() const
{
	return m_queue->size();
}

template <typename TKey, typename TValue> bool sc::FifoCache<TKey,TValue>::empty() const
{
	return m_queue->empty();
}

template <typename TKey, typename TValue> bool sc::FifoCache<TKey,TValue>::full() const
{
	return m_queue->full();
}

template <typename TKey, typename TValue> bool sc::FifoCache<TKey,TValue>::push(const std::pair<TKey,TValue> &obj)
{
	auto it = m_map.find(obj.first);
	if (it != m_map.end())
		return true;

	if (full())
		return false;

	m_map.insert({obj.first, obj.second});
	m_queue->push(obj.first);
	return true;
}

template <typename TKey, typename TValue> bool sc::FifoCache<TKey,TValue>::pull(std::pair<TKey,TValue> &obj)
{
	if (empty())
		return false;

	TKey key;
	m_queue->pull(key);
	obj.first = key;

	auto it = m_map.find(key);
	obj.second = it->second;
	m_map.erase(it);

	return true;
}

template <typename TKey, typename TValue> bool sc::FifoCache<TKey,TValue>::peak(std::pair<TKey,TValue> &obj) const
{
	if (empty())
		return false;

	auto it = m_map.find(obj.first);
	if (it == m_map.end())
		return false;

	obj.second = it->second;
	return true;
}
