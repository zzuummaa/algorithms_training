#include "testcase_random.h"
#include <chrono>

static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::default_random_engine gen(seed);

std::default_random_engine &random_engine() {
	return gen;
}

matrix<int> random_matrix(size_t n) {
	std::vector<int> numbers;
	for(size_t i = 0; i < n * n; i++) numbers.push_back(i);

	std::shuffle(numbers.begin(), numbers.end(), random_engine());

	matrix<int> m;
	for (size_t x = 0; x < n; x++) {
		m.emplace_back();
		for (size_t y = 0; y < n; y++) {
			m.back().push_back(numbers[y + x * n]);
		}
	}
	return m;
}

