#include "Common.h"

SOLUTION(2025, 6) {
	using Set = std::pair<std::vector<size_t>, char>;
	using Grid = std::vector<std::vector<size_t>>;

	constexpr std::vector<Set> Parse1(const std::vector<std::string>& lines) {
		std::vector<Set> sets;
		sets.reserve(lines.back().size());
		auto operators = Constexpr::Split(lines.back(), " ");
		for(size_t col = 0; col < operators.size(); col++) {
			sets.emplace_back(std::vector<size_t>(lines.size() - 1), operators[col][0]);
		}
		for(size_t row = 0; row < lines.size() - 1; row++) {
			auto line = Constexpr::Split(lines[row], " ");
			for(size_t col = 0; col < line.size(); col++) {
				Constexpr::ParseNumber(line[col], sets[col].first[row]);
			}
		}
		return sets;
	}
	/*
	constexpr std::pair<Grid, std::vector<char>> ParseInput(const std::vector<std::string>& lines) {
		Grid grid;
		grid.reserve(lines.size() - 1);
		for(size_t row = 0; row < lines.size() - 1; row++) {
			auto line = Constexpr::Split(lines[row], " ");
			std::vector<size_t> gridRow(line.size(), 0);
			for (size_t i = 0; i < gridRow.size(); i++) {
				Constexpr::ParseNumber(line[i], gridRow[i]);
			}
			grid.emplace_back(gridRow);
		}
		std::vector<char> operations;
		operations.reserve(grid[0].size());
		auto opLine = Constexpr::Split(lines.back(), " ");
		for (const auto& opStr : opLine) {
			operations.push_back(opStr[0]);
		}
		return { grid, operations };
	}
	*/
	constexpr std::vector<Set> Parse2(const std::vector<std::string>& lines) {
		std::vector<Set> sets;
		sets.reserve(lines.back().size());
		Set current{};
		for(size_t col = 0; col < lines.back().size(); col++) {
			if(lines.back()[col] != ' ') {
				if (!current.first.empty()) {
					sets.push_back(current);
					current = Set{};
				}
				current.second = lines.back()[col];
			}
			size_t val = 0;
			for(size_t row = 0; row < lines.size() - 1; row++) {
				auto c = lines[row][col];
				if (c == ' ') continue;
				val = val * 10 + (c - '0');
			}
			if(val > 0) {
				current.first.push_back(val);
			}
		}
		sets.push_back(current);
		return sets;
	}

	constexpr size_t Subtotal(const Set& set) {
		auto plus = [](size_t a, size_t b) { return a + b; };
		auto mul = [](size_t a, size_t b) { return a * b; };
		size_t subtotal = set.second == '+' ? 0 : 1;
		auto op = set.second == '+' ? plus : mul;
		for(auto num : set.first) {
			subtotal = op(subtotal, num);
		}
		return subtotal;
	}

	constexpr size_t Solve(const std::vector<Set>& sets) {
		size_t total = 0;

		for(const auto& set : sets) {
			total += Subtotal(set);
		}
		return total;
	}

	PART(1) {
		auto sets = Parse1(lines);
		return Solve(sets);
	}
	PART(2) {
		auto sets = Parse2(lines);
		return Solve(sets);
	}

	constexpr size_t Sample1() {
		std::vector<std::string> input = {
			"123 328  51 64 ",
			" 45  64 387 23 ",
			"  6  98 215 314",
			"*   +   *   +  "
		};

		auto sets = Parse1(input);
		return Solve(sets);
	}
	static_assert(Sample1() == 4277556);

	constexpr size_t Sample2() {
		std::vector<std::string> input = {
			"123 328  51 64 ",
			" 45 64  387 23 ",
			"  6 98  215 314",
			"*   +   *   +  "
		};
		auto sets = Parse2(input);
		return Solve(sets);
	}

	static_assert(Sample2() == 3263827);
}