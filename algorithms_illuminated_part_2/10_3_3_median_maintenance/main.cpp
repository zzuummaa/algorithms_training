#include <queue>
#include <cassert>
#include "testcase.h"

template <typename T>
class median_maintainer {
	std::priority_queue<T, std::vector<T>, std::less<>> left; // descending
	std::priority_queue<T, std::vector<T>, std::greater<>> right; // ascending

public:
	void push(const T& v) {
		if (left.empty()) {
			left.push(v);
			return;
		}

		if (right.empty() || left.size() > right.size()) {
			right.push(v);
		} else {
			left.push(v);
		}

		if (left.top() > right.top()) {
			auto top_left = left.top();
			auto top_right = right.top();
			left.pop();
			right.pop();
			left.push(top_right);
			right.push(top_left);
		}
	}

	[[nodiscard]] const T& median() const {
		if (left.size() < right.size()) {
			return right.top();
		} else {
			return left.top();
		}
	}
};

int main() {
	for (const auto & in_file_name : testcase_iterator("./")) {
		std::cout << in_file_name << " - ";
		std::flush(std::cout);

		testcase_streams streams;
		if (!make_testcase_streams(streams, in_file_name)) {
			std::cout << "invalid streams" << std::endl;
			continue;
		}

		median_maintainer<int> mm;
		int median_sum = 0;
		int value;
		while (streams.in >> value) {
			mm.push(value);
			median_sum += mm.median();
		}

		if (streams.out >> value) {
			std::cout << (median_sum + value ? "ok" : "failed") << std::endl;
		} else {
			std::cout << "invalid out stream" << std::endl;
		}
	}
}