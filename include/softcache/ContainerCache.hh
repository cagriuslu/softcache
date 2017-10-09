#pragma once

#include <utility>
#include <unordered_map>
#include "Container.hh"
#include "CircularQueue.hh"

namespace sc
{
	template <typename TContainer, typename TKey, typename TValue> class ContainerCache : public Container< std::pair<TKey,TValue> >
	{
		std::unordered_map<TKey,TValue> m_map;
		TContainer *m_cont;

	public:
		ContainerCache(std::size_t capacity);
		~ContainerCache();

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
template <typename TContainer, typename TKey, typename TValue> sc::ContainerCache<TContainer,TKey,TValue>::ContainerCache(std::size_t capacity)
{
	m_cont = new TContainer(capacity);
}

template <typename TContainer, typename TKey, typename TValue> sc::ContainerCache<TContainer,TKey,TValue>::~ContainerCache()
{
	delete m_cont;
}

template <typename TContainer, typename TKey, typename TValue> std::size_t sc::ContainerCache<TContainer,TKey,TValue>::capacity() const
{
	return m_cont->capacity();
}

template <typename TContainer, typename TKey, typename TValue> std::size_t sc::ContainerCache<TContainer,TKey,TValue>::size() const
{
	return m_cont->size();
}

template <typename TContainer, typename TKey, typename TValue> bool sc::ContainerCache<TContainer,TKey,TValue>::empty() const
{
	return m_cont->empty();
}

template <typename TContainer, typename TKey, typename TValue> bool sc::ContainerCache<TContainer,TKey,TValue>::full() const
{
	return m_cont->full();
}

template <typename TContainer, typename TKey, typename TValue> bool sc::ContainerCache<TContainer,TKey,TValue>::push(const std::pair<TKey,TValue> &obj)
{
	auto it = m_map.find(obj.first);
	if (it != m_map.end())
		return true;

	if (full())
		return false;

	m_map.insert({obj.first, obj.second});
	m_cont->push(obj.first);
	return true;
}

template <typename TContainer, typename TKey, typename TValue> bool sc::ContainerCache<TContainer,TKey,TValue>::pull(std::pair<TKey,TValue> &obj)
{
	if (empty())
		return false;

	TKey key;
	m_cont->pull(key);
	obj.first = key;

	auto it = m_map.find(key);
	obj.second = it->second;
	m_map.erase(it);

	return true;
}

template <typename TContainer, typename TKey, typename TValue> bool sc::ContainerCache<TContainer,TKey,TValue>::peak(std::pair<TKey,TValue> &obj) const
{
	if (empty())
		return false;

	auto it = m_map.find(obj.first);
	if (it == m_map.end())
		return false;

	obj.second = it->second;
	return true;
}
