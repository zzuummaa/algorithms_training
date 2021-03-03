#ifndef ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H
#define ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H

#include <vector>
#include <fstream>

template<typename T>
std::vector<std::vector<T>> collect_adj_list_from_edges(std::ifstream &is) {
	std::vector<std::vector<T>> adj_list;
	T v, w;
	while (is >> v >> w) {
		if (adj_list.size() < v) adj_list.resize(v);
		if (adj_list.size() < w) adj_list.resize(w);
		adj_list[v - 1].emplace_back(w - 1);
	}

	return adj_list;
}

template<typename T>
std::vector<T> collect_sequence(std::ifstream &is) {
	std::vector<T> sequence;
	T val;
	while (is >> val) sequence.emplace_back(val);
	return sequence;
}

#endif //ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H
