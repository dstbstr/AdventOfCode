#include "Common.h"

SOLUTION(2016, 15) {
    struct Disc {
        u32 Positions = 0;
        u32 Initial = 0;
    };

    constexpr bool Win(const std::vector<Disc>&discs, u32 ticks) {
        for (u32 i = 0; i < discs.size(); i++) {
            auto disc = discs[i];
            if ((disc.Initial + i + 1 + ticks) % disc.Positions != 0) {
                return false;
            }
        }

        return true;
    }

    constexpr u32 BestSpin(const std::vector<Disc>&discs) {
        u32 spin = 0;
        while (!Win(discs, spin)) {
            spin++;
        }

        return spin;
    }

    constexpr Disc ParseDisc(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        Disc result;
        Constexpr::ParseNumber(s[3], result.Positions);
        std::string initial = std::string(s.back());
        Constexpr::ParseNumber(initial.substr(0, initial.size() - 1), result.Initial);
        return result;
    }

    PART(1) {
        auto discs = ParseLines(lines, ParseDisc);

        return BestSpin(discs);
        
    }
    PART(2) {
        auto copy = lines;
        copy.emplace_back("Disc #7 has 11 positions; at time=0, it is at position 0.");

        auto discs = ParseLines(copy, ParseDisc);

        return BestSpin(discs);
    }

    static_assert(Win({ {5, 4}, {2, 1} }, 0) == false);
    static_assert(Win({ {5, 4}, {2, 1} }, 5));

    TEST(1) {
        std::vector<Disc> discs = { {5, 4}, {2, 1} };

        return BestSpin(discs) == 5;
    }
}

