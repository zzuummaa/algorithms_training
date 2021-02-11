#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

template<typename RandomAccessIterator, typename Compare>
void merge_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if (last - first > 1) {
		auto middle = first + (last - first) / 2;
		merge_sort(first, middle, comp);
		merge_sort(middle, last, comp);

		std::inplace_merge(first, middle, last);
	}
}

template<typename Container>
bool test_merge_sort(Container& container) {
	auto container_refer = container;
	std::sort(container_refer.begin(), container_refer.end());
	merge_sort(container.begin(), container.end(), std::less<>());
	bool is_eq = container == container_refer;
	if (!is_eq) {
		std::cout << "[ ";
		for (const auto& it: container) std::cout << it << " ";
		std::cout << "]" << std::endl;
	}
	return container == container_refer;
}

int main() {
	std::vector<int> a = { 1 };
	assert(test_merge_sort(a));

	std::vector<int> b = { 2, 1 };
	assert(test_merge_sort(b));

	std::vector<int> c = { 2, 3, 1 };
	assert(test_merge_sort(c));

	std::vector<int> d = { 1, 2, 4, 3, 1 };
	assert(test_merge_sort(d));
}
