#include "Common.h"

SOLUTION(2017, 5) {
    constexpr void Jump(u32 & instruction, s32 & distance) {
        instruction += distance;
        distance++;
    }

    constexpr void ShortJump(u32 & instruction, s32 & distance) {
        instruction += distance;
        if (distance > 2) {
            distance--;
        }
        else {
            distance++;
        }
    }

    constexpr u32 StepsToExit(const auto& lines, auto jumpFunc) {
        auto jumps = ParseLinesAsNumbers<s32>(lines);

        u32 steps = 0;
        u32 instruction = 0;
        while (instruction < jumps.size()) {
            jumpFunc(instruction, jumps[instruction]);
            steps++;
        }

        return steps;
    }

    PART(1) {
        return StepsToExit(lines, Jump);
    }

    PART(2) {
        return StepsToExit(lines, ShortJump);
    }

    static_assert(StepsToExit(std::vector<std::string>{ "0", "3", "0", "1", "-3" }, Jump) == 5);
    static_assert(StepsToExit(std::vector<std::string>{ "0", "3", "0", "1", "-3" }, ShortJump) == 10);
}