#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <functional>
#include "testcase.h"

const size_t INFINITY_DISTANCE = 1000 * 1000;

struct weighted_bidirectional_edge {
	size_t v;
	size_t w;
	size_t l;
	size_t dist;
};

struct less_distance : public std::binary_function<weighted_bidirectional_edge, weighted_bidirectional_edge, bool> {
	bool operator()(const weighted_bidirectional_edge& a, const weighted_bidirectional_edge& b) const {
		return a.dist > b.dist;
	}
};

std::vector<size_t> dijkstra(const std::vector<std::vector<weighted_edge>>& adj_list, size_t s) {
	std::vector<size_t> distances(adj_list.size(), INFINITY_DISTANCE);
	distances[s] = 0;

	std::vector<bool> is_visited(adj_list.size(), false);
	is_visited[s] = true;

	std::priority_queue<weighted_bidirectional_edge, std::vector<weighted_bidirectional_edge>, less_distance> adjacent;
	for (const auto& it: adj_list[s]) {
		adjacent.push(weighted_bidirectional_edge {
			.v = s,
			.w = it.v,
			.l = it.l,
			.dist = it.l
		});
	}


	while (!adjacent.empty()) {
		auto vw = adjacent.top();
		adjacent.pop();
		if (is_visited[vw.w]) continue;

		is_visited[vw.w] = true;
		distances[vw.w] = distances[vw.v] + vw.l;
		for (const auto& it: adj_list[vw.w]) {
			adjacent.push(weighted_bidirectional_edge {
				.v = vw.w,
				.w = it.v,
				.l = it.l,
				.dist = distances[vw.w] + it.l
			});
		}
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
