#include "Common.h"

SOLUTION(2020, 10) {
    constexpr auto KnownPaths = []() {
        std::array<size_t, 8> known {};
        known[0] = 1;
        known[1] = 1;
        known[2] = 2;
        for (size_t i = 0; i < 5; i++) {
            auto next = known[i] + known[i + 1] + known[i + 2];
            known[i + 3] = next;
        }
        return known;
    }();
        
    constexpr size_t CountPaths(size_t runLength) {
        return KnownPaths[runLength];
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto nums = ParseLinesAsNumbers<size_t>(lines);
        nums.push_back(0);
        std::sort(nums.begin(), nums.end());
        size_t ones = 0;
        size_t threes = 1;
        for (size_t i = 0; i < nums.size() - 1; i++) {
            auto diff = nums[i + 1] - nums[i];
            if (diff == 1) ones++;
            else if (diff == 3) threes++;
        }

        return ones * threes;
    }

    PART(1) {
        return SolvePartOne(lines);
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto nums = ParseLinesAsNumbers<size_t>(lines);
        nums.push_back(0);
        std::sort(nums.begin(), nums.end());
        size_t runLength = 0;
        size_t result = 1;
        for (size_t i = 1u; i < nums.size(); i++) {
            if (nums[i] - nums[i - 1] == 3) {
                result *= CountPaths(runLength);
                runLength = 0;
            }
            else {
                runLength++;
            }
        }
        result *= CountPaths(runLength);
        return result;
    }

    PART(2) {
        return SolvePartTwo(lines);
    }

    TEST(1) {
        std::vector<std::string> lines = {
            "16",
            "10",
            "15",
            "5",
            "1",
            "11",
            "7",
            "19",
            "6",
            "12",
            "4"
        };

        if (SolvePartOne(lines) != 35) return false;
        if (SolvePartTwo(lines) != 8) return false;

        return true;
    }
}