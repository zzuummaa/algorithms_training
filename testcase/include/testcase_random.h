#ifndef ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
#define ALGORITHMS_TRAINING_TESTCASE_RANDOM_H

#include <vector>
#include <random>
#include <algorithm>

std::default_random_engine& random_engine();

template <typename T>
using matrix = std::vector<std::vector<T>>;

matrix<int> random_matrix(size_t n);

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
std::vector<T> random_vector(size_t n) {
	std::uniform_int_distribution<T> distrib;
	std::vector<T> out(n);
	std::generate(out.begin(), out.end(), [&](){ return distrib(random_engine()); });
	return out;
}

template<typename T, std::enable_if_t<std::is_base_of<std::pair<decltype(T::first), decltype(T::second)>, T>::value, bool> = true>
std::vector<T> random_vector(size_t n) {
	std::uniform_int_distribution<decltype(T::first)> distrib_a;
	std::uniform_int_distribution<decltype(T::second)> distrib_b;
	std::vector<T> out(n);
	std::generate(out.begin(), out.end(), [&](){
		return std::make_pair(distrib_a(random_engine()), distrib_b(random_engine()));
	});
	return out;
}

#endif //ALGORITHMS_TRAINING_TESTCASE_RANDOM_H
