#include "Common.h"

SOLUTION(2015, 5) {
    static constexpr auto NaughtyPairs = { "ab", "cd", "pq", "xy" };

    constexpr bool IsVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    constexpr bool IsNice(std::string_view input) {
        auto HasBadPair = [&](std::string_view pair) {
            return input.find(pair) != input.npos;
		};
        return std::count_if(input.cbegin(), input.cend(), IsVowel) > 2
            && std::ranges::none_of(NaughtyPairs, HasBadPair)
            && std::ranges::adjacent_find(input.cbegin(), input.cend()) != input.cend();
    }

    constexpr bool IsReallyNice(std::string_view input) {
        auto hasSplit = std::ranges::any_of(std::views::iota(size_t{ 0 }, input.size() - 2), [&](size_t i) {
            return input[i] == input[i + 2];
            });
        auto hasXyx = std::ranges::any_of(std::views::iota(size_t{ 0 }, input.size() - 1), [&](size_t i) {
            auto pair = input.substr(i, 2);
            return input.find(pair, i + 2) != input.npos;
			});
		return hasSplit && hasXyx;
    }

    PART(1) {
        return std::count_if(lines.cbegin(), lines.cend(), IsNice);
    }

    PART(2) {
        return std::count_if(lines.cbegin(), lines.cend(), IsReallyNice);
    }

    static_assert(IsNice("ugknbfddgicrmopn"));
    static_assert(IsNice("aaa"));

    static_assert(!IsNice("jchzalrnumimnmhp"));
    static_assert(!IsNice("haegwjzuvuyypxyu"));
    static_assert(!IsNice("dvszwmarrgswjxmb"));

    static_assert(IsReallyNice("qjhvhtzxzqqjkmpb"));
    static_assert(IsReallyNice("xxyxx"));

    static_assert(!IsReallyNice("uurcxstgmygtbstg"));
    static_assert(!IsReallyNice("ieodomkazucvgmuy"));
    static_assert(!IsReallyNice("aaa"));
}