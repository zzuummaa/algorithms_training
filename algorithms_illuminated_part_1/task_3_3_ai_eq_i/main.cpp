/*
 * Дан отсортированный (от наименьшего до наибольшего элемента)
 * массив А из п разных целых чисел, которые могут быть положительными,
 * отрицательными или нулевыми. Определите, существует ли индекс / такой,
 * что a[i] = i. Разработайте самый быстрый алгоритм для решения этой задачи.
 * */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

template<typename RandomAccessIterator>
RandomAccessIterator find_value_eq_index(RandomAccessIterator first, RandomAccessIterator last) {
	auto left = first;
	auto right = last;

	while (right - left > 0) {
		auto mid = first + (right - left) / 2;
		if (*mid == (mid - first)) return mid;
		if (*mid > (mid - first)) {
			right = mid;
		} else {
			left = mid + 1;
		}
	}

	return last;
}

template<typename RandomAccessIterator>
RandomAccessIterator find_value_eq_index_simple(RandomAccessIterator first, RandomAccessIterator last) {
	for (auto it = first; it < last; it++) {
		if (*it == (it - first)) return it;
	}
	return last;
}

template<typename RandomAccessIterator>
bool test_find_value_eq_index(RandomAccessIterator first, RandomAccessIterator last) {
	auto value_eq_index_refer = find_value_eq_index_simple(first, last) - first;
	auto value_eq_index = find_value_eq_index(first, last) - first;

	if (value_eq_index_refer != value_eq_index) {
		std::cout << "value_eq_index_refer != value_eq_index (" << value_eq_index_refer << " != " << value_eq_index << " )" << std::endl;
		return false;
	}
	return true;
}

int main() {
	std::vector<int> a = { -1 };
	assert(test_find_value_eq_index(a.begin(), a.end()));

	std::vector<int> b = { 1 };
	assert(test_find_value_eq_index(b.begin(), b.end()));

	std::vector<int> c = { 0 };
	assert(test_find_value_eq_index(c.begin(), c.end()));

	std::vector<int> d = { -1, 0, 2, 3};
	assert(test_find_value_eq_index(d.begin(), d.end()));
}