#pragma once

#include <cassert>
#include <functional>
#include <stdexcept>
#include <iostream>

inline bool is_throw(const std::function<void()>& stmt)
{
	try {
		stmt();
	} catch (...) {
		return true;
	}
	return false;
}

template <typename ContainerA, typename ContainerB>
bool is_containers_eq(const ContainerA& a, const ContainerB& b)
{
	if (a.size() != b.size()) {
		return false;
	}
	return std::equal(a.begin(), a.end(), b.begin());
}

struct TestSuite {
	const char* name;
	explicit TestSuite(const char* n): name(n)
	{
		std::cout << "[ RUN ] " << name << std::endl;
	}

	~TestSuite()
	{
		std::cout << "[ OK  ] " << name << std::endl;
	}
};

struct Tracker {
	static inline int alive = 0;

	int value = 0;

	Tracker() { ++alive; }
	Tracker(int v) : value(v) { ++alive; }

	Tracker(const Tracker &o) : value(o.value)
	{
		maybe_throw();
		++alive;
	}

	Tracker(Tracker &&o) noexcept : value(o.value)
	{
		++alive;
		o.value = -1;
	}

	Tracker &operator=(const Tracker &o)
	{
		maybe_throw();
		value = o.value;
		return *this;
	}

	Tracker &operator=(Tracker &&o) noexcept
	{
		value = o.value;
		o.value = -1;
		return *this;
	}

	~Tracker() { --alive; }

	static inline int throw_on_copy_n = -1; // -1 = не бросать
	static inline int copy_count = 0;

	static void reset_throws()
	{
		throw_on_copy_n = -1;
		copy_count = 0;
	}

	operator int() const { return value; }

private:
	static void maybe_throw()
	{
		if (throw_on_copy_n >= 0 && copy_count++ == throw_on_copy_n) {
			throw std::runtime_error("Tracker copy error");
		}
	}
};

inline bool operator==(const Tracker& lhs, const int& rhs)
{
	return lhs.value == rhs;
}

inline bool operator==(const int& lhs, const Tracker& rhs)
{
	return rhs == lhs;
}
