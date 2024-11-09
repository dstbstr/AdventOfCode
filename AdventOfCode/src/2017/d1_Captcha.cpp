#include "Common.h"

SOLUTION(2017, 1) {
    constexpr u32 SumDigits(std::string_view line, size_t offset) {
        u32 result = 0;
        for (size_t i = 0u; i < line.size(); i++) {
            if (line[i] == line[(i + offset) % line.size()]) {
                result += line[i] - '0';
            }
        }
        return result;
    }

    PART(1) {
        return Constexpr::ToString(SumDigits(lines[0], 1));
    }

    PART(2) {
        return Constexpr::ToString(SumDigits(lines[0], lines[0].size() / 2));
    }

    static_assert(SumDigits("1122", 1) == 3);
    static_assert(SumDigits("1111", 1) == 4);
    static_assert(SumDigits("1234", 1) == 0);
    static_assert(SumDigits("91212129", 1) == 9);
    static_assert(SumDigits("1212", 2) == 6);
    static_assert(SumDigits("1221", 2) == 0);
    static_assert(SumDigits("123425", 3) == 4);
}
