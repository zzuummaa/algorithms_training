#include <vector>
#include <stack>
#include <cassert>

bool dfs(const std::vector<std::vector<int>>& adj_list, size_t start, size_t dest) {
	std::vector<bool> is_adj(adj_list.size(), false);
	std::stack<size_t> stack;
	stack.push(start);
	while (!stack.empty()) {
		auto v = stack.top();
		stack.pop();
		if (is_adj[v]) continue;
		is_adj[v] = true;
		for (auto w: adj_list[v]) {
			stack.push(w);
		}
	}
	return is_adj[dest];
}

int main() {
	std::vector<std::vector<int>> a = {
			{ 1, 2 },
			{ 0    },
			{ 1    },
			{ }
	};
	assert(dfs(a, 0, 0));
	assert(dfs(a, 0, 1));
	assert(!dfs(a, 0, 3));
}