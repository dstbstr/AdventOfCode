#include "Common.h"
#include <bit>

SOLUTION(2016, 13) {
    constexpr bool IsOpen(UCoord coord, u64 key) {
        u64 val = (coord.X * coord.X) + (3 * coord.X) + (coord.X * coord.Y * 2) + coord.Y + (coord.Y * coord.Y) + key;
        return std::popcount(val) % 2 == 0;
    }

    constexpr std::vector<UCoord> GetNeighbors(UCoord pos) {
        std::vector<UCoord> result{ {pos.X + 1, pos.Y}, {pos.X, pos.Y + 1} };
        if(pos.X > 0) result.emplace_back(pos.X - 1, pos.Y);
		if (pos.Y > 0) result.emplace_back(pos.X, pos.Y - 1);
		return result;
    }

    constexpr u32 Bfs(u32 key, UCoord target) {
        Constexpr::BigSet<UCoord, 1000> seen;
        struct State { UCoord Pos; u32 depth; };
        Constexpr::Queue<State> moves;
        moves.push({ {1, 1}, 0 });
        while (!moves.is_empty()) {
            auto [pos, depth] = moves.front(); moves.pop();
            if (pos == target) return depth;

            for (auto move : GetNeighbors(pos)) {
                if (!seen.contains(move) && IsOpen(move, key)) {
                    seen.insert(move);
                    moves.push({ move, depth + 1 });
                }
            }
        }
		return 0; //error case, out of moves
    }

    constexpr u32 CountUniquePositions(u32 key, u32 maxSteps) {
        Constexpr::BigSet<UCoord, 1000> seen;
		struct State { UCoord Pos; u32 depth; };
        Constexpr::Queue<State> moves;
        moves.push({ {1, 1}, 0 });
        seen.insert({ 1, 1 });
        while (!moves.is_empty()) {
            auto [pos, depth] = moves.front();
            moves.pop();
            if (depth >= maxSteps) {
                continue;
            }
            for (auto& move : GetNeighbors(pos)) {
                if (!seen.contains(move) && IsOpen(move, key)) {
                    seen.insert(move);
                    moves.push({ move, depth + 1 });
                }
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
		return CountUniquePositions(key, 50);
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


