#include "Common.h"

SOLUTION(2024, 10) {
	using ResultMap = Constexpr::BigSet<RowCol, 1'000>;

	void Recurse(const std::vector<std::string>& map, RowCol limits, RowCol pos, ResultMap& outResult) {
		auto current = map[pos.Row][pos.Col];
		if (current == '9') {
			outResult.insert(pos);
			return;
		}
		auto neighbors = GetDirectNeighbors(pos, limits);
		for(auto n : neighbors) {
			if(map[n.Row][n.Col] == current + 1) {
				Recurse(map, limits, n, outResult);
			}
		}
	}

	constexpr u32 SolvePart1(const std::vector<std::string>& lines) {
		auto limits = GetLimits<RowCol>(lines);
		ResultMap result(RowCol{9999, 9999});
		u32 count = 0;
		Constexpr::ForEach(limits, [&](RowCol start) {
			//I feel like there's a way to solve this without clearing the set every time
			// Maybe starting with 9s and going down to 0s?
			// Or maybe storing the number of 9s that can be reached by each point?
			result.clear();
			if (lines[start.Row][start.Col] == '0') {
				Recurse(lines, limits, start, result);
				count += static_cast<u32>(result.size());
			}
		});

		return count;
	}

	PART(1) {
		return Constexpr::ToString(SolvePart1(lines));
	}
	
	using ResultMap2 = Constexpr::BigMap<RowCol, u32, 10'000>;
	u32 Recurse2(const std::vector<std::string>& map, RowCol limits, RowCol pos, ResultMap2& outResult) {
		if (outResult.contains(pos)) return outResult[pos];
		auto current = map[pos.Row][pos.Col];
		if (current == '9') return 1;
		auto neighbors = GetDirectNeighbors(pos, limits);
		for (auto n : neighbors) {
			if (map[n.Row][n.Col] == current + 1) {
				outResult[pos] += Recurse2(map, limits, n, outResult);
			}
		}
		return outResult[pos];
	}

	constexpr u32 SolvePart2(const std::vector<std::string>& lines) {
		auto limits = GetLimits<RowCol>(lines);
		ResultMap2 result(RowCol{9999, 9999});
		u32 count = 0;
		Constexpr::ForEach(limits, [&](RowCol start) {
			if (lines[start.Row][start.Col] == '0') {
				count += Recurse2(lines, limits, start, result);
			}
		});

		return count;
	}

	PART(2) {
		return Constexpr::ToString(SolvePart2(lines));
	}

	TEST(1) {
		std::vector<std::string> lines{
			"...0...",
			"...1...",
			"...2...",
			"6543456",
			"7.....7",
			"8.....8",
			"9.....9"
		};

		return SolvePart1(lines) == 2;
	}

	TEST(2) {
		std::vector<std::string> lines{
			"..90..9",
			"...1.98",
			"...2..7",
			"6543456",
			"765.987",
			"876....",
			"987...."
		};

		return SolvePart1(lines) == 4;
	}

	TEST(3) {
		std::vector<std::string> lines{
			"10..9..",
			"2...8..",
			"3...7..",
			"4567654",
			"...8..3",
			"...9..2",
			".....01"
		};

		return SolvePart1(lines) == 3;
	}
	TEST(4) {
		std::vector<std::string> lines{
			"89010123",
			"78121874",
			"87430965",
			"96549874",
			"45678903",
			"32019012",
			"01329801",
			"10456732"
		};

		return SolvePart1(lines) == 36;
	}

	TEST(5) {
		std::vector<std::string> lines{
			"89010123",
			"78121874",
			"87430965",
			"96549874",
			"45678903",
			"32019012",
			"01329801",
			"10456732"
		};

		return SolvePart2(lines) == 81;
	}
}