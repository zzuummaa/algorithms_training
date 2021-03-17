#include <vector>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "testcase.h"

template <typename T>
size_t two_sum(const std::vector<T>& values, std::pair<T, T> sum) {
	std::vector<bool> is_satisfy(sum.second - sum.first + 1, false);

	std::unordered_set<T> h;
	h.insert(values.begin(), values.end());

	for (auto t = sum.first; t <= sum.second; t++) {
		for (auto& y: values) {
			auto x = t - y;
			if (x == y) continue;
			if (h.count(x) > 0) {
				is_satisfy[t - sum.first] = true;
				break;
			}
		}
	}

	return std::count_if(is_satisfy.begin(), is_satisfy.end(), [](auto a){ return a; });
}

int main() {
	for (const auto & in_file_name : testcase_iterator("./")) {
		std::cout << in_file_name;
		std::flush(std::cout);

		testcase_streams streams;
		make_testcase_streams(streams, in_file_name);

		auto values = collect_sequence<long long>(streams.in, '\n');
		auto result = collect_val<size_t>(streams.out);

		bool is_ok = two_sum(values, { -10000, 10000 }) == result;
		std::cout << " - " << (is_ok ? "ok" : "failed") << std::endl;
	}
}