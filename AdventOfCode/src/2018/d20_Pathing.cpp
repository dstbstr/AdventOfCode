#include "Common.h"

SOLUTION(2018, 20) {
    using Grid = Constexpr::SmallMap<Coord, std::vector<Coord>>;

    constexpr void Connect(Coord & a, const Coord & b, Grid & grid) {
        grid[a].push_back(b);
        grid[b].push_back(a);
        a = b;
    }

    constexpr void Recurse(std::string_view chars, size_t & index, Coord pos, Grid & grid) {
        auto origin = pos;
        auto& [x, y] = pos;
        while (true) {
            switch (chars[index++]) {
            case 'N': Connect(pos, { x, y - 1 }, grid); break;
            case 'S': Connect(pos, { x, y + 1 }, grid); break;
            case 'E': Connect(pos, { x - 1, y }, grid); break;
            case 'W': Connect(pos, { x + 1, y }, grid); break;
            case '(': Recurse(chars, index, pos, grid); break;
            case ')': return;
            case '|': pos = origin; break;
            case '$': return;
            }
        }
    }

    constexpr size_t Solve(std::string_view line, auto OnStep) {
        size_t index = 1;
        Grid grid{};
        Coord origin = { 0, 0 };
        Recurse(line, index, origin, grid);

        Constexpr::SmallSet<Coord> visited;
        Constexpr::Queue<std::pair<Coord, int>> queue;
        queue.push({ origin, 0 });

        size_t result = 0;
        while (!queue.is_empty()) {
            auto [pos, length] = queue.front();
            queue.pop();
            if (visited.contains(pos)) continue;
            visited.insert(pos);
            OnStep(length, result);
            for (auto neighbor : grid[pos]) {
                queue.push({ neighbor, length + 1 });
            }
        }

        return result;
    }
    PART(1) {
        auto result = Solve(lines[0], [](size_t length, size_t& r) {
            r = std::max(length, r);
            });
        return Constexpr::ToString(result);
    }

    PART(2) {
        auto result = Solve(lines[0], [](size_t length, size_t& r) {
            r += length >= 1000;
            });
        return Constexpr::ToString(result);
    }

    constexpr void TestKeepMax(size_t length, size_t& r) {
        r = std::max(length, r);
    }

    static_assert(Solve("^WNE$", TestKeepMax) == 3);
    static_assert(Solve("^ENWWW(NEEE|SSE(EE|N))$", TestKeepMax) == 10);
    static_assert(Solve("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$", TestKeepMax) == 18);
    static_assert(Solve("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$", TestKeepMax) == 23);
    static_assert(Solve( "^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$" , TestKeepMax) == 31);
}