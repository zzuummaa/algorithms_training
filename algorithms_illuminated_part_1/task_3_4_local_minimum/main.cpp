/*
 * Дана решетка n на n разных чисел. Число является
 * локальным минимумом, если оно меньше, чем все его соседи. (Сосед числа—
 * это элемент, который находится выше, ниже, слева или справа. Большинство
 * чисел имеют четырех соседей; числа, расположенные по сторонам решетки,
 * имеют трех; четыре угловых числа имеют двух соседей.) Используйте
 * парадигму разработки алгоритмов «разделяй и властвуй» для вычисления
 * локального минимума, со сравнениями между парами чисел, со временем
 * исполнения О(n).
 *
 * (Примечание: поскольку во входных данных имеется n^2 чисел, вы не можете
 * позволить себе просматривать их все.)
 *
 * [Подсказка: выясните, как выполнять рекурсию на решетке — на — за время
 * исполнения О(n).]
 * */

#include <iostream>
#include <vector>
#include <set>
#include <iterator>
#include <cassert>
#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>

template <typename T>
using matrix = std::vector<std::vector<T>>;

struct rectangle {
	size_t left;
	size_t right;
	size_t top;
	size_t bottom;

	[[nodiscard]] size_t n() const {
		return right - left;
	}
};

template <typename T>
rectangle matrix_dimension(const matrix<T>& m) {
	if (m.empty()) {
		return rectangle{};
	} else {
		return rectangle {
			.left = 0,
			.right = m[0].size(),
			.top = 0,
			.bottom = m.size(),
		};
	}
}

std::ostream &operator<<(std::ostream &os, const std::pair<size_t, size_t> &xy) {
	os << "(" << xy.first << ", " << xy.second << ")";
	return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const matrix<T> &m) {
	std::cout << "{" << std::endl;
	for (auto& it: m) {
		std::cout << "\t{ ";
		std::copy(it.begin(), it.end(), std::ostream_iterator<T>(std::cout, ",\t"));
		std::cout << "}," << std::endl;
	}
	std::cout << "}";

	return os;
}

template<typename T>
bool is_local_minimum(const matrix<T>& m, size_t x, size_t y) {
	bool is_min = true;
	if (x > 0) {
		if (m[y][x - 1] < m[y][x]) is_min = false;
	}
	if (x < m[0].size() - 1) {
		if (m[y][x + 1] < m[y][x]) is_min = false;
	}
	if (y > 0) {
		if (m[y - 1][x] < m[y][x]) is_min = false;
	}
	if (y < m.size() - 1) {
		if (m[y + 1][x] < m[y][x]) is_min = false;
	}

	return is_min;
}

template<typename T>
std::pair<size_t , size_t> find_local_minimum(const matrix<T>& m, rectangle rect) {
	if ((rect.right - rect.left) != (rect.bottom - rect.top)) {
		std::cout << m << std::endl;
		assert((rect.right - rect.left) == (rect.bottom - rect.top));
	}

	if (rect.n() == 1) return std::make_pair(rect.left, rect.top);
	if (rect.n() == 2) {
		size_t min_y = rect.top;
		size_t min_x = rect.left;
		for (size_t x = rect.left; x < rect.right; x++) {
			for (size_t y = rect.top; y < rect.bottom; y++) {
				if (m[y][x] < m[min_y][min_x]) {
					min_x = x;
					min_y = y;
				}
			}
		}
		return std::make_pair(min_x, min_y);
	}

	size_t min_y = rect.top;
	size_t mid_x = rect.left + (rect.right - rect.left) / 2;
	for (size_t y = rect.top; y < rect.bottom; ++y) {
		if (is_local_minimum(m, mid_x, y)) return std::make_pair(mid_x, y);
		if (m[y][mid_x] < m[min_y][mid_x]) {
			min_y = y;
		}
	}

//	if (is_local_minimum(m, mid_x, rect.top)) return std::make_pair(mid_x, rect.top);
//	if (is_local_minimum(m, mid_x, min_y)) return std::make_pair(mid_x, min_y);
//	if (is_local_minimum(m, mid_x, rect.bottom - 1)) return std::make_pair(mid_x, rect.bottom - 1);

	if (m[min_y][mid_x - 1] < m[min_y][mid_x + 1]) {
		rect.right = mid_x;
	} else {
		rect.left = mid_x + (rect.right - rect.left) % 2;
	}

	size_t min_x = rect.left;
	size_t mid_y = rect.top + (rect.bottom - rect.top) / 2;
	for (size_t x = rect.left; x < rect.right; ++x) {
		if (is_local_minimum(m, x, mid_y)) return std::make_pair(x, mid_y);
		if (m[mid_y][x] < m[mid_y][min_x]) {
			min_x = x;
		}
	}

//	if (is_local_minimum(m, rect.left, mid_y)) return std::make_pair(rect.left, mid_y);
//	if (is_local_minimum(m, min_x, mid_y)) return std::make_pair(min_x, mid_y);
//	if (is_local_minimum(m, rect.right - 1, mid_y)) return std::make_pair(rect.right - 1, mid_y);

	if (m[mid_y + 1][min_x] < m[mid_y - 1][min_x]) {
		rect.top = mid_y + (rect.bottom - rect.top) % 2;
	} else {
		rect.bottom = mid_y;
	}

	return find_local_minimum(m, rect);
}

