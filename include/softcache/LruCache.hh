#pragma once

#include <utility>
#include <unordered_map>
#include <list>
#include "Container.hh"

namespace sc
{
	template <typename TKey, typename TValue> class LruCache : public Container< std::pair<TKey,TValue> >
	{
		std::unordered_map<TKey, std::pair<typename std::list<TKey>::iterator,TValue> > m_map;
		std::list<TKey> m_list;
		std::size_t m_capacity;

	public:
		LruCache(std::size_t capacity);
		~LruCache();

		std::size_t capacity() const;
		std::size_t size() const;
		bool empty() const;
		bool full() const;

		bool push(const std::pair<TKey,TValue> &obj);
		bool pull(std::pair<TKey,TValue> &obj);
		bool peak(std::pair<TKey,TValue> &obj) const;

	private:
		void moveToFront(typename std::list<TKey>::iterator it);
	};
}

template <typename TKey, typename TValue> sc::LruCache<TKey,TValue>::LruCache(std::size_t capacity)
{
	if (capacity < 1)
		throw std::invalid_argument("Capacity is given as 0 (zero)");

	m_capacity = capacity;
	m_map.reserve(capacity);
}

template <typename TKey, typename TValue> sc::LruCache<TKey,TValue>::~LruCache() { }

template <typename TKey, typename TValue> std::size_t sc::LruCache<TKey,TValue>::capacity() const
{
	return m_capacity;
}

template <typename TKey, typename TValue> std::size_t sc::LruCache<TKey,TValue>::size() const
{
	return m_list.size();
}

template <typename TKey, typename TValue> bool sc::LruCache<TKey,TValue>::empty() const
{
	return (size() == 0);
}

template <typename TKey, typename TValue> bool sc::LruCache<TKey,TValue>::full() const
{
	return (size() == m_capacity);
}

template <typename TKey, typename TValue> bool sc::LruCache<TKey,TValue>::push(const std::pair<TKey,TValue> &obj)
{
	auto mapIt = m_map.find(obj.first);
	if (mapIt != m_map.end())
	{
		moveToFront(mapIt->second.first);
		return true;
	}

	if (full())
		return false;

	m_list.push_front(obj.first);
	auto listIt = m_list.begin();
	m_map.insert({obj.first, {listIt, obj.second}});
	return true;
}

template <typename TKey, typename TValue> bool sc::LruCache<TKey,TValue>::pull(std::pair<TKey,TValue> &obj)
{
	if (empty())
		return false;

	auto listIt = m_list.rbegin();
	obj.first = *listIt;
	m_list.pop_back();

	auto mapIt = m_map.find(obj.first);
	obj.second = mapIt->second.second;
	m_map.erase(mapIt);

	return true;
}

template <typename TKey, typename TValue> bool sc::LruCache<TKey,TValue>::peak(std::pair<TKey,TValue> &obj) const
{
	if (empty())
		return false;

	auto it = m_map.find(obj.first);
	if (it == m_map.end())
		return false;

	obj.second = it->second.second;
	return true;
}

template <typename TKey, typename TValue> void sc::LruCache<TKey,TValue>::moveToFront(typename std::list<TKey>::iterator it)
{
	if (it != m_list.begin())
		m_list.splice(m_list.begin(), m_list, it, std::next(it));
}
