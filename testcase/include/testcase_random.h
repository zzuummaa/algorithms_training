#ifndef ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
#define ALGORITHMS_TRAINING_TESTCASE_RANDOM_H

#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

template <typename T>
using matrix = std::vector<std::vector<T>>;

matrix<int> random_matrix(size_t n);

template<typename T>
std::vector<T> random_vector(size_t n) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<T> distrib;

	std::vector<T> out(n);
	std::generate(out.begin(), out.end(), [&](){ return distrib(gen); });
	return out;
}

#endif //ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
