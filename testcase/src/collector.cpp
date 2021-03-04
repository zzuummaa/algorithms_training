#include <testcase_collector.h>

std::istream &operator>>(std::istream &is, weighted_edge &we) {
	std::string str;

	if (!std::getline(is, str, ',')) {
		is.setstate(is.rdstate() | std::ios::failbit);
		return is;
	}

	std::istringstream iss(str);
	iss >> we.v;
	is >> we.l;
	we.v--;

	return is;
}