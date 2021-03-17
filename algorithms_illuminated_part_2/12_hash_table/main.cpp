#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "testcase_random.h"

template <typename Value>
struct hash_table_node {
	hash_table_node(size_t next_idx, Value value, size_t hash) : next_idx(next_idx), value(value), hash(hash) {}

	size_t next_idx;
	Value value;
	size_t hash;
};

template <typename K, typename V>
class hash_table {
	typedef std::pair<const K, V> value_type;

	std::vector<hash_table_node<value_type>> node_storage;
	std::vector<size_t> bucket_start_idxs;

	float max_load_factor_;

public:

	hash_table() : bucket_start_idxs(1, SIZE_MAX), max_load_factor_(1.0) { }

	float max_load_factor() {
		return max_load_factor_;
	}

	float load_factor() {
		return static_cast<float>(node_storage.size()) / bucket_start_idxs.size();
	}

	void rehash(size_t count) {
		std::fill(bucket_start_idxs.begin(), bucket_start_idxs.end(), SIZE_MAX);
		bucket_start_idxs.resize(count, SIZE_MAX);

		for (size_t i = 0; i < node_storage.size(); i++) {
			auto& node = node_storage[i];
			auto start_idx = bucket_start_idxs[node.hash % bucket_start_idxs.size()];
			node.next_idx = start_idx;
			bucket_start_idxs[node.hash % bucket_start_idxs.size()] = i;
		}
	}

	void insert(const value_type& value) {
		auto hash = std::hash<K>{}(value.first);

		size_t start_idx = bucket_start_idxs[hash % bucket_start_idxs.size()];
		if (start_idx == SIZE_MAX) {
			bucket_start_idxs[hash % bucket_start_idxs.size()] = node_storage.size();
			node_storage.emplace_back(SIZE_MAX, value, hash);
		} else {
			auto* node = node_storage.data() + start_idx;
			while (node->next_idx != SIZE_MAX) {
				if (node->value.first == value.first) return;
				node = node_storage.data() + node->next_idx;
			}

			bucket_start_idxs[hash % bucket_start_idxs.size()] = node_storage.size();
			node_storage.emplace_back(start_idx, value, hash);
		}

		if (load_factor() > max_load_factor()) rehash(bucket_start_idxs.size() * 2);
	}

	std::optional<value_type*> find(const K& key) {
		auto hash = std::hash<K>{}(key);

		size_t start_idx = bucket_start_idxs[hash % bucket_start_idxs.size()];
		if (start_idx == SIZE_MAX) return std::nullopt;

		auto* node = node_storage.data() + start_idx;
		while (true) {
			if (node->value.first == key) return std::make_optional(&node->value);
			if (node->next_idx == SIZE_MAX) return std::nullopt;
			node = node_storage.data() + node->next_idx;
		};
	}
};

template <typename K, typename V>
bool test_hash_table(const std::vector<std::pair<K, V>>& container_insert, const std::vector<std::pair<K, V>>& container_find) {
	std::unordered_map<K, V> ref_table;
	hash_table<K, V> table;
	for (const auto& it: container_insert) {
		ref_table.insert(it);
		table.insert(it);
	}

	for (const auto& it: container_find) {
		auto first = ref_table.find(it.first);
		auto second = table.find(it.first);
		if (first == ref_table.end() && !second.has_value()) continue;
		if (!(first != ref_table.end() && second.has_value())) {
			std::cout << "invalid find result" << std::endl;
			return false;
		}

		if (*first != *second.value()) {
			std::cout << "invalid find result" << std::endl;
			return false;
		}
	}

	return true;
}

int main() {
	assert( (test_hash_table<int, int>({ {1, 0} }, { {1, 0}, {1, 1}, {2, 0} })) );
	assert( (test_hash_table<int, int>({ {1, 0}, {2, 0} }, { {1, 0}, {1, 1}, {2, 0} })) );

	for (size_t i = 1; i < 30 * 1000; i += 100) {
		auto container_insert = random_vector<std::pair<int, int>>(i);
		auto container_find = random_vector<std::pair<int, int>>(i);
		std::copy(container_insert.begin(), container_insert.end(), std::back_inserter(container_find));
		assert( (test_hash_table(container_insert,container_find)) );
	}
}