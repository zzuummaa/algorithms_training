#include <cassert>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>

std::optional<size_t> parse_bit_mask(std::string_view str)
{
	if (str.empty()) return 0;

	enum class ParserState {
		FindNumber,
		FindComa,
	};

	ParserState state{};

	size_t result = 0;

	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == ' ') continue;

		if (state == ParserState::FindNumber && std::isdigit(str[i])) {
			int bit = 0;
			while (std::isdigit(str[i]) && i < str.size()) {
				bit = bit * 10 + (str[i] - '0');
				if (bit > 64) return std::nullopt;
				++i;
			}
			--i;

			if (bit == 0) return std::nullopt;
			result |= 1ULL << (bit - 1);
			state = ParserState::FindComa;

			continue;
		}

		if (state == ParserState::FindComa && str[i] == ',') {
			state = ParserState::FindNumber;
			continue;
		}

		return std::nullopt;
	}

	if (state == ParserState::FindNumber && result != 0) return std::nullopt;

	return result;
}

int main()
{
	assert(parse_bit_mask("") == 0);

	assert(parse_bit_mask("1") == 1ULL << 0);
	assert(parse_bit_mask("64") == 1ULL << 63);
	assert(parse_bit_mask(",") == std::nullopt);
	assert(parse_bit_mask("65") == std::nullopt);
	assert(parse_bit_mask("999") == std::nullopt);
	assert(parse_bit_mask("999999999999999999999999999999999") == std::nullopt);

	assert(parse_bit_mask("1,") == std::nullopt);
	assert(parse_bit_mask(",1") == std::nullopt);
	assert(parse_bit_mask(",65") == std::nullopt);
	assert(parse_bit_mask("65,") == std::nullopt);

	assert(parse_bit_mask("1,1") == 1ULL << 0);
	assert(parse_bit_mask("1,2") == 1ULL << 0 | 1ULL << 1);
	assert(parse_bit_mask("2,1") == 1ULL << 0 | 1ULL << 1);

	assert(parse_bit_mask(" ") == 0);

	assert(parse_bit_mask(" 1") == 1ULL << 0);
	assert(parse_bit_mask("1 ") == 1ULL << 0);

	assert(parse_bit_mask(" 1,20") == 1ULL << 0 | 1ULL << 19);
	assert(parse_bit_mask("1 ,20") == 1ULL << 0 | 1ULL << 19);
	assert(parse_bit_mask("1, 20") == 1ULL << 0 | 1ULL << 19);
	assert(parse_bit_mask("1,20 ") == 1ULL << 0 | 1ULL << 19);

}
