#include "Common.h"
#include "KnotHash.h"
#include "Core/Algorithms/FloodFill.h"

#include <bit>

SOLUTION(2017, 14) {
    using Grid = std::vector<std::vector<bool>>;

    constexpr Grid CreateGrid(std::string_view key) {
        Grid result;
        std::string prefix = std::string(key);
        for (auto i = 0; i < 128; i++) {
            std::string toHash = prefix + "-" + Constexpr::ToString(i);
            auto hash = KnotHash::Hash(toHash);
            std::vector<bool> line;

            for (auto value : hash) {
                for (size_t bit = 7; bit > 0; bit--) {
                    auto mask = 1u << bit;
                    line.push_back((value & mask) == mask);
                }
                line.push_back((value & 1) == 1);
            }

            result.push_back(line);
        }

        return result;
    }

    constexpr bool FindGroupStart(const Grid & grid, RowCol & out) {
        for (size_t row = 0; row < grid.size(); row++) {
            for (size_t col = 0; col < grid[row].size(); col++) {
                if (grid[row][col]) {
                    out = { row, col };
                    return true;
                }
            }
        }

        return false;
    }
    
    constexpr void FloodFill(Grid & grid, const RowCol & start) {
        auto limits = RowCol{ 127, 127 };
        std::vector<RowCol> current;
        current.push_back(start);

        while (!current.empty()) {
            auto pos = current.back();
            current.pop_back();
            grid[pos.Row][pos.Col] = false;
            auto neighbors = GetDirectNeighbors(pos, limits);
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(current), [&grid](const RowCol& rc) {
                return grid[rc.Row][rc.Col];
                });
        }
    }
    
    PART(1) {
        const auto& key = std::string(lines[0]);
        u32 used = 0;
        for (auto i = 0; i < 128; i++) {
            std::string toHash = key + "-" + Constexpr::ToString(i);
            auto hash = KnotHash::Hash(toHash);
            for (auto value : hash) {
                used += std::popcount(value);
            }
        }

        return used;
    }

    PART(2) {
        auto grid = CreateGrid(lines[0]);
        auto pos = RowCol{ 0, 0 };
        /*
        std::vector<RowCol> allPoints;
        for (size_t row = 0; row < grid.size(); row++) {
            for (size_t col = 0; col < grid.at(row).size(); col++) {
                if (grid[row][col]) {
                    allPoints.push_back({ row, col });
                }
            }
        }

        auto groups = GetAllFloodFillGroups<RowCol>(allPoints, [](const RowCol& rc) { return GetDirectNeighbors(rc, { 127, 127 }); });
        return groups.size();
        */
        ///this version is faster, but would prefer to not have 2 FloodFill methods floating around
        
        u32 groups = 0;
        while (FindGroupStart(grid, pos)) {
            groups++;
            FloodFill(grid, pos);
        }

        return groups;
    }

    /*
    TEST(1) {
        std::vector<std::string> lines = { "flqrgnkx" };
        if(PartOne(lines) != "8108") return false;
        if(PartTwo(lines) != "1242") return false;
        
        return true;
    }
    */
}