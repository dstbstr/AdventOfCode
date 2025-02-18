#include "Common.h"
#include "HexGrid.h"

SOLUTION(2017, 11) {
    PART(1) {
        auto split = Constexpr::Split(lines[0], ",");
        FlatHex::Pos pos{ 0, 0, 0 };
        for (auto sv : split) {
            FlatHex::Move(pos, FlatHex::ParseDirection(sv));
        }

        return FlatHex::MDistance(pos, { 0, 0, 0 });
    }

    PART(2) {
        auto split = Constexpr::Split(lines[0], ",");
        u32 max = 0;
        FlatHex::Pos pos{ 0, 0, 0 };
        const FlatHex::Pos origin{ 0, 0, 0 };
        for (auto sv : split) {
            Move(pos, FlatHex::ParseDirection(sv));
            max = std::max(max, MDistance(pos, origin));
        }

        return max;
    }

    //static_assert(PartOne({"ne,ne,ne"}) == "3");
    //static_assert(PartOne({"ne,ne,sw,sw"}) == "0");
    //static_assert(PartOne({"ne,ne,s,s"}) == "2");
    //static_assert(PartOne({ "se,sw,se,sw,sw" }) == "3");
}