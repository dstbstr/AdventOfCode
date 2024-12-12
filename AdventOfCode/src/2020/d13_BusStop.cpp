#include "Common.h"

SOLUTION(2020, 13) {
    constexpr size_t SolvePartOne(const auto & lines) {
        size_t earliestTime;
        Constexpr::ParseNumber(lines[0], earliestTime);
        std::vector<size_t> values;
        auto split = Constexpr::Split(lines[1], ",");
        for (const auto& val : split) {
            if (val[0] == 'x') continue;
            size_t number;
            Constexpr::ParseNumber(val, number);
            values.push_back(number);
        }

        size_t best = 99999;
        size_t result = 0;
        for (const auto& val : values) {
            auto diff = val - (earliestTime % val);
            if (diff < best) {
                best = diff;
                result = val * diff;
            }
        }

        return result;
    }

    PART(1) {
        return SolvePartOne(lines);
    }

    //77 % 7 == 0
    //77 % 13 == 12
    //7 * 13 = 91
    //77 + 91 = 168
    //168 % 7 == 0
    //168 % 13 == 12

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto split = Constexpr::Split(lines[1], ",");
        size_t jump;
        Constexpr::ParseNumber(split[0], jump);

        size_t result = 0;
        for (size_t i = 1u; i < split.size(); i++) {
            if (split[i][0] == 'x') continue;
            size_t val;
            Constexpr::ParseNumber(split[i], val);
            size_t targetRemainder = val - (i % val);
            while (result % val != targetRemainder) {
                result += jump;
            }
            jump *= val;
        }
        return result;
    }

    PART(2) {
        return SolvePartTwo(lines);
    }

    TEST(1) {
        std::vector<std::string> lines = {
           "939",
           "7,13,x,x,59,x,31,19"
        };

        if (SolvePartOne(lines) != 295) return false;
        if (SolvePartTwo(lines) != 1068781) return false;
        
        return true;
    }
}