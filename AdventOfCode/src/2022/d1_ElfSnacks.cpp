#include "Common.h"

SOLUTION(2022, 1) {
    constexpr size_t CountCalories(const std::vector<std::string>& lines) {
        auto nums = ParseLinesAsNumbers<u32>(lines);
        return std::accumulate(nums.begin(), nums.end(), 0u);
    }

    constexpr std::vector<size_t> GetElves(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        std::vector<size_t> elves;
        for (const auto& group : groups) {
            elves.push_back(CountCalories(group));
        }
        std::sort(elves.begin(), elves.end());
        return elves;
    }

    PART(1) {
        auto elves = GetElves(lines);
        return elves.back();
    }

    PART(2) {
        auto elves = GetElves(lines);
        return elves.back() + elves[elves.size() - 2] + elves[elves.size() - 3];
    }

    constexpr bool TestGetElves() {
        std::vector<std::string> lines = {
            "1000",
            "2000",
            "3000",
            "",
            "4000",
            "",
            "5000",
            "6000",
            "",
            "7000",
            "8000",
            "9000",
            "",
            "10000"
        };
        auto elves = GetElves(lines);
        if (elves.back() != 24'000) return false;
        if (elves[elves.size() - 1] + elves[elves.size() - 2] + elves[elves.size() - 3] != 45'000) return false;
        return true;
    }
    
    static_assert(TestGetElves());
    TEST(1) {
        return TestGetElves();
    }
}