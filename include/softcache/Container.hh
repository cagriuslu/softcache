#pragma once

#include <cstdlib>

namespace sc
{
	template <typename T> class Container
	{
	public:
		virtual std::size_t capacity() const = 0;
		virtual std::size_t size() const = 0;
		virtual bool empty() const = 0;
		virtual bool full() const = 0;

		virtual bool push(const T &obj) = 0;
		virtual bool pull(T &obj) = 0;
		virtual bool peak(T &obj) const = 0;
	};
}
