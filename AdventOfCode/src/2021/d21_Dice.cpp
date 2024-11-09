#include "Common.h"

SOLUTION(2021, 21) {
    constexpr bool Roll(u64& dieRolls, u64& dieValue, u64& pos, u64& score) {
        auto spaces = (dieValue + 1) * 3;
        pos = (pos + spaces) % 10;
        score += pos + 1;
        dieRolls += 3;
        dieValue += 3;
        if (dieValue > 100) dieValue -= 100;
        return score >= 1000;
    }

    //s1, s2, p1, p2
    using Cache = std::array<std::array<std::array<std::array<std::pair<u64, u64>, 10>, 10>, 21>, 21>;
    constexpr auto Missing = std::make_pair<u64, u64>(0, 0);

    constexpr std::vector<u64> GetPositions(u64 current) {
        return { (current + 1) % 10, (current + 2) % 10, (current + 3) % 10 };
    }

    struct Player {
        u64 Score = 0;
        u64 Pos = 0;
        Cache* Cache = nullptr;
    };

    constexpr std::array<std::pair<u64, u64>, 7> SpaceCounts = { {
        {3ull, 1ull},
        {4ull, 3ull},
        {5ull, 6ull},
        {6ull, 7ull},
        {7ull, 6ull},
        {8ull, 3ull},
        {9ull, 1ull}
    } };

    constexpr std::vector<std::pair<Player, u64>> GetNext(const Player & p) {
        std::vector<std::pair<Player, u64>> result;
        for (auto& sc : SpaceCounts) {
            auto next = p;
            next.Pos = (p.Pos + sc.first) % 10;
            next.Score += next.Pos + 1;
            result.push_back(std::make_pair(next, sc.second));
        }

        return result;
    }

    //roll die 3 times, add score once
    constexpr std::pair<u64, u64> Recurse(Player p1, Player p2, bool isP1Turn) {
        auto& cache = isP1Turn ? *p1.Cache : *p2.Cache;
        auto& seen = cache[p1.Score][p2.Score][p1.Pos][p2.Pos];

        if (seen == Missing) {
            std::pair<u64, u64> wins{};
            auto nextPlayers = GetNext(isP1Turn ? p1 : p2);
            for (const auto& [p, count] : nextPlayers) {
                if (p.Score >= 21) {
                    isP1Turn ? wins.first += count : wins.second += count;
                }
                else {
                    auto r = (isP1Turn ? Recurse(p, p2, false) : Recurse(p1, p, true));
                    wins.first += (r.first * count);
                    wins.second += (r.second * count);
                }
            }

            seen = wins;
        }

        return seen;
    }

    constexpr u64 SolveP1(u64 p1Pos, u64 p2Pos) {
        //let's make these zero based
        p1Pos--;
        p2Pos--;

        u64 dieRolls = 0;
        u64 dieValue = 1;
        u64 p1Score = 0;
        u64 p2Score = 0;
        while (true) {
            if (Roll(dieRolls, dieValue, p1Pos, p1Score)) break;
            if (Roll(dieRolls, dieValue, p2Pos, p2Score)) break;
        }

        return std::min(p1Score, p2Score) * dieRolls;
    }

    PART(1) {
        size_t p1Pos, p2Pos;

        Constexpr::ParseNumber(Constexpr::Split(lines[0], " ").back(), p1Pos);
        Constexpr::ParseNumber(Constexpr::Split(lines[1], " ").back(), p2Pos);
        return Constexpr::ToString(SolveP1(p1Pos, p2Pos));
    }

    constexpr u64 SolveP2(size_t p1Pos, size_t p2Pos) {
        p1Pos--;
        p2Pos--;
        auto p1Cache = new Cache();
        auto p2Cache = new Cache();

        Player p1{ 0, p1Pos, p1Cache };
        Player p2{ 0, p2Pos, p2Cache };
        auto result = Recurse(p1, p2, true);
        delete p1Cache;
        delete p2Cache;
        return std::max(result.first, result.second);
    }
    PART(2) {
        size_t p1Pos, p2Pos;

        Constexpr::ParseNumber(Constexpr::Split(lines[0], " ").back(), p1Pos);
        Constexpr::ParseNumber(Constexpr::Split(lines[1], " ").back(), p2Pos);
        return Constexpr::ToString(SolveP2(p1Pos, p2Pos));
    }

    static_assert(SolveP1(4, 8) == 739785);
    static_assert(SolveP1(6, 10) == 853776);
    
    TEST(1) {
        return SolveP1(4, 8) == 739785;
    }

    TEST(2) {
        return SolveP2(4, 8) == 444356092776315u;
    }
}