#include "Common.h"

SOLUTION(2023, 11) {
    using RowsAndCols = std::pair<std::vector<size_t>, std::vector<size_t>>;
    constexpr RowsAndCols GetEmptyRowsAndCols(const std::vector<std::string>&lines) {
        auto emptyRow = [](const std::string& line) {
            return std::all_of(line.begin(), line.end(), [](char c) { return c == '.'; });
            };

        auto emptyCol = [&](size_t col) {
            return std::all_of(lines.begin(), lines.end(), [col](const std::string& line) {
                return line[col] == '.';
                });
            };

        std::vector<size_t> rows;
        for (size_t row = 0u; row < lines.size(); row++) {
            if (emptyRow(lines[row])) {
                rows.push_back(row);
            }
        }

        std::vector<size_t> cols;
        for (size_t col = 0u; col < lines[0].size(); col++) {
            if (emptyCol(col)) {
                cols.push_back(col);
            }
        }

        return std::make_pair(rows, cols);
    }

    constexpr std::vector<RowCol> GetGalaxies(const std::vector<std::string>& lines) {
        std::vector<RowCol> result;
        RowCol pos{};
        for (const auto& line : lines) {
            for (auto c : line) {
                if (c == '#') {
                    result.push_back(pos);
                }
                pos.Col++;
            }
            pos.Row++;
            pos.Col = 0;
        }

        return result;
    }

    constexpr size_t GetDistance(RowCol lhs, RowCol rhs, const RowsAndCols& rowsAndCols, size_t multiplier) {
        auto [rows, cols] = rowsAndCols;
        size_t rowsToAdd = std::count_if(rows.begin(), rows.end(), [lhs, rhs](size_t row) {
            return (row < lhs.Row && row > rhs.Row) || (row > lhs.Row && row < rhs.Row);
            });
        size_t colsToAdd = std::count_if(cols.begin(), cols.end(), [lhs, rhs](size_t col) {
            return (col < lhs.Col && col > rhs.Col) || (col > lhs.Col && col < rhs.Col);
            });

        return MDistance(lhs, rhs) + (rowsToAdd * multiplier) + (colsToAdd * multiplier);
    }

    constexpr size_t Solve(const std::vector<std::string>& lines, size_t factor) {
        size_t result = 0;
        auto galaxies = GetGalaxies(lines);
        auto rowsAndCols = GetEmptyRowsAndCols(lines);

        for (size_t i = 0u; i < galaxies.size(); i++) {
            for (auto j = i + 1; j < galaxies.size(); j++) {
                result += GetDistance(galaxies[i], galaxies[j], rowsAndCols, factor - 1);
            }
        }

        return result;
    }
    PART(1) {
        return Constexpr::ToString(Solve(lines, 2));
    }
    
    PART(2) {
        return Constexpr::ToString(Solve(lines, 1'000'000));
    }

    TEST(1) {
        std::vector<std::string> lines = {
            "...#......",
            ".......#..",
            "#.........",
            "..........",
            "......#...",
            ".#........",
            ".........#",
            "..........",
            ".......#..",
            "#...#....."
        };
        
        return Part1(lines) == "374";
    }
}