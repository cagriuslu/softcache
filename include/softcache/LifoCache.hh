#pragma once

#include <utility>
#include <unordered_map>
#include "ContainerCache.hh"
#include "FixedStack.hh"

namespace sc
{
	template <typename TKey, typename TValue>
	using LifoCache = ContainerCache<FixedStack<TKey>,TKey,TValue>;
}
