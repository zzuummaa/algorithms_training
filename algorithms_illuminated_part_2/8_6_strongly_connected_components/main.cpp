#include <string>
#include <iostream>
#include "testcase.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstring>
#include <stack>

std::vector<size_t> topology_order(const std::vector<std::vector<size_t>>& adj_list) {
	std::vector<size_t> ordered_vertexes(adj_list.size(), 0);
	std::vector<char> is_visited(adj_list.size(), 0);
	size_t order_counter = adj_list.size();

	std::stack<size_t> stack;
	std::stack<size_t> stack_v_idx;
	for (size_t i = 0; i < adj_list.size(); i++) {
		if (is_visited[i]) continue;
		stack.push(i);
		stack_v_idx.push(0);
		is_visited[i] = true;
		while (!stack.empty()) {
			auto v = stack.top();

			if (stack_v_idx.top() >= adj_list[v].size()) {
				stack_v_idx.pop();
				stack.pop();

				order_counter--;
				ordered_vertexes[order_counter] = v;

			} else {
				auto w = adj_list[v][stack_v_idx.top()];
				stack_v_idx.top()++;

				if (is_visited[w]) continue;
				is_visited[w] = true;
				stack.push(w);
				stack_v_idx.push(0);
			}
		}
	}

	return ordered_vertexes;
}

void dfs_scc(const std::vector<std::vector<size_t>>& adj_list,
			 std::vector<bool>& is_visited,
			 std::vector<size_t>& scc_vertexes,
			 size_t s) {

	std::stack<size_t> stack;
	stack.push(s);
	is_visited[s] = true;
	while (!stack.empty()) {
		auto v = stack.top();
		stack.pop();
		scc_vertexes.emplace_back(v);
		for (auto w: adj_list[v]) {
			if (is_visited[w]) continue;
			is_visited[w] = true;
			stack.push(w);
		}
	}
}

std::vector<std::vector<size_t>> transposed_graph(const std::vector<std::vector<size_t>>& adj_list) {
	std::vector<std::vector<size_t>> adj_list_transposed(adj_list.size());
	for (size_t v = 0; v < adj_list.size(); v++) {
		for (auto w: adj_list[v]) {
			adj_list_transposed[w].emplace_back(v);
		}
	}
	return adj_list_transposed;
}

/**
 * Algorithm Kosaraju
 *
 * @return strongly connected components with vertexes
 */
std::vector<std::vector<size_t>> scc(const std::vector<std::vector<size_t>>& adj_list) {
	auto ordered_vertexes = topology_order(transposed_graph(adj_list));

	std::vector<bool> is_visited(adj_list.size(), false);
	std::vector<std::vector<size_t>> components;

	for (auto v_it = ordered_vertexes.begin(); v_it < ordered_vertexes.end(); v_it++) {
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

	std::vector<std::vector<size_t>> b = {
			{ 1, 2 },
			{ 0    },
			{      }
	};
	components = scc(b);
	for (auto& component: components) {
		std::sort(component.begin(), component.end());
	}
	std::sort(components.begin(), components.end(), [](auto& a, auto& b){ return a.front() < b.front(); });
	assert((components[0] == std::vector<size_t>{ 0, 1 }));
	assert((components[1] == std::vector<size_t>{ 2    }));

	for (const auto & in_file_name : testcase_iterator("./")) {
		std::cout << in_file_name << std::endl;

		testcase_streams streams;
		make_testcase_streams(streams, in_file_name);

		auto adj_list = collect_adj_list_from_edges<size_t>(streams.in);
		auto counts_refer = collect_sequence<size_t>(streams.out);

		components = scc(adj_list);
		std::sort(components.begin(), components.end(), [](auto& a, auto& b){ return a.size() > b.size(); });

		assert(
			std::equal(
				components.begin(),
				components.begin() + std::min(components.size(), counts_refer.size()),
				counts_refer.begin(),
				[](const auto& a, const auto& b) { return a.size() == b;}
			)
		);
	}
}