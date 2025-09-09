#include <cassert>
#include <string>
#include <string_view>
#include <sstream>

class split_iterator
{
public:
	split_iterator() : pos(std::string_view::npos), finaly(true) {}
	split_iterator (std::string_view s, char c) : str(s), ch(c)
	{
		if (s.empty()) {
			pos = std::string_view::npos;
			finaly = true;
		} else {
			pos = 0;
			finaly = false;
			next();
		}
	}

	split_iterator& operator++()
	{
		if (pos == std::string_view::npos) {
			finaly = true;
			return *this;
		}
		next();
		return *this;
	}

	std::string_view operator*() const
	{
		return curr;
	}

	bool operator==(const split_iterator& other) const {
		if (this->finaly == other.finaly) return true;
		if (str.data() == other.str.data() && str.size() == other.str.size()) return pos == other.pos;
		return false;
	}

	bool operator!=(const split_iterator& other) const {
		return !(*this == other);
	}

private:
	void next()
	{
		size_t next = str.find(ch, pos);
		size_t end = next == std::string_view::npos ? str.size() : next;
		curr = str.substr(pos, end - pos);
		pos = next == std::string_view::npos ? std::string_view::npos : next + 1;
	}

	size_t pos;
	std::string_view str;
	std::string_view curr;
	char ch = '\0';
	bool finaly;
};

bool test(std::string_view input, std::string_view expected_output)
{
	split_iterator g (input, ',');
	split_iterator end{};

	std::stringstream ss;
	ss << "[";
	for (; g != end; ++g)
		ss << "\"" << *g << "\",";
	if (ss.view().back() != '[') ss.seekp(-1, std::ios_base::cur);
	ss << "]";

	std::string actual_output = ss.str();
	return actual_output == expected_output;
}

int main()
{
	assert(test("", R"([])"));
	assert(test("a,b", R"(["a","b"])"));
	assert(test("a,b,c,", R"(["a","b","c",""])"));
	assert(test("a,,b,", R"(["a","","b",""])"));
	assert(test("a,,b,,c,", R"(["a","","b","","c",""])"));
	assert(test("abc", R"(["abc"])"));
	assert(test("abc,a", R"(["abc","a"])"));
	assert(test("ac,abc,de", R"(["ac","abc","de"])"));

	return 0;
}
