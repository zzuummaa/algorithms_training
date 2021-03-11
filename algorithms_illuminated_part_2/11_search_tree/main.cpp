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

	void swap(tree_node<T>* node) {
		if (node->left == this) {
			node->left = left;
			left = node;
		} else {
			std::swap(node->left, left);
		}

		if (node->right == this) {
			node->right = right;
			right = node;
		} else {
			std::swap(node->right, right);
		}

		if (node->parent == this) {
			node->parent = parent;
			parent = node;
		} else {
			std::swap(node->parent, parent);
		}
	}

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
class tree_set;

template <typename T>
class tree_node_iterator : public std::iterator<std::forward_iterator_tag, T> {
	tree_node<T>* cur;
public:
	friend class tree_set<T>;

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

	void dealloc_node(tree_node<T>* np) {
		*np = tree_node<T>();
		np->swap(&nodes.back());
		nodes.pop_back();
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

	iterator erase(const T& key) {
		auto it = find(key);
		return erase(it);
	}

	iterator erase(iterator pos) {
		if (pos == end()) return pos;

		auto it = pos.cur;
		auto successor = it->successor();

		if (it->left != nullptr && it->right != nullptr) {
			it->swap(successor);
		}

		if (it->parent == nullptr) {
			top_node = successor == nullptr ? it->left : successor;
		} else {
			auto child = it->parent == it->parent->left ? &it->parent->left : &it->parent->right;
			*child = it->left != nullptr ? it->left : it->right;
		}

		dealloc_node(it);
		return iterator(successor);
	}
};

template <typename T>
bool test_tree_set(const std::vector<T>& container_insert, const std::vector<T>& container_erase) {
	std::set<T> ref_sequence;
	ref_sequence.insert(container_insert.begin(), container_insert.end());
	tree_set<T> sequence(container_insert.size());
	sequence.insert(container_insert.begin(), container_insert.end());

	if (!std::equal(ref_sequence.begin(), ref_sequence.end(), sequence.begin(), sequence.end())) {
		std::cout << "invalid insertion or iteration" << std::endl;
		return false;
	}

	if (container_erase.empty()) return true;

	for (auto& it: container_erase) {
		ref_sequence.erase(it);
		sequence.erase(it);
	}

	if (!std::equal(ref_sequence.begin(), ref_sequence.end(), sequence.begin(), sequence.end())) {
		std::cout << "invalid erasing" << std::endl;
		return false;
	}

	return true;
}

int main() {
	assert(test_tree_set<int>({ 0    }, {         }));
	assert(test_tree_set<int>({ 0, 1 }, { 2, 1, 0 }));
	assert(test_tree_set<int>({ 1, 0 }, { 1, 2, 0 }));

	for (size_t i = 0; i < 10 * 1000; i += 100) {
		assert(test_tree_set(random_vector<int>(i), random_vector<int>(i)));
	}
}