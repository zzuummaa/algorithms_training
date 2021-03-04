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
		for (auto v: adjacent) {
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
		std::cout << in_file_name;
		std::flush(std::cout);

		testcase_streams streams;
		make_testcase_streams(streams, in_file_name);

		auto adj_list = collect_adj_list<weighted_edge>(streams.in);
		auto distances = dijkstra(adj_list, 0);

		auto distances_ref = collect_sequence<size_t>(streams.out);
		std::vector<size_t> suitable_vertexes = { 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };

		bool is_ok = std::equal(
			suitable_vertexes.begin(),
			suitable_vertexes.begin() + std::min(std::min(distances.size(), distances_ref.size()), suitable_vertexes.size()),
			distances_ref.begin(),
			[&](auto a, auto b) {
				return distances[a - 1] == b;
			}
		);

		std::cout << " - " << (is_ok ? "ok" : "failed") << std::endl;
	}
}
