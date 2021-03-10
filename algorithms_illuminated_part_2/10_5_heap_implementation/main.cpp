#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <random>
#include <chrono>
#include <testcase_random.h>

template <typename T, typename Compare = std::less<>>
class priority_queue {
	std::vector<T> heap;
	Compare comp;

	typedef decltype(heap.begin()) iterator;
public:

	iterator begin() {
		return heap.begin();
	}

	iterator end() {
		return heap.end();
	}
	
	void push(const T& v) {
		heap.push_back(v);
		if (heap.size() >= 3) {
			for (size_t idx = heap.size() - 2; idx > 0; idx /= 2) {
				if (comp(heap[idx / 2 + 1], heap[idx + 1])) {
					std::swap(heap[idx + 1], heap[idx / 2 + 1]);
				} else {
					break;
				}
			}
		}

		if (heap.size() < 2) return;
		if (comp(heap[0], heap[1])) std::swap(heap[1], heap[0]);
		if (heap.size() < 3) return;
		if (comp(heap[0], heap[2])) std::swap(heap[2], heap[0]);
	}

	void pop() {
		if (heap.empty()) return;
		if (heap.size() == 1) {
			heap.clear();
			return;
		}

		heap[0] = heap.back();
		heap.resize(heap.size() - 1);

		if (heap.size() < 3) {
			if (comp(heap[0], heap[1])) std::swap(heap[0], heap[1]);
			return;
		}

		size_t idx;
		if (comp(heap[2], heap[1])) {
			if (comp(heap[0], heap[1])) {
				std::swap(heap[0], heap[1]);
				idx = 0;
			} else {
				return;
			}
		} else {
			if (comp(heap[0], heap[2])) {
				std::swap(heap[0], heap[2]);
				idx = 1;
			} else {
				return;
			}
		}

		while (true) {
			size_t lc_idx = (idx * 2);
			if (lc_idx + 1 >= heap.size()) return;

			size_t rc_idx = (idx * 2) + 1;
			if (rc_idx + 1 >= heap.size()) {
				if (comp(heap[idx + 1], heap[rc_idx + 1])) std::swap(heap[rc_idx + 1], heap[idx + 1]);
				return;
			}

			if (comp(heap[rc_idx + 1], heap[lc_idx + 1])) {
				if (comp(heap[idx + 1], heap[lc_idx + 1])) {
					std::swap(heap[lc_idx + 1], heap[idx + 1]);
					idx = lc_idx;
				} else {
					return;
				}
			} else {
				if (comp(heap[idx + 1], heap[rc_idx + 1])) {
					std::swap(heap[rc_idx + 1], heap[idx + 1]);
					idx = rc_idx;
				} else {
					return;
				}
			}
		}
	}

	T& top() {
		return heap.front();
	}

	bool empty() {
		return heap.empty();
	}
};

template<typename Container>
bool test_priority_queue(const Container& container) {
	typedef typename std::remove_const<typename std::remove_reference<decltype(container.front())>::type>::type queue_type;
	std::priority_queue<queue_type, std::vector<queue_type>> ref_queue;
	priority_queue<queue_type> queue;

	for (auto it: container) {
		ref_queue.push(it);
		queue.push(it);
		if (ref_queue.top() != queue.top()) {
			std::cout << "not eq on push" << std::endl;
			return false;
		}
	}

	while (!ref_queue.empty() && !queue.empty()) {
		if (ref_queue.top() != queue.top()) {
			std::cout << "not eq on pop" << std::endl;
			return false;
		}
		ref_queue.pop();
		queue.pop();
	}

	if (!ref_queue.empty() || !queue.empty()) {
		std::cout << "queue sizes not eq" << std::endl;
	}

	return true;
}

int main() {
	assert(test_priority_queue(std::vector<int> { 0, 1 }));

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<int> distrib;
	for (size_t i = 1; i < 1000; i++) {
		auto test_case = random_vector<int>(i);
		assert(test_priority_queue(test_case));
	}
}