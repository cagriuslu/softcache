#pragma once

#include "Container.hh"
#include <vector>
#include <stdexcept>

namespace sc
{
	template <typename T> class CircularQueue : public Container<T>
	{
		std::vector<T> m_buffer;
		std::size_t m_size;
		std::size_t m_begin;
		std::size_t m_end;
		bool m_isWrapped;

	public:
		CircularQueue(std::size_t capacity);

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
template <typename T> sc::CircularQueue<T>::CircularQueue(std::size_t capacity)
{
	if (capacity < 1)
		throw std::invalid_argument("Capacity is given as 0 (zero)");

	m_buffer.resize(capacity);
	m_size = capacity;
	m_begin = 0;
	m_end = 0;
	m_isWrapped = false;
}

template <typename T> std::size_t sc::CircularQueue<T>::capacity() const
{
	return m_size;
}

template <typename T> std::size_t sc::CircularQueue<T>::size() const
{
	if (m_isWrapped)
		return m_size;
	else if (m_begin == m_end)
		return 0;
	else if (m_begin < m_end)
		return m_end - m_begin;
	else // (m_end < m_begin)
		return m_end + m_size - m_begin;
}

template <typename T> bool sc::CircularQueue<T>::empty() const
{
	return (m_begin == m_end && m_isWrapped == false);
}

template <typename T> bool sc::CircularQueue<T>::full() const
{
	return m_isWrapped;
}

template <typename T> bool sc::CircularQueue<T>::push(const T &obj)
{
	if (m_isWrapped)
		return false;

	m_buffer[m_end] = obj;
	m_end = (m_end + 1) % m_size;

	if (m_end == m_begin)
		m_isWrapped = true;

	return true;
}

template <typename T> bool sc::CircularQueue<T>::pull(T &obj)
{
	if (peak(obj))
	{
		m_buffer[m_begin] = T();
		m_begin = (m_begin + 1) % m_size;
		m_isWrapped = false;
		return true;
	}
	else
		return false;
}

template <typename T> bool sc::CircularQueue<T>::peak(T &obj) const
{
	if (empty())
		return false;

	obj = m_buffer[m_begin];
	return true;
}
