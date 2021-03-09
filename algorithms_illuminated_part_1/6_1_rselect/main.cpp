#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>
#include <chrono>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine gen(seed);

template<typename RandomAccessIterator>
RandomAccessIterator rselect(RandomAccessIterator first, RandomAccessIterator last, size_t n_min_idx) {
	if (last - first < 1) assert(false);
	if (last - first == 1) {
		if (n_min_idx != 0) assert(false);
		return first;
	}

	if (last - first > 10) {
		std::uniform_int_distribution<size_t> distrib(0, last - first - 1);
		std::swap(*first, *(first + distrib(gen)));
	}

	auto left = first + 1;

	for (auto it = first + 1; it < last; it++) {
		if (*it < *first) {
			std::swap(*left, *it);
			left++;
		}
	}
	left--;
	std::swap(*first, *left);
	if (left - first == n_min_idx) {
		return left;
	} else if (n_min_idx < left - first) {
		return rselect(first, left, n_min_idx);
	} else {
		return rselect(left + 1, last, n_min_idx - (left + 1 - first));
	}
}

template<typename RandomAccessIterator>
RandomAccessIterator rselect_simple(RandomAccessIterator first, RandomAccessIterator last, size_t n_min_idx) {
	std::partial_sort(first, first + (n_min_idx + 1), last);
	return first + n_min_idx;
}

template<typename Container>
bool test_rselect(const Container& container, size_t n_min_idx) {
	auto container_refer = container;
	auto n_min_refer = *rselect_simple(container_refer.begin(), container_refer.end(), n_min_idx);
	auto container_copy = container;
	auto n_min = *rselect(container_copy.begin(), container_copy.end(), n_min_idx);

	if (n_min != n_min_refer) {
		std::cout << "n_min != n_min_refer (" << n_min << " != " << n_min_refer << ")" << std::endl;
		return false;
	}

	return true;
}

int main() {
	std::vector<int> a = { 1 };
	assert(test_rselect(a, 0));

	std::vector<int> b = { 1, 3, 2 };
	assert(test_rselect(b, 2));

	std::vector<int> c = { 1, 4, 10, 3 };
	for (int i = 0; i < c.size(); i++) assert(test_rselect(c, i));

	std::vector<int> d = { 1, 4, 10, 3, 5, 2, 7, 11, 9, 8, 6, 12 };
	for (int i = 0; i < d.size(); i++) assert(test_rselect(d, i));

	std::vector<int> e = { 1, 4, 10, 3, 5, 2, 7, 11, 9, 8, 6, 12, 4, 20, 0 };
	for (int i = 0; i < e.size(); i++) assert(test_rselect(e, i));
}