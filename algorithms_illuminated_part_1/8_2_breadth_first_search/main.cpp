#include <limits>
#include <vector>
#include <queue>
#include <cassert>

int bfs(const std::vector<std::vector<int>>& adj_list, size_t start, size_t dest) {
	std::vector<bool> is_adj(adj_list.size(), false);
	is_adj[start] = true;

	std::vector<size_t> dist(adj_list.size(), std::numeric_limits<size_t>::max());
	dist[start] = 0;

	std::queue<size_t> q;
	q.push(start);

	while (!q.empty()) {
		auto v = q.front();
		q.pop();
		for (auto w: adj_list[v]) {
			if (is_adj[w]) continue;
			is_adj[w] = true;
			q.push(w);
			dist[w] = dist[v] + 1;
		}
	}
	return dist[dest];
}

int main() {
	std::vector<std::vector<int>> a = {
		{ 1, 2 },
		{ 0    },
		{ 3    },
		{ 2    }
	};
	assert(bfs(a, 0, 0) == 0);
	assert(bfs(a, 0, 1) == 1);
	assert(bfs(a, 0, 3) == 2);
}