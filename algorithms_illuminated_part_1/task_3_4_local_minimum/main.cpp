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
#include <optional>
#include <cassert>
#include <cmath>

template <typename T>
using matrix = std::vector<std::vector<T>>;

struct rectangle {
	size_t left;
	size_t right;
	size_t bottom;
	size_t top;

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
			.bottom = 0,
			.top = m.size(),
		};
	}
}

template<typename T>
std::pair<size_t , size_t> find_local_minimum(const matrix<T>& m, rectangle rect) {
	assert(rect.right - rect.left == rect.bottom - rect.top);

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
	size_t mid_x = rect.left + std::ceil(float(rect.right - rect.left) / 2.f);
	for (size_t y = rect.top + 1; y < rect.bottom; ++y) {
		if (m[y][mid_x] < m[min_y][mid_x]) {
			min_y = y;
		}
	}

	// FIXME check if it local minimum
	if (std::min(m[min_y][mid_x + 1], m[min_y][mid_x - 1]) > m[min_y][mid_x]) return std::make_pair(mid_x, min_y);

	if (m[min_y][mid_x - 1] < m[min_y][mid_x + 1]) {
		rect.right = mid_x;
	} else {
		rect.left = mid_x + 1;
	}

	size_t min_x = rect.left;
	for (size_t x = rect.left + 1; x < rect.right; ++x) {
		if (m[min_y][x] < m[min_y][min_x]) {
			min_x = x;
		}
	}
}

int main() {
	matrix<int> b = {
		{ 1, 2 },
		{ 3, 4 }
	};

	find_local_minimum(b, matrix_dimension(b));
}