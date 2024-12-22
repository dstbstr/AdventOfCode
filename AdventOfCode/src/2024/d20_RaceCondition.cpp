#include "Common.h"

SOLUTION(2024, 20) {
	using Grid = std::vector<std::string>;

	constexpr std::pair<RowCol, RowCol> FindStartEnd(const Grid& grid) {
		RowCol start, end;
		Constexpr::ForEach(GetLimits<RowCol>(grid), [&](RowCol rc) {
			if (grid[rc.Row][rc.Col] == 'S') {
				start = rc;
			} else if(grid[rc.Row][rc.Col] == 'E') {
				end = rc;
			}
		});

		return { start, end };
	}

	using CostMap = Constexpr::BigMap<RowCol, u32, 20'000>;
	constexpr CostMap GetCostMap(const Grid& lines) {
		auto grid = lines;
		auto [start, end] = FindStartEnd(grid);
		grid[start.Row][start.Col] = '.';
		grid[end.Row][end.Col] = '.';
		auto limits = GetLimits<RowCol>(grid);
		CostMap costMap{};
		RowCol current = start;
		while (current != end) {
			costMap[current] = static_cast<u32>(costMap.size());
			for (const auto& n : GetDirectNeighbors(current, limits)) {
				if (grid[n.Row][n.Col] == '.' && !costMap.contains(n)) {
					current = n;
					break;
				}
			}
		}
		costMap[end] = static_cast<u32>(costMap.size());

		return costMap;
	}

	constexpr u32 Solve(const Grid& lines, u32 threshold, u32 maxSteps) {
		auto costMap = GetCostMap(lines);
		u32 result = 0;
		auto entries = costMap.GetAllEntries();
		for (size_t i = 0; i < entries.size(); i++) {
			for (size_t j = i + 1; j < entries.size(); j++) {
				auto [lhsPos, lhsProgress] = entries[i];
				auto [rhsPos, rhsProgress] = entries[j];
				auto distance = MDistance(lhsPos, rhsPos);
				if(distance <= maxSteps) {
					auto saved = Constexpr::AbsDistance(lhsProgress, rhsProgress);
					if (saved - distance >= threshold) {
						result++;
					}
				}
			}
		}
		return result;
	}

	PART(1) {
		return Solve(lines, 100, 2);
	}

	PART(2) {
		return Solve(lines, 100, 20);
	}
	TEST(1) {
		std::vector<std::string> lines{
			"###############",
			"#...#...#.....#",
			"#.#.#.#.#.###.#",
			"#S#...#.#.#...#",
			"#######.#.#.###",
			"#######.#.#...#",
			"#######.#.###.#",
			"###..E#...#...#",
			"###.#######.###",
			"#...###...#...#",
			"#.#####.#.###.#",
			"#.#...#.#.#...#",
			"#.#.#.#.#.#.###",
			"#...#...#...###",
			"###############"
		};

		return Solve(lines, 2, 2) == 44
			&& Solve(lines, 50, 20) == 285;
	}
}