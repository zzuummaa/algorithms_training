#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include "testcase_random.h"

template <typename T>
struct tree_node {
	tree_node<T>* left;
	tree_node<T>* right;
	tree_node<T>* parent;
	T value;

	tree_node(): left(nullptr), right(nullptr), parent(nullptr), value(0) {}

	void set_left(tree_node<T>* node) {
		node->parent = this;
		left = node;
	}

	void set_right(tree_node<T>* node) {
		node->parent = this;
		right = node;
	}

	tree_node* predecessor() {
		if (left != nullptr) {
			auto it = left;
			while (it->right != nullptr) it = it->right;
			return it;
		}
		if (parent != nullptr) {
			auto it = this;
			while (it->parent != nullptr) {
				if (it == it->parent->right) return it->parent;
				it = it->parent;
			}
			return nullptr;
		}

		return nullptr;
	}

	tree_node* successor() {
		if (right != nullptr) {
			auto it = right;
			while (it->left != nullptr) it = it->left;
			return it;
		}
		if (parent != nullptr) {
			auto it = this;
			while (it->parent != nullptr) {
				if (it == it->parent->left) return it->parent;
				it = it->parent;
			}
			return nullptr;
		}

		return nullptr;
	}

	tree_node* min() {
		auto it = this;
		if (it == nullptr) return nullptr;
		while (it->left != nullptr) it = it->left;
		return it;
	}

	tree_node* max() {
		auto it = this;
		if (it == nullptr) return nullptr;
		while (it->right != nullptr) it = it->right;
		return it;
	}
};

template <typename T>
class tree_node_iterator : public std::iterator<std::forward_iterator_tag, T> {
	tree_node<T>* cur;
public:
	tree_node_iterator() : cur(nullptr) {}
	explicit tree_node_iterator(tree_node<T>* cur) : cur(cur) {}

	tree_node_iterator<T>& operator++() {
		cur = cur->successor();
		return *this;
	}

	tree_node_iterator<T>& operator--() {
		cur = cur->predecessor();
		return *this;
	}

	inline T operator*() {
		return cur->value;
	}

	inline T* operator->() {
		return &cur->value;
	}

	bool operator==(const tree_node_iterator &rhs) const {
		return cur == rhs.cur;
	}

	bool operator!=(const tree_node_iterator &rhs) const {
		return !(rhs == *this);
	}
};

template <typename T>
class tree_set {
	tree_node<T>* top_node;
	std::vector<tree_node<T>> nodes;

	tree_node<T>* alloc_node(const T& key) {
		if (nodes.size() == nodes.capacity()) return nullptr;
		nodes.emplace_back();
		nodes.back().value = key;
		return &nodes.back();
	}

	inline tree_node<T>* top() {
		return top_node;
	}
public:
	typedef tree_node_iterator<T> iterator;

	explicit tree_set(size_t n): top_node(nullptr) {
		nodes.reserve(n);
	}

	iterator begin() {
		return iterator(top()->min());
	}

	iterator end() {
		return iterator();
	}

	iterator find(const T& key) {
		auto it = top();
		while (it != nullptr) {
			if (key == it->value) return iterator(it);
			if (key < it->value) {
				it = it->left;
			} else {
				it = it->right;
			}
		}
		return iterator();
	}

	iterator insert(const T& key) {
		auto it = top();

		if (it == nullptr) {
			top_node = alloc_node(key);
			return iterator(top_node);
		}

		while (true) {
			if (key == it->value) return iterator(it);
			if (key < it->value) {
				if (it->left == nullptr) {
					it->set_left(alloc_node(key));
					return iterator(it->left);
				}
				it = it->left;
			} else {
				if (it->right == nullptr) {
					it->set_right(alloc_node(key));
					return iterator(it->right);
				}
				it = it->right;
			}
		}
	}

	template< class InputIt>
	void insert(InputIt first, InputIt last) {
		for (auto it = first; it != last; it++) insert(*it);
	}
};

template <typename T>
bool test_tree_set(const std::vector<T>& container) {
	std::set<T> ref_sequence;
	ref_sequence.insert(container.begin(), container.end());
	tree_set<T> sequence(container.size());
	sequence.insert(container.begin(), container.end());

	if (!std::equal(ref_sequence.begin(), ref_sequence.end(), sequence.begin(), sequence.end())) {
		std::cout << "invalid sequence iterator" << std::endl;
		return false;
	}

	return true;
}

int main() {
	assert(test_tree_set(std::vector<int> { 0    }));
	assert(test_tree_set(std::vector<int> { 0, 1 }));
	assert(test_tree_set(std::vector<int> { 1, 0 }));

	for (size_t i = 0; i < 10 * 1000; i += 100) {
		assert(test_tree_set(random_vector<int>(i)));
	}
}