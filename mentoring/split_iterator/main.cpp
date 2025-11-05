#include <cassert>
#include <string>
#include <string_view>
#include <sstream>

class split_iterator
{
public:
	split_iterator() : pos(std::string_view::npos), finally(true) {}
	split_iterator (const std::string_view s, const char c) : str(s), delimiter(c)
	{
		if (s.empty()) {
			pos = std::string_view::npos;
			finally = true;
		} else {
			pos = 0;
			finally = false;
			next();
		}
	}

	split_iterator& operator++()
	{
		if (pos == std::string_view::npos) {
			finally = true;
			return *this;
		}
		next();
		return *this;
	}

	std::string_view operator*() const
	{
		return curr;
	}

	bool operator==(const std::default_sentinel_t&) const
	{
		return finally;
	}

	bool operator!=(const std::default_sentinel_t& other) const {
		return !(*this == other);
	}

private:
	void next()
	{
		// Находим конец текущей подстроки.
		size_t next = str.find(delimiter, pos);

		// Вычисляем текущую подстроку.
		size_t end = next == std::string_view::npos ? str.size() : next;
		curr = str.substr(pos, end - pos);

		// Вычисляем начало следующей подстроки.
		pos = next == std::string_view::npos ? std::string_view::npos : next + 1;
	}

	// Строка, которую нужно разбить.
	const std::string_view str;

	// Разделитель строки.
	const char delimiter = '\0';

	// Указывает на начало следующей подстроки (следующий символ после разделителя), либо std::string_view::npos,
	// если строка кончилась. Если последний элемент пустой (например "ac,"), то при обработке предпоследнего элемента
	// будет равен str.size().
	size_t pos;

	// Подстрока на текущем шаге итератора.
	std::string_view curr;

	// Флаг, указывающий, что итератор достиг конца строки. При обработке последнего элемента pos становится равным
	// std::string_view::npos. Возникает двусмысленность: либо достигнут конец строки, либо обрабатывается последний
	// элемент. Данный флаг позволяет различать эти два случая.
	bool finally;
};

bool test(std::string_view input, std::string_view expected_output)
{
	split_iterator g (input, ',');

	std::stringstream ss;
	for (; g != std::default_sentinel; ++g) {
		ss << "[" << *g << "]";
	}
	std::string actual_output = ss.str();

	return actual_output == expected_output;
}

int main()
{
	assert(test("", ""));
	assert(test("a", "[a]"));
	assert(test("a,", "[a][]"));
	assert(test(",a", "[][a]"));
	assert(test("a,b", "[a][b]"));
	assert(test("a,b,c,", "[a][b][c][]"));
	assert(test("a,,b,", "[a][][b][]"));
	assert(test("a,,b,,c,", "[a][][b][][c][]"));
	assert(test("abc", "[abc]"));
	assert(test("abc,a", "[abc][a]"));
	assert(test("ac,abc,de", "[ac][abc][de]"));

	return 0;
}
