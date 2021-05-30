#include <unordered_map>
#include <vector>
#include <string_view>
#include <cassert>

/**
 * Source:
 * https://www.youtube.com/watch?v=c3ZFg0-M1WA&t=1239s
 */
std::vector<size_t> unique_symbol_substr(std::string_view str) {
    std::vector<size_t> parts;
    if (str.empty()) return parts;

    std::unordered_map<char, size_t> last_idxs;
    for (size_t i = 0; i < str.size(); i++) last_idxs.insert_or_assign(str[i], i);

    for (size_t i = 0; i < str.size(); ++i) {
        size_t part_start_idx = i;
        auto last_char_pos = last_idxs.find(str[i])->second;
        while (i < last_char_pos) {
            i++;
            last_char_pos = std::max(
                last_idxs.find(str[i])->second,
                last_char_pos
            );
        }
        parts.push_back(last_char_pos - part_start_idx + 1);
    }

    return parts;
}

int main() {
    assert(unique_symbol_substr("").empty());
    assert(unique_symbol_substr("abb") == std::vector<size_t>({ 1, 2 }));
    assert(unique_symbol_substr("aba") == std::vector<size_t>({ 3 }));
    assert(unique_symbol_substr("ababcbacadefegdehijhklij") == std::vector<size_t>({ 9, 7, 8 }));
}
