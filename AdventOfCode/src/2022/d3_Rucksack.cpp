#include "Common.h"

SOLUTION(2022, 3) {
    constexpr u32 GetLetterValue(char letter) {
        if (letter <= 'Z') {
            return letter - 'A' + 27;
        }
        else {
            return letter - 'a' + 1;
        }
    }

    constexpr char FindDuplicate(std::string_view line) {
        for (size_t i = 0u; i < line.size() / 2; i++) {
            for (auto j = line.size() / 2; j < line.size(); j++) {
                if (line[i] == line[j]) {
                    return line[i];
                }
            }
        }
        return '_';
    }

    constexpr char FindBadge(std::string_view first, std::string_view second, std::string_view third) {
        for (auto c = 'a'; c <= 'z'; c++) {
            if (std::find(first.begin(), first.end(), c) != first.end()) {
                if (std::find(second.begin(), second.end(), c) != second.end()) {
                    if (std::find(third.begin(), third.end(), c) != third.end()) {
                        return c;
                    }
                }
            }
        }

        for (auto c = 'A'; c <= 'Z'; c++) {
            if (std::find(first.begin(), first.end(), c) != first.end()) {
                if (std::find(second.begin(), second.end(), c) != second.end()) {
                    if (std::find(third.begin(), third.end(), c) != third.end()) {
                        return c;
                    }
                }
            }
        }

        return '_';
    }

    PART(1) {
        return std::accumulate(lines.begin(), lines.end(), 0u, [](u32 prev, std::string_view line) {
            return prev + GetLetterValue(FindDuplicate(line));
        });
    }

    PART(2) {
        u32 result = 0;
        for (size_t i = 0u; i < lines.size(); i += 3) {
            result += GetLetterValue(FindBadge(lines[i], lines[i + 1], lines[i + 2]));
        }

        return result;
    }

    static_assert(GetLetterValue('a') == 1);
    static_assert(GetLetterValue('A') == 27);
    static_assert(GetLetterValue('z') == 26);
    static_assert(GetLetterValue('Z') == 52);
        
    static_assert(FindDuplicate("vJrwpWtwJgWrhcsFMMfFFhFp") == 'p');
    static_assert(FindDuplicate("jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL") == 'L');
    static_assert(FindDuplicate("PmmdzqPrVvPwwTWBwg") == 'P');
    static_assert(FindDuplicate("wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn") == 'v');
        
    static_assert(FindBadge(
        "vJrwpWtwJgWrhcsFMMfFFhFp",
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
        "PmmdzqPrVvPwwTWBwg") == 'r');

    static_assert(FindBadge(
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
        "ttgJtRGJQctTZtZT",
        "CrZsJsPPZsGzwwsLwLmpwMDw") == 'Z');
}