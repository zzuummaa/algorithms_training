#ifndef ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
#define ALGORITHMS_TRAINING_TESTCASE_RANDOM_H

#include <vector>
#include <random>
#include <algorithm>

std::default_random_engine& random_engine();

template <typename T>
using matrix = std::vector<std::vector<T>>;

matrix<int> random_matrix(size_t n);

template<typename T>
std::vector<T> random_vector(size_t n) {
	std::uniform_int_distribution<T> distrib;
	std::vector<T> out(n);
	std::generate(out.begin(), out.end(), [&](){ return distrib(random_engine()); });
	return out;
}

#endif //ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
