#pragma once

#include "Container.hh"
#include <stdexcept>

namespace sc
{
	template <typename TKey, typename TValue> class CacheManager
	{
		Container< std::pair<TKey,TValue> > *m_cache;

		bool (*m_fetcher)(const TKey&, TValue&);
		void (*m_disposer)(const TKey&, TValue&);

	public:
		CacheManager(Container< std::pair<TKey,TValue> > *cache, bool (*fetcher)(const TKey&, TValue&), void (*disposer)(const TKey&, TValue&) = nullptr);

		std::size_t capacity();
		std::size_t size();
		bool empty();
		bool full();

		bool retrieve(const TKey &key, TValue &out_value);
		bool prefetch(const TKey &key);
		void discard();
		void flush();

	private:
		bool fetchAndPush(std::pair<TKey,TValue>&);
	};
}

// implementations
template <typename TKey, typename TValue> sc::CacheManager<TKey,TValue>::CacheManager(Container< std::pair<TKey,TValue> > *cache, bool (*fetcher)(const TKey&, TValue&), void (*disposer)(const TKey&, TValue&))
{
	if (cache)
		m_cache = cache;
	else
		throw std::invalid_argument("Cache container is received as (null)");

	if (fetcher)
		m_fetcher = fetcher;
	else
		throw std::invalid_argument("Fetcher is received as (null)");

	m_disposer = disposer;
}

template <typename TKey, typename TValue> std::size_t sc::CacheManager<TKey,TValue>::capacity()
{
	return m_cache->capacity();
}

template <typename TKey, typename TValue> std::size_t sc::CacheManager<TKey,TValue>::size()
{
	return m_cache->size();
}

template <typename TKey, typename TValue> bool sc::CacheManager<TKey,TValue>::empty()
{
	return m_cache->empty();
}

template <typename TKey, typename TValue> bool sc::CacheManager<TKey,TValue>::full()
{
	return m_cache->full();
}

template <typename TKey, typename TValue> bool sc::CacheManager<TKey,TValue>::retrieve(const TKey &key, TValue &out_value)
{
	std::pair<TKey,TValue> pair;
	pair.first = key;

	// check if key exists in cache
	if (m_cache->peak(pair))
	{
		out_value = pair.second;
		return true;
	}

	if (fetchAndPush(pair))
	{
		out_value = pair.second;
		return true;
	}
	else
		return false;
}

template <typename TKey, typename TValue> bool sc::CacheManager<TKey,TValue>::prefetch(const TKey &key)
{
	std::pair<TKey,TValue> pair;
	pair.first = key;

	// check if key exists in cache
	if (m_cache->peak(pair))
		return true;

	return fetchAndPush(pair);
}

template <typename TKey, typename TValue> void sc::CacheManager<TKey,TValue>::discard()
{
	std::pair<TKey,TValue> pair;
	m_cache->pull(pair);
	if (m_disposer)
		(*m_disposer)(pair.first, pair.second);
}

template <typename TKey, typename TValue> void sc::CacheManager<TKey,TValue>::flush()
{
	std::size_t sz = size();
	for (std::size_t i = 0; i < sz; i++)
		discard();
}

template <typename TKey, typename TValue> bool sc::CacheManager<TKey,TValue>::fetchAndPush(std::pair<TKey,TValue> &pair)
{
	// try to fetch item
	if ((*m_fetcher)(pair.first, pair.second) == false)
		return false;

	// put item in cache
	if (m_cache->push(pair) == false)
	{
		discard();
		m_cache->push(pair);
	}

	return true;
}
