#include "Common.h"

SOLUTION(2025, 4) {
	constexpr std::vector<RowCol> GetRemovable(const std::vector<std::string>& lines) {
		RowCol max = GetLimits<RowCol>(lines);
		RowCol pos{ 0, 0 };
		std::vector<RowCol> result;
		result.reserve(1000);
		for (size_t row = 0; row < lines.size(); row++) {
			pos.Row = row;
			pos.Col = 0;
			for (size_t col = 0; col < lines[row].size(); col++) {
				pos.Col = col;
				if (lines[row][col] != '@') continue;
				auto n = GetAllNeighbors(pos, max);
				auto filled = std::count_if(n.begin(), n.end(), [&](const auto& p) { return lines[p.Row][p.Col] == '@'; });
				if(filled < 4) {
					result.push_back(pos);
				}
			}
		}
		return result;
	}

	constexpr size_t RemoveAll(const std::vector<std::string>& lines) {
		auto grid = lines;
		size_t total = 0;
		while(true) {
			auto removable = GetRemovable(grid);
			if(removable.empty()) break;
			total += removable.size();
			for(const auto& pos : removable) {
				grid[pos.Row][pos.Col] = '.';
			}
		}
		return total;
	}
	PART(1) {
		return GetRemovable(lines).size();
	}
	PART(2) {
		return RemoveAll(lines);
	}

	constexpr bool Sample() {
		std::vector<std::string> grid = {
			"..@@.@@@@.",
			"@@@.@.@.@@",
			"@@@@@.@.@@",
			"@.@@@@..@.",
			"@@.@@@@.@@",
			".@@@@@@@.@",
			".@.@.@.@@@",
			"@.@@@.@@@@",
			".@@@@@@@@.",
			"@.@.@@@.@."
		};

		return GetRemovable(grid).size() == 13;
	}

	constexpr bool Sample2() {
		std::vector<std::string> grid = {
		"..@@.@@@@.",
		"@@@.@.@.@@",
		"@@@@@.@.@@",
		"@.@@@@..@.",
		"@@.@@@@.@@",
		".@@@@@@@.@",
		".@.@.@.@@@",
		"@.@@@.@@@@",
		".@@@@@@@@.",
		"@.@.@@@.@."
		};

		return RemoveAll(grid) == 43;
	}

	static_assert(Sample());
	static_assert(Sample2());
}