template<typename T>
std::set<std::pair<size_t , size_t>> find_local_minimums_simple(const matrix<T>& m) {
	std::set<std::pair<size_t , size_t>> local_minimums;
	for (size_t x = 0; x < m[0].size(); x++) {
		for (size_t y = 0; y < m.size(); y++) {
			if (is_local_minimum(m, x, y)) local_minimums.emplace(x, y);
		}
	}
	return local_minimums;
}

bool test_find_local_minimums(const matrix<int>& m) {
	auto local_minimums_refer = find_local_minimums_simple(m);
	auto local_minimum = find_local_minimum(m, matrix_dimension(m));

	if (local_minimums_refer.find(local_minimum) == local_minimums_refer.end()) {
		std::cout << "invalid local minimum " << local_minimum << std::endl;
		std::cout << "not in: [";
		for (const auto& it: local_minimums_refer) std::cout << it << ", ";
		std::cout << "]" << std::endl;
		std::cout << m << std::endl;
		return false;
	}
	return true;
}

matrix<int> random_matrix(size_t n) {
	std::vector<int> numbers;
	for(size_t i = 0; i < n * n; i++) numbers.push_back(i);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));

	matrix<int> m;
	for (size_t x = 0; x < n; x++) {
		m.emplace_back();
		for (size_t y = 0; y < n; y++) {
			m.back().push_back(numbers[y + x * n]);
		}
	}
	return m;
}

int main() {
	matrix<int> a1 = {
		{ 3 }
	};
	assert(test_find_local_minimums(a1));

	matrix<int> a2 = {
			{ 1, 2 },
			{ 3, 4 }
	};
	assert(test_find_local_minimums(a2));

	matrix<int> a4 = {
			{ 6 ,    12,      7 ,      0,       },
			{ 2 ,    3 ,      10,      5,       },
			{ 15,    4 ,      13,      9,       },
			{ 14,    1 ,      11,      8,       }
	};
	assert(test_find_local_minimums(a4));

	matrix<int> b4 = {
			{ 5,    15,     4,      3,      },
			{ 10,   1,      14,     13,     },
			{ 8,    0,      6,      9,      },
			{ 7,    2,      12,     11,     },
	};
	assert(test_find_local_minimums(b4));

	matrix<int> a5 = {
			{ 5,    15,     2,      1,      17,     },
			{ 23,   21,     13,     22,     18,     },
			{ 7,    4,      10,     9,      12,     },
			{ 11,   6,      3,      8,      14,     },
			{ 19,   0,      20,     24,     16,     },
	};
	assert(test_find_local_minimums(a5));

	matrix<int> a9 {
			{ 62,   4,      12,     22,     5,      2,      29,     61,     42,     },
			{ 54,   70,     37,     55,     38,     75,     7,      50,     48,     },
			{ 59,   11,     78,     9,      24,     17,     18,     27,     53,     },
			{ 71,   51,     77,     39,     10,     14,     40,     36,     60,     },
			{ 8,    26,     28,     1,      41,     19,     68,     35,     21,     },
			{ 67,   79,     15,     23,     47,     25,     56,     57,     34,     },
			{ 20,   73,     76,     49,     58,     46,     74,     3,      31,     },
			{ 69,   72,     64,     6,      63,     65,     33,     30,     43,     },
			{ 44,   52,     80,     0,      13,     16,     45,     66,     32,     },
	};
	assert(test_find_local_minimums(a9));

	for (int i = 0; i < 100; i++) {
		for (int j = 2; j < 300; j++) {
			assert(test_find_local_minimums(random_matrix(j)));
		}
	}

}