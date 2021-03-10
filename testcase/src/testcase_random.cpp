#include "testcase_random.h"

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