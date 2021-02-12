#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

template<typename RandomAccessIterator, typename Compare>
void qsort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if (last - first < 2) return;

	RandomAccessIterator left = first + 1;

	for (auto it = first + 1; it < last; it++) {
		if (comp(*it, *first)) {
			std::swap(*it, *left);
			left++;
		}
	}
	std::swap(*first, *(left - 1));
	qsort(first, left - 1, comp);
	qsort(left, last, comp);
}

template<typename Container>
bool test_merge_sort(Container& container) {
	auto container_refer = container;
	std::sort(container_refer.begin(), container_refer.end());
	qsort(container.begin(), container.end(), std::less<>());
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

	std::vector<int> d = { 2, 1, 3 };
	assert(test_merge_sort(d));

	std::vector<int> e = { 2148, 9058, 7742, 3153, 6324, 609, 7628, 5469, 7017, 504 };
	assert(test_merge_sort(e));

	std::vector<int> f = {
		2148, 9058, 7742, 3153, 6324, 609, 7628, 5469, 7017, 504, 4092, 1582, 9572, 1542, 5697, 2081,
		4218, 3130, 7923, 9595, 6558, 3859, 9832, 3062, 6788, 7578, 7432, 479, 8439, 9079, 7173, 2667,
		2770, 2655, 972, 4264, 2014, 3171, 4715, 345, 4388, 3816, 8887, 3915, 3490, 2327, 123, 4596,
		4307, 8737, 4007, 6798, 6551, 1627, 1190, 4984, 2480, 3404, 2027, 4778, 2951, 2795, 5002,
		8121, 8910, 9593, 5254, 448, 6237, 5565, 1816, 392, 8143, 9310, 9293, 3138, 4869, 6756, 872,
		6183, 3517, 3513, 1676, 5498, 9172, 5739, 6108, 7538, 7671, 5780, 8666, 540, 9771, 6837, 9341,
		1590, 5689, 1605, 1103, 5859
	};
}