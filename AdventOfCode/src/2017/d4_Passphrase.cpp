#include "Common.h"

SOLUTION(2017, 4) {
    constexpr bool Validate(std::vector<std::string>& words) {
        std::sort(words.begin(), words.end());
        for (size_t i = 0; i < words.size() - 1; i++) {
            if (words[i] == words[i + 1]) return false;
        }
        return true;
    }
    constexpr bool IsValid(std::string_view line) {
        std::vector<std::string> words;
        auto split = Constexpr::Split(line, " ");
        for (const auto& sv : split) {
            words.push_back(std::string(sv));
        }
        return Validate(words);
    }

    constexpr bool IsSecure(std::string_view line) {
        std::vector<std::string> words;
        auto split = Constexpr::Split(line, " ");
        for (const auto& sv : split) {
            auto word = std::string(sv);
            std::sort(word.begin(), word.end());
            words.push_back(word);
        }

        return Validate(words);
    }

    PART(1) {
        return Constexpr::ToString(std::count_if(lines.cbegin(), lines.cend(), IsValid));
    }

    PART(2) {
        return Constexpr::ToString(std::count_if(lines.cbegin(), lines.cend(), IsSecure));
    }

    static_assert(IsValid("aa bb cc"));
    static_assert(!IsValid("aa bb aa"));
    static_assert(IsValid("aa aaa"));

    static_assert(IsSecure("abcde fghij"));
    static_assert(!IsSecure("abcde xyz ecdab"));
    //static_assert(IsSecure("a ab abc abd abf abj"));
}