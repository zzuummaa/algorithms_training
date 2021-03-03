#include "testcase.h"

testcase_streams::operator bool() const {
	return in && out;
}

testcase_iterator::testcase_iterator(): dir(nullptr) {}

testcase_iterator::testcase_iterator(const char* d): dir(opendir(d)) { increment(); }

bool make_testcase_streams(testcase_streams& streams, std::string_view n) {
	std::string in_file_name(n);
	if (in_file_name.rfind("input_", 0) != 0) return false;
	streams.in = std::ifstream(in_file_name);

	auto id_pos_start = strlen("input_");
	auto id_pos_end = in_file_name.size() - strlen(".txt");
	if (in_file_name.find(".txt", id_pos_end) == std::string::npos) return false;

	std::string out_file_name = "output" + std::string(in_file_name.begin() + id_pos_start, in_file_name.begin() + id_pos_end) + ".txt";
	streams.out = std::ifstream(out_file_name);

	return true;
}

bool testcase_iterator::increment() {
	while (true) {
		auto* entry = readdir(dir);
		if (entry == nullptr) {
			closedir(dir);
			dir = nullptr;
			in_file_name = std::string_view();
			break;
		} else {
			in_file_name = std::string_view(entry->d_name);
			testcase_streams streams;
			if (make_testcase_streams(streams, in_file_name)) break;
		}
	}

	return dir != nullptr;
}

testcase_iterator& testcase_iterator::operator++() {
	increment();
	return *this;
}

bool testcase_iterator::operator==(const testcase_iterator &rhs) const {
	return dir == rhs.dir;
}

bool testcase_iterator::operator!=(const testcase_iterator &rhs) const {
	return !(rhs == *this);
}

const std::string_view &testcase_iterator::operator*() const {
	return in_file_name;
}

const std::string_view *testcase_iterator::operator->() const {
	return &in_file_name;
}
