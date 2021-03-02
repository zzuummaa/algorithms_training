#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

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

void dfs_scc(const std::vector<std::vector<size_t>>& adj_list,
			 std::vector<bool>& is_visited,
			 std::vector<size_t>& scc_vertexes,
			 size_t s) {
	is_visited[s] = true;
	scc_vertexes.push_back(s);
	for (auto v: adj_list[s]) {
		if (is_visited[v]) continue;
		dfs_scc(adj_list, is_visited, scc_vertexes, v);
	}
}

/**
 * Algorithm Kosaraju
 *
 * @return strongly connected components with vertexes
 */
std::vector<std::vector<size_t>> scc(const std::vector<std::vector<size_t>>& adj_list) {
	auto ordered_vertexes = topology_order(adj_list);

	std::vector<bool> is_visited(adj_list.size(), false);
	std::vector<std::vector<size_t>> components;

	for (auto v_it = ordered_vertexes.rbegin(); v_it < ordered_vertexes.rend(); v_it++) {
		auto v = *v_it;
		if (is_visited[v]) continue;
		components.emplace_back();
		dfs_scc(adj_list, is_visited, components.back(), v);
	}

	return components;
}



int main() {
	/**
	 * Page 86, figure 8.16 (I am add zero vertex to first scc)
	 */
	std::vector<std::vector<size_t>> a = {
			{ 1       },
			{ 3, 0    },
			{ 4, 10   },
			{ 5       },
			{ 7       },
			{ 1       },
			{ 10      },
			{ 9       },
			{ 6       },
			{ 2, 4, 8 },
			{ 8       },
			{ 8, 6    },
	};

	auto components = scc(a);
	assert(components.size() == 4);
	for (auto& component: components) {
		std::sort(component.begin(), component.end());
	}
	std::sort(components.begin(), components.end(), [](auto& a, auto& b){ return a.front() < b.front(); });
	assert((components[0] == std::vector<size_t>{ 0, 1, 3, 5 }));
	assert((components[1] == std::vector<size_t>{ 2, 4, 7, 9 }));
	assert((components[2] == std::vector<size_t>{ 6, 8, 10   }));
	assert((components[3] == std::vector<size_t>{ 11         }));
}