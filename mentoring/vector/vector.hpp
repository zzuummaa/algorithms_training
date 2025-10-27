#pragma once

#include <stdexcept>

template<typename T>
class Vector {
public:
	using iterator = T*;
	using const_iterator = const T*;

	Vector() = default;

	explicit Vector(const size_t size)
		: m_data(alloc(size))
		, m_size(size)
	{
		for (size_t i = 0; i < size; i++) {
			std::construct_at(m_data + i);
		}
	}

	Vector(std::initializer_list<T> other)
		: m_data(alloc(other.size()))
		, m_size(other.size())
	{
		size_t i = 0;
		try {
			for (const auto &el: other) {
				std::construct_at(m_data + i, el);
				++i;
			}
		} catch (...) {
			destroy_and_dealloc(&m_data, i);
			throw;
		}
	}

	Vector(const Vector &other)
		: m_data(alloc(other.m_size))
		, m_size(other.m_size)
	{
		size_t i = 0;
		try {
			for (; i < m_size; ++i) {
				std::construct_at(m_data + i, other.m_data[i]);
			}
		} catch (...) {
			destroy_and_dealloc(&m_data, i);
			throw;
		}
	}

	Vector(Vector &&other) noexcept
	{
		destroy_and_dealloc(&m_data, m_size);
		m_size = 0;

		swap(other);
	}

	~Vector()
	{
		destroy_and_dealloc(&m_data, m_size);
	}

	Vector &operator=(const Vector &other)
	{
		if (this == &other) {
			return *this;
		}

		Vector temp(other);
		swap(temp);

		return *this;
	}

	Vector &operator=(Vector &&other) noexcept
	{
		if (this == &other) {
			return *this;
		}

		Vector temp(std::move(*this));
		swap(other);

		return *this;
	}

	size_t size() const noexcept
	{
		return m_size;
	}

	T &at(size_t index)
	{
		if (index >= m_size) throw std::out_of_range("index out of range");
		return m_data[index];
	}

	iterator begin() noexcept
	{
		return m_data;
	}

	iterator end() noexcept
	{
		return m_data + m_size;
	}

	const_iterator begin() const noexcept
	{
		return m_data;
	}

	const_iterator end() const noexcept
	{
		return m_data + m_size;
	}

private:
	T *m_data = nullptr;
	size_t m_size = 0;

	static T* alloc(const size_t count)
	{
		if (count == 0) return nullptr;
		return static_cast<T*>( ::operator new(count * sizeof(T)) );
	}

	static void dealloc(T** data)
	{
		::operator delete(*data);
		*data = nullptr;
	}

	static void destroy_and_dealloc(T** data, size_t count) noexcept
	{
		for (size_t i = 0; i < count; ++i) {
			std::destroy_at(*data + i);
		}
		dealloc(data);
	}

	void swap(Vector &other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
	}
};
