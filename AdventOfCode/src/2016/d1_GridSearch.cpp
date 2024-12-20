#include "Common.h"
#include "Facing.h"
#include <format>

SOLUTION(2016, 1) {
    constexpr size_t FindTarget(std::string_view directions) {
        auto split = Constexpr::Split(directions, ", ");
        Direction facing = Direction::North;
        u32 amount;
        Coord target = { 0, 0 };

        for (auto instruction : split) {
            facing = instruction[0] == 'L' ? TurnLeft(facing) : TurnRight(facing);
            Constexpr::ParseNumber(instruction.substr(1), amount);
            Move(target, facing, amount);
        }

        return MDistance(target);
    }

    constexpr size_t FindTarget2(std::string_view directions) {
        Constexpr::SmallSet<Coord> seen;
        Direction facing = North;
        Coord pos{ 0, 0 };
        u32 distance;

        auto steps = Constexpr::Split(directions, ", ");
        for (const auto& step : steps) {
            facing = step[0] == 'L' ? TurnLeft(facing) : TurnRight(facing);
            Constexpr::ParseNumber(step.substr(1), distance);
            for (u32 i = 0; i < distance; i++) {
                Move(pos, facing);
                if (seen.contains(pos)) return MDistance(pos, Coord{ 0, 0 });
                seen.insert(pos);
            }
        }
        return 0;
    }

    PART(1) {
        return FindTarget(lines[0]);
    }
    PART(2) {
        return FindTarget2(lines[0]);
    }

    static_assert(FindTarget("R2, L3") == 5);
    static_assert(FindTarget("R2, R2, R2") == 2);
    static_assert(FindTarget("R5, L5, R5, R3") == 12);
    static_assert(FindTarget2("R8, R4, R4, R8") == 4);
}
