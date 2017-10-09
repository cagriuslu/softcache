#pragma once

#include "Container.hh"
#include <vector>
#include <stdexcept>

namespace sc
{
	template <typename T> class FixedStack : public Container<T>
	{
		std::vector<T> m_buffer;
		std::size_t m_size;
		std::size_t m_sp;

	public:
		FixedStack(std::size_t capacity);

		std::size_t capacity() const;
		std::size_t size() const;
		bool empty() const;
		bool full() const;

		bool push(const T &obj);
		bool pull(T &obj);
		bool peak(T &obj) const;
	};
}

// implementations
template <typename T> sc::FixedStack<T>::FixedStack(std::size_t capacity)
{
	if (capacity < 1)
		throw std::invalid_argument("Capacity is given as 0 (zero)");

	m_buffer.resize(capacity);
	m_size = capacity;
	m_sp = 0;
}

template <typename T> std::size_t sc::FixedStack<T>::capacity() const
{
	return m_size;
}

template <typename T> std::size_t sc::FixedStack<T>::size() const
{
	return m_sp;
}

template <typename T> bool sc::FixedStack<T>::empty() const
{
	return (m_sp == 0);
}

template <typename T> bool sc::FixedStack<T>::full() const
{
	return (m_sp == m_size);
}

template <typename T> bool sc::FixedStack<T>::push(const T &obj)
{
	if (full())
		return false;

	m_buffer[m_sp++] = obj;
	return true;
}

template <typename T> bool sc::FixedStack<T>::pull(T &obj)
{
	if (peak(obj))
	{
		m_buffer[--m_sp] = T();
		return true;
	}
	else
		return false;
}

template <typename T> bool sc::FixedStack<T>::peak(T &obj) const
{
	if (empty())
		return false;

	obj = m_buffer[m_sp - 1];
	return true;
}
