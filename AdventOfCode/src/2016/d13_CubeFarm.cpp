#include "Common.h"
#include <bit>

SOLUTION(2016, 13) {
    constexpr bool IsOpen(UCoord coord, u64 key) {
        u64 val = (coord.X * coord.X) + (3 * coord.X) + (coord.X * coord.Y * 2) + coord.Y + (coord.Y * coord.Y) + key;
        return std::popcount(val) % 2 == 0;
    }

    constexpr u64 GetDistance(UCoord location, UCoord target) {
        size_t x = 0;
        size_t y = 0;
        if (location.X > target.X) {
            x = location.X - target.X;
        }
        else {
            x = target.X - location.X;
        }
        if (location.Y > target.Y) {
            y = location.Y - target.Y;
        }
        else {
            y = target.Y - location.Y;
        }
        return x + y;
    }

    constexpr void AddNextMoves(UCoord location, std::vector<UCoord>& next) {
        if (location.X > 0) {
            next.emplace_back(location.X - 1, location.Y);
        }
        if (location.Y > 0) {
            next.emplace_back(location.X, location.Y - 1);
        }
        next.emplace_back(location.X + 1, location.Y);
        next.emplace_back(location.X, location.Y + 1);
    }

    constexpr u32 Bfs(u32 key, UCoord target) {
        Constexpr::SmallSet<UCoord> seen;
        u32 depth = 0;
        std::vector<UCoord> moves;
        moves.reserve(100); // just a guess, could be tuned by running and seeing the max size
        moves.emplace_back(1, 1);

        while (true) {
            if (std::find(moves.cbegin(), moves.cend(), target) != moves.cend()) {
                return depth;
            }
            depth++;
            std::vector<UCoord> next;
            for (auto move : moves) {
                AddNextMoves(move, next);
            }
            moves.clear();
            std::copy_if(next.begin(), next.end(), std::back_inserter(moves), [&](UCoord move) {
                return !seen.contains(move) && IsOpen(move, key);
                });

            seen.insert(moves.begin(), moves.end());

            if (moves.size() == 0) {
                return 0; //error case, out of moves
            }
        }
        return 0;
    }

    constexpr u32 FindUniquePositions(u32 key, u32 maxSteps) {
        Constexpr::SmallSet<UCoord> seen;
        u32 depth = 0;
        std::vector<UCoord> moves;
        moves.reserve(100); // just a guess
        moves.push_back({ 1, 1 });
        seen.insert({ 1, 1 });
        while (depth < maxSteps) {
            depth++;
            std::vector<UCoord> next;
            for (auto move : moves) {
                AddNextMoves(move, next);
            }
            moves.clear();
            std::copy_if(next.begin(), next.end(), std::back_inserter(moves), [&](UCoord move) {
                return !seen.contains(move) && IsOpen(move, key);
                });

            seen.insert(moves.begin(), moves.end());

            if (moves.size() == 0) {
                return 0; //error case, out of moves
            }
        }

        return static_cast<u32>(seen.size());
    }

    PART(1) {
        u32 key;
        Constexpr::ParseNumber(lines[0], key);
        return Bfs(key, { 31, 39 });
    }
    PART(2) {
        u32 key;
        Constexpr::ParseNumber(lines[0], key);
        return FindUniquePositions(key, 50);
    }

    static_assert(IsOpen({ 0, 0 }, 10));
    static_assert(!IsOpen({ 1, 0 }, 10));
    static_assert(IsOpen({ 2, 0 }, 10));
    static_assert(IsOpen({ 0, 1 }, 10));
    static_assert(IsOpen({ 1, 1 }, 10));

    TEST(1) {
        return Bfs(10, { 7, 4 }) == 11;
    }
}


