#include "Common.h"

SOLUTION(2021, 1) {
    constexpr size_t CountIncreasing(const std::vector<u32>&nums) {
        size_t result = 0;
        for (size_t i = 1; i < nums.size(); i++) {
            result += nums[i] > nums[i - 1];
        }

        return result;
    }


    constexpr size_t CountIncreasingWindow(const std::vector<u32>&nums) {
        size_t result = 0;
        for (size_t i = 0u; i < nums.size() - 3; i++) {
            auto lhs = nums[i] + nums[i + 1] + nums[i + 2];
            auto rhs = lhs - nums[i] + nums[i + 3];
            result += lhs < rhs;
        }
        return result;
    }

    PART(1) {
        return CountIncreasing(ParseLinesAsNumbers<u32>(lines));
    }

    PART(2) {
        return CountIncreasingWindow(ParseLinesAsNumbers<u32>(lines));
    }

    static_assert(CountIncreasing({ 199, 200, 208, 210, 200, 207, 240, 269, 260, 263 }) == 7);
    static_assert(CountIncreasingWindow({ 199, 200, 208, 210, 200, 207, 240, 269, 260, 263 }) == 5);
}