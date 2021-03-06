#pragma once

#include <utility>
#include <unordered_map>
#include "ContainerCache.hh"
#include "CircularQueue.hh"

namespace sc
{
	template <typename TKey, typename TValue>
	using FifoCache = ContainerCache<CircularQueue<TKey>,TKey,TValue>;
}
