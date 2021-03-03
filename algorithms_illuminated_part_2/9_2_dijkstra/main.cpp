#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>
#include "testcase.h"

const size_t INFINITY_DISTANCE = 1000 * 1000;

std::vector<size_t> dijkstra(const std::vector<std::vector<weighted_edge>>& adj_list, size_t s) {
	std::vector<size_t> distances(adj_list.size(), INFINITY_DISTANCE);
	distances[s] = 0;

	std::vector<bool> is_visited(adj_list.size(), false);
	is_visited[s] = true;

	std::vector<size_t> adjacent;
	adjacent.reserve(adj_list.size());
	adjacent.push_back(s);

	while (adjacent.size() < adj_list.size()) {
		size_t w_min = adj_list.size();
		size_t min_dist = INFINITY_DISTANCE;
		for (size_t v = 0; v < adjacent.size(); v++) {
			for (size_t i = 0; i < adj_list[v].size(); i++) {
				auto w = adj_list[v][i].v;
				if (is_visited[w]) continue;

				auto dist = distances[v] + adj_list[v][i].l;
				if (dist < min_dist) {
					min_dist = dist;
					w_min = w;
				}
			}
		}

		if (w_min == adj_list.size()) break;

		adjacent.push_back(w_min);
		is_visited[w_min] = true;
		distances[w_min] = min_dist;
	}

	return distances;
}

int main() {
	for (const auto & in_file_name : testcase_iterator("./")) {
		std::cout << in_file_name << std::endl;

		testcase_streams streams;
		make_testcase_streams(streams, in_file_name);

		auto adj_list = collect_adj_list<weighted_edge>(streams.in);
		auto distances = dijkstra(adj_list, 0);
		std::sort(distances.begin(), distances.end(), [](auto a, auto b){
			if (a == INFINITY_DISTANCE) a = 0;
			if (b == INFINITY_DISTANCE) b = 0;
			return a > b;
		});

		auto distances_ref = collect_sequence<size_t>(streams.out);
//		std::copy(distances_ref.begin(), distances_ref.end(), std::ostream_iterator<size_t>(std::cout, " "));
		assert(
			std::equal(
				distances.begin(),
				distances.begin() + std::min(distances.size(), distances_ref.size()),
				distances_ref.begin()
			)
		);
	}
}
