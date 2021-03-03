#ifndef ALGORITHMS_TRAINING_TEST_CASE_H
#define ALGORITHMS_TRAINING_TEST_CASE_H

#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include "testcase_collector.h"

struct testcase_streams {
	std::ifstream in;
	std::ifstream out;

	testcase_streams() = default;

	explicit operator bool() const;
};

bool make_testcase_streams(testcase_streams&, std::string_view n);

class testcase_iterator {
	DIR *dir;
	std::string_view in_file_name;

public:
	testcase_iterator();
	explicit testcase_iterator(const char* d);

	bool increment();
	testcase_iterator& operator++();

	const std::string_view& operator*() const;
	const std::string_view* operator->() const;

	bool operator==(const testcase_iterator &rhs) const;
	bool operator!=(const testcase_iterator &rhs) const;
};

inline testcase_iterator begin(testcase_iterator iter) noexcept {
	return iter;
}

inline testcase_iterator end(const testcase_iterator&) noexcept {
	return testcase_iterator();
}

#endif //ALGORITHMS_TRAINING_TEST_CASE_H
