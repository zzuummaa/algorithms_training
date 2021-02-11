#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

template<typename RandomAccessIterator,
		typename Type = typename std::iterator_traits<RandomAccessIterator>::value_type>
size_t merge_count_inversions(RandomAccessIterator first, RandomAccessIterator mid, RandomAccessIterator last) {
	size_t inv_count = 0;
	std::vector<Type> merge_buf;
	merge_buf.reserve(last - first);

	auto left = first;
	auto right = mid;

	while (true) {
		if (*left <= *right) {
			merge_buf.push_back(*left);
			left++;
			if (left == mid) break;
		} else {
			inv_count += right - first - merge_buf.size();
			merge_buf.push_back(*right);
			right++;
			if (right == last) break;
		}
	}

	std::copy(left, mid, first + merge_buf.size());
	std::copy(merge_buf.begin(), merge_buf.end(), first);

	return inv_count;
}

template<typename RandomAccessIterator>
size_t sort_count_inversions(RandomAccessIterator first, RandomAccessIterator last) {
	size_t inv_count = 0;
	if ((last - first) > 1 ) {
		auto mid = first + (last - first) / 2;
		inv_count += sort_count_inversions(first, mid);
		inv_count += sort_count_inversions(mid, last);
		inv_count += merge_count_inversions(first, mid, last);
	}
	return inv_count;
}

template<typename RandomAccessIterator>
size_t count_inversions(RandomAccessIterator first, RandomAccessIterator last) {
	if (last - first < 2) return 0;

	size_t inv_count = 0;
	for (auto left = first; left < last - 1; ++left) {
		for (auto right = left + 1; right < last; ++right) {
			if (*left > *right) {
				inv_count++;
			}
		}
	}
	return inv_count;
}

template<typename Container>
bool test_sort_count_inversions(Container& container) {
	auto container_refer = container;
	size_t inv_count_refer = count_inversions(container_refer.begin(), container_refer.end());
	std::sort(container_refer.begin(), container_refer.end());

	size_t inv_count = sort_count_inversions(container.begin(), container.end());

	if (inv_count_refer != inv_count) {
		std::cout << "refer_inv_count != inv_count (" << inv_count_refer << " != " << inv_count << " )" << std::endl;
		return false;
	}

	if (container != container_refer) {
		std::cout << "[ ";
		for (const auto& it: container) std::cout << it << " ";
		std::cout << "]" << std::endl;
		return false;
	}

	return true;
}

int main() {
	std::vector<int> z = { };
	assert(test_sort_count_inversions(z));

	std::vector<int> a = { 1, 3 };
	assert(test_sort_count_inversions(a));

	std::vector<int> b = { 3, 1 };
	assert(test_sort_count_inversions(b));

	std::vector<int> c = { 1, 3, 5, 2, 4, 6 };
	assert(test_sort_count_inversions(c));

	std::vector<int> d = { 1, 1, 1, 0 };
	assert(test_sort_count_inversions(d));

	std::vector<int> e = { 6, 1, 2, 3, 4, 2 };
	assert(test_sort_count_inversions(d));
}
