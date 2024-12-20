#include "Common.h"
#include "Core/Algorithms/AStar.h"

SOLUTION(2016, 24) {
    using Walls = std::vector<std::vector<bool>>;

    constexpr Walls FindWalls(const auto& lines) {
        Walls result;
		result.reserve(lines.size());

        for (auto row = 0ull; row < lines.size(); row++) {
            std::vector<bool> line;
            for (auto col = 0ull; col < lines[row].size(); col++) {
                line.emplace_back(lines[row][col] == '#');
            }
            result.emplace_back(line);
        }

        return result;
    }

    constexpr std::vector<RowCol> FindTargets(const auto& lines) {
        std::vector<RowCol> result;
        result.reserve(lines.size());
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] > '0') {
                    result.emplace_back(row, col);
                }
            }
        }

        return result;
    }

    constexpr RowCol FindStart(const auto& lines) {
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '0') {
                    return RowCol{ row, col };
                }
            }
        }

        return {};
    }

    using Cache = Constexpr::SmallMap<RowCol, Constexpr::SmallMap<RowCol, u32>>;
    constexpr u32 FindPathLength(const std::vector<RowCol> targets, const Walls & walls, const RowCol & start, Cache& cache) {
        RowCol currentTarget = { 0, 0 };
        RowCol limits = { walls.size(), walls[0].size() };

        auto neighborFunc = [&walls, &limits](const RowCol& pos) {
            auto neighbors = GetDirectNeighbors(pos, limits);
            std::vector<RowCol> result;
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&walls](const RowCol& rc) {
                return !walls[rc.Row][rc.Col];
                });
            return result;
        };

        auto begin = start;
        u32 result = 0;
        for (const auto& target : targets) {
            currentTarget = target;
            if (!cache.contains(begin) || !cache.at(begin).contains(currentTarget)) {
                auto path = AStarMin<RowCol>(begin, currentTarget, neighborFunc);
                cache[begin][currentTarget] = static_cast<u32>(path.size() - 1);
            }
            result += cache.at(begin).at(currentTarget);
            begin = currentTarget;
        }

        return result;
    }

    constexpr u32 Solve(const auto& lines, bool extendTargets) {
        auto walls = FindWalls(lines);
        auto targets = FindTargets(lines);
        auto start = FindStart(lines);
        Cache cache{};
        u32 result = 9999;

        std::sort(targets.begin(), targets.end());
        while (std::next_permutation(targets.begin(), targets.end())) {
            if (extendTargets) {
                auto extendedTargets = targets;
                extendedTargets.emplace_back(start);
                result = std::min(result, FindPathLength(extendedTargets, walls, start, cache));
            }
            else {
                result = std::min(result, FindPathLength(targets, walls, start, cache));
            }
        }

        return result;
    }

    PART(1) {
        return Solve(lines, false);
    }
    PART(2) {
        return Solve(lines, true);
    }

    TEST(1) {
        
        std::vector<std::string> lines = {
           "###########",
           "#0.1.....2#",
           "#.#######.#",
           "#4.......3#",
           "###########"
        };
        return Solve(lines, false) == 14;
    }
}