#include <iostream>
#include <vector>
#include <cassert>

void topology_order_internal(const std::vector<std::vector<size_t>>& adj_list,
							 std::vector<bool>& is_visited,
							 std::vector<size_t>& ordered_vertexes,
							 size_t& order_counter,
							 size_t v) {
	is_visited[v] = true;

	for (auto w: adj_list[v]) {
		if (is_visited[w]) continue;
		topology_order_internal(adj_list, is_visited, ordered_vertexes, order_counter, w);
	}

	order_counter--;
	ordered_vertexes[order_counter] = v;
}

std::vector<size_t> topology_order(const std::vector<std::vector<size_t>>& adj_list) {
	std::vector<size_t> ordered_vertexes(adj_list.size(), 0);
	std::vector<bool> is_visited(adj_list.size(), false);
	size_t order_counter = adj_list.size();

	for (size_t v = 0; v < adj_list.size(); v++) {
		if (is_visited[v]) continue;
		topology_order_internal(adj_list, is_visited, ordered_vertexes, order_counter, v);
	}

	return ordered_vertexes;
}

int main() {
	std::vector<std::vector<size_t>> a = {
			{ 3    },
			{ 0, 2 },
			{ 3    },
			{      }
	};
	auto order = topology_order(a);
	assert(order[3] == 3);
	assert(order[1] == 0 || order[1] == 2);
	assert(order[2] == 0 || order[2] == 2);
	assert(order[0] == 1);
}