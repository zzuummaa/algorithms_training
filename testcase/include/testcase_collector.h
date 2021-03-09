#ifndef ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H
#define ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H

#include <vector>
#include <sstream>
#include <cassert>
#include <iterator>
#include <iostream>

template <class T>
class csv_istream_iterator: public std::iterator<std::input_iterator_tag, T>
{
	std::istream * _input;
	char _delim;
	std::string _value;
public:
	csv_istream_iterator( char delim = ',' ): _input( 0 ), _delim( delim ) {}
	csv_istream_iterator( std::istream & in, char delim = ',' ): _input( &in ), _delim( delim ) { ++*this; }

	const T operator *() const {
		std::istringstream ss( _value );
		T value;
		ss >> value;
		return value;
	}

	std::istream & operator ++() {
		if( !( getline( *_input, _value, _delim ) ) )
		{
			_input = 0;
		}
		return *_input;
	}

	bool operator !=( const csv_istream_iterator & rhs ) const {
		return _input != rhs._input;
	}
};

struct weighted_edge {
	size_t v;
	size_t l;
};

std::istream& operator>>(std::istream& is, weighted_edge& we);

template<typename T>
std::vector<std::vector<T>> collect_adj_list_from_edges(std::istream &is) {
	std::vector<std::vector<T>> adj_list;
	T v, w;
	while (is >> v >> w) {
		if (adj_list.size() < v) adj_list.resize(v);
		if (adj_list.size() < w) adj_list.resize(w);
		adj_list[v - 1].emplace_back(w - 1);
	}

	return adj_list;
}

template<typename T>
std::vector<std::vector<T>> collect_adj_list(std::istream &is) {
	std::vector<std::vector<T>> adj_list;
	std::string line;
	while (std::getline(is, line)) {
		adj_list.emplace_back();
		std::stringstream is_line(line);

		size_t idx;
		if (!(is_line >> idx)) continue;

		T val;
		while (is_line >> val) adj_list.back().emplace_back(val);
	}

	return adj_list;
}

template<typename T>
std::vector<T> collect_sequence(std::istream &is, char delim = ',') {
	std::vector<T> sequence;
	std::copy(csv_istream_iterator<T>(is, delim), csv_istream_iterator<T>(), std::back_inserter(sequence));
	return sequence;
}

#endif //ALGORITHMS_TRAINING_TESTCASE_COLLECTOR_H
