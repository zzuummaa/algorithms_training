#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
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

std::vector<std::string> list_dir(const char *path) {
	std::vector<std::string> file_names;
	struct dirent *entry;
	DIR *dir = opendir(path);

	if (dir == NULL) {
		return file_names;
	}
	while ((entry = readdir(dir)) != NULL) {
		file_names.emplace_back(entry->d_name);
	}
	closedir(dir);
	return file_names;
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

	for (const auto & in_file_name : list_dir("./")) {
		if (in_file_name.rfind("input_mostlyCycles", 0) != 0) continue;
		std::cout << in_file_name << std::endl;

		std::ifstream is_in(in_file_name);

		std::vector<std::vector<size_t>> adj_list;
		size_t v, w;
		while (is_in >> v >> w) {
			if (adj_list.size() < v) adj_list.resize(v);
			if (adj_list.size() < w) adj_list.resize(w);
			adj_list[v - 1].emplace_back(w - 1);
		}

		components = scc(adj_list);
		std::sort(components.begin(), components.end(), [](auto& a, auto& b){ return a.size() > b.size(); });

		std::string id(in_file_name.begin() + strlen("input_mostlyCycles_"), in_file_name.end() - strlen(".txt"));
		std::string out_file_name = "output_mostlyCycles_" + id + ".txt";

		std::ifstream is_out(out_file_name);
		std::vector<size_t> counts_refer;
		size_t count;
		while (is_out >> count) counts_refer.emplace_back(count);

		std::vector<size_t> counts;
		for (size_t i = 0; i < std::min(components.size(), counts_refer.size()); i++) {
			counts.emplace_back(components[i].size());
		}

		assert(counts == counts_refer);
	}
}