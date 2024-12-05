#include "Common.h"

SOLUTION(2024, 4) {
	constexpr std::vector<Coord> GetStarts(char target, const std::vector<std::string>& lines) {
		std::vector<Coord> result;
		for (size_t row = 0; row < lines.size(); row++) {
			for (size_t col = 0; col < lines[row].size(); col++) {
				if (lines[row][col] == target) {
					result.emplace_back(static_cast<s64>(col), static_cast<s64>(row));
				}
			}
		}
		return result;
	}

	constexpr size_t FindWord2(const std::vector<std::string>& lines, const std::vector<Coord>& starts) {
		size_t result = 0;
		Coord origin = { 0, 0 };
		Coord limits = { static_cast<s64>(lines[0].size() - 1), static_cast<s64>(lines.size() - 1) };

		for(const auto& start : starts) {
			auto neighbors = GetAllNeighbors(start, limits);
			for(auto n : neighbors) {
				if(lines[n.Y][n.X] == 'M') {
					auto dir = n - start;
					auto a = n + dir;
					auto s = a + dir;
					if (!s.Between(origin, limits)) continue;
					result += lines[a.Y][a.X] == 'A' && lines[s.Y][s.X] == 'S';
				}
			}
		}

		return result;
	}

	constexpr size_t SolvePart1(const std::vector<std::string>& lines) {
		auto xs = GetStarts('X', lines);
		return FindWord2(lines, xs);
	}

	PART(1) {
		return Constexpr::ToString(SolvePart1(lines));
	}

	constexpr size_t SolvePart2(const std::vector<std::string>& lines) {
		auto as = GetStarts('A', lines);
		size_t result = 0;
		for(const auto& a : as) {
			if (a.X < 1 || 
				a.Y < 1 || 
				static_cast<size_t>(a.X) >= lines[0].size() - 1 || 
				static_cast<size_t>(a.Y) >= lines.size() - 1) continue;

			auto d1 = lines[a.Y - 1][a.X - 1];
			auto d2 = lines[a.Y + 1][a.X + 1];
			auto d3 = lines[a.Y - 1][a.X + 1];
			auto d4 = lines[a.Y + 1][a.X - 1];

			auto isMas = [](char lhs, char rhs) {
				return (lhs == 'M' || lhs == 'S') && (rhs == 'M' || rhs == 'S') && (lhs != rhs);
			};

			result += isMas(d1, d2) && isMas(d3, d4);
		}
		return result;
	}

	PART(2) {
		return Constexpr::ToString(SolvePart2(lines));
	}

	constexpr std::vector<std::string> GetSimpleInput() {
		return {
			"..X...",
			".SAMX.",
			".A..A.",
			"XMAS.S",
			".X...."
		};
	}

	constexpr std::vector<std::string> GetLargeInput() {
		return {
			"MMMSXXMASM",
			"MSAMXMSMSA",
			"AMXSXMAAMM",
			"MSAMASMSMX",
			"XMASAMXAMM",
			"XXAMMXXAMA",
			"SMSMSASXSS",
			"SAXAMASAAA",
			"MAMMMXMMMM",
			"MXMXAXMASX"
		};
	}

	static_assert(SolvePart1(GetSimpleInput()) == 4);
	static_assert(SolvePart1(GetLargeInput()) == 18);
	static_assert(SolvePart2(GetLargeInput()) == 9);
}