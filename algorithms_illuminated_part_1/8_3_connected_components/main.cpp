#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

std::vector<std::vector<size_t>> connected_components(const std::vector<std::vector<size_t>>& adj_list) {
	std::vector<std::vector<size_t>> cc;
	if (adj_list.empty()) return cc;

	std::vector<size_t> is_adj(adj_list.size(), false);
	std::queue<size_t> q;

	for (size_t i = 0; i < adj_list.size(); i++) {
		if (is_adj[i]) continue;
		cc.emplace_back();
		is_adj[i] = true;
		cc.back().emplace_back(i);
		q.push(i);
		while (!q.empty()) {
			auto v = q.front();
			q.pop();
			for (auto w: adj_list[v]) {
				if (is_adj[w]) continue;
				cc.back().emplace_back(w);
				is_adj[w] = true;
			}
		}
		std::sort(cc.back().begin(), cc.back().end());
	}

	return cc;
}

int main() {
	std::vector<std::vector<size_t>> a = {
		{ 1, 2 },
		{ 2    },
		{ 1    },
		{ 4    },
		{      }
	};

	std::vector<std::vector<size_t>> expected_res = {
		{ 0, 1, 2 },
		{ 3, 4    }
	};
	auto res = connected_components(a);
	assert(res == expected_res);
}